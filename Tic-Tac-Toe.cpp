#include <iostream>

#define BOARD_SIZE 3
#define PLAYER_SYMBOL 'X'
#define AI_SYMBOL 'O'
#define EMPTY_SYMBOL ' '
#define MAX_DEPTH 9
#define ALPHA -1000
#define BETA 1000

char board[BOARD_SIZE][BOARD_SIZE];
char player;
char ai;

void initBoard() {
    for(int i = 0; i < BOARD_SIZE; i++) {
        for(int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = EMPTY_SYMBOL;
        }
    }
}

void printBoard() {
    for(int i = 0; i < BOARD_SIZE; i++) {
        std::cout << "-------" << std::endl;
        for(int j = 0; j < BOARD_SIZE; j++) {
            std::cout << "|" << board[i][j];
        }
        std::cout << "|" << std::endl;
    }
}

bool isMoveValid(int x, int y) {
    return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE && board[x][y] == EMPTY_SYMBOL;
}

bool isBoardFull() {
    for(int i = 0; i < BOARD_SIZE; i++) {
        for(int j = 0; j < BOARD_SIZE; j++) {
            if(board[i][j] == EMPTY_SYMBOL) {
                return false;
            }
        }
    }
    return true;
}

int evaluateBoard() {
    for(int i = 0; i < BOARD_SIZE; i++) {
        if(board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            if(board[i][0] == ai) {
                return 10;
            }
            else if(board[i][0] == player) {
                return -10;
            }
        }
    }

    for(int i = 0; i < BOARD_SIZE; i++) {
        if(board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            if(board[0][i] == ai) {
                return 10;
            }
            else if(board[0][i] == player) {
                return -10;
            }
        }
    }

    if(board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        if(board[0][0] == ai) {
            return 10;
        }
        else if(board[0][0] == player) {
            return -10;
        }
    }

    if(board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        if(board[0][2] == ai) {
            return 10;
        }
        else if(board[0][2] == player) {
            return -10;
        }
    }

    return 0;
}

int minimax(int depth, bool isMaximizing, int alpha, int beta) {
    int score = evaluateBoard();

    if(score == 10) {
        return score - depth;
    }

    if(score == -10) {
        return score + depth;
    }

    if(isBoardFull()) {
        return 0;
    }

    if(isMaximizing) {
        int best = ALPHA;
        for(int i = 0; i < BOARD_SIZE; i++) {
            for(int j = 0; j < BOARD_SIZE; j++) {
                if(board[i][j] == EMPTY_SYMBOL) {
                    board[i][j] = ai;
                    best = std::max(best, minimax(depth + 1, !isMaximizing, alpha, beta));
                    alpha = std::max(alpha, best);
                    board[i][j] = EMPTY_SYMBOL;
                    if(beta <= alpha) {
                        break;
                    }
                }
            }
        }
        return best;
    }
    else {
        int best = BETA;
        for(int i = 0; i < BOARD_SIZE; i++) {
            for(int j = 0; j < BOARD_SIZE; j++) {
                if(board[i][j] == EMPTY_SYMBOL) {
                    board[i][j] = player;
                    best = std::min(best, minimax(depth + 1, !isMaximizing, alpha, beta));
                    beta = std::min(beta, best);
                    board[i][j] = EMPTY_SYMBOL;
                    if(beta <= alpha) {
                        break;
                    }
                }
            }
        }
        return best;
    }
}

void makeMove() {
    int bestMove = -1;
    int bestScore = ALPHA;
    for(int i = 0; i < BOARD_SIZE; i++) {
        for(int j = 0; j < BOARD_SIZE; j++) {
            if(board[i][j] == EMPTY_SYMBOL) {
                board[i][j] = ai;
                int score = minimax(0, false, ALPHA, BETA);
                board[i][j] = EMPTY_SYMBOL;
                if(score > bestScore) {
                    bestScore = score;
                    bestMove = i * BOARD_SIZE + j;
                }
            }
        }
    }

    int x = bestMove / BOARD_SIZE;
    int y = bestMove % BOARD_SIZE;
    board[x][y] = ai;
}

void playGame() {
    std::cout << "Welcome to Tic-Tac-Toe!" << std::endl;
    std::cout << "You are playing as " << PLAYER_SYMBOL << " and the AI is playing as " << AI_SYMBOL << "." << std::endl;
    std::cout << "The board is empty. You can make a move by entering the row and column numbers." << std::endl;
    
    int turn = 0;
    std::cout << "Do you want to go first or second? Input 1 or 2: "; std::cin >> turn;
    std::cout << "Let's start the game!" << std::endl;

    initBoard();

    if(turn == 1) {
        printBoard();
    }
    
    player = PLAYER_SYMBOL;
    ai = AI_SYMBOL;

    while(true) {
        if(turn % 2 == 1) {
            std::cout << "Your turn. Enter the row and column numbers: ";
            int x, y;
            std::cin >> x >> y;
            if(isMoveValid(x, y)) {
                board[x][y] = player;
                printBoard();
                if(evaluateBoard() == -10) {
                    std::cout << "You win!" << std::endl;
                    break;
                }
            }
            else {
                std::cout << "Invalid move. Try again." << std::endl;
                continue;
            }
        } 
        else {
            std::cout << "AI's turn." << std::endl;
            makeMove();
            printBoard();
            if(evaluateBoard() == 10) {
                std::cout << "AI wins!" << std::endl;
                break;
            }
        }

        if(isBoardFull()) {
            std::cout << "It's a draw!" << std::endl;
            break;
        }

        turn++;
    }
}

int main() {
    playGame();
}