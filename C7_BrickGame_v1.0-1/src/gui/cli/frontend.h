#ifndef FRONTEND_H
#define FRONTEND_H

#include "../../brick_game/tetris/backend.h"

// Отрисовка игрового поля
void draw_field(GameInfo_t *game_info);
// Отрисовка текущей фигуры
void draw_tetrimino(Tetrimino *tetrimino);
// Отображение следующей фигуры
void drawNextTetrimino(Tetrimino *tetrimino);
// Экран приветствия
void startScreen();
// Экран паузы
void pauseGame();
// Экран окончания игры
void endScreen(int score);

#endif
