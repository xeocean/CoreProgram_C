#include "../s21_decimal.h"

int find_bit(s21_decimal value, int bit) {
  int index = bit / MAX_BIT;
  unsigned int mask = 1 << (bit % MAX_BIT);
  return (value.bits[index] & mask) ? 1 : 0;
}

s21_decimal anull_decimal() {
  s21_decimal result = {{0, 0, 0, 0}};
  return result;
}

int set_scale(s21_decimal *num, unsigned value) {
  // очищает первые 8 бит четвертого слова
  // создает маску со значением скейла смещенным вправо на 16 позиций
  // |= устанавливает первые 8 бит четвертого слова в значение скейла
  num->bits[3] &= ~(0xFF << 16);
  num->bits[3] |= (value & 0xFF) << 16;
  return 0;
}

s21_big_decimal decimal_to_big_decimal(const s21_decimal decimal) {
  s21_big_decimal tmp = {
      {decimal.bits[0], decimal.bits[1], decimal.bits[2], 0, 0, 0}, 0, 0};
  for (int i = 0; i < 8; i++) {
    if (decimal.bits[3] & _2((i + 16))) tmp.exp |= _2(i);
  }
  if (decimal.bits[3] & (_2(31))) tmp.sig = 1;
  return tmp;
}

s21_decimal big_decimal_to_decimal(const s21_big_decimal decimal) {
  // Создаем временную копию входного большого десятичного числа
  s21_big_decimal dec_tmp = decimal;
  s21_big_decimal remainder, dec_tmp2;

  // Константы для работы с числами
  const s21_big_decimal ten = {{10, 0, 0, 0, 0, 0}, 0, 0};  // Константа 10
  const s21_big_decimal five = {{5, 0, 0, 0, 0, 0}, 0, 0};  // Константа 5
  const s21_big_decimal e8 = {
      {100000000, 0, 0, 0, 0, 0}, 0, 0};  // Константа для деления на 10^8

  // Сохраняем знак и экспоненту числа
  int sig = dec_tmp.sig;
  int exp;

  // Цикл для уменьшения числа до тех пор, пока количество нулевых битов меньше
  // 68
  while (num_zero_bit(dec_tmp) < 68) {
    exp = dec_tmp.exp;  // Сохраняем текущую экспоненту
    dec_tmp = big_decimal_div(dec_tmp, e8);  // Делим на 10^8
    dec_tmp.exp = exp - 8;  // Уменьшаем экспоненту на 8
  }

  // Цикл для округления числа до целого значения
  while (num_zero_bit(dec_tmp) < 96 || dec_tmp.exp > 28) {
    exp = dec_tmp.exp;  // Сохраняем текущую экспоненту
    remainder =
        big_decimal_rem(dec_tmp, ten);  // Находим остаток от деления на 10
    int cmp = compare(remainder, five);  // Сравниваем остаток с пятеркой

    // Округление в зависимости от значения остатка
    if (cmp == 2) {
      dec_tmp = big_decimal_div(
          dec_tmp, ten);  // Если остаток больше пяти - делим на десять
    } else if (cmp == 1) {
      dec_tmp = big_decimal_sum(
          dec_tmp, ten);  // Если остаток меньше пяти - прибавляем десять
      dec_tmp = big_decimal_div(dec_tmp, ten);  // И делим на десять
    } else if (cmp == 0) {
      dec_tmp2 = big_decimal_sub(
          dec_tmp, ten);  // Если остаток равен пяти - проверяем младший бит
      if (get_bit(dec_tmp2, 0)) {
        dec_tmp = big_decimal_sum(dec_tmp,
                                  ten);  // Если младший бит равен единице -
                                         // прибавляем десять и делим на десять
        dec_tmp = big_decimal_div(dec_tmp, ten);
      }
    }
    dec_tmp.exp = exp - 1;  // Уменьшаем экспоненту на один
  }

  // Если экспонента ненулевая - продолжаем сжимать число
  if (dec_tmp.exp) {
    const s21_big_decimal zero = {{0, 0, 0, 0, 0, 0}, 0, 0};  // Константа ноль
    remainder =
        big_decimal_rem(dec_tmp, ten);  // Находим остаток от деления на десять

    while (!compare(remainder, zero) && dec_tmp.exp > 0) {
      exp = dec_tmp.exp;
      dec_tmp = big_decimal_div(dec_tmp, ten);  // Делим на десять
      dec_tmp.exp = exp - 1;  // Уменьшаем экспоненту на один
      remainder = big_decimal_rem(
          dec_tmp, ten);  // Находим новый остаток от деления на десять
    }
  }

  // Создаем обычное десятичное число и копируем младшие биты из большого числа
  s21_decimal tmp = {{dec_tmp.bits[0], dec_tmp.bits[1], dec_tmp.bits[2], 0}};

  // Устанавливаем старшие биты в зависимости от экспоненты большого числа
  for (int i = 0; i < 8; i++) {
    if (dec_tmp.exp & _2(i)) tmp.bits[3] |= _2((i + 16));
  }

  // Если число отрицательное - устанавливаем знак в старшем бите результата
  if (sig) tmp.bits[3] |= _2(31);

  return tmp;  // Возвращаем преобразованное число типа s21_decimal
}

