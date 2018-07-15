
#include "defs.h"

#define MOVE(from, to, captured, promoted, flag)                               \
  ((from) | ((to) << 7) | ((captured) << 14) | ((promoted) << 20) | (flag))
#define DROP(piece, to) (((to) << 7) | ((piece) << 20) | MFLAGDROP)

static inline void addQuietMove(const S_BOARD *pos, unsigned move,
                                S_MOVELIST *list) {
  ASSERT(CAPTURED(move) == EMPTY);
  list->moves[list->count++] = (S_MOVE){move, 0};
}

static inline void addCaptureMove(const S_BOARD *pos, unsigned move,
                                  S_MOVELIST *list) {
  ASSERT(CAPTURED(move) != EMPTY);
  list->moves[list->count++] = (S_MOVE){move, 0};
}

static inline void addEnPassantMove(const S_BOARD *pos, unsigned move,
                                    S_MOVELIST *list) {
  ASSERT(move & MFLAGEP);
  list->moves[list->count++] = (S_MOVE){move, 0};
}

static inline void addDropMove(const S_BOARD *pos, unsigned move,
                               S_MOVELIST *list) {
  ASSERT(move & MFLAGDROP);
  list->moves[list->count++] = (S_MOVE){move, 0};
}

static inline void addWhitePawnCaptureMove(const S_BOARD *pos, const int from,
                                           const int to, const int captured,
                                           S_MOVELIST *list) {
  ASSERT(pieceValid(captured));
  ASSERT(squareOnBoard(from));
  ASSERT(squareOnBoard(to));
  ASSERT(pos->side == WHITE);
  if (rankBoard[from] == RANK_7) {
    addCaptureMove(pos, MOVE(from, to, captured, wQ, 0), list);
    addCaptureMove(pos, MOVE(from, to, captured, wR, 0), list);
    addCaptureMove(pos, MOVE(from, to, captured, wB, 0), list);
    addCaptureMove(pos, MOVE(from, to, captured, wN, 0), list);
  } else {
    addCaptureMove(pos, MOVE(from, to, captured, EMPTY, 0), list);
  }
}

static inline void addWhitePawnMove(const S_BOARD *pos, const int from,
                                    const int to, S_MOVELIST *list) {
  ASSERT(squareOnBoard(from));
  ASSERT(squareOnBoard(to));
  ASSERT(pos->side == WHITE);
  if (rankBoard[from] == RANK_7) {
    addQuietMove(pos, MOVE(from, to, EMPTY, wQ, 0), list);
    addQuietMove(pos, MOVE(from, to, EMPTY, wR, 0), list);
    addQuietMove(pos, MOVE(from, to, EMPTY, wB, 0), list);
    addQuietMove(pos, MOVE(from, to, EMPTY, wN, 0), list);
  } else {
    addQuietMove(pos, MOVE(from, to, EMPTY, EMPTY, 0), list);
  }
}

static inline void addBlackPawnCaptureMove(const S_BOARD *pos, const int from,
                                           const int to, const int captured,
                                           S_MOVELIST *list) {
  ASSERT(pieceValid(captured));
  ASSERT(squareOnBoard(from));
  ASSERT(squareOnBoard(to));
  ASSERT(pos->side == BLACK);
  if (rankBoard[from] == RANK_2) {
    addCaptureMove(pos, MOVE(from, to, captured, bQ, 0), list);
    addCaptureMove(pos, MOVE(from, to, captured, bR, 0), list);
    addCaptureMove(pos, MOVE(from, to, captured, bB, 0), list);
    addCaptureMove(pos, MOVE(from, to, captured, bN, 0), list);
  } else {
    addCaptureMove(pos, MOVE(from, to, captured, EMPTY, 0), list);
  }
}

static inline void addBlackPawnMove(const S_BOARD *pos, const int from,
                                    const int to, S_MOVELIST *list) {
  ASSERT(squareOnBoard(from));
  ASSERT(squareOnBoard(to));
  ASSERT(pos->side == BLACK);
  if (rankBoard[from] == RANK_2) {
    addQuietMove(pos, MOVE(from, to, EMPTY, bQ, 0), list);
    addQuietMove(pos, MOVE(from, to, EMPTY, bR, 0), list);
    addQuietMove(pos, MOVE(from, to, EMPTY, bB, 0), list);
    addQuietMove(pos, MOVE(from, to, EMPTY, bN, 0), list);
  } else {
    addQuietMove(pos, MOVE(from, to, EMPTY, EMPTY, 0), list);
  }
}

