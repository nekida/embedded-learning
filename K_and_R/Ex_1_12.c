#include <stdio.h>

#define IN 1
#define OUT 0

#define WORD_FOUND 1
#define NO_WORD_YET 0

int main (void)
{
    int c, state, cnt_symbols, state_word;

    state = OUT;
    cnt_symbols = 0;
    state_word = NO_WORD_YET;

    while ((c = getchar()) != EOF) {    
        if (c == ' ' || c == '\n' || c == '\t') {
            if (state_word == WORD_FOUND)
                ++cnt_symbols;
            state = OUT;
        } else if (state == OUT) {
            state = IN;
            state_word = WORD_FOUND;
        }

        if (state == IN) {
            if (cnt_symbols != 0) {
                cnt_symbols = 0;
                putchar('\n');
            }
            putchar(c);
        }
    }

    if (state_word == WORD_FOUND)
        putchar('\n');

    return 0;
}