int get_bit(const s21_big_decimal decimal, const int bnum) {
  return (decimal.bits[bnum / IBS] & _2(bnum % IBS)) != 0;
}

s21_big_decimal set_bit(s21_big_decimal decimal, const int bnum) {
  decimal.bits[bnum / IBS] |= _2(bnum % IBS);
  return decimal;
}

s21_big_decimal shift_l(s21_big_decimal decimal) {
  for (int i = 5; i >= 0; i--) {
    decimal.bits[i] = decimal.bits[i]
                      << 1;  // Сдвигаем текущее слово влево на один бит
    if (i) {
      if ((decimal.bits[i - 1] & _2(31)) != 0 ? 1 : 0)
        decimal.bits[i] |= _2(0);  // Проверяем старший бит предыдущего слова
    }
  }
  return decimal;
}

s21_big_decimal shift_r(s21_big_decimal decimal) {
  for (int i = 0; i < 6; i++) {
    decimal.bits[i] =
        decimal.bits[i] >> 1;  // Сдвигаем текущее слово вправо на один бит
    if (i < 5) {
      if ((decimal.bits[i + 1] & _2(0)) != 0 ? 1 : 0)
        decimal.bits[i] |= _2(31);  // Проверяем младший бит следующего слова
    }
  }
  return decimal;  // Возвращаем модифицированное число
}

s21_big_decimal big_decimal_sum(const s21_big_decimal dec1,
                                const s21_big_decimal dec2) {
  s21_big_decimal tmp = {
      {0, 0, 0, 0, 0, 0}, 0, 0};  // Инициализация временного результата
  int overflow = 0;  // Переменная для отслеживания переполнения
  for (int i = 0; i < BL; i++) {  // Цикл по всем битам
    int b_dec1 = get_bit(dec1, i);  // Получение i-го бита первого числа
    int b_dec2 = get_bit(dec2, i);  // Получение i-го бита второго числа

    if (overflow) {            // Если есть переполнение
      if (b_dec1 && b_dec2) {  // Если оба бита равны 1
        tmp = set_bit(tmp, i);  // Устанавливаем текущий бит результата в 1
      } else if (!b_dec1 && !b_dec2) {  // Если оба бита равны 0
        tmp = set_bit(tmp, i);  // Устанавливаем текущий бит результата в 1
        overflow = 0;  // Сбрасываем переполнение
      }
    } else {                   // Если переполнения нет
      if (b_dec1 && b_dec2) {  // Если оба бита равны 1
        overflow = 1;  // Устанавливаем переполнение
      } else if (!b_dec1 && !b_dec2) {
        ;  // Ничего не делаем, текущий бит остается 0
      } else {
        tmp = set_bit(tmp, i);  // Устанавливаем текущий бит результата в 1,
                                // если один из битов равен 1
      }
    }
  }

  return tmp;  // Возвращаем результат сложения
}

