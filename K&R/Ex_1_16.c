#include <stdio.h>

#define MAXLINE (5)

static int get_line(char s[], int lim)
{
    int c, i;

    for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i) {
        s[i] = c;
    }
    if (c == '\n') {
        s[i] = c;
        ++i;
    }
    s[i] = '\0';

    return i;
}

void copy(char to[], char from[])
{
    int i = 0;
    while ((to[i] = from[i]) != '\0') {
        ++i;
    }
}

int main (void)
{
    int len;
    int max = 0;
    int max2 = 0;
    char line[MAXLINE];
    char longest[MAXLINE];

    while((len = get_line(line, MAXLINE)) > 0) {
        if (len == MAXLINE) {
            max2 += len;
        } else if (len > max) {
            max = len;
            copy(longest, line);
        }
    }

    if (max2 > 0)
        printf ("Len of big string: %d", max2);

    if (max > 0)
        printf ("%s", longest);

    return 0;
}