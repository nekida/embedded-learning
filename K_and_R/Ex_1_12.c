#include <stdio.h>

#define IN 1
#define OUT 0

#define FIRST_WORD 1
#define NOT_FIRST_WORD 0

int main (void)
{
    int c, state, cnt_symbols, state_word;

    state = OUT;
    cnt_symbols = 0;
    state_word = NOT_FIRST_WORD;

    while ((c = getchar()) != EOF) {    
        if (c == ' ' || c == '\n' || c == '\t') {
            if (state_word == FIRST_WORD)
                ++cnt_symbols;
            state = OUT;
        } else if (state == OUT) {
            state = IN;
            state_word = FIRST_WORD;
        }

        if (state == IN) {
            if (cnt_symbols != 0) {
                cnt_symbols = 0;
                putchar('\n');
            }
            putchar(c);
        }
    }

    return 0;
}
