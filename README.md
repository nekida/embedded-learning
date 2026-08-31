# embedded-learning

Учебный репозиторий для систематизации и углубления знаний по **C, embedded-разработке, архитектуре компьютеров, RTOS, Linux, C++ и системному программированию**.

Основной принцип репозитория:

> Не просто читать теорию, а проверять каждую важную идею небольшим воспроизводимым экспериментом.

Репозиторий используется как личная база знаний: код, заметки, диагностические отчёты, примеры работы компилятора, санитайзеров, отладчика и системных инструментов.

Подробный порядок обучения находится в [`LEARNING_PLAN.md`](./LEARNING_PLAN.md).

---

## Цель

Развивать знания по траектории:

```text
Embedded C / STM32 / FreeRTOS
              ↓
           Strong C
              ↓
       Computer Systems
              ↓
      RTOS / Concurrency
              ↓
         Modern C++
              ↓
 Linux System Programming
              ↓
Embedded Linux / BSP / Drivers / Kernel
```

При этом сохранять и углублять практические embedded-навыки:

- ARM Cortex-M;
- STM32;
- FreeRTOS;
- GPIO / UART / SPI / I2C / CAN / Ethernet;
- DMA;
- загрузчики;
- работа с регистрами;
- отладка на железе;
- взаимодействие MCU ↔ FPGA;
- сетевые протоколы;
- Linux / SoC.

---

# Метод обучения

Каждая тема проходит примерно такой цикл:

```text
теория
  ↓
вопрос
  ↓
минимальный пример
  ↓
компиляция
  ↓
эксперимент
  ↓
GCC / ASan / UBSan / GDB / objdump / readelf
  ↓
вывод своими словами
  ↓
README
```

Тема не считается изученной только потому, что прочитана соответствующая глава книги.

Хороший критерий понимания:

- могу объяснить тему без источника;
- могу написать минимальный пример;
- могу предсказать результат;
- понимаю диагностику компилятора или runtime-инструмента;
- могу объяснить, что гарантирует язык, ABI, ОС или архитектура;
- могу применить знание в реальном коде.

---

# Инструменты

Основная среда:

```text
Linux
GCC
GDB
Make
Git
```

Для анализа программ также используются:

```text
AddressSanitizer
UndefinedBehaviorSanitizer
readelf
objdump
nm
size
strings
man
```

## Базовая сборка C

```bash
gcc -std=c17 \
    -Wall \
    -Wextra \
    -Wpedantic \
    -Wconversion \
    -Wshadow \
    -g \
    example.c \
    -o example
```

## Сборка с санитайзерами

```bash
gcc -std=c17 \
    -Wall \
    -Wextra \
    -Wpedantic \
    -g \
    -fsanitize=address,undefined \
    example.c \
    -o example
```

---

# Структура репозитория

Структура расширяется постепенно — каталог создаётся только тогда, когда появляется соответствующая тема или эксперимент.

```text
embedded-learning/
├── README.md
├── LEARNING_PLAN.md
│
├── K_and_R/
│   └── ...
│
├── c/
│   ├── arrays/
│   ├── strings/
│   ├── pointers/
│   ├── const/
│   ├── storage-duration/
│   ├── lifetime/
│   ├── scope-linkage/
│   ├── integer-conversions/
│   ├── undefined-behavior/
│   ├── structs-unions/
│   └── memory/
│
├── toolchain/
│   ├── preprocessing/
│   ├── compiler/
│   ├── assembly/
│   ├── object-files/
│   ├── elf/
│   ├── linker/
│   ├── linker-scripts/
│   └── sanitizers/
│
├── architecture/
│   ├── stack/
│   ├── calling-convention/
│   ├── cache/
│   ├── memory-hierarchy/
│   ├── arm/
│   └── exceptions/
│
├── rtos/
│   ├── scheduler/
│   ├── tasks/
│   ├── queues/
│   ├── semaphores/
│   ├── mutexes/
│   ├── priority-inversion/
│   └── race-conditions/
│
├── linux/
│   ├── files/
│   ├── processes/
│   ├── signals/
│   ├── mmap/
│   ├── threads/
│   ├── ipc/
│   └── sockets/
│
├── cpp/
│   ├── raii/
│   ├── ownership/
│   ├── move-semantics/
│   ├── smart-pointers/
│   └── concurrency/
│
└── networking/
    ├── ethernet/
    ├── arp/
    ├── ipv4/
    ├── udp/
    └── tcp/
```

---

# Основные направления

## C

Основной текущий приоритет.

Темы:

