#include "s21_matrix.h"

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  if (rows <= 0 || columns <= 0 || result == NULL) {
    return ERROR_MATRIX;
  }

  int status = OK;

  // Выделение памяти для структуры матрицы
  result->matrix = (double **)calloc(rows, sizeof(double *));
  if (result->matrix != NULL) {
    // Выделение памяти для каждой строки матрицы
    for (int i = 0; i < rows && status == OK; i++) {
      result->matrix[i] = (double *)calloc(columns, sizeof(double));
      if (result->matrix[i] == NULL) {
        status = ERROR_MATRIX;
        // Освобождение ранее выделенной памяти в случае ошибки
        for (int j = 0; j < i; j++) {
          free(result->matrix[j]);
        }
        free(result->matrix);
      }
    }

    // Указание количества строк и столбцов, если всё успешно
    if (status == OK) {
      result->rows = rows;
      result->columns = columns;
    }
  } else {
    status = ERROR_MATRIX;
  }

  return status;
}

// Очистка матриц (remove_matrix)
void s21_remove_matrix(matrix_t *A) {
  if (A != NULL) {
    if (A->matrix != NULL) {
      // Освобождение памяти для каждой строки матрицы
      for (int i = 0; i < A->rows; i++) {
        free(A->matrix[i]);
      }
      // Освобождение памяти для массива строк
      free(A->matrix);
      A->matrix = NULL;  // Обновление указателя
    }
    // Обновление размеров матрицы
    A->rows = 0;
    A->columns = 0;
  }
}

// Сравнение матриц (eq_matrix)
int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int status = SUCCESS;
  // Проверка на валидность и размеры матриц
  if (s21_valid_matrix(A) == ERROR_MATRIX ||
      s21_valid_matrix(B) == ERROR_MATRIX || A->rows != B->rows ||
      A->columns != B->columns) {
    status = FAILURE;
  }

  for (int i = 0; i < A->rows && status == SUCCESS; i++) {
    for (int j = 0; j < A->columns && status == SUCCESS; j++) {
      if (fabs(A->matrix[i][j] - B->matrix[i][j]) > 1e-7) {
        status = FAILURE;
      }
    }
  }
  return status;
}

// Сложение (sum_matrix)
int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  return s21_sum_or_sub(A, B, result, 0);
}

// Вычитание матриц (sub_matrix)
int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  return s21_sum_or_sub(A, B, result, 1);
}

// Умножение матрицы на число (mult_number)
int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  int status = OK;
  // Проверка на валидность и размеры матриц
  if (s21_valid_matrix(A) == ERROR_MATRIX) {
    status = ERROR_MATRIX;
  }

  if (status == OK) {
    status = s21_create_matrix(A->rows, A->columns, result);
  }
  for (int i = 0; i < A->rows && status == OK; i++) {
    for (int j = 0; j < A->columns && status == OK; j++) {
      result->matrix[i][j] = A->matrix[i][j] * number;
    }
  }
  return status;
}

// Умножение двух матриц (mult_matrix)
int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int status = OK;
  // Проверка на валидность и размеры матриц
  if (s21_valid_matrix(A) == ERROR_MATRIX ||
      s21_valid_matrix(B) == ERROR_MATRIX) {
    status = ERROR_MATRIX;
  }

  // Проверка на размеры матриц
  if (A->columns != B->rows) {
    status = ERROR_CALCULATION;
  }

  if (status == OK) {
    status = s21_create_matrix(A->rows, B->columns, result);
  }

  for (int i = 0; i < A->rows && status == OK; i++) {
    for (int j = 0; j < B->columns && status == OK; j++) {
      double sum = 0.0;
      for (int k = 0; k < A->columns && status == OK; k++) {
        sum += A->matrix[i][k] * B->matrix[k][j];
      }
      result->matrix[i][j] = sum;
    }
  }

  return status;
}

// Транспонирование матрицы (transpose)
int s21_transpose(matrix_t *A, matrix_t *result) {
  int status = OK;

  if (s21_valid_matrix(A) == ERROR_MATRIX) {
    status = ERROR_MATRIX;
  }

  if (status == OK) {
    status = s21_create_matrix(A->columns, A->rows, result);
  }

  if (status == OK) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[j][i] = A->matrix[i][j];
      }
    }
  }
  return status;
}

// Минор матрицы и матрица алгебраических дополнений (calc_complements)
int s21_calc_complements(matrix_t *A, matrix_t *result) {
  int status = OK;

  if (A == NULL || result == NULL) {
    return ERROR_MATRIX;
  }

  if (s21_valid_matrix(A) == ERROR_MATRIX) {
    status = ERROR_MATRIX;
  }

  if (A->rows == 1 || A->columns == 1) {
    status = ERROR_CALCULATION;
  }

  if (A->rows != A->columns) {
    status = ERROR_CALCULATION;
  }

  if (status == OK) {
    status = s21_create_matrix(A->rows, A->columns, result);
    // Вычисление алгебраических дополнений
    for (int i = 0; i < A->rows && status == OK; i++) {
      for (int j = 0; j < A->columns && status == OK; j++) {
        matrix_t minor;
        double determinant = 0.0;
        s21_get_minor(A, i, j, &minor);
        s21_determinant(&minor, &determinant);
        result->matrix[i][j] = pow(-1, i + j) * determinant;
        s21_remove_matrix(&minor);
      }
    }
  }
  return status;
}

