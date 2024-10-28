#include "../s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int result = 0;
  if (dst == NULL) {
    result = 1;
  } else {
    *dst = (s21_decimal){0};
    // конвертация в отрицательное
    if ((src < 0) && (src != MINIMUM)) {
      src *= -1;
      s21_set_bit(dst, 127, 1);
    }
    // конвертация в положительное
    if (src == MINIMUM) {
      s21_set_bit(dst, 127, 1);
    }
    dst->bits[0] = src;
  }

  return result;
}
