#include <vector>
#include <stdlib.h>
#include <string>
#include <map>
#include <random>
#include <iostream>

using namespace std;


bool destination_is_on_board(int x1, int delta_x, int delta_y) {
    return ((0 <= (x1 + delta_x + delta_y * 8)) && ((x1 + delta_x + delta_y * 8) < 64) && (0 <= (x1 % 8 + delta_x)) &&
            ((x1 % 8 + delta_x) < 8));
}

#include <vector>
#include <algorithm>
#include <random>

using namespace std;

class Chess {
    vector<int> start_board;
    long int gj = INFINITY;
    int total_m = 3;
    vector<int> bestmoves;
    int P, B, N, R, wP, wB, wN, wR;

public:
    vector<int> board = vector<int>(64, 0);

    Chess(int P1, int B1, int N1, int R1, int wP1, int wB1, int wN1, int wR1) : P(P1), B(B1), N(N1), R(R1), wP(wP1),
                                                                                wB(wB1), wN(wN1),
                                                                                wR(wR1) { this->createBoard(); }

    void createBoard() {
        for (int i = 0; i < wP; ++i) {
            auto it = find(board.begin(), board.end(), 0);
            if (it != board.end()) {
                *it = 1;
            }
        }

        for (int i = 0; i < wB; ++i) {
            auto it = find(board.begin(), board.end(), 0);
            if (it != board.end()) {
                *it = 2;
            }
        }

        for (int i = 0; i < wR; ++i) {
            auto it = find(board.begin(), board.end(), 0);
            if (it != board.end()) {
                *it = 3;
            }
        }

        for (int i = 0; i < wN; ++i) {
            auto it = find(board.begin(), board.end(), 0);
            if (it != board.end()) {
                *it = 4;
            }
        }

        auto it = find(board.begin(), board.end(), 0);
        if (it != board.end()) {
            *it = 10;
        }

        for (int i = 0; i < P; ++i) {
            auto it = find(board.begin(), board.end(), 0);
            if (it != board.end()) {
                *it = 100;
            }
        }

        for (int i = 0; i < B; ++i) {
            auto it = find(board.begin(), board.end(), 0);
            if (it != board.end()) {
                *it = 301;
            }
        }

        for (int i = 0; i < R; ++i) {
            auto it = find(board.begin(), board.end(), 0);
            if (it != board.end()) {
                *it = 500;
            }
        }

        for (int i = 0; i < N; ++i) {
            auto it = find(board.begin(), board.end(), 0);
            if (it != board.end()) {
                *it = 300;
            }
        }

        it = find(board.begin(), board.end(), 0);
        if (it != board.end()) {
            *it = 100000000;
        }

        random_device rd;
        mt19937 g(rd());
        shuffle(board.begin(), board.end(), g);

        start_board = board;
        gj = accumulate((board).begin(), (board).end(), 0);
    }

    void printChessboard() {
        cout << "  a b c d e f g h" << endl;
        cout << "  ---------------" << endl;

        for (int row = 0; row < 8; ++row) {
            cout << 8 - row << "|";
            for (int col = 0; col < 8; ++col) {
                int index = row * 8 + col;
                int piece = board[index];

                char symbol = ' ';

                switch (piece) {
                    case 1:
                        symbol = 'P';
                        break;
                    case 2:
                        symbol = 'B';
                        break;
                    case 3:
                        symbol = 'R';
                        break;
                    case 4:
                        symbol = 'N';
                        break;
                    case 10:
                        symbol = 'K';
                        break;
                    case 100000000:
                        symbol = 'k';
                        break;
                    case 100:
                        symbol = 'p';
                        break;
                    case 301:
                        symbol = 'b';
                        break;
                    case 500:
                        symbol = 'r';
                        break;
                    case 300:
                        symbol = 'n';
                        break;
                }

                cout << symbol << '|';
            }
            cout << " " << 8 - row << endl;
        }

        cout << "  ---------------" << endl;
        cout << "  a b c d e f g h" << endl;
    }


    void domove(int move) {
        board[move % 100] = board[move / 100];
        board[move / 100] = 0;
    }


    void printWinner() {
        /*for (int element: bestmoves)
            domove(element);
        while (accumulate((board).begin(), (board).end(), 0) > 100000000){
            cout << accumulate((board).begin(), (board).end(), 0) << "||" << gj << endl;
            this->board = vector<int>(64, 0);
            createBoard(0, 0, 0, 1);
            vector<int> bbb1;
            this->gj = INFINITY;
            findMoves(3, bbb1, &this->board);
            if (gj < 0){
                cout << "HEEHHE";
                break;
            }
        }
        board = start_board;*/
        printChessboard();
        for (int element: bestmoves) {
            domove(element);
            cout << indexToChessCoordinates(element / 100).first << indexToChessCoordinates(element / 100).second
                 << "-->" << indexToChessCoordinates(element % 100).first
                 << indexToChessCoordinates(element % 100).second << " " << element << endl;
            printChessboard();
        }
        board = start_board;
        printChessboard();

    }

