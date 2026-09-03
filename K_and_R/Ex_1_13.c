#include <stdio.h>

#define IN 1
#define OUT 0

int main (void)
{
    int c, state, cnt_chars, histogram[10];

    state = OUT;
    cnt_chars = 0;
    for (int i = 0; i < 10; ++i)
        histogram[i] = 0;

    while ((c = getchar()) != EOF) {    
        if (c == ' ' || c == '\n' || c == '\t') {
            state = OUT;
        } else {
            ++cnt_chars;
            state = IN;
        }
        
        if (state == OUT && cnt_chars != 0) {
            ++histogram[cnt_chars];
            cnt_chars = 0;
        }
    }

    if (state == IN && cnt_chars != 0) {
        ++histogram[cnt_chars];
    }

    for (int i = 0; i < 10; ++i)
        printf ("%d ", histogram[i]);

    putchar('\n');

    return 0;
}
