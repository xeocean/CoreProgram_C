#include "../s21_decimal.h"
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  s21_big_decimal res = {{0, 0, 0, 0, 0, 0}, 0, 0};
  s21_big_decimal v1 = decimal_to_big_decimal(value_1);
  s21_big_decimal v2 = decimal_to_big_decimal(value_2);
  int err = 0, exp = 0, flag = 1, cmp;
  *result = big_decimal_to_decimal(res);
  if (v1.exp > 28 || v2.exp > 28) flag = 0;
  if (flag) {
    normaliz(&v1, &v2);
    cmp = compare(v1, v2);
    exp = v1.exp;
  }
  if (v1.sig && !v2.sig && flag) {
    res = big_decimal_sum(v1, v2);
    res.sig = 1;
    flag = 0;
  }
  if (!v1.sig && v2.sig && flag) {
    res = big_decimal_sum(v1, v2);
    res.sig = 0;
    flag = 0;
  }
  if (!v1.sig && !v2.sig && flag) {
    if (cmp < 2) {
      res = big_decimal_sub(v1, v2);
      res.sig = 0;
    } else {
      res = big_decimal_sub(v2, v1);
      res.sig = 1;
    }
    flag = 0;
  }
  if (v1.sig && v2.sig && flag) {
    if (cmp == 1) {
      res = big_decimal_sub(v1, v2);
      res.sig = 1;
    } else {
      res = big_decimal_sub(v2, v1);
      res.sig = 0;
    }
  }
  err = check_rew_trans(res);
  res.exp = exp;
  if (!err) {
    *result = big_decimal_to_decimal(res);
  }
  return err;
}
