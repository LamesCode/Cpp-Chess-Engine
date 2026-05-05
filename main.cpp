#include <iostream>
#include <vector>
#include <ctime>

// stores the indexes of a row and a col of a piece
struct Square {
    int row;
    int col;
};

// stores the square from which a piece is moving and a square to which it is moving
struct Move {
    Square fromSquare;
    Square toSquare;
};

// information about pieces needed for en passant and castle
struct PieceInformation {
    bool kingMoved = false;
    bool rookMovedA = false;
    bool rookMovedH = false;
    bool pawnMoved[8] = {false}; // indexes are cols e.g. 0 is a col
    int enPassantCol = -1; // index of a col where a pawn just made a double move
};

const char *PIECES[] = {
    "k","q","r","b","n","p",
    ".",
    "P","N","B","R","Q","K"
};

// piece value goes from -6 to 6 and this converts it to an index
char const* pieceChar(int pieceValue) {
    return PIECES[pieceValue + 6];
}

// for each square it converts the pieces value into a char and prints everything
// into a board format
void printBoard (int board [8][8]) {

    for (int i = 0; i < 8; i++) {
        std::cout << 8 - i << " ";
        for (int j = 0; j < 8; j++) {
            std::cout << pieceChar(board[i][j]) << " ";
        }
        std::cout << '\n';
    }

    std::cout << "  a b c d e f g h" << '\n' << '\n';

}

// gets the player move and stores it inside a Move variable and returns that value
Move getPlayerMove (int board[8][8]) {
    Move playerMove;
    char temp;

    // 1. gets the column of the square from which the player wants to move (the col must be on the board)
    // 2. gets the row of the square from which the player wants to move (must be inside the board)
    // 3. repeats the function until the player has chosen a valid square (isnt moving an empty space or a black piece)
    do {
        do {
            std::cout << "Col from which you are moving: ";
            std::cin  >> temp;
            if (std::cin.fail()) {std::cin.clear(); std::cin.ignore(10000, '\n'); temp = 'i';};
            playerMove.fromSquare.col = int(temp) - int('a');
        } while (playerMove.fromSquare.col > 7 || playerMove.fromSquare.col < 0);
        
        do {
            std::cout << "Row from which you are moving: ";
            std::cin  >> playerMove.fromSquare.row;
            if (std::cin.fail()) {std::cin.clear(); std::cin.ignore(10000, '\n');};
            playerMove.fromSquare.row = 8 - playerMove.fromSquare.row;
        } while (playerMove.fromSquare.row > 7 || playerMove.fromSquare.row < 0);

    } while (board[playerMove.fromSquare.row][playerMove.fromSquare.col] <= 0);

    // does the same thing as the previous cycle but instead it repeats itself until the player
    // chooses a square that isnt occupied by a white piece (player can only play white)
    do {
        do {
            std::cout << "Col to which you are moving: ";
            std::cin  >> temp;
            if (std::cin.fail()) {std::cin.clear(); std::cin.ignore(10000, '\n'); temp = 'i';};
            playerMove.toSquare.col = int(temp) - int('a');
        } while (playerMove.toSquare.col > 7 || playerMove.toSquare.col < 0);

        do {
            std::cout << "Row to which you are moving: ";
            std::cin  >> playerMove.toSquare.row;
            if (std::cin.fail()) {std::cin.clear(); std::cin.ignore(10000, '\n');};
            playerMove.toSquare.row = 8 - playerMove.toSquare.row;
        } while (playerMove.toSquare.row > 7 || playerMove.toSquare.row < 0);

    } while (board[playerMove.toSquare.row][playerMove.toSquare.col] > 0);

    std::cout << '\n';
    // returns the move
    return playerMove;
}

