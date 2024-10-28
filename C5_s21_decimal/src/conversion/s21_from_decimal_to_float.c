#include "../s21_decimal.h"

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  double counter = 0.0;
  *dst = 0.0;
  // извелчение экспоненты из 4 бита чисел через сдвиг
  int size = (src.bits[3] & EXP) >> 16;
  // перебираем биты и если что-то есть счетчик +
  for (int i = 0; i < MAX_BIT * (TOTAL_BYTES - 1); i++)
    if (find_bit(src, i)) counter += pow(2, i);
  // сдвигаем точку на size единиц
  while (size) {
    counter /= 10;
    size--;
  }
  // периписываем counter в тип float
  *dst = (float)counter;
  // проверяем число на отрицательное
  if (src.bits[3] & MIN_BITS) *dst = *dst * (-1);

  return 0;
}
