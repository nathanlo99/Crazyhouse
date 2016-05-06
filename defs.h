#ifndef DEFS_H
#define DEFS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint64_t U64;

#define NAME "Crazyhouse 1.0"
#define MAX_GAME_MOVES 2048

#ifdef DEBUG
#define ASSERT(n)\
  if (!(n)) { \
    printf("[ERROR] ASSERT(%s) failed @ %s : %d\n", #n, __FILE__, __LINE__); \
    exit(1); \
  }
#else
#define ASSERT(n)
#endif

enum { EMPTY, wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ,  bK };
enum { FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H };
enum { RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8 };

enum { WHITE, BLACK, BOTH };

enum { WKCA = 1, WQCA = 2, BKCA = 4, BQCA = 8 };

enum { A1 = 21, B1, C1, D1, E1, F1, G1, H1,
       A8 = 91, B8, C8, D8, E8, F8, G8, H8,
       NO_SQ, OFFBOARD };

typedef struct {
  int move;
  int castlePerm;
  int enPas;
  int fiftyMove;
  U64 posKey;
} S_UNDO;

typedef struct {
  int pieces[120];                   // pieces on board
  int promoted[120];                 // whether or not the pieces were promoted:
                                     // needed to decide whether to pocket a pawn 
  U64 pawns[3];                      // bitboards
  int side;                          // side to move
  int enPas;                         // en passant square
  int fiftyMove;                     // fifty move counter in half moves
  int ply;                           // depth of search
  int hisPly;                        // half moves played thus far
  U64 posKey;                        // 64-bit position key
  int pieceNum[13];                  // number of pieces on the board
  int dropNum[13];                   // number of captured pieces
  int bigPieces[2];                  // number of big pieces
  int majPieces[2];                  // number of major pieces
  int minPieces[2];                  // number of minor pieces
  S_UNDO* history[MAX_GAME_MOVES];   // game history
  int pieceList[13][16];             // piece list (extreme case is 16 pawns)

} S_BOARD;

// MACROS
#define FR2SQ(f, r) (21 + (f) + (r) * 10)

// GLOBALS
extern int SQ64[120], SQ120[64];

// FUNCTIONS
// main.c
int main(void);

// init.c
void init(void);

#endif