- arrays vs pointers;
- C strings;
- string literals;
- `sizeof`;
- `const`;
- pointer to const / const pointer;
- storage duration;
- lifetime;
- scope;
- linkage;
- `static`;
- `extern`;
- integer promotions;
- signed / unsigned;
- alignment;
- aliasing;
- object representation;
- structs / unions / padding;
- `memcpy`;
- Undefined Behavior;
- unspecified behavior;
- implementation-defined behavior.

Основные источники:

- **Kernighan & Ritchie — The C Programming Language**;
- **Robert Seacord — Effective C**.

---

## Toolchain

Цель — понимать путь программы:

```text
source.c
   ↓
preprocessor
   ↓
compiler
   ↓
assembly
   ↓
object file
   ↓
linker
   ↓
ELF
   ↓
execution
```

Темы:

- preprocessing;
- compilation;
- assembly;
- object files;
- symbols;
- relocations;
- ELF;
- static / dynamic linking;
- linker scripts;
- sections `.text`, `.rodata`, `.data`, `.bss`.

Инструменты:

```bash
gcc
nm
readelf
objdump
size
```

---

## Computer Architecture

Темы:

- ISA;
- registers;
- stack;
- calling convention;
- memory hierarchy;
- cache;
- pipeline;
- exceptions;
- interrupts;
- ARM Cortex-M;
- memory barriers.

Основные источники:

- **Bryant & O'Hallaron — Computer Systems: A Programmer's Perspective (CS:APP)**;
- **Patterson & Hennessy**;
- **Jim Ledin**;
- документация ARM.

---

## RTOS / Concurrency

Темы:

- scheduler;
- task states;
- priorities;
- preemption;
- queues;
- semaphores;
- mutexes;
- race conditions;
- critical sections;
- priority inversion;
- priority inheritance;
- ISR interaction;
- deadlock;
- starvation.

Основной практический объект — **FreeRTOS**.

---

## Linux System Programming

Темы:

- file descriptors;
- `open/read/write/close`;
- processes;
- `fork/exec/wait`;
- signals;
- `mmap`;
- virtual memory;
- pthreads;
- IPC;
- sockets;
- `select/poll/epoll`.

Основные источники:

- **Michael Kerrisk — The Linux Programming Interface**;
- **Stevens / Rago — Advanced Programming in the UNIX Environment**.

---

## Modern C++

Темы:

- RAII;
- object lifetime;
- constructors / destructors;
- copy / move semantics;
- Rule of 0/3/5;
- smart pointers;
- STL containers;
- lambdas;
- templates;
- `constexpr`;
- type safety;
- threads;
- mutexes;
- atomics.

Цель — не «писать всё классами», а использовать C++ там, где он реально улучшает:

- ownership;
- resource management;
- type safety;
- encapsulation;
- testability.

---

## Embedded Linux

Дальнейшее направление после Linux system programming.

Темы:

- cross compilation;
- sysroot;
- Buildroot;
- Yocto;
- U-Boot;
- Device Tree;
- kernel configuration;
- character devices;
- platform drivers;
- GPIO / I2C / SPI;
- interrupts;
- kernel ↔ userspace interfaces.

---

## Networking

Темы:

```text
Ethernet
ARP
IPv4
ICMP
UDP
TCP
DHCP
DNS
routing
MTU
```

Практическая цель — уметь пройти путь данных:

```text
Ethernet frame
      ↓
IP packet
      ↓
TCP/UDP
      ↓
network stack
      ↓
application protocol
```

---

# Формат учебного эксперимента

Для законченной темы желательно иметь:

```text
topic/
├── README.md
├── example.c
└── Makefile
```

Пример структуры `README.md`:

```markdown
# Название темы

## Вопрос

Что именно проверяется?

## Теория

Краткое объяснение своими словами.

## Код

Минимальный воспроизводимый пример.

## Сборка

Команда GCC или Makefile.

## Ожидаемый результат

Что должно произойти?

## Фактический результат

Что произошло?

## Диагностика

GCC / ASan / UBSan / GDB / objdump / readelf.

## Вывод

Что гарантирует язык или система?

## Ошибочная модель

Как тема понималась раньше?

## Правильная модель

Как её следует понимать?
```

---

# Пример: массивы символов и C-строки

Один из первых экспериментов:

```c
char a[] = {'a', 'b', 'c'};
char b[] = {'a', 'b', 'c', '\0'};
char c[] = "abc";
char *p = "abc";
const char *cp = "abc";
```

Исследуются:

```c
sizeof a;
sizeof b;
sizeof c;
sizeof p;
```

