#include "../s21_decimal.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  s21_big_decimal res = {{0, 0, 0, 0, 0, 0}, 0, 0};
  s21_big_decimal v1 = decimal_to_big_decimal(value_1);
  s21_big_decimal v2 = decimal_to_big_decimal(value_2);
  int err = 0, flag = 1;
  if ((!compare(v1, res) || !compare(v2, res)) && (v1.sig || v2.sig)) {
    res.sig = 1;
    flag = 0;
  }
  *result = big_decimal_to_decimal(res);
  if (flag && v1.exp < 29 && v2.exp < 29) {
    int exp = v1.exp + v2.exp;
    int sig;
    if ((v1.sig && v2.sig) || (!v1.sig && !v2.sig))
      sig = 0;
    else
      sig = 1;
    res = big_decimal_mul(v1, v2);
    res.exp = exp;
    res.sig = sig;
    err = check_rew_trans(res);
    if (!err) *result = big_decimal_to_decimal(res);
  }
  return err;
}