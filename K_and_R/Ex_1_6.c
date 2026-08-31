#include <stdio.h>

int main (void)
{
    /**
     * Двойной вывод "Result operation:  ..." вызван обработкой введенного символа + 'Enter'
     */
    int c = getchar();
    int res_oper = (c != EOF);
    while (res_oper != 0) {
        printf ("Result operation: %d\n", res_oper);

        putchar(c);
        putchar('\n');
        
        c = getchar();

        res_oper = (c != EOF);
    }
    printf ("Result operation: %d\n", res_oper);

    return 0;
}