void s21_get_minor(matrix_t *A, int row, int col, matrix_t *minor) {
  // Создаем подматрицу
  minor->rows = A->rows - 1;
  minor->columns = A->columns - 1;
  s21_create_matrix(minor->rows, minor->columns, minor);

  // Заполняем подматрицу, пропуская нужную строку и столбец
  int minor_i = 0;
  for (int i = 0; i < A->rows; i++) {
    if (i == row) continue;  // Пропускаем строку

    int minor_j = 0;
    for (int j = 0; j < A->columns; j++) {
      if (j == col) continue;  // Пропускаем столбец

      minor->matrix[minor_i][minor_j] = A->matrix[i][j];
      minor_j++;
    }
    minor_i++;
  }
}

// Определитель матрицы (determinant)
int s21_determinant(matrix_t *A, double *result) {
  int status = OK;

  if (A == NULL || result == NULL) {
    return ERROR_MATRIX;
  }

  // Проверка на валидность матрицы
  if (s21_valid_matrix(A) == ERROR_MATRIX) {
    status = ERROR_MATRIX;
  }

  // Проверка на квадратную матрицу
  if (A->rows != A->columns) {
    status = ERROR_CALCULATION;
  }

  if (status == OK) {
    // Базовый случай для матрицы 1x1
    if (A->rows == 1) {
      *result = A->matrix[0][0];
    }
    // Базовый случай для матрицы 2x2
    else if (A->rows == 2) {
      *result =
          A->matrix[0][0] * A->matrix[1][1] - A->matrix[0][1] * A->matrix[1][0];
    }
    // Рекурсивный случай для матриц больше 2x2
    else {
      double determinant = 0.0;
      for (int j = 0; j < A->columns; j++) {
        matrix_t minor;
        double minor_det;
        s21_get_minor(A, 0, j,
                      &minor);  // Получаем минор для строки 0 и столбца j
        s21_determinant(&minor, &minor_det);  // Вычисляем определитель минора
        determinant += pow(-1, j) * A->matrix[0][j] * minor_det;
        s21_remove_matrix(&minor);  // Освобождаем память для минора
      }
      *result = determinant;
    }
  }
  return status;
}

// Обратная матрица (inverse_matrix)
int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  if (A->rows == 1 && A->columns == 1) {
    if (fabs(A->matrix[0][0]) < 1e-7) return 2;
    s21_create_matrix(1, 1, result);
    result->matrix[0][0] = 1.0 / A->matrix[0][0];
    return 0;
  }

  double det;
  if (s21_determinant(A, &det) != 0 || fabs(det) < 1e-7) return 2;

  matrix_t complements, transpose;
  if (s21_calc_complements(A, &complements) != 0) return 1;
  if (s21_transpose(&complements, &transpose) != 0) {
    s21_remove_matrix(&complements);
    return 1;
  }

  s21_mult_number(&transpose, 1.0 / det, result);

  s21_remove_matrix(&complements);
  s21_remove_matrix(&transpose);

  return 0;
}

// Вычитание или сложение матриц (sum_or_sub)
int s21_sum_or_sub(matrix_t *A, matrix_t *B, matrix_t *result, int action) {
  int status = OK;
  // Проверка на валидность и размеры матриц
  if (s21_valid_matrix(A) == ERROR_MATRIX ||
      s21_valid_matrix(B) == ERROR_MATRIX) {
    status = ERROR_MATRIX;
  }
  // Проверка на размеры матриц
  if (A->rows != B->rows || A->columns != B->columns) {
    status = ERROR_CALCULATION;
  }

  if (status == OK) {
    status = s21_create_matrix(A->rows, A->columns, result);
  }
  for (int i = 0; i < A->rows && status == OK; i++) {
    for (int j = 0; j < A->columns && status == OK; j++) {
      if (action == 0)
        result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
      else if (action == 1)
        result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
    }
  }
  return status;
}

// Проверка матрицы (valid_matrix)
int s21_valid_matrix(matrix_t *A) {
  int valid_status = OK;
  if (A == NULL || A->matrix == NULL || A->rows <= 0 || A->columns <= 0) {
    valid_status = ERROR_MATRIX;
  }
  return valid_status;
}

// Инициализация матрицы (init_matrix)
void s21_init_matrix(matrix_t *A, double number) {
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      A->matrix[i][j] = number;
      number += 1.0;
    }
  }
}

// Вывод матрицы (print_matrix)
void s21_print_matrix(matrix_t *A) {
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      printf("%f\t", A->matrix[i][j]);
    }
    printf("\n");
  }
}
