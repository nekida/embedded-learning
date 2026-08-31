#include <stdio.h>

int main (void)
{
    int c, cnt_space;
    c = cnt_space = 0;
    while ((c = getchar()) != EOF) {    
        if (c == ' ')
            ++cnt_space;
        else if (cnt_space != 0) {
            cnt_space = 0;
            putchar(' ');
            putchar(c);
        } else
            putchar(c);
        
    }

    if (cnt_space != 0)
        putchar(' ');

    return 0;
}
