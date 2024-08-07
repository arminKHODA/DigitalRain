#include <windows.h>
#include <iostream>
#include <vector>
#include <time.h>
#include <process.h>

enum COLORS {
    BLACK       = 0,
    DARK_BLUE   = 1,
    DARK_GREEN  = 2,
    TEAL        = 3,
    DARK_RED    = 4,
    DARK_PURPLE = 5,
    GOLD        = 6,
    GREY        = 7,
    DARK_WHITE  = 8,
    BLUE        = 9,
    GREEN       = 10,
    CYAN        = 11,
    RED         = 12,
    PURPLE      = 13,
    YELLOW      = 14,
    WHITE       = 15
};

void RemoveCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO CursoInfo;
    CursoInfo.dwSize = 1;
    CursoInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &CursoInfo);
}

void SetColor(const int foreground) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, foreground);
}

void PlaceCursor(const int x, const int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD PlaceCursorHere;
    PlaceCursorHere.X = x;
    PlaceCursorHere.Y = y;
    SetConsoleCursorPosition(hConsole, PlaceCursorHere);
}

class Matrix {
private:
    bool isDefault_;
    bool erase_;
    unsigned __int32 loopWhite_;
    unsigned __int32 loopLgreen_;
    unsigned __int32 loopDgreen_;
    unsigned __int16 speed_[3];
    unsigned __int8 length_;
    unsigned __int8 lenCnt_;
    COORD position_;
    SMALL_RECT matrixBox_;

public:
    Matrix();
    Matrix(const SMALL_RECT);
    Matrix(const unsigned __int8, const unsigned __int8, const unsigned __int8, const unsigned __int8);

    void display();

    void setErase(const bool);
    bool getErase();

    void setSpeed(const unsigned __int16, const unsigned __int16, const unsigned __int16);
    unsigned __int16 getSpeed(const unsigned __int8);

private:
    void randLength();
    void randSpeed();
    void randPos();

    void setDefault(const bool);
    bool getDefault();

    void setLength(const unsigned __int8);
    unsigned __int8 getLength();

    void setPosition(const COORD);
    void setPosition(const unsigned __int8, const unsigned __int8);
    COORD getPosition();

    void setMatrixBox(const SMALL_RECT);
    void setMatrixBox(const unsigned __int8, const unsigned __int8, const unsigned __int8, const unsigned __int8);
    SMALL_RECT getMatrixBox();
};

using std::cout;

Matrix::Matrix() {
    srand(static_cast<unsigned int>(_getpid()) ^ static_cast<unsigned int>(clock()) ^ static_cast<unsigned int>(time(NULL)));
    setDefault(true);
    setErase(false);
    setPosition((rand() % 80 + 0), (rand() % 22 + 0));
    setLength((rand() % (getPosition().Y + 1) + 0));
    setSpeed((rand() % 50 + 50), (rand() % 100 + 100), (rand() % 150 + 150));
    loopWhite_ = GetTickCount();
    loopLgreen_ = GetTickCount();
    loopDgreen_ = GetTickCount();
    lenCnt_ = 0;
}

Matrix::Matrix(const SMALL_RECT box) {
    srand(static_cast<unsigned int>(_getpid()) ^ static_cast<unsigned int>(clock()) ^ static_cast<unsigned int>(time(NULL)));
    setMatrixBox(box);
    setDefault(false);
    setErase(false);
    setPosition((rand() % getMatrixBox().Right + getMatrixBox().Left), (rand() % getMatrixBox().Bottom + getMatrixBox().Top));
    setLength((rand() % (getPosition().Y + 1) + 0));
    setSpeed((rand() % 50 + 50), (rand() % 100 + 100), (rand() % 150 + 150));
    loopWhite_ = GetTickCount();
    loopLgreen_ = GetTickCount();
    loopDgreen_ = GetTickCount();
    lenCnt_ = 0;
}

Matrix::Matrix(const unsigned __int8 left, const unsigned __int8 top, const unsigned __int8 bottom, const unsigned __int8 right) {
    srand(static_cast<unsigned int>(_getpid()) ^ static_cast<unsigned int>(clock()) ^ static_cast<unsigned int>(time(NULL)));
    setMatrixBox(top, bottom, left, right);
    setDefault(false);
    setErase(false);
    setPosition((rand() % getMatrixBox().Right + getMatrixBox().Left), (rand() % getMatrixBox().Bottom + getMatrixBox().Top));
    setLength((rand() % (getPosition().Y + 1) + 0));
    setSpeed((rand() % 50 + 50), (rand() % 100 + 100), (rand() % 150 + 150));
    loopWhite_ = GetTickCount();
    loopLgreen_ = GetTickCount();
    loopDgreen_ = GetTickCount();
    lenCnt_ = 0;
}

