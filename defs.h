#ifndef DEFS_H
#define DEFS_H

#include <stdbool.h> // for true and false constants.
#include <stdint.h>  // for uint64_t.
#include <stdio.h>   // for printf.
#include <stdlib.h>  // for exit.
#include <string.h>  // for memset.
#include <time.h>    // for time.

typedef uint64_t U64;

#ifdef DEBUG
#define ASSERT(n)                                                              \
  if (!(n)) {                                                                  \
    printf("[ERROR] ASSERT(%s) failed at %s : %d\n", #n, __FILE__, __LINE__);  \
    exit(1);                                                                   \
  }
#else
#define ASSERT(n)
#endif

/*
MOVE FORMAT:
  The move format used to store a chess move into an 8-byte integer variable is
  taken from Bluefever Software's Chess Engine Tutorial on YouTube, and modified
  in order to accomodate crazyhouse drops.

  The move format stores the following information:
    FROM:       The square from which the piece moved. Needs 7 bits in order to
                  store the numbers 21 - 98.
    TO:         The square to which the piece moved. Also needs 7 bits.
    CAPTURED:   The piece, if any, that was captured in this move. Needs 4 bits
                  in order to store the piece numbers 0 - 13.
    EN PASSANT: This single bit flag is set if the move was an en passant
                capture.
    PAWN START: This single bit flag is set if the move was a pawn start.
                  (A pawn moving 2 squares instead of one on its first move.)
    PROMOTED:   The piece, if any, that was promoted to in this move. Needs
                  4 bits.
    CASTLE:     This single bit flag is set if the move was a castling move.
    DROP:       This single bit flag is set if the move was a drop, and the
                  type of piece dropped is stored in the 'promoted' section.
  The information is stored in the following way, with the 6 most significant
  bits unused:

      28   24   20   16   12    8    4    0 | OFFSET
    0000 0000 0000 0000 0000 0000 0111 1111 | FROM SQUARE
    0000 0000 0000 0000 0011 1111 1000 0000 | TO SQUARE
    0000 0000 0000 0011 1100 0000 0000 0000 | CAPTURED PIECE
    0000 0000 0000 0100 0000 0000 0000 0000 | EN PASSANT CAPTURE FLAG
    0000 0000 0000 1000 0000 0000 0000 0000 | PAWN START FLAG
    0000 0000 1111 0000 0000 0000 0000 0000 | PROMOTED PIECE
    0000 0001 0000 0000 0000 0000 0000 0000 | CASTLE FLAG
    0000 0010 0000 0000 0000 0000 0000 0000 | DROP FLAG
    1111 1100 0000 0000 0000 0000 0000 0000 | *UNUSED*
*/

/* DEFINES, TYPEDEFS, ENUMS */

// Based on Bluefever Software's tutorial on Programming a Chess Engine in C,
// in which he names his engine "VICE 1.0".
#define NAME "Crazyhouse 1.0"

// The maximum number of half moves in a chess game.
#define MAX_GAME_MOVES 2048

// The maximum number of possible moves in any position.
#define MAX_POSITION_MOVES 512

// The deepest the engine will attempt to search to.
#define MAX_SEARCH_DEPTH 64

// The FEN (Forsyth–Edwards Notation) for the classical starting position.
#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

// A sentinel value for a null move.
#define NO_MOVE 0

// The type used for bitboards and position hash-keys.
typedef uint64_t U64;

// Defines the pieces, starting at EMPTY = 0.
enum { EMPTY, wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK };
//         0   1   2   3   4   5   6   7   8   9  10  11  12

// Defines files and ranks.
enum { FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H };
enum { RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8 };

// Defines the colors.
enum { WHITE, BLACK, BOTH };

// Defines the indices for important squares on the board.
enum {
  A1 = 21,
  B1,
  C1,
  D1,
  E1,
  F1,
  G1,
  H1,
  A8 = 91,
  B8,
  C8,
  D8,
  E8,
  F8,
  G8,
  H8,
  NO_SQ,
  OFFBOARD
};

// Defines the bit-values of the castling permissions. To toggle a castling
// permission, XOR the current castling permission with the given castling bit.
enum { WKCA = 1, WQCA = 2, BKCA = 4, BQCA = 8 };

typedef struct {
  unsigned move;
  unsigned score;
} S_MOVE;

typedef struct {
  S_MOVE moves[MAX_POSITION_MOVES];
  unsigned count;
} S_MOVELIST;

typedef struct {
  unsigned move;
  unsigned castlePerm;
  unsigned enPas;
  unsigned fiftyMove;
  U64 posKey;
} S_UNDO;

typedef struct {
  unsigned pieces[120];            // pieces on board
  bool promoted[120];              // whether or not the pieces were promoted:
                                   // needed to decide whether to pocket a pawn
  U64 pawns[3];                    // bitboards
  unsigned side;                   // side to move
  unsigned enPas;                  // en passant square
  unsigned fiftyMove;              // fifty move counter in half moves
  unsigned castlePerm;             // castling permission
  unsigned ply;                    // depth of search
  unsigned hisPly;                 // half moves played thus far
  U64 posKey;                      // 64-bit position key
  unsigned pieceNum[13];           // number of pieces on the board
  unsigned dropNum[13];            // number of captured pieces
  unsigned bigPiece[2];            // number of big pieces
  unsigned majPiece[2];            // number of major pieces
  unsigned minPiece[2];            // number of minor pieces
  unsigned material[2];            // material, in centi-pawns
  S_UNDO *history[MAX_GAME_MOVES]; // game history
  unsigned pieceList[13][16];      // piece list (extreme case is 16 pawns)

} S_BOARD;

/* MACROS */
#define FR2SQ(f, r) (21 + (f) + (r)*10)

// Short-hands for clearing and setting a bit in a bitboard.
#define CLRBIT(bb, sq) ((bb) &= clearMask[(sq)])
#define SETBIT(bb, sq) ((bb) |= setMask[(sq)])

#define squareOnBoard(sq) (fileBoard[(sq)] != OFFBOARD)
#define squareOffBoard(sq) (fileBoard[(sq)] == OFFBOARD)
#define sideValid(side) ((side) == WHITE || (side) == BLACK)
#define pieceValid(piece) ((piece) >= wP && (piece) <= bK)
#define pieceValidEmpty(piece) ((piece) <= bK)

// Macros for breaking the move format into its constituent parts.
#define FROMSQ(m) (((m)) & 0x7F)
#define TOSQ(m) (((m) >> 07) & 0x7F)
#define CAPTURED(m) (((m) >> 14) & 0xF)
#define PROMOTED(m) (((m) >> 20) & 0xF)
#define DROPPED(m) (((m) >> 20) & 0xF)

// Constants for checking flags in a move in the above move format.
#define MFLAGEP 0x00040000
#define MFLAGCAP 0x0007C000
#define MFLAGPS 0x00080000
#define MFLAGPROM 0x00F00000
#define MFLAGCA 0x01000000
#define MFLAGDROP 0x02000000

// Short-hands for checking pieces
#define IsBQ(p) (pieceBishopQueen[(p)])
#define IsRQ(p) (pieceRookQueen[(p)])

// For printing readable versions of pieces.
#define pieceChar " PNBRQKpnbrqk"

/* GLOBALS */
// The integer from 0 - 63 corresponding to the index of the 64-based board
// given the index on the 120-based board.
extern unsigned SQ64[120];

// The integer from 0 - 119 corresponding to the index of the 120-based board
// given the index on the 64-based board.
extern unsigned SQ120[64];

// The 64-bit integers with which you 'OR' your bitboard in order to set and
// clear a given bit.
extern U64 setMask[64], clearMask[64];

// The 64-bit integers with which you bit-wise 'OR' your position key with, in
// order to generate unique keys for each position.
extern U64 pieceKeys[13][120], sideKey, castleKeys[16], dropKeys[13][8];

// Look-up tables for the files and ranks of a given square.
extern unsigned fileBoard[120], rankBoard[120];

// Look-up tables for pieces.
extern bool pieceBig[13], pieceMaj[13], pieceMin[13];
extern unsigned pieceVal[13], pieceCol[13];
extern bool pieceRookQueen[13], pieceBishopQueen[13], pieceSlides[13];

/* FUNCTIONS */

// attack.c
extern bool squareAttacked(const unsigned sq, const unsigned side,
                           const S_BOARD *pos);

// board.c
extern bool parseFEN(char *fen, S_BOARD *pos);
extern U64 generatePosKey(const S_BOARD *pos);
extern void resetBoard(S_BOARD *pos);
extern bool checkBoard(const S_BOARD *pos);

// init.c
extern void init(void);

// io.c
extern const char *printSquare(const unsigned sq);
extern const char *printMove(const unsigned move);
extern const char *printFEN(const S_BOARD *pos);
extern void printBoard(const S_BOARD *pos);
extern void printMoveList(const S_MOVELIST *list);

// movegen.c
extern void generateAllMoves(const S_BOARD *pos, S_MOVELIST *list);

// util.c
extern int getTimeMs(void);

#endif
