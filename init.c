#include "defs.h"

int SQ64[120], SQ120[64];

static inline void initSQ64SQ120(void) {
	int sq64 = 0;
	for (int i = 0; i < 120; i++) SQ64[i] = 65;
	for (int i = 0; i < 64; i++) SQ120[i] = 120;
	for (int rank = RANK_1; rank <= RANK_8; rank++) {
		for (int file = FILE_A; file <= FILE_H; file++) {
			int sq = FR2SQ(file, rank);
			SQ120[sq64] = sq;
			SQ64[sq] = sq64;
			sq64++;
		}
	}
}

void init(void) {
	initSQ64SQ120();
}