// displays a move by changing the board
void makeMove (int board[8][8], Move move, PieceInformation &WhitePieceInfo, PieceInformation &BlackPieceInfo) {

    WhitePieceInfo.enPassantCol = -1;
    BlackPieceInfo.enPassantCol = -1;
    int piece = board[move.fromSquare.row][move.fromSquare.col]; 
    char temp;
    int promotePiece = -1;
    int capturedPiece = board[move.toSquare.row][move.toSquare.col];
    // square on the board to which someone is moving will be changed to the piece that is on the square
    // from which someone is moving
    board[move.toSquare.row][move.toSquare.col] = board[move.fromSquare.row][move.fromSquare.col];
    // square from which someone is moving is reset to 0 because it is now empty
    board[move.fromSquare.row][move.fromSquare.col] = 0;

    // if king has moved then sets the kingMoved to true to prevent castling later on
    if (piece == 6) {WhitePieceInfo.kingMoved = true;};
    if (piece == -6) {BlackPieceInfo.kingMoved = true;};

    // if the rook has moved change the rookMoved to true
    if (piece ==  4 && move.fromSquare.row == 7 && move.fromSquare.col == 7) {WhitePieceInfo.rookMovedH = true;};
    if (piece ==  4 && move.fromSquare.row == 7 && move.fromSquare.col == 0) {WhitePieceInfo.rookMovedA = true;};
    if (piece == -4 && move.fromSquare.row == 0 && move.fromSquare.col == 7) {BlackPieceInfo.rookMovedH = true;};
    if (piece == -4 && move.fromSquare.row == 0 && move.fromSquare.col == 0) {BlackPieceInfo.rookMovedA = true;};

    // if someone captured the rook set the rookMoved to true to prevent castling
    if (move.toSquare.row == 7 && move.toSquare.col == 7) {WhitePieceInfo.rookMovedH = true;};
    if (move.toSquare.row == 7 && move.toSquare.col == 0) {WhitePieceInfo.rookMovedA = true;};
    if (move.toSquare.row == 0 && move.toSquare.col == 7) {BlackPieceInfo.rookMovedH = true;};
    if (move.toSquare.row == 0 && move.toSquare.col == 0) {BlackPieceInfo.rookMovedA = true;};

    // moving the rook when white is castling long and short
    if (
        move.fromSquare.row == 7 && move.fromSquare.col == 4 &&
        move.toSquare.row == 7 && move.toSquare.col == 2 && piece == 6
    ) {board[7][0] = 0; board[7][3] = 4; WhitePieceInfo.rookMovedA = true;};
    if (
        move.fromSquare.row == 7 && move.fromSquare.col == 4 &&
        move.toSquare.row == 7 && move.toSquare.col == 6 && piece == 6
    ) {board[7][7] = 0; board[7][5] = 4; WhitePieceInfo.rookMovedH = true;};

    // moving the rook when black is castling long and short
    if (
        move.fromSquare.row == 0 && move.fromSquare.col == 4 &&
        move.toSquare.row == 0 && move.toSquare.col == 2 && piece == -6
    ) {board[0][0] = 0; board[0][3] = -4; BlackPieceInfo.rookMovedA = true;};
    if (
        move.fromSquare.row == 0 && move.fromSquare.col == 4 &&
        move.toSquare.row == 0 && move.toSquare.col == 6 && piece == -6
    ) {board[0][7] = 0; board[0][5] = -4; BlackPieceInfo.rookMovedH = true;};

    // sets the captured pawn from en passant to empty space
    if (piece == 1 && move.fromSquare.row == 3 &&
        abs(move.fromSquare.col - move.toSquare.col) == 1 &&
        capturedPiece == 0) {
        board[3][move.toSquare.col] = 0;
    }
    if (piece == -1 && move.fromSquare.row == 4 &&
        abs(move.fromSquare.col - move.toSquare.col) == 1 &&
        capturedPiece == 0) {
        board[4][move.toSquare.col] = 0;
    }

    // setting the en passant col if a player did a double move
    if (piece == 1 && move.fromSquare.row == 6 && move.toSquare.row == 4) {BlackPieceInfo.enPassantCol = move.toSquare.col;}
    if (piece == -1 && move.fromSquare.row == 1 && move.toSquare.row == 3) {WhitePieceInfo.enPassantCol = move.toSquare.col;}

    // promotions
    if (piece == 1 && move.toSquare.row == 0) {
        board[move.toSquare.row][move.toSquare.col] =  5;
    }
    if (piece == -1 && move.toSquare.row == 7) {
        board[move.toSquare.row][move.toSquare.col] = -5;
    }
}

// from the square does a reverse move for every piece type to see if there is a piece that can check that square
bool isSquareInCheck (int board[8][8], const int SIDE, Square square) {
    int row;
    int col;

    // tests if a knight is checking that square
    int knightOffsets[8][2] = {
        {-2,-1},{-2,1},
        {-1,-2},{-1,2},
        { 1,-2},{ 1,2},
        { 2,-1},{ 2,1}
    };

    for (int i = 0; i < 8; i++) {
        row = square.row + knightOffsets[i][0];
        col = square.col + knightOffsets[i][1];

        if (row < 0 || row > 7 || col < 0 || col > 7) {continue;};
        if (SIDE == 1 && board[row][col] == -2) {return true;};
        if (SIDE == 0 && board[row][col] ==  2) {return true;};
    }

    // tests if a bishop or a queen diagonal is checking that square
    int diagonalOffsets[4][2] = {
        {-1,-1},{-1, 1},
        { 1,-1},{ 1, 1}
    };

    for (int i = 0; i < 4; i++) {
        row = square.row;
        col = square.col;
        
        while (true) {
            row += diagonalOffsets[i][0];
            col += diagonalOffsets[i][1];
            
            if (row < 0 || row > 7 || col < 0 || col > 7) {break;};

            if (board[row][col] != 0) {
                if (SIDE == 1 && (board[row][col] == -3 || board[row][col] == -5)) {return true;};
                if (SIDE == 0 && (board[row][col] ==  3 || board[row][col] ==  5)) {return true;};
                break;
            }
        }
    }

    // tests if a rook or a queen horizontal or vertical move is checking that square
    int horVerOffsets[4][2] = {
        {-1, 0},{ 1, 0},
        { 0, 1},{ 0,-1}
    };

    for (int i = 0; i < 4; i++) {
        row = square.row;
        col = square.col;

        while (true) {
            row += horVerOffsets[i][0];
            col += horVerOffsets[i][1];

            if (row < 0 || row > 7 || col < 0 || col > 7) {break;};

            if (board[row][col] != 0) {
                if (SIDE == 1 && (board[row][col] == -4 || board[row][col] == -5)) {return true;};
                if (SIDE == 0 && (board[row][col] ==  4 || board[row][col] ==  5)) {return true;};
                break;
            }
        }
    }

    // tests if a king is checking that square
    int kingOffsets[8][2] = {
        {-1,-1},{-1, 0},{-1, 1},
        { 0,-1},{ 0, 1},
        { 1,-1},{ 1, 0},{ 1, 1}
    };

    for (int i = 0; i < 8; i++) {
        row = square.row + kingOffsets[i][0];
        col = square.col + kingOffsets[i][1];

        if (row < 0 || row > 7 || col < 0 || col > 7) {continue;};
        if (SIDE == 1 && board[row][col] == -6) {return true;};
        if (SIDE == 0 && board[row][col] ==  6) {return true;};
    }

    // last case, checking if a pawn is checking that square
    row = square.row;
    col = square.col;

    if (SIDE == 1) {
        if (row > 0) {
            if (col > 0 && board[row - 1][col - 1] == -1) {return true;};
            if (col < 7 && board[row - 1][col + 1] == -1) {return true;};
        }
    }
    if (SIDE == 0) {
        if (row < 7) {
            if (col > 0 && board[row + 1][col - 1] ==  1) {return true;};
            if (col < 7 && board[row + 1][col + 1] ==  1) {return true;};
        }
    }

    return false;
}

