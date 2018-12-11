//
// board.h
// Project 4
// Matthew White and Jeffrey Martin

#ifndef board_h
#define board_h

#include "game_piece.h"


typedef struct Board {

	GamePiece *board_array[8][8];
} Board;


#endif
