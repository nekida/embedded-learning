#include <stdio.h>

#define IN 1
#define OUT 0

int main (void)
{
    int c, state, cnt_symbols;

    state = OUT;
    cnt_symbols = 0;

    while ((c = getchar()) != EOF) {    
        if (c == ' ' || c == '\n' || c == '\t') {
            state = OUT;
            ++cnt_symbols;
        } else if (state == OUT) {
            state = IN;
        }

        if (state == IN) {
            if (cnt_symbols != 0) {
                cnt_symbols = 0;
                putchar('\n');
            }
            putchar(c);
        }
    }

    putchar('\n');

    return 0;
}
