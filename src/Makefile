OS = $(shell uname)
ifeq ("$(OS)", "Linux")
    BUILD_FILE_TMP = Maze
    BUILD_FILE = Maze
    OPEN = xdg-open
    LEAKS_RUN = valgrind --track-origins=yes ./test
else
    BUILD_FILE_TMP = Maze.app
    BUILD_FILE = Maze.app
    OPEN = open
    LEAKS_RUN = CK_FORK=no leaks -atExit -- ./test
endif

FLAGS = -Wall -Wextra -Werror -std=c++17
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
CC = g++

#-------------------------------------------------------
#    Расположение каталога
#-------------------------------------------------------

BUILD_DIR = ../build
DIST_DIR = ../"Maze"
DVI_DIR = ../"doc_Maze"
REPORT_DIR = ../report

#-------------------------------------------------------
#    Переменная для создания каталогов
#-------------------------------------------------------

BUILD = @mkdir -p $(BUILD_DIR)
DIST = @mkdir -p $(DIST_DIR)
REPORT = @mkdir -p $(REPORT_DIR)

#-------------------------------------------------------
#    dist name
#-------------------------------------------------------

DIST_NAME = $(DIST_DIR).tar.gz

#-------------------------------------------------------
#    Цели
#-------------------------------------------------------

all: install

install:
	$(BUILD)
	@cd $(BUILD_DIR) && cmake ../src -B . && cmake --build .

uninstall:
	rm -rf $(BUILD_DIR)

dvi:
	makeinfo -o $(DVI_DIR) --html --no-warn --force ./Maze_info.texi
	$(OPEN) $(DVI_DIR)/index.html

dist:
	$(DIST)
	cp -r Makefile main.cc CMakeLists.txt ./controller ./model ./source ./view $(DIST_DIR)
	tar -cvzf $(DIST_NAME) $(DIST_DIR)
	rm -rf $(DIST_DIR)

tests:
	$(CC) ./tests/tests.cc ./model/maze.cc ./model/cave.cc -o test -lgtest -pthread $(GCOV_FLAGS) $(FLAGS)
	./test

gcov_report: tests
	$(REPORT)
	gcovr -r . --html --html-details -o report/report.html
	$(MAKE) clean_src
	$(OPEN) ./report/report.html

google_style:
	find . -type f \( -name "*.cc" -o -name "*.h" \) -exec clang-format -verbose -style=google -i {} +

clang:
	find . -type f \( -name "*.cc" -o -name "*.h" \) -exec clang-format -verbose -style=google -n {} +

cppcheck:
	find . -type f \( -name "*.cc" -o -name "*.h" \) -exec cppcheck --language=c++ --enable=all --suppress=missingIncludeSystem {} +

leaks:tests
	$(LEAKS_RUN)
	$(MAKE) clean_src

clean_src:
	rm -rf $(BUILD_DIR)
	rm -rf $(DIST_DIR)
	rm -rf $(BUILD_DIR)
	rm -rf $(DIST_NAME)
	rm -rf $(DVI_DIR)
	rm -rf *.gcda *.gcno *.gcov

clean: clean_src
	rm -rf report test .clang-format

.PHONY: all tests clean