// filters every square to find the friendly king and returns the square of the king
Square findKing (int board[8][8], const int SIDE) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (SIDE == 1 && board[i][j] ==  6) {return {i, j};}
            if (SIDE == 0 && board[i][j] == -6) {return {i, j};}
        }
    }
    return {-1, -1}; // if there is not a king which should not happen
}

// checks if the king is in check after the move
bool moveLeavesKingInCheck (int board[8][8], const int SIDE, Move move, PieceInformation WhitePieceInfo, PieceInformation BlackPieceInfo) {
    int tempBoard[8][8];

    // rewriting the board into a temp board
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            tempBoard[i][j] = board[i][j];
        }
    }

    makeMove(tempBoard, move, WhitePieceInfo, BlackPieceInfo);

    Square kingSquare = findKing(tempBoard, SIDE);
    if (isSquareInCheck(tempBoard, SIDE, kingSquare)) {return true;};

    return false;
}

// gets every legal move for a knight and retturns it inside of an array
std::vector <Move> getLegalMovesKnight (int board[8][8],const int SIDE, Square fromSquare) {

    // every possible square to which can a knight move ideally represented in offsets
    // we add these values to the row and col of the piece and that is a possible move for the knight
    int knightOffsets[8][2] = {
        {-2,-1},{-2,1},
        {-1,-2},{-1,2},
        { 1,-2},{ 1,2},
        { 2,-1},{ 2,1}
    };
    // vector in which we store the legal moves
    std::vector <Move> legalMoves;
    Square toSquare;

    // repeats this code for each offset
    for (int i = 0; i < 8; i++) {

        // makes a toSquare based on the legal offsets
        toSquare.row = fromSquare.row + knightOffsets[i][0];
        toSquare.col = fromSquare.col + knightOffsets[i][1];

        // checks if the move is outside of the board
        if (
            toSquare.row < 0 || toSquare.row > 7 ||
            toSquare.col < 0 || toSquare.col > 7
        ) {continue;};

        // for each side checks if the square is occupied by the same colored piece
        if (SIDE == 1 && board[toSquare.row][toSquare.col] > 0) {continue;};
        if (SIDE == 0 && board[toSquare.row][toSquare.col] < 0) {continue;};

        legalMoves.push_back({fromSquare, toSquare});
    }

    return legalMoves;
}

// goes in one direction (row, col, diagonal) and adds legal moves if the piece can move to that square and ends if it cant move 
// forward in that direction
std::vector <Move> getLegalMovesInDirection (int board[8][8], const int SIDE, Square fromSquare, int rowOffset, int colOffset) {
    Square toSquare = fromSquare;
    std::vector <Move> legalMoves;

    // while the piece can move
    while (true) {
        // going in a direction
        toSquare.row += rowOffset;
        toSquare.col += colOffset;

        // outside of the board
        if (
            toSquare.row < 0 || toSquare.row > 7 ||
            toSquare.col < 0 || toSquare.col > 7 
        ) {break;};

        // friendly piece
        if (SIDE == 1 && board[toSquare.row][toSquare.col] > 0) {break;};
        if (SIDE == 0 && board[toSquare.row][toSquare.col] < 0) {break;};

        // empty space
        if (board[toSquare.row][toSquare.col] == 0) {legalMoves.push_back({fromSquare, toSquare});};

        // enemy piece
        if (board[toSquare.row][toSquare.col] != 0) {legalMoves.push_back({fromSquare, toSquare}); break;};
    }

    return legalMoves;
}

