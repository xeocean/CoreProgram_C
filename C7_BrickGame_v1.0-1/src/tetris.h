#ifndef TETRIS_H
#define TETRIS_H

#include "gui/cli/frontend.h"

// Определение действия пользователя на основе нажатой клавиши
UserAction_t getUserAction(int key);
// Обработка ввода пользователя и соответствующее обновление состояния игры
void userInput(UserAction_t action, GameInfo_t *game_info,
               Tetrimino *current_tetrimino);
// Обновление текущего состояния игры (например, уровня, скорости, рекорда)
GameInfo_t *updateCurrentState(GameInfo_t *game_info);
// Сброс состояния игры для новой игровой сессии
void resetGameState(GameInfo_t *game_info, Tetrimino *current_tetrimino,
                    Tetrimino *next_tetrimino);
// Основной игровой цикл, включает управление и логику игры
int playGame(GameInfo_t game_info, Tetrimino current_tetrimino,
             Tetrimino next_tetrimino);
// Точка входа в программу
int main();

#endif
