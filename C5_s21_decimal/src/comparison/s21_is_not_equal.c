#include "../s21_decimal.h"

int s21_is_not_equal(s21_decimal value1, s21_decimal value2) {
  return s21_is_equal(value1, value2) == 0;
}
// инвертируем выражение приравниванием, если функция даст 1, то V1!=V2