
// board.h

#ifndef BOARD_H
#define BOARD_H

// includes

#include "colour.h"
#include "piece.h"
#include "square.h"
#include "util.h"

// constants

const int Empty = 0;
const int Edge = Knight64; // HACK: uncoloured knight

const int WP = WhitePawn256;
const int WN = WhiteKnight256;
const int WB = WhiteBishop256;
const int WR = WhiteRook256;
const int WQ = WhiteQueen256;
const int WK = WhiteKing256;

const int BP = BlackPawn256;
const int BN = BlackKnight256;
const int BB = BlackBishop256;
const int BR = BlackRook256;
const int BQ = BlackQueen256;
const int BK = BlackKing256;

const int FlagsNone = 0;
const int FlagsWhiteKingCastle  = 1 << 0;
const int FlagsWhiteQueenCastle = 1 << 1;
const int FlagsBlackKingCastle  = 1 << 2;
const int FlagsBlackQueenCastle = 1 << 3;

const int StackSize = 4096;

// macros

#define KING_POS(board,colour) ((board)->piece[colour][0])

// types
/*
#ifdef 64bit_ver

typedef unsigned long Bitboard;

#define Remove(a,b)  ((a) = (a^(UNIT<<b)))
#define RemoveFirst(a)   ((a) = ((a) & ((a)-1)))
#define UNIT   ((Bitboard)1)
#define EMPTY  ((Bitboard)0)

extern int RotateR90[64],RotateL90[64];
extern int RotateR45[64],RotateL45[64],InvRotateR45[64],InvRotateL45[64];
extern Bitboard Mask[64],FPMask1,FPMask2,RankMask[8];

struct board_t {

   int piece_material[ColourNb]; // Thomas
   
   int bm_move[ColourNb];
   int bm_value[ColourNb];

   Bitboard All;  
   Bitboard R90,R45,L45;
   Bitboard Pieces[ColourNb];
   Bitboard Pawns[ColourNb];
   Bitboard Rooks[ColourNb];
   Bitboard Knights[ColourNb];
   Bitboard Bishops[ColourNb];
   Bitboard Queens[ColourNb];

   uint8 piece[ColourNb][17]; // was 32
   int piece_size[ColourNb];

   uint8 pawn[ColourNb][9]; // was 16
   int pawn_size[ColourNb];

   int piece_nb;
   int number[12]; // was 16

   int pawn_file[ColourNb][FileNb];

   int turn;
   int flags;
   int ep_square;
   int ply_nb;
   int sp; // TODO: MOVE ME?

   int cap_sq;

   int opening;
   int endgame;

   uint64 key;
   uint64 pawn_key;
   uint64 material_key;

   uint64 stack[StackSize];
};
#else */
struct board_t {

   int piece_material[ColourNb]; // Thomas
   
   int bm_move[ColourNb];
   int bm_value[ColourNb];

   int square[SquareNb];
   int pos[SquareNb];

   sq_t piece[ColourNb][17]; // was 32
   int piece_size[ColourNb];

   sq_t pawn[ColourNb][9]; // was 16
   int pawn_size[ColourNb];

   int piece_nb;
   int number[12]; // was 16

   int pawn_file[ColourNb][FileNb];

   int turn;
   int flags;
   int ep_square;
   int ply_nb;
   int sp; // TODO: MOVE ME?

   int cap_sq;

   int opening;
   int endgame;

   uint64 key;
   uint64 pawn_key;
   uint64 material_key;

   uint64 stack[StackSize];
};
//#endif

// functions

extern bool board_is_ok         (const board_t * board);

extern void board_clear         (board_t * board);
extern void board_copy          (board_t * dst, const board_t * src);

extern void board_init_list     (board_t * board);

extern bool board_is_legal      (const board_t * board);
extern bool board_is_check      (const board_t * board);
extern bool board_is_mate       (const board_t * board);
extern bool board_is_stalemate  (board_t * board);

extern bool board_is_repetition (const board_t * board);

extern int  board_material      (const board_t * board);
extern int  board_opening       (const board_t * board);
extern int  board_endgame       (const board_t * board);

#endif // !defined BOARD_H

// end of board.h

