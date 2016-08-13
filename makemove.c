
/*
makeMove(move, board):
        Store current state into history array
        Move current piece from 'from' to 'to'
        If a cpature was made, remove cpatured from the piece list
        Update fifty move rule
        Promotions
        En passant captures
        Set En passant square
        Update position counters, piece lists, promoted array, etc
        Maintain position key
        Castle permissions
        Change side, increment ply and hisPly
*/

#define HASH_PCE (piece, sq) (pos->posKey ^= pieceKeys[(piece)][(sq])])
#define HASH_CA (pos->posKey ^= castleKeys[pos->castlePerm])
#define HASH_SIDE (pos->posKey ^= sideKey)
#define HASH_EP (pos->posKey ^= pieceKeys[EMPTY][pos->enPas])