s21_big_decimal big_decimal_sub(const s21_big_decimal dec1,
                                const s21_big_decimal dec2) {
  s21_big_decimal tmp = {
      {0, 0, 0, 0, 0, 0}, 0, 0};  // Инициализация временного результата
  int loan = 0;  // Переменная для отслеживания заимствования
  for (int i = 0; i < BL; i++) {  // Цикл по всем битам
    int b_dec1 = get_bit(dec1, i);  // Получение i-го бита первого числа
    int b_dec2 = get_bit(dec2, i);  // Получение i-го бита второго числа

    if (loan) {  // Если есть заимствование
      if (b_dec1 && !b_dec2) {  // Если первый бит равен 1 и второй бит равен 0
        loan = 0;  // Сбрасываем заимствование
      } else if ((!b_dec1 && !b_dec2) ||
                 (b_dec1 && b_dec2)) {  // Если оба бита равны 0 или оба равны 1
        tmp = set_bit(tmp, i);  // Устанавливаем текущий бит результата в 1
      }
    } else {  // Если заимствования нет
      if (b_dec1 && !b_dec2) {  // Если первый бит равен 1 и второй бит равен 0
        tmp = set_bit(tmp, i);  // Устанавливаем текущий бит результата в 1
      } else if (!b_dec1 &&
                 b_dec2) {  // Если первый бит равен 0 и второй бит равен 1
        tmp = set_bit(tmp, i);  // Устанавливаем текущий бит результата в 1
        loan = 1;  // Устанавливаем заимствование
      }
    }
  }

  return tmp;  // Возвращаем результат вычитания
}

s21_big_decimal big_decimal_mul(const s21_big_decimal dec1,
                                const s21_big_decimal dec2) {
  s21_big_decimal tmp = {
      {0, 0, 0, 0, 0, 0}, 0, 0};  // Инициализация временного результата
  s21_big_decimal mult = dec1;  // Копируем первое число для умножения
  for (int i = 0; i < BL / 2; i++) {  // Цикл по всем битам второй переменной
    if (get_bit(dec2, i))  // Если i-й бит второго числа равен 1
      tmp = big_decimal_sum(
          tmp, mult);  // Добавляем текущее значение mult к результату
    mult = shift_l(mult);  // Сдвигаем значение mult влево (умножаем на 2)
  }
  return tmp;  // Возвращаем результат умножения
}

s21_big_decimal big_decimal_div(const s21_big_decimal dividend,
                                const s21_big_decimal divisor) {
  s21_big_decimal quotient = {
      {0, 0, 0, 0, 0, 0}, 0, 0};  // Инициализация частного
  s21_big_decimal remainder =
      dividend;  // Копируем делимое для получения остатка
  const s21_big_decimal zero = {{0, 0, 0, 0, 0, 0}, 0, 0};  // Константа ноль
  const s21_big_decimal one = {{1, 0, 0, 0, 0, 0}, 0, 0};  // Константа единица

  // Проверяем, является ли делитель нулем
  if (compare(zero, divisor)) {
    // Если делитель не равен нулю
    if (compare(dividend, divisor) == 0 || compare(dividend, divisor) == 2) {
      // Если делимое равно делителю или меньше делителя
      if (!compare(dividend, divisor)) quotient.bits[0] = 1;  // Частное равно 1
    } else {
      // Если делимое больше делителя
      while (compare(remainder, divisor) < 2) {
        // Пока остаток меньше делителя
        s21_big_decimal divisor_tmp = divisor;  // Копируем делитель
        s21_big_decimal quotient_tmp = one;  // Инициализируем временное частное
        while (compare(remainder, divisor_tmp) < 2) {
          // Пока остаток меньше временного делителя
          quotient_tmp = shift_l(quotient_tmp);  // Сдвигаем временное частное
                                                 // влево (умножаем на 2)
          divisor_tmp = shift_l(divisor_tmp);  // Сдвигаем временный делитель
                                               // влево (умножаем на 2)
        }
        quotient_tmp = shift_r(
            quotient_tmp);  // Сдвигаем временное частное вправо (делим на 2)
        quotient = big_decimal_sum(
            quotient,
            quotient_tmp);  // Добавляем временное частное к результату
        divisor_tmp = shift_r(
            divisor_tmp);  // Сдвигаем временный делитель вправо (делим на 2)
        remainder = big_decimal_sub(
            remainder, divisor_tmp);  // Вычитаем временный делитель из остатка
      }
    }
  }

  return quotient;  // Возвращаем результат деления
}

