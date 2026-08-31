#include <stdio.h>

/**
 * '\0' нужен, если массив должен использоваться как C-строка.
 *
 * Без завершающего '\0' передавать такой массив функциям,
 * ожидающим null-terminated string, например printf("%s"),
 * strlen(), strcpy() и т. п., нельзя.
 *
 * Они могут прочитать данные за границами массива, что приводит
 * к undefined behavior.
 *
 * Само объявление массива без '\0' совершенно корректно,
 * если использовать его именно как массив char.
 */
static char array1_1[] = {'a', 'b', 'c'};
static char array1_2[] = {'a', 'b', 'c', '\0'};

/**
 * '\0' ставится автоматически
 */
static char array2[] = "abc";

/**
 * p указывает на строковый литерал.
 * Изменение строкового литерала приводит к undefined behavior.
 *
 * В C объявление char *p = "abc" допустимо, однако это не делает
 * строковый литерал изменяемым.
 *
 * На практике следует использовать:
 * const char *p = "abc";
 * Тогда попытка изменить литерал через p будет диагностирована
 * компилятором.
 */
static char * p = "abc";

int main (void)
{
    // Undefined behavior: %s ожидает C-строку.
    // При поиске '\0' printf прочитает данные за границами array1_1.
    printf("array1 without '\\0': ");
    printf("%s\n", array1_1);

    printf("array1 with '\\0': ");
    printf("%s\n", array1_2);

    printf("array2: ");
    printf("%s\n", array2);

    printf("p: ");
    printf("%s\n", p);

    /**
     * sizeof обычно определяется на этапе компиляции и не вычисляет свой операнд.
     * Исключение — случаи, связанные с массивами переменной длины (VLA), размер которых определяется во время выполнения.
     */

    // Тип array1_1 — char[3].
    // sizeof(char) по определению языка C равен 1,
    // поэтому sizeof(array1_1) == 3.
    printf("sizeof array1 without '\\0': ");
    printf("%zu\n", sizeof(array1_1));

    // 3 символа + нуль-терминатор = 4 байта
    printf("sizeof array1 with '\\0': ");
    printf("%zu\n", sizeof(array1_2));

    // 3 символа + нуль-терминатор = 4 байта
    printf("sizeof array2: ");
    printf("%zu\n", sizeof(array2));

    // sizeof(p) — размер указателя.
    // На моей x86-64 Fedora результат равен 8 байтам.
    printf("sizeof p: ");
    printf("%zu\n", sizeof(p));

    array1_2[0] = 'x';
    array1_2[1] = 'y';
    array1_2[2] = 'z';
    
    printf("Modified array1: ");
    printf("%s\n", array1_2);
    
    array2[0] = 'x';
    array2[1] = 'y';
    array2[2] = 'z';

    printf("Modified array2: ");
    printf("%s\n", array2);

    /**
     * Модификация строкового литерала — undefined behavior.
     *
     * На моей Fedora этот пример завершается Segmentation fault:
     * в данной реализации строковый литерал размещён в памяти,
     * недоступной для записи.
     *
     * Стандарт C не гарантирует Segmentation fault:
     * результат undefined behavior в общем случае не определён.
     */
    p[0] = 'x';
    p[1] = 'y';
    p[2] = 'z';

    printf("Modified p: ");
    printf("%s\n", p);

    return 0;
}