// calls the getLegalMovesInDirection function to get legal moves for every direction a rook can move in
std::vector <Move> getLegalMovesRook (int board[8][8], const int SIDE, Square fromSquare) {

    std::vector <Move> legalMoves;
    std::vector <Move> moves;

    // goes in every 4 directions a rook can move in
    moves = getLegalMovesInDirection(board, SIDE, fromSquare, -1,  0);
    legalMoves.insert(legalMoves.end(), moves.begin(), moves.end());

    moves = getLegalMovesInDirection(board, SIDE, fromSquare,  1,  0);
    legalMoves.insert(legalMoves.end(), moves.begin(), moves.end());

    moves = getLegalMovesInDirection(board, SIDE, fromSquare,  0,  1);
    legalMoves.insert(legalMoves.end(), moves.begin(), moves.end());

    moves = getLegalMovesInDirection(board, SIDE, fromSquare,  0, -1);
    legalMoves.insert(legalMoves.end(), moves.begin(), moves.end());

    return legalMoves;
}

// calls the getLegalMovesInDirection function to get legal moves for every direction a bishop can move in
std::vector <Move> getLegalMovesBishop (int board[8][8], const int SIDE, Square fromSquare) {

    std::vector <Move> legalMoves;
    std::vector <Move> moves;

    // goes in every 4 directions a bishop can move in
    moves = getLegalMovesInDirection(board, SIDE, fromSquare, -1, -1);
    legalMoves.insert(legalMoves.end(), moves.begin(), moves.end());

    moves = getLegalMovesInDirection(board, SIDE, fromSquare, -1,  1);
    legalMoves.insert(legalMoves.end(), moves.begin(), moves.end());

    moves = getLegalMovesInDirection(board, SIDE, fromSquare,  1, -1);
    legalMoves.insert(legalMoves.end(), moves.begin(), moves.end());

    moves = getLegalMovesInDirection(board, SIDE, fromSquare,  1,  1);
    legalMoves.insert(legalMoves.end(), moves.begin(), moves.end());

    return legalMoves;
}

// calls the rook and bishop function because the queen moves as the rook and bishop combined
std::vector <Move> getLegalMovesQueen (int board[8][8], const int SIDE, Square fromSquare) {
    std::vector <Move> legalMoves;
    std::vector <Move> moves;

    // diagonal squares
    moves = getLegalMovesBishop(board, SIDE, fromSquare);
    legalMoves.insert(legalMoves.end(), moves.begin(), moves.end());

    // horizontal and vertical squares
    moves = getLegalMovesRook(board, SIDE, fromSquare);
    legalMoves.insert(legalMoves.end(), moves.begin(), moves.end());

    return legalMoves;
}

// first half does the same thing as the knight and the second one checks if the conditions for castling are true
// and if yes then adds them to the legal moves
std::vector <Move> getLegalMovesKing (int board[8][8], const int SIDE, Square fromSquare, PieceInformation PieceInfo) {
    int kingOffsets[8][2] = {
        {-1,-1},{-1, 0},{-1, 1},
        { 0,-1},{ 0, 1},
        { 1,-1},{ 1, 0},{ 1, 1}
    };
    std::vector <Move> legalMoves;
    Square toSquare;

    // normal moves
    for (int i = 0; i < 8; i++) {
        toSquare.row = fromSquare.row + kingOffsets[i][0];
        toSquare.col = fromSquare.col + kingOffsets[i][1];

        // checks if the move is outside of the board
        if (
            toSquare.row < 0 || toSquare.row > 7 ||
            toSquare.col < 0 || toSquare.col > 7
        ) {continue;};

        // for each side checks if the square is occupied by the same colored piece
        if (SIDE == 1 && board[toSquare.row][toSquare.col] > 0) {continue;};
        if (SIDE == 0 && board[toSquare.row][toSquare.col] < 0) {continue;};

        legalMoves.push_back({fromSquare, toSquare});
    }

    // white side long castle and short castle
    if (
        SIDE == 1 && PieceInfo.kingMoved == false && PieceInfo.rookMovedA == false &&
        board[7][1] == 0 && isSquareInCheck(board, SIDE, {7,1}) == false && 
        board[7][2] == 0 && isSquareInCheck(board, SIDE, {7,2}) == false && 
        board[7][3] == 0 && isSquareInCheck(board, SIDE, {7,3}) == false &&
        isSquareInCheck(board, SIDE, {7, 4}) == false
    ) {legalMoves.push_back({fromSquare, {7,2}});};
    if (
        SIDE == 1 && PieceInfo.kingMoved == false && PieceInfo.rookMovedH == false &&
        board[7][5] == 0 && isSquareInCheck(board, SIDE, {7,5}) == false && 
        board[7][6] == 0 && isSquareInCheck(board, SIDE, {7,6}) == false &&
        isSquareInCheck(board, SIDE, {7, 4}) == false
    ) {legalMoves.push_back({fromSquare, {7,6}});};

    // black side long castle and short castle
    if (
        SIDE == 0 && PieceInfo.kingMoved == false && PieceInfo.rookMovedA == false &&
        board[0][1] == 0 && isSquareInCheck(board, SIDE, {0,1}) == false && 
        board[0][2] == 0 && isSquareInCheck(board, SIDE, {0,2}) == false && 
        board[0][3] == 0 && isSquareInCheck(board, SIDE, {0,3}) == false &&
        isSquareInCheck(board, SIDE, {0, 4}) == false
    ) {legalMoves.push_back({fromSquare, {0,2}});};
    if (
        SIDE == 0 && PieceInfo.kingMoved == false && PieceInfo.rookMovedH == false &&
        board[0][5] == 0 && isSquareInCheck(board, SIDE, {0,5}) == false && 
        board[0][6] == 0 && isSquareInCheck(board, SIDE, {0,6}) == false &&
        isSquareInCheck(board, SIDE, {0, 4}) == false
    ) {legalMoves.push_back({fromSquare, {0,6}});};

    return legalMoves;
}

