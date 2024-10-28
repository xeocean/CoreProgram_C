#include "../s21_decimal.h"

int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
  s21_big_decimal val1 = decimal_to_big_decimal(value_1);
  s21_big_decimal val2 = decimal_to_big_decimal(value_2);
  // преборазование в структуру из трёх элементов мантисса, экспонента, знак
  int ret = 0, flag = 1;
  // инициируем значениями переменные результата и флага
  if (!val1.sig && val2.sig) {
    ret = 1;
    flag = 0;
  }
  // проверили V1>V2 по знаку
  if (val1.sig && !val2.sig && flag) {
    ret = 0;
    flag = 0;
  }
  // проверили V1<V2 по знаку
  if (flag) {
    normaliz(&val1, &val2);
    int cmp = compare(val1, val2);
    // привели экспоненты, сравнили мантиссы
    if (cmp == 1) {
      if (!val1.sig)
        ret = 1;  // V1>V2
      else
        ret = 0;  // V1<V2
    } else if (cmp == 2) {
      if (val1.sig)
        ret = 1;  // V1>V2
      else
        ret = 0;  // V1<V2
    }
    // обработали по знаку - положительное и отрицательное значение
  }
  return ret;
  // вернули результат если V1>V2
}