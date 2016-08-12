#include "defs.h"

#define TEMP                                                                   \
  "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"

int main(void) {
  init();
  S_BOARD pos[1];
  S_MOVELIST list[1];

  parseFEN(TEMP, pos);
  printBoard(pos);
  generateAllMoves(pos, list);
  printMoveList(list);
  return 0;
}
