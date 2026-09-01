#include <stdio.h>

#define IN 1
#define OUT 0

int main (void)
{
    int c, state;

    state = OUT;

    while ((c = getchar()) != EOF) {    
        if (c == ' ' || c == '\n' || c == '\t')
            state = OUT;
        else if (state == OUT) {
            state = IN;
        }

        if (state == IN) {
            putchar(c);
        } else {
            putchar('\n');
        }
    }

    return 0;
}
