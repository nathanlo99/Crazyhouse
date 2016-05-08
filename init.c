#include "defs.h"

unsigned int SQ64[120], SQ120[64];
U64 setMask[64], clearMask[64];
U64 pieceKeys[13][120], sideKey, castleKeys[16], dropKeys[13][8];
unsigned int fileBoard[120], rankBoard[120];

// Generates a 64-bit random number by concatenating 3 15-bit random numbers and
// a 4-bit random number.
#define RAND_64 ((U64)rand()               | \
                 (U64)rand()         << 15 | \
                 (U64)rand()         << 30 | \
                 (U64)rand()         << 45 | \
                ((U64)rand() & 0xf)  << 60)

inline void init() {
  // Initialize the index converting arrays.
  unsigned int sq64 = 0;
  for (unsigned int i = 0; i < 120; i++) SQ64[i] = 65;
  for (unsigned int i = 0; i < 64; i++) SQ120[i] = 120;
  for (unsigned int rank = RANK_1; rank <= RANK_8; rank++) {
    for (unsigned int file = FILE_A; file <= FILE_H; file++) {
      const unsigned int sq = FR2SQ(file,rank);
      SQ120[sq64] = sq;
      SQ64[sq] = sq64++;
    }
  }

  // Initialize the bit masks.
  U64 mask = 1ULL;
  for (unsigned int i = 0; i < 64; i++) {
    setMask[i] = mask;
    clearMask[i] = ~mask;
    mask <<= 1;
  }
  
  // Initialize the hashkey masks to 64-bit random numbers.
  for (unsigned int i = 0; i < 13; i++)
    for (unsigned int j = 0; j < 120; j++)
      pieceKeys[i][j] = RAND_64;
  sideKey = RAND_64;
  for (unsigned int i = 0; i < 16; i++)
    castleKeys[i] = RAND_64;
  for (unsigned int i = 0; i < 13; i++)
    for (unsigned int j = 0; j < 8; j++)
      dropKeys[i][j] = RAND_64;

  for (unsigned int i = 0; i < 120; i++) {
    fileBoard[i] = OFFBOARD;
    rankBoard[i] = OFFBOARD;
  }

  // Initializes the file and rank lookup tables.
  for (unsigned int rank = RANK_1; rank <= RANK_8; rank++) {
    for (unsigned int file = FILE_A; file <= FILE_H; file++) {
      const unsigned int sq = FR2SQ(file, rank);
      fileBoard[sq] = file;
      rankBoard[sq] = rank;
    }
  }
}
