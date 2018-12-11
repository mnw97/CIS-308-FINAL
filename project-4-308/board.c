
// board.c
// Project 4
// Matthew White and Jeffrey Martin

#include <stdlib.h>
#include "game_piece.h"
#include "board.h"



void initial_board() {
	for(int i = 0; i<8; i++){
                for(int j=0; j<8; j++){
                        if(i%2 == j%2 && j<3){
                                GamePiece *piece = malloc(sizeof(GamePiece);
                                piece->PawnIdentifier = pawn;
                                piece->Color = red;

                                board_array[i][j] = piece;
                        }
                        else if(i%2 == j%2 && j>4){
                                GamePiece *piece = malloc(sizeof(GamePiece)
                                piece->PawnIdentifier = pawn;
                                piece->Color = black;

                                board_array[i][j] = piece;
			}
                }
        }
}

void move_piece(int start_row, int start_col, int end_row, int end_col){
	board_array[end_row][end_col] = board_array[start_row][start_col];
	board_array[start_row][start_col] = NULL;

}

//
//Thie method computes if the users move is valid. If so, the method returns 0, if not, the method 
//returns 1.
//
int valid_move(){

}

//
//This method kings a piece if it has reached the opposite end of the board
//
void king_piece(GamePiece * piece){

	piece->PawnIdentifier = king;

}

//
//This method checks to see if someone has won the game
//
int check_win(){

}

//
//This method removes a piece if it has been jumped
//
void remove_jumped(){

}
