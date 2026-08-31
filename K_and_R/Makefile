CC := gcc
CFLAGS := -std=c17 -Wall -Wextra -Wpedantic

.PHONY: build clean

build:
	@test -n "$(SRC)" || (echo "Использование: make SRC=путь/файл.c"; exit 1)
	$(CC) $(CFLAGS) "$(SRC)" -o "$(SRC:.c=.out)"

clean:
	rm -f *.out