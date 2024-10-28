#include "../s21_decimal.h"

#define MAX_EXP 28  // Максимальная экспонента для проверки переполнения

int s21_truncate(s21_decimal value, s21_decimal *result) {
  // Проверка на NULL указатель результата
  if (result == NULL) return 1;
  int flag = 0;

  // Определяем константу 'ten' для деления
  const s21_big_decimal ten = {{10, 0, 0, 0, 0, 0}, 0, 0};

  // Инициализируем делитель как единицу
  s21_big_decimal divisor = {{1, 0, 0, 0, 0, 0}, 0, 0};

  // Преобразуем входной decimal в big decimal
  s21_big_decimal val = decimal_to_big_decimal(value);

  // Сохраняем знак числа
  int sig = val.sig;

  // Если число имеет дробную часть
  if (val.exp) {
    // Увеличиваем делитель на 10 в соответствии с экспонентой
    for (int i = 0; i < val.exp; i++) {
      divisor = big_decimal_mul(divisor, ten);
      if (divisor.exp > MAX_EXP) {  // Проверяем переполнение
        flag = 1;  // Возвращаем ошибку при переполнении
      }
    }
    if (flag == 0) {
      s21_big_decimal res = big_decimal_div(val, divisor);
      res.exp = 0;    // Устанавливаем экспоненту в 0
      res.sig = sig;  // Восстанавливаем знак

      // Проверяем возможность преобразования результата в s21_decimal
      if (check_rew_trans(res) == 0) {
        *result =
            big_decimal_to_decimal(res);  // Преобразуем обратно в s21_decimal
      } else {
        return 1;  // Ошибка преобразования
      }
    }
  } else {
    // Если число уже целое, просто копируем значение
    *result = value;
  }

  return flag;
}
