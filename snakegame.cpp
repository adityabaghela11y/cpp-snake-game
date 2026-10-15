#include <iostream>
#include <string>
#include <conio.h>
#include <windows.h>

using namespace std;

bool gameOver;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;

enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;

void Setup() {
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    nTail = 0;
}

void Draw() {
    // Windows Terminal aur VS Code ke liye clean screen reset
    cout << "\033[H";

    string frame = "";

    // Top border
    for (int i = 0; i < width + 2; i++) frame += "#";
    frame += "\n";

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0) frame += "#";

            if (i == y && j == x) {
                frame += "O"; // Snake Head
            } else if (i == fruitY && j == fruitX) {
                frame += "*"; // Fruit
            } else {
                bool printTail = false;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        frame += "o"; // Tail
                        printTail = true;
                        break;
                    }
                }
                if (!printTail) frame += " ";
            }

            if (j == width - 1) frame += "#";
        }
        frame += "\n";
    }

    // Bottom border
    for (int i = 0; i < width + 2; i++) frame += "#";
    frame += "\n";

    frame += "Score: " + to_string(score) + "\n";
    frame += "Controls: W (Up), S (Down), A (Left), D (Right), X (Exit)\n";

    cout << frame;
}

void Input() {
    if (_kbhit()) {
        switch (_getch()) {
            case 'a':
            case 'A':
                if (dir != RIGHT) dir = LEFT;
                break;
            case 'd':
            case 'D':
                if (dir != LEFT) dir = RIGHT;
                break;
            case 'w':
            case 'W':
                if (dir != DOWN) dir = UP;
                break;
            case 's':
            case 'S':
                if (dir != UP) dir = DOWN;
                break;
            case 'x':
            case 'X':
                gameOver = true;
                break;
        }
    }
}

void Logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;

    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir) {
        case LEFT:  x--; break;
        case RIGHT: x++; break;
        case UP:    y--; break;
        case DOWN:  y++; break;
        default:    break;
    }

    // Wall hit collision
    if (x < 0 || x >= width || y < 0 || y >= height)
        gameOver = true;

    // Self collision
    for (int i = 0; i < nTail; i++) {
        if (tailX[i] == x && tailY[i] == y)
            gameOver = true;
    }

    // Fruit collection
    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;
    }
}


    int main() {
    char choice;
    do {
        system("cls");
        Setup();

        while (!gameOver) {
            Draw();
            Input();
            Logic();
            Sleep(60);
        }

        // Screen freeze & Game Over Card
        cout << "\n========================================" << endl;
        cout << "              GAME OVER!                " << endl;
        cout << "           Final Score: " << score        << endl;
        cout << "========================================" << endl;
        cout << "Press [Y] to Play Again or [N] to Exit: ";

        // Buffer clean karo (pichli keys skip karne ke liye)
        while (_kbhit()) {
            _getch();
        }

        // User ke decision ka wait karo
        do {
            choice = _getch();
        } while (choice != 'y' && choice != 'Y' && choice != 'n' && choice != 'N');

    } while (choice == 'y' || choice == 'Y');

    cout << "\n\nThanks for playing! Press any key to close...";
    _getch();
    return 0;
}