а также:

- массив `char` vs C-строка;
- строковый литерал;
- массив vs указатель;
- `const`;
- выход за границы объекта;
- Undefined Behavior;
- AddressSanitizer.

Пример намеренной ошибки:

```c
char a[] = {'a', 'b', 'c'};

printf("%s\n", a);
```

`a` является корректным массивом `char`, но не C-строкой.

При запуске с AddressSanitizer такая ошибка может быть обнаружена как:

```text
AddressSanitizer: global-buffer-overflow
```

---

# Упражнения K&R

Решения упражнений хранятся отдельно.

Цель упражнений — не просто получить работающий код, а:

1. самостоятельно понять условие;
2. написать первую версию;
3. проверить граничные случаи;
4. использовать debugger / sanitizer при необходимости;
5. после решения сформулировать, чему именно научило упражнение.

Если упражнение не решается слишком долго, допускается посмотреть небольшую подсказку или решение, но после этого желательно:

1. закрыть источник;
2. объяснить идею своими словами;
3. написать код заново;
4. добавить собственные тесты.

---

# Работа с Undefined Behavior

В репозитории намеренно могут находиться программы с UB.

Такие примеры должны быть явно отмечены:

```c
/*
 * Intentional Undefined Behavior.
 * Используется только для демонстрации диагностики.
 */
```

Важно:

```text
Undefined Behavior != Segmentation fault
```

UB означает:

> стандарт языка больше не определяет поведение программы.

Падение программы — лишь один из возможных результатов.

---

# Принципы

## Не запоминать код без модели

Плохо:

> «так принято писать».

Лучше:

> «почему именно этот код корректен?»

---

## Не доверять работающей программе как доказательству корректности

Программа может:

```text
скомпилироваться
+
запуститься
+
показать ожидаемый результат
```

и всё равно содержать Undefined Behavior.

---

## Проверять предположения инструментами

Если можно проверить утверждение через:

```text
compiler
sanitizer
debugger
disassembler
ELF tools
```

— проверить.

---

## Различать уровни гарантий

Всегда полезно понимать, откуда происходит конкретное свойство:

```text
C standard
compiler
ABI
operating system
CPU architecture
specific MCU
library implementation
```

Например:

```text
sizeof(char) == 1
```

— гарантия C.

А:

```text
sizeof(void *) == 8
```

на конкретной x86-64 Linux-системе — свойство платформы/ABI, а не универсальное правило C.

---

# Источники

Основной набор литературы:

### C

- Kernighan & Ritchie — *The C Programming Language*;
- Robert Seacord — *Effective C*;
- Kamran Amini — *Extreme C*;
- Ben Klemens — *21st Century C*.

### Computer Systems / Architecture

- Bryant & O'Hallaron — *Computer Systems: A Programmer's Perspective*;
- Patterson & Hennessy;
- Jim Ledin;
- Charles Petzold — *Code*.

### Operating Systems / Linux

- Michael Kerrisk — *The Linux Programming Interface*;
- Stevens / Rago — *Advanced Programming in the UNIX Environment*;
- Andrew Tanenbaum — *Modern Operating Systems*.

### Networks

- Tanenbaum / Wetherall;
- Олифер / Олифер.

### Software Engineering

- Kernighan / Pike — *The Practice of Programming*;
- Martin Fowler — *Refactoring*;
- *The Pragmatic Programmer*.

Полный учебный маршрут и приоритеты: [`LEARNING_PLAN.md`](./LEARNING_PLAN.md).

---

# Что не является целью

Репозиторий не ставит задачей:

- прочитать все имеющиеся книги;
- решить каждое упражнение без единой подсказки;
- запомнить стандарт C;
- знать все регистры STM32 наизусть;
- изучать одновременно C, C++, Linux, FPGA, DSP и сети;
- собирать как можно больше учебных примеров ради количества.

Цель другая:

> постепенно построить системную модель от языка C до железа и операционной системы и уметь подтверждать эту модель практикой.

---

# Статус

Репозиторий находится в активной разработке.

Текущий основной фокус:

```text
C fundamentals
+
K&R exercises
+
memory / arrays / strings
+
Undefined Behavior
+
GCC / sanitizers
```

Следующий крупный этап:

```text
Computer Systems / CS:APP
```

---

# Главный критерий прогресса

Не количество прочитанных страниц и не количество файлов в репозитории.

А количество вещей, про которые можно уверенно сказать:

> Я понимаю, почему это работает, могу воспроизвести это в минимальном примере и могу проверить своё объяснение инструментами.
