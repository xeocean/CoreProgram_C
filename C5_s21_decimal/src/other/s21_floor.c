#include "../s21_decimal.h"

int s21_floor(s21_decimal value, s21_decimal *result) {
  // Усекаем значение, чтобы получить целую часть
  if (!s21_truncate(value, result)) {
    // Если значение отрицательное и есть дробная часть
    if (getSign(value) && !s21_is_equal(*result, value)) {
      // Уменьшаем результат на 1
      s21_sub(*result, (s21_decimal){{1, 0, 0, 0}}, result);
    }
  }

  return 0;  // Возвращаем код успешного выполнения
}