void generateAllMoves(const S_BOARD *pos, S_MOVELIST *list) {
  static const int directions[13][8] = {
      {0},                                // EMPTY
      {0},                                // P
      {-8, -19, -21, -12, 8, 19, 21, 12}, // N
      {-9, -11, 11, 9},                   // B
      {-1, -10, 1, 10},                   // R
      {-9, -11, 11, 9, -1, -10, 1, 10},   // Q
      {-9, -11, 11, 9, -1, -10, 1, 10},   // K
      {0},                                // P
      {-8, -19, -21, -12, 8, 19, 21, 12}, // N
      {-9, -11, 11, 9},                   // B
      {-1, -10, 1, 10},                   // R
      {-9, -11, 11, 9, -1, -10, 1, 10},   // Q
      {-9, -11, 11, 9, -1, -10, 1, 10},   // K
  };

  static const int numDirections[13] = {0, 0, 8, 4, 4, 8, 8, 0, 8, 4, 4, 8, 8};
  ASSERT(checkBoard(pos));
  list->count = 0;

  unsigned side = pos->side, sq, temp_sq;
  int dir;

  // PAWNS AND CASTLING
  if (side == WHITE) {
    for (unsigned pieceNum = 0; pieceNum < pos->pieceNum[wP]; pieceNum++) {
      sq = pos->pieceList[wP][pieceNum];
      ASSERT(squareOnBoard(sq));

      if (pos->pieces[sq + 10] == EMPTY) {
        addWhitePawnMove(pos, sq, sq + 10, list);
        if (rankBoard[sq] == RANK_2 && pos->pieces[sq + 20] == EMPTY) {
          addQuietMove(pos, MOVE(sq, sq + 20, EMPTY, EMPTY, MFLAGPS), list);
        }
      }

      if (squareOnBoard(sq + 9) && pieceCol[pos->pieces[sq + 9]] == BLACK) {
        addWhitePawnCaptureMove(pos, sq, sq + 9, pos->pieces[sq + 9], list);
      }
      if (squareOnBoard(sq + 11) && pieceCol[pos->pieces[sq + 11]] == BLACK) {
        addWhitePawnCaptureMove(pos, sq, sq + 11, pos->pieces[sq + 11], list);
      }

      if (pos->enPas != NO_SQ && sq + 9 == pos->enPas) {
        addEnPassantMove(pos, MOVE(sq, sq + 9, EMPTY, EMPTY, MFLAGEP), list);
      }
      if (pos->enPas != NO_SQ && sq + 11 == pos->enPas) {
        addEnPassantMove(pos, MOVE(sq, sq + 11, EMPTY, EMPTY, MFLAGEP), list);
      }
    }

    if (pos->castlePerm & WKCA) {
      if (pos->pieces[F1] == EMPTY && pos->pieces[G1] == EMPTY) {
        if (!squareAttacked(E1, BLACK, pos) &&
            !squareAttacked(F1, BLACK, pos)) {
          addQuietMove(pos, MOVE(E1, G1, EMPTY, EMPTY, MFLAGCA), list);
        }
      }
    }

    if (pos->castlePerm & WQCA) {
      if (pos->pieces[D1] == EMPTY && pos->pieces[C1] == EMPTY &&
          pos->pieces[B1] == EMPTY) {
        if (!squareAttacked(E1, BLACK, pos) &&
            !squareAttacked(D1, BLACK, pos) &&
            !squareAttacked(C1, BLACK, pos)) {
          addQuietMove(pos, MOVE(E1, C1, EMPTY, EMPTY, MFLAGCA), list);
        }
      }
    }

    if (pos->dropNum[wP] > 0) {
      for (unsigned i = 8; i < 64; i++) {
        sq = SQ120[i];
        if (pos->pieces[sq] == EMPTY) {
          addDropMove(pos, DROP(wP, sq), list);
        }
      }
    }
  } else {
    for (unsigned pieceNum = 0; pieceNum < pos->pieceNum[bP]; pieceNum++) {
      sq = pos->pieceList[bP][pieceNum];
      ASSERT(squareOnBoard(sq));

      if (pos->pieces[sq - 10] == EMPTY) {
        addBlackPawnMove(pos, sq, sq - 10, list);
        if (rankBoard[sq] == RANK_7 && pos->pieces[sq - 20] == EMPTY) {
          addQuietMove(pos, MOVE(sq, sq - 20, EMPTY, EMPTY, MFLAGPS), list);
        }
      }

      if (squareOnBoard(sq - 9) && pieceCol[pos->pieces[sq - 9]] == WHITE) {
        addBlackPawnCaptureMove(pos, sq, sq - 9, pos->pieces[sq - 9], list);
      }
      if (squareOnBoard(sq - 11) && pieceCol[pos->pieces[sq - 11]] == WHITE) {
        addBlackPawnCaptureMove(pos, sq, sq - 11, pos->pieces[sq - 11], list);
      }

      if (pos->enPas != NO_SQ && sq - 9 == pos->enPas) {
        addEnPassantMove(pos, MOVE(sq, sq - 9, EMPTY, EMPTY, MFLAGEP), list);
      }
      if (pos->enPas != NO_SQ && sq - 11 == pos->enPas) {
        addEnPassantMove(pos, MOVE(sq, sq - 11, EMPTY, EMPTY, MFLAGEP), list);
      }
    }

    if (pos->castlePerm & BKCA) {
      if (pos->pieces[F8] == EMPTY && pos->pieces[G8] == EMPTY) {
        if (!squareAttacked(E8, WHITE, pos) &&
            !squareAttacked(F8, WHITE, pos)) {
          addQuietMove(pos, MOVE(E8, G8, EMPTY, EMPTY, MFLAGCA), list);
        }
      }
    }

    if (pos->castlePerm & BQCA) {
      if (pos->pieces[D8] == EMPTY && pos->pieces[C8] == EMPTY &&
          pos->pieces[B8] == EMPTY) {
        if (!squareAttacked(E8, WHITE, pos) &&
            !squareAttacked(D8, WHITE, pos) &&
            !squareAttacked(C8, WHITE, pos)) {
          addQuietMove(pos, MOVE(E8, C8, EMPTY, EMPTY, MFLAGCA), list);
        }
      }
    }

    if (pos->dropNum[bP] > 0) {
      for (unsigned i = 0; i < 56; i++) {
        sq = SQ120[i];
        if (pos->pieces[sq] == EMPTY) {
          addDropMove(pos, DROP(bP, sq), list);
        }
      }
    }
  }

  for (unsigned piece = (side == WHITE ? wN : bN),
                pieceEnd = (side == WHITE ? wK : bK);
       piece <= pieceEnd; piece++) {
    if (pieceSlides[piece]) {
      for (unsigned pieceNum = 0; pieceNum < pos->pieceNum[piece]; pieceNum++) {
        sq = pos->pieceList[piece][pieceNum];
        for (int i = 0; i < numDirections[piece]; i++) {
          dir = directions[piece][i];
          temp_sq = sq + dir;
          while (squareOnBoard(temp_sq)) {

            if (pos->pieces[temp_sq] != EMPTY) {
              if (pieceCol[pos->pieces[temp_sq]] == (side ^ 1)) {
                addCaptureMove(
                    pos,
                    MOVE(sq, temp_sq, pos->pieces[temp_sq], EMPTY, MFLAGCAP),
                    list);
              }
              break;
            }
            addQuietMove(pos, MOVE(sq, temp_sq, EMPTY, EMPTY, 0), list);
            temp_sq += dir;
          }
        }
      }
    } else {
      for (unsigned pieceNum = 0; pieceNum < pos->pieceNum[piece]; pieceNum++) {
        sq = pos->pieceList[piece][pieceNum];
        for (int i = 0; i < numDirections[piece]; i++) {
          dir = directions[piece][i];
          temp_sq = sq + dir;
          if (squareOffBoard(temp_sq)) {
            continue;
          }

          if (pos->pieces[temp_sq] != EMPTY) {
            if (pieceCol[pos->pieces[temp_sq]] == (side ^ 1)) {
              addCaptureMove(
                  pos, MOVE(sq, temp_sq, pos->pieces[temp_sq], EMPTY, MFLAGCAP),
                  list);
            }
            continue;
          }
          addQuietMove(pos, MOVE(sq, temp_sq, EMPTY, EMPTY, 0), list);
        }
      }
    }

    if (pos->dropNum[piece] > 0) {
      for (unsigned i = 0; i < 64; i++) {
        sq = SQ120[i];
        if (pos->pieces[sq] == EMPTY) {
          addDropMove(pos, DROP(piece, sq), list);
        }
      }
    }
  }
}
