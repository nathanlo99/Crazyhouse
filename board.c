#include "defs.h"

inline U64 generatePosKey(const S_BOARD *pos) {
  U64 finalKey = 0ULL;

  for (unsigned piece = wP; piece <= bK; piece++) {
    for (unsigned i = 0, n = pos->pieceNum[piece]; i < n; i++)
      finalKey ^= pieceKeys[piece][pos->pieceList[piece][i]];
   	finalKey ^= dropKeys[piece][pos->dropNum[piece]];
  }

  if (pos->side == WHITE) finalKey ^= sideKey;
  if (pos->enPas != NO_SQ) finalKey ^= pieceKeys[EMPTY][pos->enPas];

  return finalKey ^ castleKeys[pos->castlePerm];
}

// Update the piece lists in a given position.
static inline void updateListsMaterial(S_BOARD *pos) {
  for (unsigned i = 0; i < 64; i++) {
    const unsigned piece = pos->pieces[SQ120[i]];
    if (piece == EMPTY) continue;
    int colour = pieceCol[piece];

    if (pieceBig[piece]) pos->bigPiece[colour]++;
    if (pieceMin[piece]) pos->minPiece[colour]++;
    if (pieceMaj[piece]) pos->majPiece[colour]++;

    pos->material[colour] += pieceVal[piece];
    pos->pieceList[piece][pos->pieceNum[piece]++] = SQ120[i];

    if (piece == wP) {
      SETBIT(pos->pawns[WHITE], i);
      SETBIT(pos->pawns[BOTH], i);
    } else if (piece == bP) {
      SETBIT(pos->pawns[BLACK], i);
      SETBIT(pos->pawns[BOTH], i);
    }
  }
}

// Resets the board.
inline void resetBoard(S_BOARD *pos) {
  memset(pos->pieces, OFFBOARD, sizeof(pos->pieces));
  for (unsigned i = 0; i < 64; i++) pos->pieces[SQ120[i]] = EMPTY;
  pos->bigPiece[WHITE] = pos->bigPiece[BLACK] = 0;
  pos->majPiece[WHITE] = pos->majPiece[BLACK] = 0;
  pos->minPiece[WHITE] = pos->minPiece[BLACK] = 0;
  pos->material[WHITE] = pos->material[BLACK] = 0;
  pos->pawns[WHITE] = pos->pawns[BLACK] = pos->pawns[BOTH] = 0ULL;
  for (unsigned i = EMPTY; i <= bK; i++) pos->pieceNum[i] = 0;
  for (unsigned i = EMPTY; i <= bK; i++) pos->dropNum[i] = 0;
  pos->side = BOTH;
  pos->enPas = NO_SQ;
  pos->fiftyMove = 0;
  pos->ply = 0;
  pos->hisPly = 0;
  pos->castlePerm = 0;
  pos->posKey = 0ULL;
}

// Parses an FEN string into a board.
inline bool parseFEN(char *fen, S_BOARD *pos) {

  int rank = RANK_8;
  int file = FILE_A;
  unsigned count = 0;
  unsigned piece = EMPTY;

  resetBoard(pos);

  while ((rank >= RANK_1) && *fen) {
    count = 1;
    switch (*fen) {
      case 'p': piece = bP; break;
      case 'r': piece = bR; break;
      case 'n': piece = bN; break;
      case 'b': piece = bB; break;
      case 'k': piece = bK; break;
      case 'q': piece = bQ; break;
      case 'P': piece = wP; break;
      case 'R': piece = wR; break;
      case 'N': piece = wN; break;
      case 'B': piece = wB; break;
      case 'K': piece = wK; break;
      case 'Q': piece = wQ; break;

      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
        piece = EMPTY;
        count = *fen - '0';
        break;

      case '/':
      case ' ':
        rank--;
        file = FILE_A;
        fen++;
        continue;

      default:
        printf("FEN error \n");
        return false;
    }

    for (unsigned i = 0; i < count; i++) {
      pos->pieces[FR2SQ(file, rank)] = piece;
      file++;
    }
    fen++;
  }

  pos->side = *fen == 'w' ? WHITE : BLACK;
  fen += 2;

  for (unsigned i = 0; i < 4; i++) {
    if (*fen == ' ') break;
    switch(*fen) {
      case 'K': pos->castlePerm |= WKCA; break;
      case 'Q': pos->castlePerm |= WQCA; break;
      case 'k': pos->castlePerm |= BKCA; break;
      case 'q': pos->castlePerm |= BQCA; break;
      default:                            break;
    }
    fen++;
  }
  fen++;

  if (*fen != '-') pos->enPas = FR2SQ(fen[0] - 'a', fen[1] - '1');

  updateListsMaterial(pos);
  pos->posKey = generatePosKey(pos);

  return true;
}

