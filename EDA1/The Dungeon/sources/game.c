//
// Created by rcarlini on 4/1/20.
//
#include <stdio.h>
#include <string.h>

#include "../headers/utils.h"
#include "../headers/state.h"
#include "../headers/dungeon_utils.h"
#include "../headers/game.h"

int print_wall_info(Wall* wall, char* side) {

    int doors = 0;
    if (has_door(wall) == TRUE) {
        printf("There is a door at the %s wall. ", side);
        doors++;

        if (has_open_door(wall) == TRUE) {
            printf("Seems to be OPEN.   ");

        }
        else {
            printf("Seems to be CLOSED. ");
        }

        //        if (strlen(wall->label) > 0) {
        //            printf("It has a label: \"%s\". ", wall->label);
        //        }
        printf("\n");
    }

    return doors;
}

char choose_direction(Room* room) {

    int doors = 0;

    printf("\n");
    doors += print_wall_info(get_wall(room, NORTH), "North");
    doors += print_wall_info(get_wall(room, EAST), "East");
    doors += print_wall_info(get_wall(room, SOUTH), "South");
    doors += print_wall_info(get_wall(room, WEST), "West");

    char option;
    if (doors == 0) {
        option = read_char_option("There are no doors! Press Q for quit and try again...\n");

    }
    else {
        option = read_char_option("Which direction do you want to go? (N/E/S/W)\nPress P for showing path, Q for quit\n");
    }

    return option;
}

int is_valid_direction_option(char direction) {
    if (direction == NORTH || direction == EAST ||
        direction == SOUTH || direction == WEST) {
        return TRUE;

    }
    else {
        return INVALID_OPTION;
    }
}

void do_move(State* state, char direction) {

    Position position = get_current_position(state);

    int move_result = move(state, direction);
    switch (move_result) {
    case SUCCESS:
        add_as_last_step(state, position, direction);
        break;

    case NO_DOOR_ERROR:
        printf("This side has no door!\n");
        break;

    case INVALID_MOVE:
        printf("This move is invalid!\n");
        break;

    case INVALID_DIRECTION:
        printf("This direction is invalid!\n");
        break;

    default:
        printf("Unexpected move result code!\n");
        break;
    }
}

/**
 * Print the path stored in the state. If there are no steps, shows the message 'No steps yet'. If there are, should
 * print the starting position and the sequence of directions followed, e.g. 'Starting from (0, 0): N E'.
 *
 * @param state The state storing the path to be printed.
 */
void print_path(State* state) {

    if (!state->dungeon || !state->path.first)
        return;

    Position starting_position = get_starting_position(state->dungeon);
    printf("Starting from (%d, %d): ", starting_position.row, starting_position.column);
    for (Step* s = state->path.first; s; s = s->next)
    {
        if (s->next)
            printf("%c, ", s->direction);
        else printf("%c", s->direction);
    }
}

void start_game(Dungeon* dungeon) {

    State state;
    init_state(&state, dungeon);

    int quit = FALSE;
    printf("Starting game...\n\n");
    while (quit == FALSE) {

        Room* current_room = get_current_room(&state);
        if (current_room == NULL) {
            quit = TRUE;
            printf("No current room found! Quitting...\n");

        }
        else {
            draw_dungeon(stdout, dungeon, current_room, TRUE);

            char direction = choose_direction(current_room);

            if (is_valid_direction_option(direction) == TRUE) {
                do_move(&state, direction);

                if (is_finished(&state) == TRUE) {
                    quit = TRUE;
                }

            }
            else if (direction == PATH) {
                print_path(&state);

            }
            else if (direction == QUIT) {
                quit = TRUE;

            }
            else {
                printf("Invalid option! Try again...\n");
            }
        }
    }

    if (is_finished(&state) == TRUE) {
        printf("You found the exit!\n\n");
        print_path(&state);
    }
    reset(&state);
}

// Header needed for the recursion
int check_current_room(State* state);

/**
 * Checks if there is a door in the input direction. If there is and it is an exit door, returns TRUE. If it is not an
 * exit door, it should move and check the new current room by calling check_current_room. If that call returns TRUE, it
 * means that the step was correct, so it has to be stored in the path. Take into account that it should also go back
 * to the original room reverting the move in all cases.
 *
 * @param state The current state of the solving process.
 * @param room The current room.
 * @param direction The direction to be checked.
 * @return TRUE if this step leads to an exit, FALSE otherwise.
 */
int check_direction(State* state, Room* room, char direction) {

    Position position = get_current_position(state);

    int status = move(state, direction);
    if (status == TRUE)
    {
        if (!is_finished(state))
        {
            status = check_current_room(state);
            if (!status)
            {
                go_back(state, direction);
                return FALSE;
            }
        }

        add_as_first_step(state, position, direction);
        return TRUE;
    }

    return FALSE;
}

/**
 *  Checks if the current room is already visited. If it was not visited, checks each direction in order to find an exit.
 *  If one of the directions returns TRUE, it means that this room is part of the path to the exit and returns TRUE as well.
 *
 * @param state The current state of the solving process.
 * @return TRUE if the room belongs to the path to an exit.
 */
int check_current_room(State* state) {

    Room* room = get_current_room(state);
    if (!room)
        return FALSE;

    if (is_visited(room))
        return FALSE;
    mark_visited(room);

    int status = check_direction(state, room, EAST);
    if (status)
        return TRUE;

    status = check_direction(state, room, NORTH);
    if (status)
        return TRUE;

    status = check_direction(state, room, WEST);
    if (status)
        return TRUE;

    status = check_direction(state, room, SOUTH);
    if (status)
        return TRUE;

    clear_visited(room);

    return FALSE;
}

/**
 * Given a dungeon, searches for a path to an exit, printing the path if found or printing the message 'No solution found' otherwise.
 * @param dungeon
 */
void solve(Dungeon* dungeon) {
    State state;
    init_state(&state, dungeon);

    int solved = check_current_room(&state);
    if (solved == TRUE) {
        printf("\nSolution:\n");

        print_path(&state);
        printf("\n");
    }
    else {
        printf("\nNo solution found!\n");
    }

    reset(&state);
}