s21_big_decimal big_decimal_rem(const s21_big_decimal dividend,
                                const s21_big_decimal divisor) {
  s21_big_decimal quotient = {
      {0, 0, 0, 0, 0, 0}, 0, 0};  // Инициализация частного
  s21_big_decimal remainder =
      dividend;  // Копируем делимое для получения остатка
  const s21_big_decimal zero = {{0, 0, 0, 0, 0, 0}, 0, 0};  // Константа ноль
  const s21_big_decimal one = {{1, 0, 0, 0, 0, 0}, 0, 0};  // Константа единица

  int cmp = compare(zero, divisor);  // Сравниваем делитель с нулем
  if (cmp) {  // Если делитель не равен нулю
    cmp = compare(dividend, divisor);  // Сравниваем делимое с делителем
    if (cmp == 0 || cmp == 2) {  // Если делимое равно делителю или больше его
      if (!cmp)
        remainder = zero;  // Если делимое равно делителю, остаток равен нулю
    } else {  // Если делимое меньше делителя
      while (cmp < 2) {  // Пока остаток меньше делителя
        s21_big_decimal divisor_tmp = divisor;  // Копируем делитель
        s21_big_decimal quotient_tmp = one;  // Инициализируем временное частное
        while (cmp < 2) {  // Пока остаток меньше временного делителя
          quotient_tmp = shift_l(quotient_tmp);  // Сдвигаем временное частное
                                                 // влево (умножаем на 2)
          divisor_tmp = shift_l(divisor_tmp);  // Сдвигаем временный делитель
                                               // влево (умножаем на 2)
          cmp =
              compare(remainder,
                      divisor_tmp);  // Сравниваем остаток с временным делителем
        }
        quotient_tmp = shift_r(
            quotient_tmp);  // Сдвигаем временное частное вправо (делим на 2)
        quotient = big_decimal_sum(
            quotient,
            quotient_tmp);  // Добавляем временное частное к результату
        divisor_tmp = shift_r(
            divisor_tmp);  // Сдвигаем временный делитель вправо (делим на 2)
        remainder = big_decimal_sub(
            remainder, divisor_tmp);  // Вычитаем временный делитель из остатка
        cmp = compare(remainder,
                      divisor);  // Обновляем сравнение остатка с делителем
      }
    }
  }

  return remainder;  // Возвращаем остаток от деления
}

