#include <iostream>
#include <ctime>
#include <conio.h>
#include <chrono>
#include <thread>

using namespace std;

bool gameOver;
enum snake_direction {NONE = 0, UP, DOWN, LEFT, RIGHT};
snake_direction direction;
char hborder = '-';
char vborder = '|';
char fruit = 'F';
int x;
int y;
int xf;
int yf;
int tailx[499];
int taily[499];
int length;
int winScore = 4990; //(fruits needed to win * 10) ----- MAX 4990
int prevx;
int prevy;
int score;
int high_score = 1;
bool replay;
char input;
bool win;
const int width = 25;
const int height = 20;

void MoveFruit() {
    bool valid_pos = true;
    do {
        xf = rand() % (width - 1);
        yf = rand() % (height - 1);
        if (xf == x && yf == y) {
            valid_pos = false;
        }
        for (int i = 0; i < length; ++i) {
            if (tailx[i] == xf && taily[i] == yf) {
                valid_pos = false;
            }
        }
    } while (!valid_pos);
}

void GameSetup() {
    score = 0;
    gameOver = false;
    x = (width / 2) + 1;
    y = (height / 2) + 1;
    MoveFruit();
    direction = NONE;
    length = 0;
    for (int i = 0; i < 499; ++i) {
        tailx[i] = 1000;
        taily[i] = 1000;
    }
}

void DrawGame() {

    system("clear");

    cout << vborder;
    for (int i = 0; i < width; ++i) {
        cout << hborder;
    }
    cout << vborder;
    cout << endl;

    for (int j = 0; j < height; ++j) {
        cout << vborder;
        for (int i = 0; i < width; ++i) {
            if (i == x && j == y) {
                cout << "0";
            }
            else if (i == xf && j == yf) {
                cout << fruit;
            }
            else {
                bool print = false;
                for (int k = 0; k < length; ++k) {
                    if (tailx[k] == i && taily[k] == j) {
                        cout << "o";
                        print = true;
                    }
                }
                if (!print) {
                    cout << ' ';
                }
            }
        }
        cout << vborder;
        cout << endl;
    }

    cout << vborder;
    for (int i = 0; i < width; ++i) {
        cout << hborder;
    }
    cout << vborder;
    cout << endl;
    cout << "Score: " << score << endl;
    if (high_score > 1) {
        cout << "High score: " << high_score << endl;
    }
    cout << "Press 'x' to end game" << endl;
}

void GetInput() {

    if(_kbhit()) {
        switch(getch()) {
            case 'w':
            direction = UP;
            break;
            case 'a':
            direction = LEFT;
            break;
            case 's':
            direction = DOWN;
            break;
            case 'd':
            direction = RIGHT;
            break;
            case 'x':
            gameOver = true;
            break;
        }
    }

}

void UpdateTail() {
    if (length == 1) {
        tailx[0] = prevx;
        taily[0] = prevy;
    }
    else if (length > 1) {
        for (int i = length - 1; i > 0; --i) {
            tailx[i] = tailx[i-1];
            taily[i] = taily[i-1];
        }
        tailx[0] = prevx;
        taily[0] = prevy;
    }
}

void CheckGame() {
    prevx = x;
    prevy = y;
    switch(direction) {
        case NONE:
        break;
        case UP:
        --y;
        break;
        case DOWN:
        ++y;
        break;
        case LEFT:
        --x;
        break;
        case RIGHT:
        ++x;
        break;
    }

    if (x == xf && y == yf) {
        score += 10;
        if (score == winScore) {
            gameOver = true;
            win = true;
        }
        else {
            ++length;
            MoveFruit();
        }
    }

    if (x < 0 || x > width - 1 || y < 0 || y > height - 1) {
        gameOver = true;
        win = false;
    }
    for (int i = 0; i < length; ++i) {
        if (tailx[i] == x && taily[i] == y) {
            gameOver = true;
            win = false;
        }
    }
    UpdateTail();
}

void EndGame() {
    system ("clear");
    if (win) {
        cout << endl;
        cout << "########################" << endl;
        cout << "#                      #" << endl;
        cout << "#       YOU WIN!       #" << endl;
        cout << "#                      #" << endl;
        cout << "########################" << endl;
        cout << endl;
    }
    else {
        cout << endl;
        cout << "#########################" << endl;
        cout << "#                       #" << endl;
        cout << "#       GAME OVER       #" << endl;
        cout << "#                       #" << endl;
        cout << "#########################" << endl;
        cout << endl;
        cout << "Score: " << score;
        if (score == high_score) {
            cout << " (High score!)" << endl;
        }
        else {
            cout << endl;
        }
    }
}

int main() {

    srand(time(0));

    do {
        GameSetup();
        while (!gameOver) {
            DrawGame();
            GetInput();
            CheckGame();
            std::this_thread::sleep_for(std::chrono::milliseconds(150));
        }
        if (score > high_score) {
            high_score = score;
        }
        EndGame();

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << endl << "Play again? (y/n): " << endl;
        cin >> input;
        if (input == 'y' || input == 'Y') {
            replay = true;
        }
        else {
            replay = false;
        }
    } while (replay);

    return 0;
}