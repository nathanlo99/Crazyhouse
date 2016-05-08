#include "defs.h"

int main(void) {
	init();
	S_BOARD pos[1];
	parseFEN(START_FEN, pos);
	printBoard(pos);
	return 0;
}
