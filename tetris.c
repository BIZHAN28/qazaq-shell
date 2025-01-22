#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <conio.h>  // Для функции getch() в Windows (замените на ncurses для Linux)

#define WIDTH 10
#define HEIGHT 20
#define EMPTY ' '
#define BLOCK '#'

char board[HEIGHT][WIDTH];  // Игровое поле

// Тетромино (форма блоков)
int tetrisShapes[7][4] = {
    {1, 3, 5, 7},  // I
    {2, 4, 5, 7},  // Z
    {3, 5, 4, 6},  // S
    {3, 5, 4, 7},  // T
    {2, 4, 5, 6},  // O
    {3, 5, 4, 6},  // L
    {3, 5, 4, 6}   // J
};

// Функции для работы с игровым полем
void initializeBoard() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            board[y][x] = EMPTY;
        }
    }
}

void printBoard() {
    system("clear"); // Для Windows используйте system("cls");
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            printf("%c ", board[y][x]);
        }
        printf("\n");
    }
}

int checkCollision(int shape[], int xOffset, int yOffset) {
    for (int i = 0; i < 4; i++) {
        int x = shape[i] % 2 + xOffset;  // Определение координаты X
        int y = shape[i] / 2 + yOffset;  // Определение координаты Y

        if (x < 0 || x >= WIDTH || y >= HEIGHT || board[y][x] != EMPTY) {
            return 1;  // Есть коллизия
        }
    }
    return 0;  // Нет коллизии
}

void rotateShape(int shape[]) {
    // Поворот формы (реализуем поворот тетромино с 90 градусов)
    int temp[4];
    for (int i = 0; i < 4; i++) {
        temp[i] = shape[i];
    }
    for (int i = 0; i < 4; i++) {
        shape[i] = temp[(i + 1) % 4];
    }
}

void placeShape(int shape[], int xOffset, int yOffset) {
    for (int i = 0; i < 4; i++) {
        int x = shape[i] % 2 + xOffset;
        int y = shape[i] / 2 + yOffset;
        board[y][x] = BLOCK;
    }
}

int main() {
    srand(time(NULL));
    initializeBoard();
    int shape[4];
    int xOffset = WIDTH / 2 - 1;
    int yOffset = 0;

    // Играем в Тетрис до тех пор, пока не будет коллизии
    while (1) {
        // Выбор случайной фигуры
        int randomShape = rand() % 7;
        for (int i = 0; i < 4; i++) {
            shape[i] = tetrisShapes[randomShape][i];
        }

        // Выводим игровое поле
        printBoard();

        // Ожидаем ввода
        char key = getch();  // Используйте getch() в Windows, в Linux используйте библиотеку ncurses
        if (key == 'q') {
            break;  // Выход из игры
        }

        // Движение и повороты
        if (key == 'a') {
            // Влево
            if (!checkCollision(shape, xOffset - 1, yOffset)) {
                xOffset--;
            }
        } else if (key == 'd') {
            // Вправо
            if (!checkCollision(shape, xOffset + 1, yOffset)) {
                xOffset++;
            }
        } else if (key == 's') {
            // Низ
            if (!checkCollision(shape, xOffset, yOffset + 1)) {
                yOffset++;
            }
        } else if (key == 'w') {
            // Поворот
            rotateShape(shape);
            if (checkCollision(shape, xOffset, yOffset)) {
                rotateShape(shape); // Возвращаем в исходное положение
                rotateShape(shape); // Два поворота, чтобы вернуться
                rotateShape(shape);
            }
        }

        // Если фигура не может двигаться вниз, она фиксируется на поле
        if (checkCollision(shape, xOffset, yOffset + 1)) {
            placeShape(shape, xOffset, yOffset);
            yOffset = 0;
            xOffset = WIDTH / 2 - 1;
        } else {
            yOffset++;
        }

        // Ожидаем 1 секунду перед обновлением экрана
        usleep(100000);
    }

    return 0;
}
