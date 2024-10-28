#include "../s21_decimal.h"

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  const s21_big_decimal zero = {{0, 0, 0, 0, 0, 0}, 0, 0};
  // константа ноль, для сравнения
  s21_big_decimal val1 = decimal_to_big_decimal(value_1);
  s21_big_decimal val2 = decimal_to_big_decimal(value_2);
  // преборазование в структуру из трёх элементов мантисса, экспонента, знак
  int ret = 0, flag = 1;
  // инициируем значениями переменные результата и флага
  if ((!val1.sig && val2.sig) || (val1.sig && !val2.sig)) {
    flag = 0;
  }
  // проверили на знак
  if (!compare(val1, zero) && !compare(val2, zero)) {
    flag = 0;
    ret = 1;
  }
  // проверили на ноль, если нули то 1
  if (flag) {
    normaliz(&val1, &val2);
    if (!compare(val1, val2)) ret = 1;
  }
  // привели экспоненты, сравнили мантиссы, если 0 то эквивалентны
  return ret;
  // вернули результат, эквивалентны 1
}