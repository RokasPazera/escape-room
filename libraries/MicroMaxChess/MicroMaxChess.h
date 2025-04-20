#ifndef MICRO_MAX_CHESS_H
#define MICRO_MAX_CHESS_H

class MicroMaxChess {
    public:
        MicroMaxChess();

        void printBoard(Print &print);

        /// @brief a move from a human player
        /// @param move the move in lowercase e.g. "e2e4"
        /// @return if move is valid
        bool move(const char *move);

        /// @brief generate a move
        /// @return the move in lowercase e.g. "e7e5" or nullptr if no move can be made
        const char* move(void);

        /// @brief determine if current player has lost; call directly after a player has moved 
        bool hasLost(void);

        /// @brief get the current move
        int moveNumber(void);

    private:
        long N, T;                /* N=evaluated positions+S, T=recursion limit */
        short Q, O, K, R, k;      /* k=moving side */
        char *p, c[5];            /* p=pointer to c, c=user input, computer output */
        int Z;                    /* Z=recursion counter */
        int L;
        int r = 0;                /* result of determine best move D() */
        int mn;                   /* move number */

        unsigned int seed;
        uint32_t byteBoard[8];

        static const int w[];     /* relative piece values */
        static const int o[];     /* step-vector lists */
        static const char sym[];  /* chess symbols (piece letters) */

        static const char bb[129];   /* initial board values */
        char b[129];                 /* board is left part, center-pts table is right part, and dummy */

        unsigned short myrand(void);
        short D(short q, short l, short e, unsigned char E, unsigned char z, unsigned char n);
};

#endif