#define _2(bit) (1U << bit)
#define IBS (unsigned char)(sizeof(int) * 8)
#define BL IBS * 6
#define MAX_BIT 32
#define MAXIMUM 2147483648
#define MINIMUM -2147483648
#define EXP 0x00ff0000
#define TOTAL_BYTES 4
#define MIN_BITS 0x80000000
#define MAX_EXP 28
#define MAX_FLOAT 79228162514264337593543950335.0f
#define MIN_FLOAT \
  0.00000000000000000000000000010000000031710768509710513471352647538147514756461109f

typedef struct {
  int bits[4];
} s21_decimal;

typedef struct {
  unsigned int bits[6];
  int exp;
  int sig;  // [0 + ] [1 - ]
} s21_big_decimal;

// Получение статуса произвольного бита
int get_bit(const s21_big_decimal decimal, const int bnum);

// Проверка на возможность преобразования в decimal
// 0-можно 1-большое 2-маленькое
int check_rew_trans(const s21_big_decimal decimal);

// Сравнение без учета знака и экспонент
// (0 =) (1 dec1 > dec2) (2 dec1 < dec2)
int compare(s21_big_decimal dec1, s21_big_decimal dec2);

// Количество нулевых старших бит
int num_zero_bit(s21_big_decimal decimal);

// Установка произвольного бита
s21_big_decimal set_bit(s21_big_decimal decimal, const int bnum);

// Сброс произвольного бита
s21_big_decimal reset_bit(s21_big_decimal decimal, const int bnum);

// Преобразование базового decimal в расширенную структуру
// для удобства дальнейшей обработки
s21_big_decimal decimal_to_big_decimal(const s21_decimal decimal);

// Преобразование s21_big_decimal в s21_decimal
// предварительно необходимо сделать проверку check_rew_trans
s21_decimal big_decimal_to_decimal(const s21_big_decimal decimal);

// Сдвиг влево
s21_big_decimal shift_l(s21_big_decimal decimal);

// Сдвиг вправо
s21_big_decimal shift_r(s21_big_decimal decimal);

// Суммирование служебное без знака и учета экспонент
s21_big_decimal big_decimal_sum(const s21_big_decimal dec1,
                                const s21_big_decimal dec2);

// Вычитание служебное без знака и учета экспонент
s21_big_decimal big_decimal_sub(const s21_big_decimal dec1,
                                const s21_big_decimal dec2);

// Умножение служебное без знака и учета экспонент
s21_big_decimal big_decimal_mul(const s21_big_decimal dec1,
                                const s21_big_decimal dec2);

// Деление целочисленное служебное без знака без остатка и учета экспонент
s21_big_decimal big_decimal_div(const s21_big_decimal dividend,
                                const s21_big_decimal divisor);

// Остаток целочисленного деления служебное без знака без остатка и учета
// экспонент
s21_big_decimal big_decimal_rem(const s21_big_decimal dividend,
                                const s21_big_decimal divisor);

// Уравнивание экспоненты
void normaliz(s21_big_decimal *dec1, s21_big_decimal *dec2);

// Максимальное увеличение экспоненты
s21_big_decimal to_max_exp(const s21_big_decimal decimal);

// Распечатать содержимое структуры
void print_wdec(const s21_big_decimal dec);

// Распечатать содержимое структуры
void print_dec(const s21_decimal dec);

// //доп. функции
s21_decimal *s21_set_sigh(s21_decimal *value, int sign);
int s21_get_sigh(s21_decimal value);
s21_decimal *s21_reset(s21_decimal *value);

int s21_get_scale(s21_decimal value);
int getSign(s21_decimal value);
void s21_set_bit(s21_decimal *decimal, int position, int value);

int find_bit(s21_decimal value, int bit);

s21_decimal anull_decimal();

int set_scale(s21_decimal *num, unsigned value);
