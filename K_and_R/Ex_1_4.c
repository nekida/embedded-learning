#include <stdio.h>

int main (void)
{
    float fahr, celsius;
    int lower, upper, step;

    lower = 0;
    upper = 300;
    step = 20;

    printf ("Celsius\tFahr\n");

    celsius = lower;
    while (celsius <= upper) {
        fahr = celsius / (5.0 / 9.0) + 32.0;
        printf ("%6.1f\t%3.0f\n", celsius, fahr);
        celsius += step;
    }


    return 0;
}
