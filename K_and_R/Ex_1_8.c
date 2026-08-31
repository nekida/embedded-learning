#include <stdio.h>

int main (void)
{
    int c, cnt_space, cnt_tab, cnt_newline;
    c = cnt_space = cnt_tab = cnt_newline = 0;
    while ((c = getchar()) != EOF) {    
        if (c == ' ')
            ++cnt_space;
        else if (c == '\t')
            ++cnt_tab;
        else if (c == '\n')
            ++cnt_newline;
    }
    printf ("Num space: %d\n", cnt_space);
    printf ("Num tabulations: %d\n", cnt_tab);
    printf ("Num symbols of new line: %d\n", cnt_newline);

    return 0;
}
