#include <iostream>
#include <vector>
#include <stdlib.h>

class TikTakToe {
public:
    std::string name1, name2;

    void move(int x, int y) {
        turn((y - 1) * 3 + x);
    }

private:
    int board[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    bool player = 0;

    void turn(int id) {
        if (win_check() == 0) {
            // play
            if (win_check() != 0) on_win();
        } else {
            on_win();
        }

    }

    int win_check() {}

    void on_win() {}
};

std::vector<TikTakToe> create_boards(int players_amount) {
    std::vector<TikTakToe> boards;
    for (int i = 0; i < players_amount;) {
        for (int j = i + 1; j < players_amount;) {
            boards[i].name1 = std::to_string(i);
            boards[i].name2 = std::to_string(i);

        }
    }
};

using namespace std;

int main() {
    vector<vector<string>> board;

    return 0;
}