void Matrix::randLength() {
    setLength((rand() % (getPosition().Y + 1) + 5));
}

void Matrix::randSpeed() {
    setSpeed((rand() % 50 + 50), (rand() % 100 + 100), (rand() % 150 + 150));
}

void Matrix::randPos() {
    if (getDefault()) {
        setPosition((rand() % 80 + 0), (rand() % 22 + 0));
    } else {
        setPosition((rand() % getMatrixBox().Right + getMatrixBox().Left), (rand() % getMatrixBox().Bottom + getMatrixBox().Top));
    }
}

void Matrix::display() {
    unsigned int whiteDelay = getSpeed(0);
    unsigned int greenDelay = getSpeed(1);
    unsigned int darkGreenDelay = getSpeed(2);

    if (GetTickCount() >= (loopWhite_ + whiteDelay)) {
        loopWhite_ = GetTickCount();

        PlaceCursor(getPosition().X, getPosition().Y);
        if (!getErase()) { SetColor(WHITE); }
        else { SetColor(BLACK); }
        cout << (char)(rand() % 121 + 34);

        if (GetTickCount() >= (loopLgreen_ + greenDelay)) {
            loopLgreen_ = GetTickCount();

            PlaceCursor(getPosition().X, getPosition().Y);
            if (!getErase()) { SetColor(GREEN); }
            else { SetColor(BLACK); }
            cout << (char)(rand() % 121 + 34);

            if (GetTickCount() >= (loopDgreen_ + darkGreenDelay)) {
                loopDgreen_ = GetTickCount();

                PlaceCursor(getPosition().X, getPosition().Y);
                if (rand() % 2) {
                    if (!getErase()) { SetColor(DARK_GREEN); }
                    else { SetColor(BLACK); }
                }
                cout << (char)(rand() % 121 + 34);

                setPosition(getPosition().X, (getPosition().Y + 1));

                if (lenCnt_ >= getLength()) {
                    randPos();
                    randSpeed();
                    randLength();
                    lenCnt_ = 0;
                } else {
                    lenCnt_++;
                }
            }
        }
    }
}

void Matrix::setDefault(const bool setting) {
    isDefault_ = setting;
}

bool Matrix::getDefault() {
    return isDefault_;
}

void Matrix::setErase(const bool setting) {
    erase_ = setting;
}

bool Matrix::getErase() {
    return erase_;
}

void Matrix::setSpeed(const unsigned __int16 white, const unsigned __int16 light_green, const unsigned __int16 dark_green) {
    speed_[0] = white;
    speed_[1] = light_green;
    speed_[2] = dark_green;
}

unsigned __int16 Matrix::getSpeed(const unsigned __int8 color) {
    return speed_[color];
}

void Matrix::setLength(const unsigned __int8 length) {
    length_ = length;
}

unsigned __int8 Matrix::getLength() {
    return length_;
}

void Matrix::setPosition(const COORD position) {
    position_.X = position.X;
    position_.Y = position.Y;
}

void Matrix::setPosition(const unsigned __int8 X, const unsigned __int8 Y) {
    position_.X = X;
    position_.Y = Y;
}

COORD Matrix::getPosition() {
    return position_;
}

void Matrix::setMatrixBox(const SMALL_RECT matrixBox) {
    matrixBox_.Top = matrixBox.Top;
    matrixBox_.Bottom = matrixBox.Bottom;
    matrixBox_.Left = matrixBox.Left;
    matrixBox_.Right = matrixBox.Right;
}

void Matrix::setMatrixBox(const unsigned __int8 top, const unsigned __int8 bottom, const unsigned __int8 left, const unsigned __int8 right) {
    matrixBox_.Top = top;
    matrixBox_.Bottom = bottom;
    matrixBox_.Left = left;
    matrixBox_.Right = right;
}

SMALL_RECT Matrix::getMatrixBox() {
    return matrixBox_;
}

int main() {
    RemoveCursor();

    #if (_WIN32_WINNT == _WIN32_WINNT_WINXP)
        if (!SetConsoleTitle("Matrix - Win32 Console")) {
            std::cout << "SetConsoleTitle returned an error: " << GetLastError();
        }

        SMALL_RECT windowSize = { 0, 0, 79, 49 };
        if (!SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &windowSize)) {
            std::cout << "SetConsoleWindowInfo returned an error: " << GetLastError();
        }
    #else
        ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
    #endif

    std::vector<Matrix> matrix;
    for (int y = 0; y < 15; y++) {
        matrix.push_back(Matrix());
    }

    for (int z = 0; z < 5; z++) {
        matrix.push_back(Matrix()); matrix.at(z).setErase(true);
    }

    while (1) {
        Sleep(1);
        for (int x = 0; x < matrix.size(); x++) {
            matrix.at(x).display();
        }
    }

    return 0;
}
