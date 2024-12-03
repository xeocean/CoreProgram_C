
#include <check.h>

#include "../tetris.h"

// Тестирование генерации случайной фигуры
START_TEST(test_generate_tetrimino) {
  Tetrimino tetrimino;
  generate_tetrimino(&tetrimino);

  ck_assert_int_ge(tetrimino.x, 0);
  ck_assert_int_lt(tetrimino.x, WIDTH);
  ck_assert_int_eq(tetrimino.y, 0);
  ck_assert_int_eq(tetrimino.rotation, 0);
  ck_assert_int_eq(tetrimino.shape[0][0], 1);
}
END_TEST

// Тестирование функции check_collision
START_TEST(test_check_collision_no_collision) {
  Tetrimino tetrimino = {.x = 0, .y = 0, .rotation = 0};
  GameInfo_t game_info = {0};
  ck_assert_int_eq(check_collision(&tetrimino, &game_info, 0, 0), 0);
}

START_TEST(test_check_collision_with_collision) {
  Tetrimino tetrimino = {
      .x = 0,
      .y = 0,
      .rotation = 0,
      .shape = {{1, 1, 1, 1},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0}}  // Тетромино "I"
  };

  GameInfo_t game_info = {0};
  game_info.field[0][1] = 1;
  ck_assert_int_eq(check_collision(&tetrimino, &game_info, 0, 0), 1);
}
END_TEST

// Тестирование функции place_tetrimino
START_TEST(test_place_tetrimino) {
  Tetrimino tetrimino = {
      .x = 1,
      .y = 1,
      .rotation = 0,
      .shape = {{1, 1, 0, 0},
                {1, 1, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0}}  // Тетромино "O"
  };

  GameInfo_t game_info = {0};

  place_tetrimino(&tetrimino, &game_info);

  ck_assert_int_eq(game_info.field[1][1], 1);
  ck_assert_int_eq(game_info.field[1][2], 1);
  ck_assert_int_eq(game_info.field[2][1], 1);
  ck_assert_int_eq(game_info.field[2][2], 1);
  ck_assert_int_eq(game_info.field[0][0], 0);
  ck_assert_int_eq(game_info.field[3][3], 0);
}
END_TEST

START_TEST(test_clear_lines) {
  GameInfo_t game_info = {0};

  for (int i = HEIGHT - 1; i >= HEIGHT - 4; i--) {
    for (int x = 0; x < WIDTH; x++) {
      game_info.field[i][x] = 1;
    }
  }

  clear_lines(&game_info);
  ck_assert_int_eq(game_info.score, 1500);

  for (int i = HEIGHT - 1; i >= HEIGHT - 3; i--) {
    for (int x = 0; x < WIDTH; x++) {
      game_info.field[i][x] = 1;
    }
  }

  clear_lines(&game_info);
  ck_assert_int_eq(game_info.score, 2200);

  for (int i = HEIGHT - 1; i >= HEIGHT - 2; i--) {
    for (int x = 0; x < WIDTH; x++) {
      game_info.field[i][x] = 1;
    }
  }

  clear_lines(&game_info);
  ck_assert_int_eq(game_info.score, 2500);

  for (int i = HEIGHT - 1; i >= HEIGHT - 1; i--) {
    for (int x = 0; x < WIDTH; x++) {
      game_info.field[i][x] = 1;
    }
  }

  clear_lines(&game_info);
  ck_assert_int_eq(game_info.score, 2600);
}
END_TEST

// Тестирование функции checkEndGame
START_TEST(test_checkEndGame) {
  GameInfo_t game_info = {0};
  game_info.field[0][0] = 1;

  ck_assert_int_eq(checkEndGame(&game_info), 1);
}
END_TEST

START_TEST(test_checkEndGame_game_continues) {
  GameInfo_t game_info = {0};
  game_info.field[0][0] = 0;

  ck_assert_int_eq(checkEndGame(&game_info), 0);
}
END_TEST

// Тестирование загрузки высокого счета
START_TEST(test_loadHighScore_file_exists) {
  const char *filename = "test_highscore.txt";
  GameInfo_t game_info = {.high_score = 500};

  saveHighScore(filename, &game_info);

  GameInfo_t loaded_game_info = {0};
  int score = loadHighScore(filename, &loaded_game_info);

  ck_assert_int_eq(score, 500);
  ck_assert_int_eq(loaded_game_info.high_score, 500);
}
END_TEST