s21_big_decimal to_max_exp(const s21_big_decimal decimal) {
  const s21_big_decimal ten = {{10, 0, 0, 0, 0, 0}, 0, 0};  // Константа десять
  const s21_big_decimal e18 = {
      {-1486618624, 232830643, 0, 0, 0, 0}, 0, 0};  // Константа для умножения
  const s21_big_decimal zero = {{0, 0, 0, 0, 0, 0}, 0, 0};  // Константа ноль
  s21_big_decimal dec_tmp = decimal;  // Копируем входное число для работы
  int sig = decimal.sig;  // Сохраняем знак входного числа

  // Проверяем, что число больше нуля
  if (compare(decimal, zero)) {
    int exp = dec_tmp.exp;  // Сохраняем текущую экспоненту
    dec_tmp = big_decimal_mul(dec_tmp, e18);  // Умножаем на константу e18
    dec_tmp.exp = exp + 18;  // Увеличиваем экспоненту на 18

    // Увеличиваем число до тех пор, пока количество нулевых битов больше
    // четырех
    while (num_zero_bit(dec_tmp) > 4) {
      exp = dec_tmp.exp;  // Сохраняем текущую экспоненту
      dec_tmp = big_decimal_mul(dec_tmp, ten);  // Умножаем на десять
      dec_tmp.exp = exp + 1;  // Увеличиваем экспоненту на один
    }

    dec_tmp.sig = sig;  // Восстанавливаем знак числа
  }

  return dec_tmp;  // Возвращаем преобразованное число
}

void normaliz(s21_big_decimal *dec1, s21_big_decimal *dec2) {
  int sh = dec1->exp - dec2->exp;  // Вычисляем разницу в экспонентах
  int exp, sig;
  s21_big_decimal ten = {{10, 0, 0, 0, 0, 0}, 0, 0};  // Константа десять

  // Если первое число имеет большую экспоненту
  if (sh > 0) {
    exp = dec2->exp;  // Сохраняем экспоненту второго числа
    sig = dec2->sig;  // Сохраняем знак второго числа
    for (int i = 0; i < sh; i++) {  // Увеличиваем экспоненту второго числа
      *dec2 = big_decimal_mul(*dec2, ten);  // Умножаем второе число на десять
      exp++;  // Увеличиваем экспоненту
    }
    dec2->exp = exp;  // Обновляем экспоненту второго числа
    dec2->sig = sig;  // Восстанавливаем знак второго числа
  }

  // Если второе число имеет большую экспоненту
  if (sh < 0) {
    exp = dec1->exp;  // Сохраняем экспоненту первого числа
    sig = dec1->sig;  // Сохраняем знак первого числа
    for (int i = 0; i > sh; i--) {  // Увеличиваем экспоненту первого числа
      *dec1 = big_decimal_mul(*dec1, ten);  // Умножаем первое число на десять
      exp++;  // Увеличиваем экспоненту
    }
    dec1->exp = exp;  // Обновляем экспоненту первого числа
    dec1->sig = sig;  // Восстанавливаем знак первого числа
  }
}

int compare(s21_big_decimal dec1, s21_big_decimal dec2) {
  int res = 0;  // Инициализируем результат сравнения
  for (int i = BL - 1; i >= 0 && !res;
       i--) {  // Проходим по битам с конца к началу
    int b1 = get_bit(dec1, i);  // Получаем i-й бит первого числа
    int b2 = get_bit(dec2, i);  // Получаем i-й бит второго числа
    if (b1 > b2)
      res = 1;  // Если бит первого числа больше, устанавливаем res = 1
    if (b1 < b2)
      res = 2;  // Если бит первого числа меньше, устанавливаем res = 2
  }
  return res;  // Возвращаем результат сравнения
}

int num_zero_bit(s21_big_decimal decimal) {
  int res = 0;
  for (int i = BL - 1; i >= 0 && !get_bit(decimal, i); i--) res++;
  return res;
}