// checks every ideal possible move a pawn can make and returns the ones that the pawn can actually make based on the rules
std::vector <Move> getLegalMovesPawn (int board[8][8], const int SIDE, Square fromSquare, PieceInformation PieceInfo) {
    int whitePawnOffsets[4][2] = {
        {-1,-1},{-1,1}, // captures
        {-1,0},{-2,0}   // moving forward
    };
    int blackPawnOffsets[4][2] = {
        { 1,-1},{ 1,1}, // captures
        { 1,0},{ 2,0}   // moving forward
    };
    Square toSquare;
    std::vector <Move> legalMoves;

    for (int i = 0; i < 4; i++) {
        if (SIDE == 1) {
            toSquare.row = fromSquare.row + whitePawnOffsets[i][0];
            toSquare.col = fromSquare.col + whitePawnOffsets[i][1];

            // outside the board
            if (toSquare.col < 0 || toSquare.col > 7) {continue;};
            if (i == 0) { // capture left
                if (board[toSquare.row][toSquare.col] < 0 || (PieceInfo.enPassantCol == toSquare.col && fromSquare.row == 3)) {
                    legalMoves.push_back({fromSquare, toSquare});
                }
            }
            if (i == 1) { // capture right
                if (board[toSquare.row][toSquare.col] < 0 || (PieceInfo.enPassantCol == toSquare.col && fromSquare.row == 3)) {
                    legalMoves.push_back({fromSquare, toSquare});
                }
            }
            if (i == 2) { // moving one forward
                if (board[toSquare.row][toSquare.col] == 0) {
                    legalMoves.push_back({fromSquare, toSquare});
                }
            }
            if (i == 3) { // moving two forward
                if (board[toSquare.row][toSquare.col] == 0 && board[toSquare.row + 1][toSquare.col] == 0 && fromSquare.row == 6) {
                    legalMoves.push_back({fromSquare, toSquare});
                }
            }
        }
        if (SIDE == 0) {
            toSquare.row = fromSquare.row + blackPawnOffsets[i][0];
            toSquare.col = fromSquare.col + blackPawnOffsets[i][1];

            // outside the board
            if (toSquare.col < 0 || toSquare.col > 7) {continue;};
            if (i == 0) { // capture left
                if (board[toSquare.row][toSquare.col] > 0 || (PieceInfo.enPassantCol == toSquare.col && fromSquare.row == 4)) {
                    legalMoves.push_back({fromSquare, toSquare});
                }
            }
            if (i == 1) { // capture right
                if (board[toSquare.row][toSquare.col] > 0 || (PieceInfo.enPassantCol == toSquare.col && fromSquare.row == 4)) {
                    legalMoves.push_back({fromSquare, toSquare});
                }
            }
            if (i == 2) { // moving one forward
                if (board[toSquare.row][toSquare.col] == 0) {
                    legalMoves.push_back({fromSquare, toSquare});
                }
            }
            if (i == 3) { // moving two forward
                if (board[toSquare.row][toSquare.col] == 0 && board[toSquare.row - 1][toSquare.col] == 0 && fromSquare.row == 1) {
                    legalMoves.push_back({fromSquare, toSquare});
                }
            }
        }     
    }

    return legalMoves;
}