START_TEST(test_loadHighScore_file_not_exists) {
  const char *filename = "test_nonexistent_highscore.txt";
  GameInfo_t game_info = {0};
  int score = loadHighScore(filename, &game_info);
  ck_assert_int_eq(score, 0);
}
END_TEST

// Тестирование сохранения высокого счета
START_TEST(test_saveHighScore) {
  const char *filename = "test_save_highscore.txt";
  GameInfo_t game_info = {.high_score = 1000};

  saveHighScore(filename, &game_info);

  FILE *file = fopen(filename, "r");
  int saved_score;
  fscanf(file, "%d", &saved_score);
  fclose(file);

  ck_assert_int_eq(saved_score, 1000);
}
END_TEST

// Тестирование инициализации поля
START_TEST(test_initField) {
  GameInfo_t game_info;
  initField(&game_info);

  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      ck_assert_int_eq(game_info.field[y][x], 0);
    }
  }
}
END_TEST

// Тестирование вращения фигуры
START_TEST(test_rotate_tetrimino) {
  Tetrimino tetrimino;

  tetrimino.shape[0][0] = 1;
  tetrimino.shape[0][1] = 0;
  tetrimino.shape[0][2] = 0;
  tetrimino.shape[0][3] = 0;
  tetrimino.shape[1][0] = 1;
  tetrimino.shape[1][1] = 0;
  tetrimino.shape[1][2] = 0;
  tetrimino.shape[1][3] = 0;
  tetrimino.shape[2][0] = 1;
  tetrimino.shape[2][1] = 1;
  tetrimino.shape[2][2] = 0;
  tetrimino.shape[2][3] = 0;
  tetrimino.shape[3][0] = 0;
  tetrimino.shape[3][1] = 0;
  tetrimino.shape[3][2] = 0;
  tetrimino.shape[3][3] = 0;

  tetrimino.rotation = 0;
  rotate_tetrimino(&tetrimino);

  ck_assert_int_eq(tetrimino.shape[0][0], 0);
  ck_assert_int_eq(tetrimino.shape[0][1], 1);
  ck_assert_int_eq(tetrimino.shape[0][2], 1);
  ck_assert_int_eq(tetrimino.shape[0][3], 1);

  ck_assert_int_eq(tetrimino.shape[1][0], 0);
  ck_assert_int_eq(tetrimino.shape[1][1], 1);
  ck_assert_int_eq(tetrimino.shape[1][2], 0);
  ck_assert_int_eq(tetrimino.shape[1][3], 0);

  ck_assert_int_eq(tetrimino.shape[2][0], 0);
  ck_assert_int_eq(tetrimino.shape[2][1], 0);
  ck_assert_int_eq(tetrimino.shape[2][2], 0);
  ck_assert_int_eq(tetrimino.shape[2][3], 0);

  ck_assert_int_eq(tetrimino.shape[3][0], 0);
  ck_assert_int_eq(tetrimino.shape[3][1], 0);
  ck_assert_int_eq(tetrimino.shape[3][2], 0);
  ck_assert_int_eq(tetrimino.shape[3][3], 0);

  ck_assert_int_eq(tetrimino.rotation, 90);
}
END_TEST

Suite *tetris_suite(void) {
  Suite *s = suite_create("Tetris Tests");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_generate_tetrimino);
  tcase_add_test(tc_core, test_check_collision_no_collision);
  tcase_add_test(tc_core, test_check_collision_with_collision);
  tcase_add_test(tc_core, test_place_tetrimino);
  tcase_add_test(tc_core, test_clear_lines);
  tcase_add_test(tc_core, test_checkEndGame);
  tcase_add_test(tc_core, test_checkEndGame_game_continues);
  tcase_add_test(tc_core, test_loadHighScore_file_exists);
  tcase_add_test(tc_core, test_loadHighScore_file_not_exists);
  tcase_add_test(tc_core, test_saveHighScore);
  tcase_add_test(tc_core, test_initField);
  tcase_add_test(tc_core, test_rotate_tetrimino);

  suite_add_tcase(s, tc_core);
  return s;
}

int main(void) {
  int number_failed;
  Suite *s = tetris_suite();
  SRunner *sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? 0 : 1;
}
