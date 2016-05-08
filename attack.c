#include "defs.h"

const int RkDir[4] = { -1, -10, 1, 10 };
const int BiDir[4] = { -9, -11, 11, 9 };
const int KnDir[8] = { -8, -21, 8, 21, 19, -19, 12, -12 };
const int KiDir[8] = { -1, -10, 1, 10, -9, -11, 11, 9 };

// Returns true if the given square is attacked by any piece of the given side.
inline bool sqAttacked(const unsigned int sq, const unsigned int side, const S_BOARD *pos) {
  unsigned int piece, t_sq;

  const unsigned int king = wK + side * 6;
  const unsigned int knight = wN + side * 6;

  for (int i = 0; i < 8; i++) {
    if (pos->pieces[sq + KiDir[i]] == king
      || pos->pieces[sq + KnDir[i]] == knight) {
      return true;
    }
  }

  if (side == WHITE && (pos->pieces[sq - 11] == wP || pos->pieces[sq - 9] == wP)) {
    return true;
  }

  if (side == BLACK && (pos->pieces[sq + 11] == bP || pos->pieces[sq + 9] == bP)) {
    return true;
  }

  for (unsigned int i = 0; i < 4; i++) {
    t_sq = sq + RkDir[i];
    while (squareOnBoard(t_sq)) {
      piece = pos->pieces[t_sq];
      if (piece != EMPTY) {
        if (IsRQ(piece) && pieceCol[piece] == side) {
          return true;
        }
        break;
      }
      t_sq += RkDir[i];
    }

    t_sq = sq + BiDir[i];
    while (squareOnBoard(t_sq)) {
      piece = pos->pieces[t_sq];
      if (piece != EMPTY) {
        if (IsBQ(piece) && pieceCol[piece] == side) {
          return true;
        }
        break;
      }
      t_sq += BiDir[i];
    }
  }

  return false;
}