// based on the square of a piece, board and the side this function chooses the right function
// to get all of the legal moves for that piece
std::vector <Move> getLegalMoves (
    Square fromSquare, int board[8][8], const int SIDE, 
    const PieceInformation &WhitePieceInfo, const PieceInformation &BlackPieceInfo
) {
    const int PIECE = abs(board[fromSquare.row][fromSquare.col]);
    std::vector <Move> legalMoves;
    std::vector <Move> tempLegalMoves;

    // choosing the right function to get the legal moves
    switch (PIECE) {
        case 1:
            if (SIDE == 1) {
                tempLegalMoves = getLegalMovesPawn(board, SIDE, fromSquare, BlackPieceInfo);
            }
            else if (SIDE == 0) {
                tempLegalMoves = getLegalMovesPawn(board, SIDE, fromSquare, WhitePieceInfo);
            }
            break;
        case 2:
            tempLegalMoves = getLegalMovesKnight(board, SIDE, fromSquare);
            break;
        case 3:
            tempLegalMoves = getLegalMovesBishop(board, SIDE, fromSquare);
            break;
        case 4:
            tempLegalMoves = getLegalMovesRook(board, SIDE, fromSquare);
            break;
        case 5:
            tempLegalMoves = getLegalMovesQueen(board, SIDE, fromSquare);
            break;
        case 6:
            if (SIDE == 1) {
                tempLegalMoves = getLegalMovesKing(board, SIDE, fromSquare, WhitePieceInfo);
            }
            else if (SIDE == 0) {
                tempLegalMoves = getLegalMovesKing(board, SIDE, fromSquare, BlackPieceInfo);
            }
            break;
    }

    // only returns the move if after the move the king is not in check
    for (int i = 0; i < tempLegalMoves.size(); i++) {
        if (moveLeavesKingInCheck(board, SIDE, tempLegalMoves[i], WhitePieceInfo, BlackPieceInfo) == false) {
            legalMoves.push_back(tempLegalMoves[i]);
        }
    }

    return legalMoves;
}

// goes for every piece of a side and gets all legal moves for that piece
std::vector <Move> getAllLegalMoves (
    int board[8][8], const int SIDE, 
    const PieceInformation &WhitePieceInfo, const PieceInformation &BlackPieceInfo
) {
    std::vector <Move> allLegalMoves;

    // gets legal moves for a piece on the board at the square i,j
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (SIDE == 1 && board[i][j] > 0) {
                std::vector <Move> moves = getLegalMoves({i, j}, board, SIDE, WhitePieceInfo, BlackPieceInfo);
                allLegalMoves.insert(allLegalMoves.end(), moves.begin(), moves.end());
            }
            if (SIDE == 0 && board[i][j] < 0) {
                std::vector <Move> moves = getLegalMoves({i, j}, board, SIDE, WhitePieceInfo, BlackPieceInfo);
                allLegalMoves.insert(allLegalMoves.end(), moves.begin(), moves.end());
            }
        }
    }

    return allLegalMoves;
}

// compares every legal move to the player move and returns true if it sees a match
bool isPlayerMoveLegal (Move playerMove, std::vector <Move> legalMoves) {
    for (int i = 0; i < legalMoves.size(); i++) {
        if (
            playerMove.fromSquare.row == legalMoves[i].fromSquare.row &&
            playerMove.fromSquare.col == legalMoves[i].fromSquare.col &&
            playerMove.toSquare.row == legalMoves[i].toSquare.row &&
            playerMove.toSquare.col == legalMoves[i].toSquare.col 
        ) {return true;};
    }
    return false;
}

