#include "defs.h"

inline const char *printSquare(const unsigned sq) {
  static char result[3];
  if (sq >= 120 || squareOffBoard(sq))
    return "--";
  result[0] = 'a' + fileBoard[sq];
  result[1] = '1' + rankBoard[sq];
  result[2] = 0;
  return result;
}

inline const char *printMove(const unsigned move) {
  static char result[6];
  const unsigned from = FROMSQ(move);
  const unsigned to = TOSQ(move);
  ASSERT(squareOnBoard(to));
  if (move & MFLAGDROP) {
    result[0] = pieceChar[PROMOTED(move) % 6];
    result[1] = '@';
    result[2] = 'a' + fileBoard[to];
    result[3] = '1' + rankBoard[to];
    result[4] = 0;
  } else {
    ASSERT(squareOnBoard(from));
    result[0] = 'a' + fileBoard[from];
    result[1] = '1' + rankBoard[from];
    result[2] = 'a' + fileBoard[to];
    result[3] = '1' + rankBoard[to];
    result[4] = PROMOTED(move) ? "__nbrq__nbrq_"[PROMOTED(move)] : 0;
    result[5] = 0;
  }
  return result;
}

inline void printMoveList(const S_MOVELIST *list) {
  int score;
  unsigned move;
  printf("\n============================ MOVE LIST "
         "=============================\n");
  printf(" LIST HAS %d MOVES:\n", list->count);
  for (unsigned i = 0; i < list->count; i++) {
    move = list->moves[i].move;
    score = list->moves[i].score;
    printf("MOVE %3d: %5s (SCORE: %d)\n", i + 1, printMove(move), score);
  }
  printf("\n========================== END MOVE LIST "
         "===========================\n");
}

// Prints a formatted version of the game board into the console window.
inline void printBoard(const S_BOARD *pos) {
  static const char *temp = "   +---+---+---+---+---+---+---+---+";
  printf("\n============================ GAME BOARD "
         "============================\n\n");
  printf("     a   b   c   d   e   f   g   h\n");
  printf("%s\n", temp);
  for (unsigned rank = RANK_8; rank != (unsigned)(RANK_1 - 1); rank--) {
    printf(" %c | ", '1' + rank);
    for (unsigned file = FILE_A; file <= FILE_H; file++) {
      const unsigned piece = pos->pieces[FR2SQ(file, rank)];
      ASSERT(pieceValidEmpty(piece));
      printf("%c | ", pieceChar[piece]);
    }
    printf("%c", '1' + rank);
    switch (rank) {
    case RANK_8:
      printf("          DROPPABLE PIECES\n%s         +---+---+---+---+---+\n",
             temp);
      break;
    case RANK_7:
      printf("       | P | N | B | R | Q |");
      printf("\n%s     +---+---+---+---+---+---+\n", temp);
      break;
    case RANK_6:
      printf("   | W | %d | %d | %d | %d | %d |", pos->dropNum[wP],
             pos->dropNum[wN], pos->dropNum[wB], pos->dropNum[wR],
             pos->dropNum[wQ]);
      printf("\n%s     +---+---+---+---+---+---+\n", temp);
      break;
    case RANK_5:
      printf("   | B | %d | %d | %d | %d | %d |", pos->dropNum[bP],
             pos->dropNum[bN], pos->dropNum[bB], pos->dropNum[bR],
             pos->dropNum[bQ]);
      printf("\n%s     +---+---+---+---+---+---+\n", temp);
      break;
    default:
      printf("\n%s\n", temp);
      break;
    }
  }
  printf("     a   b   c   d   e   f   g   h\n\n");

  ASSERT(sideValid(pos->side));
  if (pos->hisPly > 0) {
    const unsigned lastMove = pos->history[pos->hisPly - 1]->move;
    printf("  LAST MOVE: %s\n", printMove(lastMove));
  } else {
    printf("  LAST MOVE: NONE\n");
  }
  printf("  SIDE TO MOVE: %s\n", pos->side == WHITE ? "WHITE" : "BLACK");
  printf("  EN PASSANT SQUARE: %s\n", printSquare(pos->enPas));
  printf("  CASTLING PERMISSIONS: %c%c%c%c\n",
         pos->castlePerm & WKCA ? 'K' : '-', pos->castlePerm & WQCA ? 'Q' : '-',
         pos->castlePerm & BKCA ? 'k' : '-',
         pos->castlePerm & BQCA ? 'q' : '-');
  printf("  HASHKEY: %llX\n", pos->posKey);
  printf("\n========================== END GAME BOARD "
         "==========================\n\n");
}
