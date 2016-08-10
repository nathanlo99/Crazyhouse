#include <stdbool.h> // for true and false constants.

// Look-up tables for pieces.
const bool pieceBig[13] = {0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1};
const bool pieceMaj[13] = {0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1};
const bool pieceMin[13] = {0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0};
const unsigned pieceVal[13] = {0,   100, 325, 325, 550,  1000, 50000,
                               100, 325, 325, 550, 1000, 50000};
const unsigned pieceCol[13] = {2, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1};
const bool pieceRookQueen[13] = {0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0};
const bool pieceBishopQueen[13] = {0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0};
const bool pieceKnight[13] = {0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0};
const bool pieceKing[13] = {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1};
const bool pieceSlides[13] = {0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0};