#ifdef DEBUG

inline bool checkBoard(const S_BOARD *pos) {
  unsigned t_pieceNum[13] = { 0 };
  unsigned t_bigPiece[2] = { 0 };
  unsigned t_majPce[2] = { 0 };
  unsigned t_minPce[2] = { 0 };
  unsigned t_material[2] = { 0 };

  U64 t_pawns[3] = { pos->pawns[WHITE], pos->pawns[BLACK], pos->pawns[BOTH] };

  for (unsigned i = wP; i <= bK; ++i)
    for (unsigned j = 0, n = pos->pieceNum[i]; j < n; ++j)
      ASSERT(pos->pieces[pos->pieceList[i][j]] == i);

  for (unsigned sq64 = 0; sq64 < 64; ++sq64) {
    const unsigned p = pos->pieces[SQ120[sq64]];
    const unsigned colour = pieceCol[p];
    t_pieceNum[pos->pieces[SQ120[sq64]]]++;
    if (pieceBig[p]) t_bigPiece[colour]++;
    if (pieceMin[p]) t_minPce[colour]++;
    if (pieceMaj[p]) t_majPce[colour]++;
    t_material[colour] += pieceVal[p];
  }

  for (unsigned i = wP; i <= bK; ++i) {
    ASSERT(t_pieceNum[i] == pos->pieceNum[i]);
  }

  ASSERT(CNT(t_pawns[WHITE]) == pos->pieceNum[wP]);
  ASSERT(CNT(t_pawns[BLACK]) == pos->pieceNum[bP]);
  ASSERT(CNT(t_pawns[BOTH]) == pos->pieceNum[bP] + pos->pieceNum[wP]);

  while (t_pawns[WHITE])
    ASSERT(pos->pieces[SQ120[POP(&t_pawns[WHITE])]] == wP);

  while (t_pawns[BLACK])
    ASSERT(pos->pieces[SQ120[POP(&t_pawns[BLACK])]] == bP);

  while (t_pawns[BOTH]) {
    const int sq64 = POP(&t_pawns[BOTH]);
    ASSERT((pos->pieces[SQ120[sq64]] == bP) ||
      (pos->pieces[SQ120[sq64]] == wP));
  }

  ASSERT(t_material[WHITE] == pos->material[WHITE]);
  ASSERT(t_material[BLACK] == pos->material[BLACK]);
  ASSERT(t_minPce[WHITE] == pos->minPiece[WHITE]);
  ASSERT(t_minPce[BLACK] == pos->minPiece[BLACK]);
  ASSERT(t_majPce[WHITE] == pos->majPiece[WHITE]);
  ASSERT(t_majPce[BLACK] == pos->majPiece[BLACK]);
  ASSERT(t_bigPiece[WHITE] == pos->bigPiece[WHITE]);
  ASSERT(t_bigPiece[BLACK] == pos->bigPiece[BLACK]);

  ASSERT(pos->side==WHITE || pos->side==BLACK);
  ASSERT(generatePosKey(pos) == pos->posKey);

  ASSERT(pos->enPas==NO_SQ ||
    (rankBoard[pos->enPas] == RANK_6 && pos->side == WHITE) ||
    (rankBoard[pos->enPas] == RANK_3 && pos->side == BLACK));

  return true;
}

#else

inline bool checkBoard(const S_BOARD *pos) { return true; }

#endif
