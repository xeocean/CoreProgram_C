#include "../s21_decimal.h"

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  s21_decimal tmp_src;
  bool is_valid = true;

  int scale = (src.bits[3] >> 16) & 0x7F;
  bool is_negative = (src.bits[3] & (1u << 31)) != 0;
  int reserved_bits = src.bits[3] & 0x7F00FFFF;

  // проверка на коректность числа
  if (scale > 28 || reserved_bits != 0) {
    is_valid = false;
    // проверка успешности операции при округлении
  } else if (dst == NULL || s21_truncate(src, &tmp_src)) {
    is_valid = false;
    // прверка помещается ли число в int
  } else if (tmp_src.bits[1] || tmp_src.bits[2]) {
    is_valid = false;
  } else {
    unsigned int number = tmp_src.bits[0];

    if (is_negative) {
      // проверка на переполнение с минусом
      if (number > (unsigned int)MAXIMUM) {
        is_valid = false;
      } else {
        *dst = (number == (unsigned int)MAXIMUM) ? MINIMUM : -((int)number);
      }
    } else {
      // проверка на переполнение с плюсом
      if (number > MAXIMUM - 1) {
        is_valid = false;
      } else {
        *dst = (int)number;
      }
    }
  }
  // 0 - успех, 1 - ошибка
  return is_valid ? 0 : 1;
}