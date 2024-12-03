CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
LIBS_INSTALL = -lncurses
LIBS_TEST = -lcheck -lm -lpthread -lsubunit -lrt

BACKEND = brick_game/tetris
FRONTEND = gui/cli
TEST = tests
BUILD_DIR = build

SRC_BACKEND = $(wildcard $(BACKEND)/*.c)
SRC_FRONTEND = $(wildcard $(FRONTEND)/*.c)
SRC_TEST = $(wildcard $(TEST)/*.c)
SRC_MAIN = tetris.c

OBJ_BACKEND = $(SRC_BACKEND:.c=.o)
OBJ_FRONTEND = $(SRC_FRONTEND:.c=.o)
OBJ_MAIN = $(SRC_MAIN:.c=.o)

# Цели
all: install

install: tetris
	mkdir -p $(BUILD_DIR)
	mv tetris $(BUILD_DIR)

uninstall:
	rm -rf $(BUILD_DIR)

backend_lib: $(OBJ_BACKEND)
	ar rcs libbackend.a $(OBJ_BACKEND)
	@rm $(OBJ_BACKEND)

frontend_lib: $(OBJ_FRONTEND)
	ar rcs libfrontend.a $(OBJ_FRONTEND)
	@rm $(OBJ_FRONTEND)

$(OBJ_BACKEND): $(SRC_BACKEND)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_FRONTEND): $(SRC_FRONTEND)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_MAIN): $(SRC_MAIN)
	$(CC) $(CFLAGS) -c $< -o $@

tetris: backend_lib frontend_lib $(OBJ_MAIN)
	$(CC) $(CFLAGS) $(OBJ_MAIN) libbackend.a libfrontend.a -o tetris $(LIBS_INSTALL)
	@rm $(OBJ_MAIN)

clean:
	rm -f $(OBJ_BACKEND) $(OBJ_FRONTEND) $(OBJ_MAIN) libbackend.a libfrontend.a tetris
	rm -f *.o *.a *.gcno *.gcda *.info *.out *.gcov *.txt *.gz
	rm -rf ./report
	rm -f ./unit_test
	rm -rf ./build
	rm -rf ./docs

test: backend_lib
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(SRC_TEST) $(SRC_BACKEND) libbackend.a -o unit_test $(LIBS_INSTALL) $(LIBS_TEST)
	./unit_test

gcov_report: test
	lcov -t "unit_test" -o test.info -c -d .
	genhtml -o report test.info
	open report/index.html

dvi:
	doxygen ./Doxyfile
	open docs/html/index.html

dist: install
	@rm -rf dist.tar.gz
	tar -cvzf dist.tar.gz build/tetris

.PHONY: all install uninstall clean dvi dist test gcov_report