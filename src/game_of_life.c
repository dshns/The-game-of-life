#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>

#define HEIGHT 25
#define WIDTH 80

void input(int matrix[HEIGHT][WIDTH]);
void output(int matrix[HEIGHT][WIDTH]);
int kol_sos(int matrix[HEIGHT][WIDTH], int i, int j);
void enumeration_update(int matrix[HEIGHT][WIDTH]);
int has_life(int matrix[HEIGHT][WIDTH]);
int change_speed(char c, int sec);

int main(void) {
    int matrix[HEIGHT][WIDTH];
    int sec = 3;

    input(matrix);
    if (freopen("/dev/tty", "r", stdin)) initscr();
    nodelay(stdscr, TRUE);

    cbreak();
    noecho();
    curs_set(0);

    output(matrix);

    while (has_life(matrix)) {
        char c = getch();

        if (c == '1' || c == '2' || c == '3')
            sec = change_speed(c, sec);
        else if (c == ' ')
            break;

        enumeration_update(matrix);
        usleep(sec * 100000);
    }

    endwin();

    printf("\nThe colony is dead, the game is over.\n\n");
    return 0;
}

void input(int matrix[HEIGHT][WIDTH]) {
    int ch;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            ch = getchar();
            if (ch == '\n') ch = getchar();
            matrix[i][j] = ch;
        }
    }
}

void output(int matrix[HEIGHT][WIDTH]) {
    move(0, 0);
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (matrix[i][j] == '1')
                addch('o');
            else
                addch('_');
        }
        if (i < HEIGHT - 1) addch('\n');
    }
    printw("\n\nTo change the speed, select one of the three modes:");
    printw("\n1 - slow;\n2 - medium;\n3 - fast;\n");
    printw("\nIf you want to exit the game, click 'Space'.");
}

int kol_sos(int matrix[HEIGHT][WIDTH], int i, int j) {
    int num = 0;
    int a, b;
    for (int x = i - 1; x <= i + 1; x++) {
        for (int y = j - 1; y <= j + 1; y++) {
            a = x;
            b = y;
            if (x == -1) a = HEIGHT - 1;
            if (y == -1) b = WIDTH - 1;
            if (x == HEIGHT) a = 0;
            if (y == WIDTH) b = 0;

            if (!(a == i && b == j) && matrix[a][b] == '1') num++;
        }
    }

    return num;
}

void enumeration_update(int matrix[HEIGHT][WIDTH]) {
    int new_matrix[HEIGHT][WIDTH];

    int neighbors;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            neighbors = kol_sos(matrix, i, j);
            if (matrix[i][j] == '1' && (neighbors < 2 || neighbors > 3)) {
                new_matrix[i][j] = '0';
                move(i, j);
                addch('_');
            } else if (matrix[i][j] == '1')
                new_matrix[i][j] = '1';
            else if (matrix[i][j] == '0' && neighbors == 3) {
                new_matrix[i][j] = '1';
                move(i, j);
                addch('o');
            } else
                new_matrix[i][j] = '0';
        }
    }

    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++) matrix[i][j] = new_matrix[i][j];
}

int has_life(int matrix[HEIGHT][WIDTH]) {
    int flag = 0;
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            if (matrix[i][j] == '1') flag = 1;

    return flag;
}
int change_speed(char c, int sec) {
    if (c == '1')
        sec = 9;
    else if (c == '2')
        sec = 4;
    else if (c == '3')
        sec = 1;

    return sec;
}