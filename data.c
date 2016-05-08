// Look-up tables for pieces.
const unsigned int pieceBig[13] = { 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1 };
const unsigned int pieceMaj[13] = { 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1 };
const unsigned int pieceMin[13] = { 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0 };
const unsigned int pieceVal[13]= { 0, 100, 325, 325, 550, 1000, 50000,
                                      100, 325, 325, 550, 1000, 50000  };
const unsigned int pieceCol[13] = { 2, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1 };
const unsigned int pieceRookQueen[13] = { 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0 };
const unsigned int pieceBishopQueen[13] = { 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0 };
const unsigned int pieceKnight[13] = { 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 };
const unsigned int pieceKing[13] = { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1};