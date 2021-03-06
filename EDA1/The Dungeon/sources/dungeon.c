//
// Created by rcarlini on 14/10/19.
//
#include <stdio.h>

#include "../headers/dungeon.h"

/**
 * Sets the value for row and column into the position.
 *
 * @param position The position to be modified.
 * @param row The row value.
 * @param column The column value.
 *
 * Pre:
 * Post:
 */
void set_position(Position* position, int row, int column) {
    position->row = row;
    position->column = column;
}

/**
 * Sets the starting position of the dungeon.
 * @param dungeon The dungeon to be set.
 *
 * Pre:
 * Post:
 */
void set_starting_position(Dungeon *dungeon, int row, int column) {
    set_position(&dungeon->initial_position, row, column);
}

/**
 * Returns the starting position of the dungeon.
 * @param dungeon The dungeon to be queried.
 * @return The starting position of the dungeon.
 *
 * Pre:
 * Post:
 */
Position get_starting_position(Dungeon *dungeon) {
    Position position = dungeon->initial_position;
    return position;
}

/**
 * Returns the starting room of the dungeon.
 * @param dungeon The dungeon to be queried.
 * @return The starting room of the dungeon.
 *
 * Pre:
 * Post:
 */
Room* get_starting_room(Dungeon *dungeon) {
    return &dungeon->rooms[dungeon->initial_position.row][dungeon->initial_position.column];
}

/**
 * Checks if the row and column values fits in the dungeon room matrix.
 *
 * @param row The row value.
 * @param column The column value.
 *
 * @return TRUE if the values are between 0 and ROWS for row and between 0 and COLUMN for column. If the row value is out of range, it returns INVALID_ROW code. If the column value is out of range, it returns INVALID_COLUMN code.
 *
 * Pre:
 * Post.
 */
int is_valid_coordinates(int row, int column) {
    if (row < 0 || row >= ROwS)
        return INVALID_ROW;
    if (column < 0 || column >= COLUMNS)
        return INVALID_COLUMN;
    return TRUE;
    //return row >= 0 && row < ROwS && column >= 0 && column < COLUMNS;
}

/**
 * Checks if the position fits in the dungeon room matrix.
 *
 * @param position The position to be checked.
 * @return TRUE if the position is between 0 and ROWS and between 0 and COLUMN. If the row value is out of range, it returns INVALID_ROW code. If the column value is out of range, it returns INVALID_COLUMN code.
 *
 * Pre:
 * Post:
 */
int is_valid_position(Position position) {
    return is_valid_coordinates(position.row, position.column);
}

/**
 * Returns the dungeon room at the input row and column position.
 *
 * @param dungeon The dungeon from which the room should be retrieved.
 * @param row The row position of the room.
 * @param column The column position of the room.
 * @return The (reference to the) room of the dungeon corresponding to the row and column values if those are valid. NULL otherwise.
 *
 * Pre:
 * Post:
 */
Room *get_room_at(Dungeon *dungeon, int row, int column) {
    return is_valid_coordinates(row, column) == TRUE ? &dungeon->rooms[row][column] : NULL;
}

/**
 * Returns the dungeon room at the input position.
 *
 * @param dungeon The dungeon from which the room should be retrieved.
 * @param position The position of the room.
 * @return The (reference to the) room of the dungeon corresponding to the position if valid. NULL otherwise.
 *
 * Pre:
 * Post:
 */
Room* get_room_at_position(Dungeon *dungeon, Position position) {
    return is_valid_position(position) == TRUE ? &dungeon->rooms[position.row][position.column] : NULL;
}

/**
 * Initializes the dungeon, setting the starting position to (0,0) and initializes each room in the matrix.
 * @param dungeon The dungeon to be initialized
 * @return SUCCESS code if the initialization was successful, ERROR code if something went wrong.
 *
 * Pre:
 * Post:
 */
int init_dungeon(Dungeon *dungeon) {
    set_starting_position(dungeon, 0, 0);
    for (int r = 0; r < ROwS; ++r)
        for (int c = 0; c < COLUMNS; ++c)
            init_room(&dungeon->rooms[r][c]);
    return SUCCESS;
}