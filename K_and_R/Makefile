CC := gcc
FLAGS := -std=c17 -Wall -Wextra -Wpedantic

build:
	@test -n "$(SRC)" || (echo "Использование: make SRC=путь/файл.c"; exit 1)
	$(CC) $(FLAGS) "$(SRC)" -o "$(SRC:.c=.out)"

clean:
	rm -f *.out