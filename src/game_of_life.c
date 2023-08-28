#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>

#define HEIGHT 25
#define WIDTH 80

void cellCon(int arr[][WIDTH], int state[][WIDTH]);
void addNeb(int arr[][WIDTH]);
void printField(int A[][WIDTH]);
void reField(int A[][WIDTH]);
int inputData(int A[][WIDTH]);
void delAddCon(int arr[][WIDTH], int state[][WIDTH]);

int main() {
    int arr[HEIGHT][WIDTH] = {0};

    //ввели коорбинаты
    if (inputData(arr) != -1) {
        int state[HEIGHT][WIDTH] = {0};
        int flag = 1;
        int speed = 10000;
        initscr();
        noecho();
        curs_set(0);

        nodelay(stdscr, TRUE);

        while (flag) {
            //отрисовка поля
            clear();
            printField(arr);
            refresh();
            //прибовление единичек к сосдям

            addNeb(arr);

            cellCon(arr, state);

            delAddCon(arr, state);

            reField(arr);

            if (freopen("/dev/tty", "r", stdin)) {
                char ch = getch();
                if (ch == 'q' || ch == 'Q') {
                    flag = 0;
                } else if (ch == '=') {
                    if (speed - 25000 >= 25000) {
                        speed -= 25000;
                    }
                } else if (ch == '-') {
                    if (speed < 150000) {
                        speed += 25000;
                    }
                }
            }

            usleep(speed);
            // break;
        }

        echo();
        endwin();
    } else {
        printf("n/a");
    }
    return 0;
}

//функция для подсчёта клеток  клеток при переначелении
void cellCon(int arr[][WIDTH], int state[][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (arr[i][j] >= 1000) {
                int count = 0;
                for (int t1 = -1; t1 < 1 + 1; t1++) {
                    for (int t2 = -1; t2 < 1 + 1; t2++) {
                        if (arr[(i + t1 + HEIGHT) % HEIGHT][(j + t2 + WIDTH) % WIDTH] >= 1000) {
                            if (!(t1 == 0 && t2 == 0)) count++;
                        }
                    }
                }

                if (count != 3 && count != 2) {
                    state[(i + HEIGHT) % HEIGHT][(j + WIDTH) % WIDTH] = 1;

                } else if (count == 3) {
                    state[i][j] = 3;
                }
            }
        }
    }
}

void delAddCon(int arr[][WIDTH], int state[][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (state[i][j] == 1) {
                arr[i][j] = 0;
                state[i][j] = 0;
            }
            if (state[i][j] == 3) {
                arr[i][j] = 1000;
                state[i][j] = 0;
            }
        }
    }
}

void addNeb(int arr[][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (arr[i][j] >= 1000) {
                for (int t1 = -1; t1 < 1 + 1; t1++) {
                    for (int t2 = -1; t2 < 1 + 1; t2++) {
                        arr[(i + t1 + HEIGHT) % HEIGHT][(j + t2 + WIDTH) % WIDTH]++;
                    }
                }
            }
        }
    }
}

//отрисовка поля:
void printField(int A[][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (A[i][j] >= 1000) {
                printw("*");
            } else {
                printw(" ");
            }
        }
        i != HEIGHT - 1 ? printw("\n") : i;
    }
}

//функция для зачистки поля поля и замены на клетки:
void reField(int A[][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (A[i][j] != 3 && A[i][j] < 1000) {
                A[i][j] = 0;
            } else {
                A[i][j] = 1000;
            }
        }
    }
}

// функция принятия данных из файла и из консоли:
int inputData(int A[][WIDTH]) {
    int m, n, rt = 0, num;

    if (fscanf(stdin, "%d", &num) != 1 || num < 0) {
        rt = -1;
    } else {
        for (int i = 0; i < num; i++) {
            if (fscanf(stdin, "%d %d", &m, &n) != 2 || m < 0 || n < 0) {
                rt = -1;
            } else {
                A[m][n] = 1000;
            }
        }
    }
    return rt;
}
