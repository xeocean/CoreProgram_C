#include "backend.h"

// Функция для генерации случайной фигуры
void generate_tetrimino(Tetrimino *tetrimino) {
  // Массив возможных форм
  int shapes[7][4][4] = {
      {{1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // квадрат
      {{1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // линия
      {{0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // L-образная
      {{1, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // T-образная
      {{1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // S-образная
      {{0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // Z-образная
      {{1, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}  // J-образная
  };

  // Генерация случайного индекса для фигуры (от 0 до 6)
  int random_shape = rand() % 7;

  // Присваиваем случайную форму фигуры
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      tetrimino->shape[y][x] = shapes[random_shape][y][x];
    }
  }

  // Начальная позиция фигуры
  tetrimino->x = WIDTH / 2 - 2;  // Центр по X
  tetrimino->y = 0;  // Начинаем с верхней части поля
  tetrimino->rotation = 0;  // Начальный угол вращения 0 градусов
}

// Функция для проверки столкновения
int check_collision(Tetrimino *tetrimino, GameInfo_t *game_info, int dx,
                    int dy) {
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (tetrimino->shape[y][x]) {
        int newX = tetrimino->x + x + dx;
        int newY = tetrimino->y + y + dy;
        if (newX < 0 || newX >= WIDTH || newY >= HEIGHT ||
            (newY >= 0 && game_info->field[newY][newX])) {
          return 1;  // Столкновение
        }
      }
    }
  }
  return 0;
}

// Функция для размещения фигуры на поле
void place_tetrimino(Tetrimino *tetrimino, GameInfo_t *game_info) {
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (tetrimino->shape[y][x]) {
        int newX = tetrimino->x + x;
        int newY = tetrimino->y + y;
        if (newY >= 0) {
          game_info->field[newY][newX] = 1;  // Закрепляем блок на поле
        }
      }
    }
  }
}

void clear_lines(GameInfo_t *game_info) {
  int lines_cleared = 0;  // Количество очищенных линий за текущий цикл

  for (int y = HEIGHT - 1; y >= 0; y--) {
    int full_line = 1;

    // Проверка, заполнена ли строка
    for (int x = 0; x < WIDTH; x++) {
      if (game_info->field[y][x] == 0) {
        full_line = 0;
        break;
      }
    }

    // Если строка полностью заполнена
    if (full_line) {
      lines_cleared++;  // Увеличиваем счётчик очищенных линий

      // Сдвигаем все строки сверху на одну вниз
      for (int i = y; i > 0; i--) {
        for (int x = 0; x < WIDTH; x++) {
          game_info->field[i][x] = game_info->field[i - 1][x];
        }
      }

      // Очистить верхнюю строку
      for (int x = 0; x < WIDTH; x++) {
        game_info->field[0][x] = 0;
      }

      // Повторно проверяем ту же строку
      y++;
    }
  }

  // Начисляем очки
  switch (lines_cleared) {
    case 1:
      game_info->score += 100;
      break;
    case 2:
      game_info->score += 300;
      break;
    case 3:
      game_info->score += 700;
      break;
    case 4:
      game_info->score += 1500;
      break;
  }
}


// Функция поворота фигуры
void rotate_tetrimino(Tetrimino *tetrimino) {
  int temp_shape[4][4];

  // Обновляем угол поворота
  int new_rotation =
      (tetrimino->rotation + 90) % 360;  // Увеличиваем угол на 90 градусов
  tetrimino->rotation = new_rotation;

  // Вращаем фигуру на 90 градусов по часовой стрелке
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      temp_shape[x][y] = tetrimino->shape[y][x];  // Транспонируем матрицу
    }
  }

  // Зеркалим по вертикали
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 2; x++) {
      int temp = temp_shape[y][x];
      temp_shape[y][x] = temp_shape[y][3 - x];
      temp_shape[y][3 - x] = temp;
    }
  }

  // Копируем вращенную форму обратно в фигуру
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      tetrimino->shape[y][x] = temp_shape[y][x];
    }
  }
}

// Проверка окончания игры
int checkEndGame(GameInfo_t *game_info) {
  for (int x = 0; x < WIDTH; x++) {
    // Если в верхней строке поля есть хотя бы один блок, игра завершена
    if (game_info->field[0][x] != 0) {
      return 1;  // Завершение игры
    }
  }
  return 0;  // Игра продолжается
}

// Загрузка рекорда
int loadHighScore(const char *filename, GameInfo_t *game_info) {
  FILE *file = fopen(filename, "r");

  if (!file) {  // Если файл не существует
    // Создаем новый файл с нулевым значением
    file = fopen(filename, "w");
    if (file) {
      fprintf(file, "%d", game_info->high_score);
      fclose(file);
    } else {
      perror("Ошибка при создании файла");
      return -1;  // Возвращаем ошибку
    }
  } else {
    // Если файл существует, читаем из него значение
    fscanf(file, "%d", &game_info->high_score);
    fclose(file);
  }
  return game_info->high_score;
}

// Сохрание рекорда
void saveHighScore(const char *filename, GameInfo_t *game_info) {
  FILE *file = fopen(filename, "w");  // Открытие файла для записи
  if (file == NULL) {
    return;
  }
  fprintf(file, "%d", game_info->high_score);  // Запись числа в файл
  fclose(file);
}

// Инициализация поля
void initField(GameInfo_t *game_info) {
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      game_info->field[y][x] = 0;
    }
  }
}