    std::pair<char, int> indexToChessCoordinates(int index) {

        if (index < 0 || index >= 64) {
            throw std::out_of_range("Недопустимый индекс");
        }


        char letter = 'a' + (index % 8);


        int number = 8 - (index / 8);

        return std::make_pair(letter, number);
    }

    vector<vector<int>> bestMoveSequence;


    void findMoves(int remainingMoves, vector<int> hist, vector<int> b) {
        int totalValue = accumulate(b.begin(), b.end(), 0);
        if (totalValue < 1000000 && gj > totalValue) {
            gj = -totalValue / pow(10, remainingMoves);
            bestMoveSequence = {hist};
            return;
        } else if (totalValue < gj) {
            gj = totalValue;
            bestMoveSequence = {hist};
        }
        //cout << remainingMoves;
        if (remainingMoves == 0) return;
        for (int i = 0; i < 64; i++) {
            if (b[i] == 1) {
                Pawn(i, remainingMoves, hist, b);
            } else if (b[i] == 2) {
                Bishop(i, remainingMoves, hist, b);
            } else if (b[i] == 3) {
                Rook(i, remainingMoves, hist, b);
            } else if (b[i] == 4) {
                Knight(i, remainingMoves, hist, b);
            } else if (b[i] == 10) {
                King(i, remainingMoves, hist, b);
            }
        }
    }


    void printBestMoveSequence() {
        printChessboard();
        for (const vector<int> &move: bestMoveSequence) {
            for (int element: move) {
                domove(element);
                cout << indexToChessCoordinates(element / 100).first
                     << indexToChessCoordinates(element / 100).second
                     << "-->" << indexToChessCoordinates(element % 100).first
                     << indexToChessCoordinates(element % 100).second << " " << element << endl;
                printChessboard();
            }
            cout << "------------" << endl;
            board = start_board;
        }
    }

    void Rook(int cord, int move, vector<int> hist, vector<int> b) {
        int dest = 0;
        for (int i = cord + 1; destination_is_on_board(i - 1, 1, 0); i++) {
            if ((b)[i] > 99 or (b)[i] == 0) {
                vector<int> hist1 = hist;
                hist1.push_back(cord * 100 + i);
                vector<int> b1 = b;
                b1[i] = b1[cord];
                b1[cord] = 0;
                findMoves(move - 1, hist1, b1);
            }
            if (b[i] != 0) break;
        }
        for (int i = cord + 8; destination_is_on_board(i - 8, 0, 1); i += 8) {
            if ((b)[i] > 99 or (b)[i] == 0) {
                vector<int> hist1 = hist;
                hist1.push_back(cord * 100 + i);
                vector<int> b1 = b;
                b1[i] = b1[cord];
                b1[cord] = 0;
                findMoves(move - 1, hist1, b1);
            }
            if (b[i] != 0) break;
        }
        for (int i = cord - 8; destination_is_on_board(i + 8, 0, -1); i -= 8) {
            if ((b)[i] > 99 or (b)[i] == 0) {
                vector<int> hist1 = hist;
                hist1.push_back(cord * 100 + i);
                vector<int> b1 = b;
                b1[i] = b1[cord];
                b1[cord] = 0;
                findMoves(move - 1, hist1, b1);
            }
            if (b[i] != 0) break;
        }
        for (int i = cord - 1; destination_is_on_board(i + 1, -1, 0); i--) {
            if ((b)[i] > 99 or (b)[i] == 0) {
                vector<int> hist1 = hist;
                hist1.push_back(cord * 100 + i);
                vector<int> b1 = b;
                b1[i] = b1[cord];
                b1[cord] = 0;
                findMoves(move - 1, hist1, b1);
            }
            if (b[i] != 0) break;
        }
    }

