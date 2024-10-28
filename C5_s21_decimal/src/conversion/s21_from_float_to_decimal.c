#include "../s21_decimal.h"

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int result = 0;
  // проверка на NULL, бесконечность(isinf), не число(isnan)
  if (!dst || isinf(src) || isnan(src)) {
    result = 1;
    // проверка допустимости диапазона
  } else if (fabsf(src) > MAX_FLOAT ||
             (fabsf(src) < MIN_FLOAT && fabsf(src) > 0)) {
    result = 1;
    // устанавливаем -
  } else if (src == -0.0) {
    s21_set_sigh(dst, 1);
    result = 0;
  } else {
    *dst = anull_decimal();
    // Определение знака 1 = -, 0 = +)
    int sign = src < 0;
    // Приведение к положительному для вычислений
    if (sign) src = -src;
    char buffer[25];
    // Преобразование числа в строку формата %.6E (1.234567E+02)
    sprintf(buffer, "%.6E", src);
    // целая часть и дробная часть
    int integer_part = atoi(strtok(buffer, "."));
    int fractional_part = atoi(strtok(NULL, "E"));
    int exponent = atoi(strtok(NULL, "E"));
    // Объединение целой и дробной частей числа
    integer_part = integer_part * 1000000 + fractional_part;
    if (integer_part != 0) exponent -= 6;
    // Удаление завершающих нулей
    while (integer_part % 10 == 0 && integer_part != 0) {
      integer_part /= 10;
      exponent++;
    }

    // удаление чисел после определенной степени и округления
    int tmp = 0;
    while (exponent < -28) {
      tmp = integer_part % 10;
      integer_part /= 10;
      exponent++;
    }
    if (tmp >= 5) integer_part++;

    // Конвертация в s21_decimal
    if (s21_from_int_to_decimal(integer_part, dst)) {
      result = 1;
    } else {
      // Масштабирование по экспоненте
      s21_decimal ten = {{10, 0, 0, 0}};
      while (exponent > 0) {
        if (s21_mul(*dst, ten, dst) != 0) {
          result = 1;
        } else {
          exponent--;
        }
      }
      set_scale(dst, (unsigned int)(-exponent));
      if (sign) s21_set_sigh(dst, 1);
    }
  }
  return result;
}