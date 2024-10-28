CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
LIBS = -lcheck -lm -lpthread -lsubunit -lrt
SOURCE = tests/

all: s21_matrix.a

s21_matrix.a:
	$(CC) $(CFLAGS) -c *.c
	ar rcs s21_matrix.a s21_*.o
	ranlib s21_matrix.a

test: s21_matrix.a
	$(CC) $(CFLAGS) $(SOURCE)/*.c s21_matrix.a -o unit_test $(LIBS)
	./unit_test

gcov_report: s21_matrix.a
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(SOURCE)/*.c s21_matrix.a -o unit_test $(LIBS)
	./unit_test
	lcov -t "unit_test" -o test.info -c -d .
	genhtml -o report test.info

clean:
	rm -f *.o *.a *.gcno *.gcda *.info *.out *.gcov
	rm -rf ./report
	rm -f ./unit_test

val: s21_matrix.a
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(SOURCE)/*.c s21_matrix.a -o unit_test $(LIBS)
	valgrind --tool=memcheck --leak-check=yes ./unit_test

rebuild: clean
	$(CC) $(CFLAGS) -c *.c
	ar rcs s21_matrix.a s21_*.o
	ranlib s21_matrix.a

.PHONY: all s21_matrix.a test gcov_report clean val rebuild
