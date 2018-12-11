//
// game_piece.h
// Project 4
// Matthew White and Jeffrey Martin

#ifndef game_piece_h
#define game_piece_h

typedef enum Color {

	purple=0,
	white=1

} Color;

typedef enum PawnIdentifier {

	pawn=0,
	king=1

} PawnIdentifier;

typedef struct GamePiece {

	PawnIdentifier type;
	Color color;

} GamePiece;

#endif