    void Bishop(int cord, int move, vector<int> hist, vector<int> b) {
        int dest = 0;
        for (int i = cord + 9; destination_is_on_board(i - 9, 1, 1); i += 9) {
            if ((b)[i] > 99 or (b)[i] == 0) {
                vector<int> hist1 = hist;
                hist1.push_back(cord * 100 + i);
                vector<int> b1 = b;
                b1[i] = b1[cord];
                b1[cord] = 0;
                findMoves(move - 1, hist1, b1);
            }
            if (b[i] != 0) break;
        }
        for (int i = cord - 7; destination_is_on_board(i + 7, 1, -1); i -= 7) {
            if ((b)[i] > 99 or (b)[i] == 0) {

                vector<int> hist1 = hist;
                hist1.push_back(cord * 100 + i);
                vector<int> b1 = b;
                b1[i] = b1[cord];
                b1[cord] = 0;
                findMoves(move - 1, hist1, b1);
            }
            if (b[i] != 0) break;

        }
        for (int i = cord - 9; destination_is_on_board(i + 9, -1, -1); i -= 9) {
            if ((b)[i] > 99 or (b)[i] == 0) {
                vector<int> hist1 = hist;
                hist1.push_back(cord * 100 + i);
                vector<int> b1 = b;
                b1[i] = b1[cord];
                b1[cord] = 0;
                findMoves(move - 1, hist1, b1);
            }
            if (b[i] != 0) break;
        }
        for (int i = cord + 7; destination_is_on_board(i - 7, -1, 1); i += 7) {
            if ((b)[i] > 99 or (b)[i] == 0) {
                vector<int> hist1 = hist;
                hist1.push_back(cord * 100 + i);
                vector<int> b1 = b;
                b1[i] = b1[cord];
                b1[cord] = 0;
                findMoves(move - 1, hist1, b1);
            }
            if (b[i] != 0) break;
        }

    }

    void Pawn(int cord, int move, vector<int> hist, vector<int> b) {
        if (destination_is_on_board(cord, 0, 1)) {
            if ((b)[cord + 8] == 0) {
                int i = cord + 8;
                vector<int> hist1 = hist;
                hist1.push_back(cord * 100 + i);
                vector<int> b1 = b;
                b1[i] = b1[cord];
                b1[cord] = 0;
                findMoves(move - 1, hist1, b1);
            }

        }
        if (destination_is_on_board(cord, 1, 1)) {
            if ((b)[cord + 9] > 99) {
                int i = cord + 9;

                vector<int> hist1 = hist;
                hist.push_back(cord * 100 + i);
                vector<int> b1 = b;
                b1[i] = b1[cord];
                b1[cord] = 0;
                findMoves(move - 1, hist1, b1);
            }

        }
        if (destination_is_on_board(cord, -1, 1)) {
            if ((b)[cord + 7] > 99) {
                int i = cord + 7;

                vector<int> hist1 = hist;
                hist1.push_back(cord * 100 + i);
                vector<int> b1 = b;
                b1[i] = b1[cord];
                b1[cord] = 0;
                findMoves(move - 1, hist1, b1);
            }

        }
    }

    void Knight(int cord, int move, vector<int> hist, vector<int> b) {
        int dest[] = {1, 2, 2, 1, 2, -1, 1, -2, -1, -2, -2, -1, -2, 1, -1, 2};
        for (int j = 0; j < 16; j += 2) {
            if (destination_is_on_board(cord, dest[j], dest[j + 1])) {
                if ((b)[cord + dest[j] + dest[j + 1] * 8] > 99 or (b)[cord + dest[j] + dest[j + 1] * 8] == 0) {
                    int i = cord + dest[j] + dest[j + 1] * 8;
                    vector<int> hist1 = hist;
                    hist1.push_back(cord * 100 + i);
                    vector<int> b1 = b;
                    b1[i] = b1[cord];
                    b1[cord] = 0;
                    findMoves(move - 1, hist1, b1);
                }
            }
        }
    }

    void King(int cord, int move, vector<int> hist, vector<int> b) {
        int dest[] = {-1, -1, -1, 0, -1, 1, 0, -1, 0, 1, 1, -1, 1, 0, 1, 1};
        for (int j = 0; j < 16; j += 2) {
            if (destination_is_on_board(cord, dest[j], dest[j + 1])) {
                if ((b[cord + dest[j] + dest[j + 1] * 8] > 99) or (b[cord + dest[j] + dest[j + 1] * 8] == 0)) {
                    int i = cord + dest[j] + dest[j + 1] * 8;
                    vector<int> hist1 = hist;
                    hist1.push_back(cord * 100 + i);
                    vector<int> b1 = b;
                    b1[i] = b1[cord];
                    b1[cord] = 0;
                    findMoves(move - 1, hist1, b1);
                }
            }
        }
    }

};

int main() {
    Chess party(0, 0, 0, 1, 0, 0, 0, 1);
    vector<int> bbb;
    party.printChessboard();
    party.findMoves(3, bbb, party.board);
    party.printBestMoveSequence();
    return 0;
}
