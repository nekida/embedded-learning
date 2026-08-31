all:
	cc array_of_chars.c -o array_of_chars.out

san:
	gcc -std=c17 -Wall -Wextra -Wpedantic -g \
    -fsanitize=address,undefined \
    array_of_chars.c -o array_of_chars.out

clean:
	rm array_of_chars.out