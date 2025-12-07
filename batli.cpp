#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <stdexcept>

using namespace std;

class Card {
private:
    char value;
    bool revealed;
    bool matched;
public:
    Card(char v = ' ') : value(v), revealed(false), matched(false) {}
    char getValue() const { return value; }
    bool isRevealed() const { return revealed; }
    bool isMatched() const { return matched; }

    void reveal() { if (!matched) revealed = true; }
    void hide() { if (!matched) revealed = false; }
    void setMatched() { matched = true; revealed = true; }
};

class Board {
private:
    int rows;
    int cols;
    vector<Card> cards; // row-major

    static vector<char> makeValues(int pairCount) {
        string pool = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                      "abcdefghijklmnopqrstuvwxyz"
                      "0123456789"
                      "!@#$%^&*";
        vector<char> vals;
        for (size_t i = 0; i < pool.size() && (int)vals.size() < pairCount; ++i)
            vals.push_back(pool[i]);
        // repeat if necessary (rare)
        for (size_t i = 0; (int)vals.size() < pairCount; ++i)
            vals.push_back(pool[i % pool.size()]);
        return vals;
    }

public:
    Board(int r = 4, int c = 4) : rows(r), cols(c) {
        if (rows <= 0 || cols <= 0 || (rows * cols) % 2 != 0)
            throw runtime_error("Board must have positive rows/cols and an even number of cells.");

        int pairs = (rows * cols) / 2;
        vector<char> values = makeValues(pairs);

        vector<char> deck;
        deck.reserve(rows * cols);
        for (int i = 0; i < pairs; ++i) {
            deck.push_back(values[i]);
            deck.push_back(values[i]);
        }

        random_device rd;
        mt19937 gen(rd());
        shuffle(deck.begin(), deck.end(), gen);

        cards.reserve(rows * cols);
        for (char ch : deck) cards.emplace_back(Card(ch));
    }

    int getRows() const { return rows; }
    int getCols() const { return cols; }
    int getSize() const { return rows * cols; }

    Card& at(int r, int c) {
        return cards[r * cols + c];
    }
    const Card& at(int r, int c) const {
        return cards[r * cols + c];
    }

    void revealAt(int r, int c) { at(r,c).reveal(); }
    void hideAt(int r, int c) { at(r,c).hide(); }
    void matchAt(int r, int c) { at(r,c).setMatched(); }

    bool allMatched() const {
        for (const Card& cd : cards) if (!cd.isMatched()) return false;
        return true;
    }

    void display(bool showCoords = true) const {
        cout << '\n';
        if (showCoords) {
            cout << "    ";
            for (int c = 0; c < cols; ++c) cout << setw(3) << c+1 << ' ';
            cout << '\n';
        }
        cout << "   +" << string(cols * 4, '-') << "+\n";
        for (int r = 0; r < rows; ++r) {
            if (showCoords) cout << setw(2) << r+1 << " |";
            else cout << "   |";
            for (int c = 0; c < cols; ++c) {
                const Card& cd = at(r,c);
                if (cd.isMatched()) cout << ' ' << cd.getValue() << " |";
                else if (cd.isRevealed()) cout << ' ' << cd.getValue() << " |";
                else cout << ' ' << '*' << " |";
            }
            cout << '\n';
            cout << "   +" << string(cols * 4, '-') << "+\n";
        }
        cout << '\n';
    }
};

class Game {
private:
    Board board;
    int moves;

    bool valid(int r, int c) const {
        return r >= 0 && r < board.getRows() && c >= 0 && c < board.getCols();
    }

    pair<int,int> readPos(const string& prompt) {
        while (true) {
            cout << prompt;
            string line;
            if (!getline(cin, line)) continue;
            stringstream ss(line);
            int r, c;
            if ( (ss >> r >> c) && valid(r-1, c-1) ) {
                return make_pair(r-1, c-1);
            }
            cout << "Invalid input. Use: <row> <col>  (e.g. 2 3)\n";
        }
    }

public:
    Game(int r = 4, int c = 4) : board(r, c), moves(0) {}

    void play() {
        cout << "Memory Puzzle (no timers, press Enter when asked)\n";
        cout << "Board: " << board.getRows() << "x" << board.getCols() << "\n";
        cout << "Choose cards by entering row and column numbers separated by space.\n";
        cout << "Press Enter to start...";
        string tmp; getline(cin, tmp);

        while (!board.allMatched()) {
            // show board
            board.display(true);
            cout << "Moves: " << moves << '\n';

            pair<int,int> p1 = readPos("Select first card (row col): ");
            int r1 = p1.first, c1 = p1.second;
            if (board.at(r1,c1).isMatched()) {
                cout << "That card is already matched. Choose another.\n";
                continue;
            }
            board.revealAt(r1,c1);
            board.display(true);

            pair<int,int> p2 = readPos("Select second card (row col): ");
            int r2 = p2.first, c2 = p2.second;
            if (r1 == r2 && c1 == c2) {
                cout << "You selected the same card twice. Try again.\n";
                board.hideAt(r1,c1);
                continue;
            }
            if (board.at(r2,c2).isMatched()) {
                cout << "Second card already matched. Try again.\n";
                board.hideAt(r1,c1);
                continue;
            }

            board.revealAt(r2,c2);
            board.display(true);

            moves++;

            if (board.at(r1,c1).getValue() == board.at(r2,c2).getValue()) {
                cout << "MATCH! (" << board.at(r1,c1).getValue() << ")\n";
                board.matchAt(r1,c1);
                board.matchAt(r2,c2);
            } else {
                cout << "Not a match.\n";
                cout << "Press Enter to continue and hide the two cards...";
                string s; getline(cin, s); // wait for user so they can see the values
                board.hideAt(r1,c1);
                board.hideAt(r2,c2);
            }
        }

        board.display(true);
        cout << "CONGRATULATIONS! All pairs matched.\n";
        cout << "Total moves: " << moves << "\n";
    }
};

int main() {
    try {
        cout << "Enter board size (rows cols) or press Enter for default 4 4:\n> ";
        string line;
        getline(cin, line);
        int r = 4, c = 4;
        if (!line.empty()) {
            stringstream ss(line);
            if (!(ss >> r >> c)) {
                cout << "Invalid input. Using default 4x4.\n";
                r = 4; c = 4;
            }
        }
        if (r <= 0 || c <= 0 || (r*c) % 2 != 0) {
            cout << "Invalid board dimensions. Using default 4x4.\n";
            r = 4; c = 4;
        }

        Game g(r, c);
        g.play();
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}
