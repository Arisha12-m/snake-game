// Snake Game in C++
// A console-based Snake Game for Windows, featuring colored console output,
// pause/resume, invalid key handling, and a play-again loop.
//
// Course: Programming Fundamentals
// Submitted to: Maam Sidra Ejaz
//
// Submitted by:
// Dua Imran     - FA25B1-SE-028
// Arisha Maryam - FA25B1-SE-036
// Zoya Rashad   - FA25B1-SE-050

#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <cctype>
using namespace std;

const int W = 50, H = 18;

void gotoxy(int x, int y) {
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int main() {
    srand(time(0));
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE); // For colors

    // Hide cursor
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

    int x, y;
    int fx, fy;
    int tailX[200], tailY[200];
    int len;
    char dir;
    bool over;
    bool paused; // Pause state
    char f = '*';

    while (true) { // Play again loop

        // Initialize/reset variables inside the loop
        x = W / 2;
        y = H / 2;
        fx = rand() % W;
        fy = rand() % H;
        len = 0;
        dir = ' ';
        over = false;
        paused = false; // Reset pause state

        while (!over) {
            // Move cursor to top-left instead of clearing screen
            gotoxy(0, 0);

            // Instructions + Score
            cout << "WELCOME TO SNAKE GAME!\n";
            cout << "MOVE:\n";
            cout << "  Up    = W    Down = S\n";
            cout << "  Left  = A    Right = D\n";
            cout << "  Pause/Resume = P   Quit = Q\n";
            cout << "SCORE: " << len << endl;
            cout << "Touch wall or tail = GAME OVER!\n\n";

            // Draw board
            for (int i = 0; i <= H + 1; i++) {
                for (int j = 0; j <= W + 1; j++) {
                    if (i == 0 || i == H + 1 || j == 0 || j == W + 1) {
                        SetConsoleTextAttribute(h, 14); // Yellow walls
                        cout << "#";
                        SetConsoleTextAttribute(h, 7);
                    } else if (i == y + 1 && j == x + 1) {
                        SetConsoleTextAttribute(h, 10); // Green head
                        cout << "O";
                        SetConsoleTextAttribute(h, 7);
                    } else if (i == fy + 1 && j == fx + 1) {
                        SetConsoleTextAttribute(h, 12); // Red fruit
                        cout << f;
                        SetConsoleTextAttribute(h, 7);
                    } else {
                        bool p = false;
                        for (int t = 0; t < len; t++) {
                            if (tailX[t] + 1 == j && tailY[t] + 1 == i) {
                                SetConsoleTextAttribute(h, 11); // Cyan tail
                                cout << "o";
                                SetConsoleTextAttribute(h, 7);
                                p = true;
                                break;
                            }
                        }
                        if (!p) cout << " ";
                    }
                }
                cout << endl;
            }

            // Input handling with invalid key check
            if (_kbhit()) {
                bool validKey = false;
                while (!validKey && _kbhit()) {
                    char c = tolower(_getch());
                    if (c == 'w' && dir != 'D') { dir = 'U'; validKey = true; }
                    else if (c == 's' && dir != 'U') { dir = 'D'; validKey = true; }
                    else if (c == 'a' && dir != 'R') { dir = 'L'; validKey = true; }
                    else if (c == 'd' && dir != 'L') { dir = 'R'; validKey = true; }
                    else if (c == 'p') {
                        paused = !paused; // 'P' is Pause/Resume
                        validKey = true;
                    } else if (c == 'q') {
                        over = true;
                        validKey = true; // 'Q' is Quit
                    } else {
                        cout << "\aInvalid key! Press W/A/S/D/P/Q.\n";
                    }
                }
            }

            // Pause check
            if (!paused) {
                // Move tail
                for (int i = len - 1; i > 0; i--) {
                    tailX[i] = tailX[i - 1];
                    tailY[i] = tailY[i - 1];
                }
                if (len > 0) { tailX[0] = x; tailY[0] = y; }

                // Move head
                if (dir == 'U') y--;
                else if (dir == 'D') y++;
                else if (dir == 'L') x--;
                else if (dir == 'R') x++;

                // Collision with wall
                if (x < 0 || x >= W || y < 0 || y >= H) { over = true; cout << "\a"; }

                // Collision with tail
                for (int i = 0; i < len; i++)
                    if (tailX[i] == x && tailY[i] == y) { over = true; cout << "\a"; }

                // Eat fruit
                if (x == fx && y == fy) {
                    len++;
                    fx = rand() % W;
                    fy = rand() % H;
                }
            }

            Sleep(60); // Speed of the movement
        }

        // Game over message
        gotoxy(0, H + 5);
        cout << "\n GAME OVER! Score: " << len << endl;

        char ch;
        do {
            cout << " Play again? (Y/N): ";
            cin >> ch;
            ch = tolower(ch);
            cin.ignore(256, '\n'); // Crash/loop fix
        } while (ch != 'y' && ch != 'n');

        if (ch == 'n') break;
    }

    return 0;
}
