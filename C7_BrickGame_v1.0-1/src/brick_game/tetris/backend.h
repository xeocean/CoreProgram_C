#ifndef BACKEND_H
#define BACKEND_H

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

// Константы ширины и высоты игрового поля
#define WIDTH 10
#define HEIGHT 20

// Структура состояния игры
typedef struct {
  int field[HEIGHT][WIDTH];  // Игровое поле
  int score;                 // Текущий счёт
  int high_score;            // Рекордный счёт
  char *high_score_file;     // Файл для хранения рекорда
  int pause;                 // Флаг паузы
  int game_over;             // Флаг завершения игры
  int level;                 // Текущий уровень
  int speed;                 // Скорость игры
} GameInfo_t;

// Структура фигуры
typedef struct {
  int x, y;         // Координаты фигуры
  int shape[4][4];  // Матрица формы фигуры
  int rotation;     // Угол поворота фигуры
} Tetrimino;

// Действия пользователя
typedef enum {
  Start,      // Начало игры
  Pause,      // Пауза
  Terminate,  // Завершение игры
  Left,       // Движение влево
  Right,      // Движение вправо
  Down,       // Движение вниз
  Action,     // Вращение фигуры
  None        // Отсутствие действия
} UserAction_t;

// Генерация новой фигуры
void generate_tetrimino(Tetrimino *tetrimino);
// Проверка столкновений фигуры с игровым полем
int check_collision(Tetrimino *tetrimino, GameInfo_t *game_info, int dx,
                    int dy);
// Установка фигуры на игровом поле
void place_tetrimino(Tetrimino *tetrimino, GameInfo_t *game_info);
// Очистка заполненных линий на поле
void clear_lines(GameInfo_t *game_info);
// Вращение фигуры
void rotate_tetrimino(Tetrimino *tetrimino);
// Проверка конца игры
int checkEndGame(GameInfo_t *game_info);
// Загрузка рекордного счёта из файла
int loadHighScore(const char *filename, GameInfo_t *game_info);
// Сохранение рекордного счёта в файл
void saveHighScore(const char *filename, GameInfo_t *game_info);
// Инициализация игрового поля
void initField(GameInfo_t *game_info);

#endif
