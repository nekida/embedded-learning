#include <stdio.h>

int main (void)
{
    /**
     * Одиночный '\' не печатается, печатается символ за ним.
     * При сборке будет warning "unknown escape seqence: '\q'
     * Проект соберется
     */
    printf("hello\qworld\n");

    return 0;
}
