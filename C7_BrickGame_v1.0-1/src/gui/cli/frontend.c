#include "frontend.h"

// Функция для отрисовки поля
void draw_field(GameInfo_t *game_info) {
  clear();
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      if (game_info->field[y][x]) {
        mvprintw(y, x * 2, "[]");  // Отображаем блок
      }
    }
  }

  for (int y = 0; y < HEIGHT; y++) {
    mvprintw(y, (WIDTH) * 2, "|");  // Правая рамка
  }

  // Отображение счёта и уровня
  mvprintw(0, WIDTH * 2 + 2, "Score: %d", game_info->score);
  mvprintw(1, WIDTH * 2 + 2, "Level: %d", game_info->level);
  mvprintw(2, WIDTH * 2 + 2, "High Score: %d", game_info->high_score);
  mvprintw(3, WIDTH * 2 + 2, "Status: Run");
  mvprintw(4, WIDTH * 2 + 2, "Next Figure:");
  mvprintw(5, WIDTH * 2 + 2, "------------");
  mvprintw(8, WIDTH * 2 + 2, "------------");
  mvprintw(9, WIDTH * 2 + 2, "Delay: %d ms", game_info->speed);

  refresh();
}

// Функция для отрисовки фигуры
void draw_tetrimino(Tetrimino *tetrimino) {
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (tetrimino->shape[y][x]) {
        mvprintw(tetrimino->y + y, (tetrimino->x + x) * 2, "[]");
      }
    }
  }
  refresh();
}

// Функция для отображения следующей фигуры
void drawNextTetrimino(Tetrimino *tetrimino) {
  int x_offset = 24;
  int y_offset = 6;

  // Очистка области для следующей фигуры
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (tetrimino->shape[i][j]) {
        mvprintw(y_offset + i, x_offset + j * 2, "[]");
      }
    }
  }
}

void startScreen() {
  clear();
  mvprintw(0, 0, "Welcome to Tetris!");
  mvprintw(1, 0, "Press ENTER to start the game");

  while (true) {
    int ch = getch();
    if (ch == '\n') {  // Enter
      break;
    }
  }
  refresh();
}

void pauseGame() {
  mvprintw(3, WIDTH * 2 + 2, "Status: Pause");
  refresh();
  while (true) {
    int ch = getch();
    if (ch == 'p') {  // Возобновление игры
      break;
    }
  }
}

void endScreen(int score) {
  clear();
  mvprintw(0, 0, "Game Over! Your Score: %d", score);
  mvprintw(1, 0, "Press ENTER to restart the game");
  mvprintw(2, 0, "Press Q to quit.");
  refresh();
}
