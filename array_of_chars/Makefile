all:
	cc array_of_chars.c -o array_of_chars

san:
	gcc -std=c17 -Wall -Wextra -Wpedantic -g \
    -fsanitize=address,undefined \
    array_of_chars.c -o array_of_chars

clean:
	rm array_of_chars