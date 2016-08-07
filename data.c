// Look-up tables for pieces.
const unsigned pieceBig[13] = { 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1 };
const unsigned pieceMaj[13] = { 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1 };
const unsigned pieceMin[13] = { 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0 };
const unsigned pieceVal[13] = { 0, 100, 325, 325, 550, 1000, 50000, 100, 325,
                                325, 550, 1000, 50000 };
const unsigned pieceCol[13] = { 2, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1 };
const unsigned pieceRookQueen[13] = { 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0 };
const unsigned pieceBishopQueen[13] = { 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0 };
const unsigned pieceKnight[13] = { 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 };
const unsigned pieceKing[13] = { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1 };
