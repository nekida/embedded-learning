all:
	cc -std=c17 -Wall -Wextra -Wpedantic array_of_chars.c -o array_of_chars.out

san:
	gcc -std=c17 -Wall -Wextra -Wpedantic -g \
    -fsanitize=address,undefined \
    array_of_chars.c -o array_of_chars.out

clean:
	rm -f array_of_chars.out