// returns a score that tells who is winning
int evaluateBoard (int board[8][8]) {
    // empty space, pawn, knight, bishop, rook, queen, king
    const int pieceValues[] = { 0, 100, 320, 330, 500, 900, 20000 };
    int score = 0;

    // boards show where a piece is stronger
    static const int pawnTable[8][8] = {
        { 0,  0,  0,  0,  0,  0,  0,  0},
        {50, 50, 50, 50, 50, 50, 50, 50},
        {10, 10, 20, 30, 30, 20, 10, 10},
        { 5,  5, 10, 25, 25, 10,  5,  5},
        { 0,  0,  0, 20, 20,  0,  0,  0},
        { 5, -5,-10,  0,  0,-10, -5,  5},
        { 5, 10, 10,-20,-20, 10, 10,  5},
        { 0,  0,  0,  0,  0,  0,  0,  0}
    };

    static const int knightTable[8][8] = {
        {-50,-40,-30,-30,-30,-30,-40,-50},
        {-40,-20,  0,  0,  0,  0,-20,-40},
        {-30,  0, 10, 15, 15, 10,  0,-30},
        {-30,  5, 15, 20, 20, 15,  5,-30},
        {-30,  0, 15, 20, 20, 15,  0,-30},
        {-30,  5, 10, 15, 15, 10,  5,-30},
        {-40,-20,  0,  5,  5,  0,-20,-40},
        {-50,-40,-30,-30,-30,-30,-40,-50}
    };

    static const int bishopTable[8][8] = {
        {-20,-10,-10,-10,-10,-10,-10,-20},
        {-10,  0,  0,  0,  0,  0,  0,-10},
        {-10,  0,  5, 10, 10,  5,  0,-10},
        {-10,  5,  5, 10, 10,  5,  5,-10},
        {-10,  0, 10, 10, 10, 10,  0,-10},
        {-10, 10, 10, 10, 10, 10, 10,-10},
        {-10,  5,  0,  0,  0,  0,  5,-10},
        {-20,-10,-10,-10,-10,-10,-10,-20}
    };

    static const int rookTable[8][8] = {
        { 0,  0,  0,  5,  5,  0,  0,  0},
        { 5, 10, 10, 10, 10, 10, 10,  5},
        {-5,  0,  0,  0,  0,  0,  0, -5},
        {-5,  0,  0,  0,  0,  0,  0, -5},
        {-5,  0,  0,  0,  0,  0,  0, -5},
        {-5,  0,  0,  0,  0,  0,  0, -5},
        {-5,  0,  0,  0,  0,  0,  0, -5},
        { 0,  0,  0,  5,  5,  0,  0,  0}
    };

    static const int queenTable[8][8] = {
        {-20,-10,-10, -5, -5,-10,-10,-20},
        {-10,  0,  0,  0,  0,  0,  0,-10},
        {-10,  0,  5,  5,  5,  5,  0,-10},
        { -5,  0,  5,  5,  5,  5,  0, -5},
        {  0,  0,  5,  5,  5,  5,  0, -5},
        {-10,  5,  5,  5,  5,  5,  0,-10},
        {-10,  0,  5,  0,  0,  0,  0,-10},
        {-20,-10,-10, -5, -5,-10,-10,-20}
    };

    static const int kingTable[8][8] = {
        {-30,-40,-40,-50,-50,-40,-40,-30},
        {-30,-40,-40,-50,-50,-40,-40,-30},
        {-30,-40,-40,-50,-50,-40,-40,-30},
        {-30,-40,-40,-50,-50,-40,-40,-30},
        {-20,-30,-30,-40,-40,-30,-30,-20},
        {-10,-20,-20,-20,-20,-20,-20,-10},
        { 20, 20,  0,  0,  0,  0, 20, 20},
        { 20, 30, 10,  0,  0, 10, 30, 20} 
    };

    // for every piece adds the real value of that piece to the score
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int piece = board[i][j];

            if (piece == 0) {continue;};

            if (piece > 0) {
                switch (piece) {
                    case 1:
                        score += (pieceValues[piece] + pawnTable[i][j]);
                        break;
                    case 2:
                        score += (pieceValues[piece] + knightTable[i][j]);
                        break;
                    case 3:
                        score += (pieceValues[piece] + bishopTable[i][j]);
                        break;
                    case 4:
                        score += (pieceValues[piece] + rookTable[i][j]);
                        break;
                    case 5:
                        score += (pieceValues[piece] + queenTable[i][j]);
                        break;
                    case 6:
                        score += (pieceValues[piece] + kingTable[i][j]);
                        break;
                } 
            }

            if (piece < 0) {
                switch (abs(piece)) {
                    case 1:
                        score -= (pieceValues[-piece] + pawnTable[7 - i][j]);
                        break;
                    case 2:
                        score -= (pieceValues[-piece] + knightTable[7 - i][j]);
                        break;
                    case 3:
                        score -= (pieceValues[-piece] + bishopTable[7 - i][j]);
                        break;
                    case 4:
                        score -= (pieceValues[-piece] + rookTable[7 - i][j]);
                        break;  
                    case 5:
                        score -= (pieceValues[-piece] + queenTable[7 - i][j]);
                        break;
                    case 6:
                        score -= (pieceValues[-piece] + kingTable[7 - i][j]);
                        break;
                } 
            }
        }
    }

    return score;
}

int minimax (
    int board[8][8], int depth, int alpha, int beta, bool isMaximizing,
    PieceInformation WhitePieceInfo, PieceInformation BlackPieceInfo
) {
    // end of the search
    if (depth == 0) {return evaluateBoard(board);};

    int side = isMaximizing ? 1 : 0;
    std::vector <Move> moves = getAllLegalMoves(board, side, WhitePieceInfo, BlackPieceInfo);

    if (moves.empty()) {
        // checkmate
        if (isSquareInCheck(board, side, findKing(board, side))) {return isMaximizing ? -30000 : 30000;};
        // stalemate
        return 0;
    }

    // white wants to play the best move
    if (isMaximizing) {
        int bestScore = -1000000;

        for (int i = 0; i < moves.size(); i++) {
            int tempBoard[8][8];

            // copying information
            for (int j = 0; j < 8; j++) {
                for (int k = 0; k < 8; k++) {
                    tempBoard[j][k] = board[j][k];
                }
            }

            PieceInformation tempWhiteInfo = WhitePieceInfo;
            PieceInformation tempBlackInfo = BlackPieceInfo;

            // simulating the move and looking for the best one
            makeMove(tempBoard, moves[i], tempWhiteInfo, tempBlackInfo);
            int score = minimax(tempBoard, depth - 1, alpha, beta, false, tempWhiteInfo, tempBlackInfo);
            bestScore = std::max(bestScore, score);
            alpha = std::max(alpha, bestScore);
            if (beta <= alpha) {break;};
        }

        return bestScore;
    }

    // black wants to play the best move
    else {
        int bestScore =  1000000;

        for (int i = 0; i < moves.size(); i++) {
            int tempBoard[8][8];

            // copying information
            for (int j = 0; j < 8; j++) {
                for (int k = 0; k < 8; k++) {
                    tempBoard[j][k] = board[j][k];
                }
            }

            PieceInformation tempWhiteInfo = WhitePieceInfo;
            PieceInformation tempBlackInfo = BlackPieceInfo;

            // simulating the move and looking for the best one
            makeMove(tempBoard, moves[i], tempWhiteInfo, tempBlackInfo);
            int score = minimax(tempBoard, depth - 1, alpha, beta, true, tempWhiteInfo, tempBlackInfo);
            bestScore = std::min(bestScore, score);
            alpha = std::min(beta, bestScore);
            if (beta <= alpha) {break;};
        }

        return bestScore;
    }
}

