#include <stdio.h>

#define IN 1
#define OUT 0

#define LEN_WORD (10)

int main (void)
{
    int c, state, cnt_chars, histogram[LEN_WORD];

    state = OUT;
    cnt_chars = 0;
    for (int i = 0; i < LEN_WORD; ++i)
        histogram[i] = 0;

    while ((c = getchar()) != EOF) {    
        if (c == ' ' || c == '\n' || c == '\t') {
            state = OUT;
        } else {
            ++cnt_chars;
            state = IN;
        }
        
        if (state == OUT && cnt_chars != 0) {
            if (cnt_chars >= LEN_WORD)
                ++histogram[LEN_WORD - 1];
            else
                ++histogram[cnt_chars];
            cnt_chars = 0;
        }
    }

    if (state == IN && cnt_chars != 0) {
        if (cnt_chars >= LEN_WORD)
            ++histogram[LEN_WORD - 1];
        else
            ++histogram[cnt_chars];
    }

    for (int i = 1; i < LEN_WORD; ++i) {
        printf ("%d - ", histogram[i]);
        for (int j = 0; j < histogram[i]; ++j)
            printf ("*");
        printf ("\n");
    }

    putchar('\n');

    return 0;
}
