#include "tetris.h"

// Определяем действие пользователя на основе нажатой клавиши
UserAction_t getUserAction(int key) {
  switch (key) {
    case KEY_LEFT:
      return Left;  // Движение влево
    case KEY_RIGHT:
      return Right;  // Движение вправо
    case KEY_DOWN:
      return Down;  // Ускорение падения
    case ' ':
      return Action;  // Вращение фигуры
    case '\n':
      return Start;  // Начало игры
    case 'p':
      return Pause;  // Пауза
    case 'q':
      return Terminate;  // Выход из игры
    default:
      return None;  // Никакого действия
  }
}
// Обрабатываем действия пользователя
void userInput(UserAction_t action, GameInfo_t *game_info,
               Tetrimino *current_tetrimino) {
  switch (action) {
    case Left:
      if (!check_collision(current_tetrimino, game_info, -1, 0)) {
        current_tetrimino->x--;  // Сдвиг фигуры влево
      }
      break;
    case Right:
      if (!check_collision(current_tetrimino, game_info, 1, 0)) {
        current_tetrimino->x++;  // Сдвиг фигуры вправо
      }
      break;
    case Down:
      if (!check_collision(current_tetrimino, game_info, 0, 1)) {
        current_tetrimino->y++;  // Сдвиг фигуры вниз
      }
      break;
    case Action:
      rotate_tetrimino(current_tetrimino);  // Вращаем фигуру
      if (check_collision(current_tetrimino, game_info, 0, 0)) {
        // Если вращение приводит к столкновению, отменяем его
        rotate_tetrimino(current_tetrimino);
        rotate_tetrimino(current_tetrimino);
        rotate_tetrimino(current_tetrimino);
      }
      break;
    case Pause:
      pauseGame();  // Ставим игру на паузу
      break;
    case Terminate:
      game_info->game_over = 1;  // Завершаем игру
      break;
    case Start:
    case None:
      break;  // Ничего не делаем
  }
}
// Обновляем состояние игры (уровень, рекорд)
GameInfo_t *updateCurrentState(GameInfo_t *game_info) {
  if (game_info->score >= game_info->level * 600 && game_info->level < 10) {
    game_info->level++;  // Повышаем уровень
    game_info->speed -= 100;  // Увеличиваем скорость падения
  }

  if (game_info->score > game_info->high_score) {
    game_info->high_score = game_info->score;  // Обновляем рекорд
  }

  return game_info;
}
// Сбрасываем параметры игры к начальному состоянию
void resetGameState(GameInfo_t *game_info, Tetrimino *current_tetrimino,
                    Tetrimino *next_tetrimino) {
  game_info->score = 0;
  game_info->level = 1;
  game_info->speed = 1100;  // Начальная скорость
  game_info->high_score_file = "record.txt";
  game_info->high_score = loadHighScore(game_info->high_score_file, game_info);
  game_info->pause = 0;
  game_info->game_over = 0;
  current_tetrimino->x = 0;  // Начальное положение текущей фигуры
  current_tetrimino->y = 0;
  next_tetrimino->x = 0;  // Начальное положение следующей фигуры
  next_tetrimino->y = 0;

  // Очищаем игровое поле
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      game_info->field[i][j] = 0;
    }
  }
}
// Основной игровой цикл
int playGame(GameInfo_t game_info, Tetrimino current_tetrimino,
             Tetrimino next_tetrimino) {
  generate_tetrimino(&current_tetrimino);  // Генерация текущей фигуры
  generate_tetrimino(&next_tetrimino);  // Генерация следующей фигуры

  while (!game_info.game_over) {
    game_info = *updateCurrentState(&game_info);  // Обновляем параметры игры
    timeout(game_info.speed);  // Устанавливаем таймер на скорость игры
    int key = getch();  // Читаем ввод пользователя

    UserAction_t action = getUserAction(key);  // Определяем действие
    userInput(action, &game_info, &current_tetrimino);

    if (check_collision(&current_tetrimino, &game_info, 0, 1)) {
      // Если фигура достигает нижней границы или другого объекта
      place_tetrimino(&current_tetrimino, &game_info);  // Помещаем её на поле
      clear_lines(&game_info);                 // Удаляем линии
      generate_tetrimino(&current_tetrimino);  // Генерируем новую фигуру
      current_tetrimino =
          next_tetrimino;  // Следующая фигура становится текущей
      generate_tetrimino(&next_tetrimino);  // Генерируем новую следующую фигуру
    } else {
      current_tetrimino.y++;  // Фигура падает ниже
    }

    if (checkEndGame(&game_info)) {
      game_info.game_over = 1;  // Проверяем окончание игры
    }

    // Отрисовываем игровое поле и фигуры
    draw_field(&game_info);
    draw_tetrimino(&current_tetrimino);
    drawNextTetrimino(&next_tetrimino);
  }

  saveHighScore(game_info.high_score_file, &game_info);  // Сохраняем рекорд
  return game_info.score;  // Возвращаем финальный счёт
}

int main() {
  initscr();    // Инициализация ncurses
  cbreak();     // Отключаем буферизацию
  noecho();     // Не отображаем ввод
  curs_set(0);  // Скрываем курсор
  keypad(stdscr, TRUE);  // Разрешаем ввод функциональных клавиш
  nodelay(stdscr, TRUE);  // Неблокирующий ввод

  srand(time(NULL));  // Инициализация случайного числа

  GameInfo_t game_info;
  Tetrimino current_tetrimino;
  Tetrimino next_tetrimino;

  // Основной цикл программы
  while (true) {
    startScreen();  // Показать стартовый экран

    resetGameState(&game_info, &current_tetrimino, &next_tetrimino);
    int final_score = playGame(game_info, current_tetrimino,
                               next_tetrimino);  // Запускаем игру

    endScreen(final_score);  // Показать экран завершения

    while (true) {
      int key = getch();
      UserAction_t action = getUserAction(key);
      userInput(action, &game_info, &current_tetrimino);

      if (action == Start) {
        break;  // Перезапуск игры
      } else if (action == Terminate) {
        endwin();  // Завершаем ncurses
        return 0;
      }
    }
  }

  endwin();  // Завершаем ncurses при выходе
  return 0;
}