Move getBestBotMove(int board[8][8], int depth, PieceInformation WhitePieceInfo, PieceInformation BlackPieceInfo) {
    // bot can only play black
    std::vector <Move> moves = getAllLegalMoves(board, 0, WhitePieceInfo, BlackPieceInfo);
    Move bestMove = moves[0];
    int bestScore = 1000000; 

    for (int i = 0; i < moves.size(); i++) {
        int tempBoard[8][8];

        // copying information
        for (int j = 0; j < 8; j++) {
            for (int k = 0; k < 8; k++) {
                tempBoard[j][k] = board[j][k];
            }
        }

        PieceInformation tempWhiteInfo = WhitePieceInfo;
        PieceInformation tempBlackInfo = BlackPieceInfo;

        makeMove(tempBoard, moves[i], tempWhiteInfo, tempBlackInfo);

        int score = minimax(tempBoard, depth - 1, -1000000, 1000000, true, tempWhiteInfo, tempBlackInfo);

        if (score < bestScore) {
            bestScore = score;
            bestMove = moves[i];
        }
    }
    return bestMove;
}

int main () {

    // positive numbers = white pieces
    // negative numbers = black pieces
    // 0 = empty space
    int board [8][8] = {
        {-4,-2,-3,-5,-6,-3,-2,-4},
        {-1,-1,-1,-1,-1,-1,-1,-1},
        { 0, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0},
        { 1, 1, 1, 1, 1, 1, 1, 1},
        { 4, 2, 3, 5, 6, 3, 2, 4},
    };
    // 1 is white pieces and 0 is black pieces
    const int PLAYERSIDE = 1;
    const int BOTSIDE = 0;

    PieceInformation WhitePieceInfo;
    PieceInformation BlackPieceInfo;

    Move playerMove;
    Move botMove;

    std::vector <Move> legalMoves;
    bool legalMove;

    srand(time(NULL));

    printBoard(board);
    while (true) {

        // gets a players legal move
        do {
            playerMove = getPlayerMove(board);
            legalMoves = getLegalMoves(playerMove.fromSquare, board, PLAYERSIDE, WhitePieceInfo, BlackPieceInfo);
            legalMove = isPlayerMoveLegal(playerMove, legalMoves);
            if (legalMove == true) {
                makeMove(board, playerMove, WhitePieceInfo, BlackPieceInfo);
                printBoard(board);
                std::cout << "score: " << evaluateBoard(board) << '\n' << '\n';
            }
            else if (legalMove == false) {
                std::cout << "illegal move\n\n";
            }
        } while (legalMove == false);

        // checks for game end
        if (
            isSquareInCheck(board, BOTSIDE, findKing(board, BOTSIDE)) == true &&
            getAllLegalMoves(board, BOTSIDE, WhitePieceInfo, BlackPieceInfo).size() == 0
        ) {std::cout << "Checkmate, player won."; break;};
        if (
            isSquareInCheck(board, BOTSIDE, findKing(board, BOTSIDE)) == false &&
            getAllLegalMoves(board, BOTSIDE, WhitePieceInfo, BlackPieceInfo).size() == 0
        ) {std::cout << "Tie, it is a stalemate"; break;};

        // gets a random move from the bot
        legalMoves = getAllLegalMoves(board, BOTSIDE, WhitePieceInfo, BlackPieceInfo);
        botMove = getBestBotMove(board, 5, WhitePieceInfo, BlackPieceInfo);
        makeMove(board, botMove, WhitePieceInfo, BlackPieceInfo);
        std::cout << "Bot played from col: " << char(int('a') + botMove.fromSquare.col) << '\n';
        std::cout << "Bot played from row: " << 8 - botMove.fromSquare.row << '\n';
        std::cout << "Bot played to col: " << char(int('a') + botMove.toSquare.col)<< '\n';
        std::cout << "Bot played to row: " << 8 - botMove.toSquare.row << '\n' << '\n';

        printBoard(board);
        std::cout << "score: " << evaluateBoard(board) << '\n' << '\n';

        // checks for game end
        if (
            isSquareInCheck(board, PLAYERSIDE, findKing(board, PLAYERSIDE)) == true &&
            getAllLegalMoves(board, PLAYERSIDE, WhitePieceInfo, BlackPieceInfo).size() == 0
        ) {std::cout << "Checkmate, bot won."; break;};
        if (
            isSquareInCheck(board, PLAYERSIDE, findKing(board, PLAYERSIDE)) == false &&
            getAllLegalMoves(board, PLAYERSIDE, WhitePieceInfo, BlackPieceInfo).size() == 0
        ) {std::cout << "Tie, it is a stalemate"; break;};
    }

    return 0;
}