// может ли большое десятичное число типа s21_big_decimal быть преобразовано без
// потери точности при переводе в другой формат
int check_rew_trans(const s21_big_decimal decimal) {
  int ret = 0, exp, sig;  // Инициализация переменных: ret для результата, exp
                          // для экспоненты, sig для знака
  const s21_big_decimal zero = {{0, 0, 0, 0, 0, 0}, 0, 0};  // Константа ноль
  const s21_big_decimal ten = {{10, 0, 0, 0, 0, 0}, 0, 0};  // Константа десять
  const s21_big_decimal e9 = {
      {1000000000, 0, 0, 0, 0, 0}, 0, 0};  // Константа для деления на миллиард
  s21_big_decimal dec_tmp = decimal;  // Копируем входное число для работы с ним
  sig = dec_tmp.sig;  // Сохраняем знак входного числа

  // Уменьшаем число до тех пор, пока количество нулевых битов меньше 67
  while (num_zero_bit(dec_tmp) < 67) {
    exp = dec_tmp.exp;  // Сохраняем текущую экспоненту
    dec_tmp = big_decimal_div(dec_tmp, e9);  // Делим на миллиард
    dec_tmp.exp = exp - 9;  // Уменьшаем экспоненту на 9
  }

  dec_tmp.sig = sig;  // Восстанавливаем знак числа

  // Проверяем количество нулевых битов после деления
  if (num_zero_bit(dec_tmp) < 96) {
    if (!dec_tmp.exp) {  // Если экспонента равна нулю
      if (dec_tmp.sig)
        ret = 2;  // Отрицательное число
      else
        ret = 1;  // Положительное число
    } else {
      while (num_zero_bit(dec_tmp) < 96 && dec_tmp.exp > 0) {
        exp = dec_tmp.exp;  // Сохраняем текущую экспоненту
        dec_tmp = big_decimal_div(dec_tmp, ten);  // Делим на десять
        dec_tmp.exp = exp - 1;  // Уменьшаем экспоненту на один
      }
      if (num_zero_bit(dec_tmp) <
          96) {  // Если количество нулевых битов все еще меньше 96
        if (decimal.sig)
          ret = 2;  // Отрицательное число
        else
          ret = 1;  // Положительное число
      }
    }
  }

  // Проверяем экспоненту на превышение допустимого значения
  if (dec_tmp.exp > 28) {
    while (dec_tmp.exp > 28 && compare(dec_tmp, zero)) {
      exp = dec_tmp.exp;
      dec_tmp = big_decimal_div(dec_tmp, ten);  // Делим на десять
      dec_tmp.exp = exp - 1;  // Уменьшаем экспоненту на один
    }
    if (dec_tmp.exp > 28)
      ret = 2;  // Если экспонента все еще больше допустимого значения
  }

  return ret;  // Возвращаем результат проверки:
               // ret == 0 - нормальное значение,
               // ret == 1 - положительное,
               // ret == 2 - отрицательное или превышение диапазона.
}

int getSign(s21_decimal value) {
  int result = !!(value.bits[3] & (1u << 31));
  return result;
}

// Функция для сброса значения s21_decimal
s21_decimal *s21_reset(s21_decimal *value) {
  if (value == NULL) {
    return NULL;  // Возвращаем NULL в случае ошибки
  }
  for (int x = 0; x < 4; x += 1) {
    value->bits[x] = 0;
  }
  return value;
}

// Функция для получения знака десятичного числа
int s21_get_sigh(s21_decimal value) {
  return (value.bits[3] & 0x80000000) ? 1 : 0;  // Проверка знакового бита
}

// Функция для установки знака десятичного числа
s21_decimal *s21_set_sigh(s21_decimal *value, int sign) {
  if (sign) {
    value->bits[3] |= 0x80000000;  // Устанавливаем знаковый бит
  } else {
    value->bits[3] &= 0x7FFFFFFF;  // Сбрасываем знаковый бит
  }
  return value;
}

void s21_set_bit(s21_decimal *decimal, int position, int value) {
  if (position < 0 || position >= 128) {
    // Неправильная позиция бита
    return;
  }

  int bit_index = position % 32;  // Индекс бита в 32-битном слове
  int word_index = position / 32;  // Индекс 32-битного слова

  if (value) {
    decimal->bits[word_index] |= (1 << bit_index);  // Устанавливаем бит в 1
  } else {
    decimal->bits[word_index] &= ~(1 << bit_index);  // Устанавливаем бит в 0
  }
}
