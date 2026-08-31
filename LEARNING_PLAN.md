# План развития: Embedded / System C / C++ / Linux

Цель этого плана — построить автономную программу обучения, которая не зависит от ChatGPT, конкретного онлайн-курса или постоянного доступа к интернету.

Основная идея:

> Не читать книги подряд «от корки до корки», а использовать их как источники для конкретных тем, которые сразу закрепляются маленькими экспериментами в репозитории `embedded-learning`.

---

## 1. Карьерная траектория

Текущее направление:

```text
Embedded C / STM32 / FreeRTOS / периферия / отладка железа
```

Целевая траектория:

```text
Strong C
    ↓
Computer Systems
    ↓
RTOS / concurrency
    ↓
Modern C++
    ↓
Linux system programming
    ↓
Embedded Linux / BSP / drivers / kernel
```

При этом не терять сильные стороны embedded-разработчика:

- микроконтроллеры и периферия;
- SPI / I2C / UART / CAN / Ethernet;
- FreeRTOS;
- загрузчики, DMA;
- отладка на железе;
- осциллограф и логический анализатор;
- понимание схемотехники;
- опыт с FPGA и SoC.

---

## 2. Основной метод обучения

Для каждой темы использовать цикл:

```text
1. Прочитать небольшой фрагмент
2. Сформулировать вопрос
3. Написать минимальный пример
4. Собрать с предупреждениями
5. Запустить sanitizer / debugger / objdump при необходимости
6. Сформулировать вывод своими словами
7. Сохранить код и README в embedded-learning
```

Тема считается закреплённой, если можно:

- объяснить её без книги;
- написать небольшой пример;
- предсказать результат;
- объяснить диагностику компилятора / sanitizer / debugger;
- применить знание в рабочем коде.

---

## 3. Инструменты по умолчанию

Обычная учебная сборка C:

```bash
gcc -std=c17 \
    -Wall \
    -Wextra \
    -Wpedantic \
    -Wconversion \
    -Wshadow \
    -g \
    program.c \
    -o program
```

Для экспериментов с памятью и UB:

```bash
gcc -std=c17 \
    -Wall \
    -Wextra \
    -Wpedantic \
    -g \
    -fsanitize=address,undefined \
    program.c \
    -o program
```

Дополнительные инструменты:

```text
gdb
readelf
objdump
nm
size
strings
make
git
```

Для Linux активно использовать `man`:

```bash
man 2 open
man 2 mmap
man 2 socket
man 3 printf
man 7 signal
man 7 tcp
```

---

## 4. Структура репозитория

Создавать каталоги по мере появления экспериментов:

```text
embedded-learning/
├── README.md
├── LEARNING_PLAN.md
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
├── toolchain/
│   ├── preprocessing/
│   ├── compiler/
│   ├── assembly/
│   ├── object-files/
│   ├── elf/
│   ├── linker/
│   ├── linker-scripts/
│   └── sanitizers/
├── architecture/
│   ├── stack/
│   ├── calling-convention/
│   ├── cache/
│   ├── memory-hierarchy/
│   ├── arm/
│   └── exceptions/
├── rtos/
│   ├── scheduler/
│   ├── tasks/
│   ├── queues/
│   ├── semaphores/
│   ├── mutexes/
│   ├── priority-inversion/
│   └── race-conditions/
├── linux/
│   ├── files/
│   ├── processes/
│   ├── signals/
│   ├── mmap/
│   ├── threads/
│   ├── ipc/
│   └── sockets/
├── cpp/
│   ├── raii/
│   ├── ownership/
│   ├── move-semantics/
│   ├── smart-pointers/
│   └── concurrency/
└── networking/
    ├── ethernet/
    ├── arp/
    ├── ipv4/
    ├── udp/
    └── tcp/
```

---

# 5. Фаза 1 — укрепление C

## Основные книги

### K&R — «Язык программирования C»

Использовать как компактный фундамент. Не читать механически. На каждой теме задавать вопросы:

- какой тип имеет выражение;
- где находится объект;
- каков его lifetime;
- какой у него storage duration;
- где массив преобразуется в указатель;
- что вычисляет `sizeof`;
- где возможно UB.

### Robert Seacord — «Эффективный C»

Основной современный источник по корректности C.

Особенно внимательно:

- типы;
- целочисленные преобразования;
- указатели;
- массивы;
- объекты;
- память;
- undefined behavior;
- интерфейсы функций;
- стандартная библиотека.

## Темы, которые нужно закрыть

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
- declaration vs definition;
- translation unit;
- `static`;
- `extern`;
- integer promotions;
- usual arithmetic conversions;
- signed / unsigned;
- overflow;
- shifts;
- aliasing;
- alignment;
- object representation;
- `memcpy`;
- structs / unions / padding;
- undefined / unspecified / implementation-defined behavior.

## Уже начатый эксперимент

```c
char a[] = {'a', 'b', 'c'};
char b[] = {'a', 'b', 'c', '\0'};
char c[] = "abc";
char *p = "abc";
const char *cp = "abc";
```

Исследовать:

```c
sizeof a
sizeof b
sizeof c
sizeof p
```

Проверить:

- `%s` для массива без `\0`;
- AddressSanitizer;
- строковый литерал;
- `const`;
- array vs pointer.

## Критерий завершения фазы

Можно без подсказки объяснить:

```text
array
pointer
object
lifetime
storage duration
scope
linkage
const
string literal
integer promotion
UB
```

и написать для каждой темы минимальный пример.

---

# 6. Фаза 2 — Computer Systems

## Главная книга

### Bryant, O'Hallaron — «Компьютерные системы. Архитектура и программирование» (CS:APP)

Главная цель — связать:

```text
C
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
machine code
↓
CPU
↓
memory
↓
OS
```

## Основные темы

- представление целых чисел;
- floating point;
- assembly;
- регистры;
- вызовы функций;
- стек;
- ABI;
- object files;
- symbols;
- relocations;
- linking;
- ELF;
- static/dynamic libraries;
- memory hierarchy;
- cache;
- virtual memory;
- processes;
- exceptions;
- signals;
- I/O.

## Практика

Для программы:

```c
static int x;
static int y = 10;
static const char *p = "hello";

int foo(int a)
{
    return a + y;
}
```

использовать:

```bash
gcc -c test.c -o test.o
nm test.o
readelf -S test.o
readelf -s test.o
objdump -d test.o
objdump -s test.o
```

Ответить:

- где находится `x`;
- где находится `y`;
- где строковый литерал;
- где символ `foo`;
- какие секции присутствуют;
- какие symbols имеют local/global binding;
- какие relocations создаются.

---

# 7. Фаза 3 — Архитектура компьютера и ARM

## Книги

### Patterson / Hennessy

Основные темы:

- ISA;
- datapath;
- pipeline;
- hazards;
- cache;
- memory hierarchy;
- performance;
- parallelism.

### Jim Ledin

Использовать как современную обзорную карту ARM / RISC-V / MMU / multicore.

### Petzold — «Код»

Читать без обязательного конспекта для связывания уровней:

```text
логика → память → регистры → ALU → CPU → инструкции → программа
```

## ARM Cortex-M

Отдельно по документации ARM изучить:

- registers;
- MSP / PSP;
- exception model;
- NVIC;
- exception priorities;
- exception entry / return;
- stack frame;
- EXC_RETURN;
- HardFault;
- MemManage;
- BusFault;
- UsageFault;
- SCB;
- `DMB`, `DSB`, `ISB`;
- MPU.

## Практика

- посмотреть ассемблер простой функции;
- проследить вызов функции;
- посмотреть prologue/epilogue;
- понять, где сохраняется return address;
- намеренно вызвать fault на STM32;
- снять fault registers;
- восстановить место падения;
- посмотреть stack frame.

---

# 8. Фаза 4 — RTOS и concurrency

## Источники

- официальная документация FreeRTOS;
- Mastering the FreeRTOS Real Time Kernel;
- Таненбаум — соответствующие главы ОС.

## Темы

- task states;
- scheduler;
- preemption;
- priorities;
- tick;
- blocking;
- `vTaskDelay`;
- queues;
- semaphores;
- mutexes;
- priority inversion;
- priority inheritance;
- event groups;
- task notifications;
- critical sections;
- ISR API;
- race conditions;
- atomicity;
- deadlock;
- starvation;
- producer/consumer.

## Практика

Для каждого механизма:

1. минимальный пример;
2. намеренная ошибка;
3. исправленный вариант;
4. объяснение timing.

Особенно закрепить:

> `volatile` не является механизмом синхронизации потоков и не исправляет race condition.

---

# 9. Фаза 5 — Linux system programming

## Основные книги

### Michael Kerrisk — «Linux API»

Основной учебник.

### Stevens / Rago — APUE

Второй источник и справочник.

## Темы

### Файлы

- file descriptor;
- `open`, `read`, `write`, `close`;
- `lseek`;
- `stat`;
- blocking/non-blocking I/O.

### Процессы

- process;
- PID;
- `fork`;
- `exec`;
- `wait`;
- exit status.

### Сигналы

- `sigaction`;
- signal mask;
- async-signal-safe functions.

### Память

- virtual memory;
- `mmap`;
- shared memory.

### IPC

- pipe;
- FIFO;
- shared memory;
- message queues;
- sockets.

### Threads

- pthread;
- mutex;
- condition variable;
- race;
- deadlock.

### Сеть

- sockets;
- TCP;
- UDP;
- client/server;
- blocking I/O;
- `select` / `poll` / `epoll`.

## Практические проекты

1. TCP echo server.
2. Multi-client server: один клиент → threads → `poll/epoll`.
3. `mmap()` файла.
4. Корректное завершение процесса по `SIGINT`.

---

# 10. Фаза 6 — современный C++

Цель — рабочий системный C++, а не отказ от C.

## Темы

- references;
- constructors/destructors;
- RAII;
- object lifetime;
- copy semantics;
- move semantics;
- Rule of 0/3/5;
- `std::unique_ptr`;
- `std::shared_ptr`;
- `std::weak_ptr`;
- `std::vector`;
- `std::array`;
- `std::string`;
- `std::span`;
- lambdas;
- templates;
- `constexpr`;
- strong enums;
- exceptions;
- `std::thread`;
- mutex;
- condition variable;
- atomics.

## Старые книги C++

Scott Meyers (`Effective C++`, `More Effective C++`) использовать концептуально и выборочно. Помнить, что существенная часть материала относится к C++03.

## Практика

Переписать несколько небольших C-модулей в C++ только там, где появляется конкретная польза:

- ownership;
- RAII;
- type safety;
- encapsulation;
- testability.

---

# 11. Фаза 7 — Embedded Linux

После базового Linux system programming.

## Темы

- cross compilation;
- sysroot;
- toolchain;
- device tree;
- boot process;
- U-Boot;
- Buildroot;
- Yocto;
- kernel configuration;
- character devices;
- platform driver;
- device tree bindings;
- interrupts;
- GPIO;
- I2C;
- SPI;
- DMA;
- sysfs;
- debugfs;
- procfs;
- `dmesg`;
- kernel/userspace interfaces.

## Практический маршрут

```text
user-space GPIO
↓
simple kernel module
↓
character driver
↓
platform driver
↓
device tree
↓
real hardware
```

---

# 12. Networking

## Книги

- Таненбаум / Уэзеролл;
- Олифер / Олифер.

## Темы

- Ethernet frame;
- MAC;
- ARP;
- IPv4;
- ICMP;
- UDP;
- TCP;
- ports;
- checksum;
- routing;
- DHCP;
- DNS;
- MTU;
- fragmentation;
- TCP handshake;
- retransmission;
- flow control;
- congestion control.

## Практика

Использовать:

```bash
tcpdump
wireshark
```

Для собственного STM32/LwIP устройства пройти путь:

```text
Ethernet
↓
IP
↓
TCP/UDP
↓
LwIP
↓
application protocol
```

---

# 13. FPGA / DSP / электроника

Это вторичные направления. Не включать в основной маршрут, пока они не нужны по работе.

## FPGA

- Verilog;
- synchronous logic;
- FSM;
- timing;
- clock domains;
- CDC;
- FIFOs;
- MCU ↔ FPGA interfaces.

## DSP

Использовать Lyons при задачах:

- ADC;
- sampling;
- FFT;
- FIR;
- IIR;
- spectral analysis.

## Электроника

Поддерживать понимание:

- pull-up / pull-down;
- RC;
- ADC input;
- op-amp basics;
- level shifting;
- signal integrity;
- ground;
- power;
- noise;
- oscilloscope measurements.

---

# 14. Какие книги не читать подряд

Не нужно последовательно проходить все общие учебники C:

```text
K&R
Prata
Deitel
Head First C
Schildt
Подбельский
Кольцов
...
```

Основной маршрут по C:

```text
K&R + Seacord
```

Остальные книги использовать как:

- справочник;
- альтернативное объяснение;
- источник упражнений;
- вторую точку зрения.

---

# 15. Роль книг из библиотеки

## Основное ядро

```text
K&R
Seacord — Effective C
CS:APP
Patterson / Hennessy
Kerrisk — Linux API
```

## Второй эшелон

```text
Ledin
Tanenbaum — Operating Systems
Tanenbaum — Networks
Stevens / Rago — APUE
Amini — Extreme C
Kernighan / Pike — Practice of Programming
```

## Читать свободно

```text
Petzold — Code
Steinhart — Secret Life of Programs
Pragmatic Programmer
Mythical Man-Month
Refactoring
```

## Использовать выборочно

```text
Prata
Deitel
Head First C
Schildt
Подбельский
Кольцов
Design Patterns
старые Meyers C++
```

---

# 16. Шаблон README для эксперимента

```markdown
# Название темы

## Вопрос

Что я хочу проверить?

## Теория

Коротко своими словами.

## Код

Минимальный пример.

## Сборка

Команда GCC / Makefile.

## Ожидаемый результат

Что должно произойти и почему.

## Фактический результат

Что произошло.

## Диагностика

GCC / ASan / UBSan / GDB / objdump / readelf.

## Вывод

Что гарантирует язык или система.

## Ошибочная модель

Как я понимал это раньше.

## Правильная модель

Как следует понимать теперь.
```

README не должен быть большим ради объёма. Главное — точность.

---

# 17. Режим занятий

Не превращать обучение во вторую работу.

## Будни

```text
30–60 минут
```

Одна маленькая тема или один эксперимент.

## Выходной

```text
1–2 часа
```

Более крупная тема или оформление README.

Лучше:

```text
2 хорошо понятые темы в неделю
```

чем:

```text
200 страниц книги без практики
```

---

# 18. Правило остановки для упражнений

Если задача не решается:

```text
0–20 минут
    самостоятельная попытка

20–40 минут
    трассировка, debugger, бумага

после ~40 минут
    небольшая подсказка или просмотр решения
```

После просмотра решения:

1. закрыть источник;
2. объяснить идею своими словами;
3. написать решение заново;
4. придумать дополнительный тест.

Цель — обучение, а не доказательство способности бесконечно сидеть над одной задачей.

---

# 19. Как использовать AI

AI — дополнительный инструмент, а не источник истины.

```text
сначала собственная попытка
↓
вопрос
↓
объяснение
↓
проверка экспериментом
↓
README своими словами
```

Для спорных вопросов C проверять несколько источников: стандарт/документацию, compiler diagnostics, Seacord, эксперимент.

---

# 20. Автономная работа без интернета

Локально сохранить:

- ARM Cortex-M documentation;
- reference manuals STM32;
- datasheets используемых MCU;
- FreeRTOS documentation / book;
- GCC manuals;
- GDB documentation;
- Linux man-pages;
- datasheets периферии и микросхем;
- необходимые документы по протоколам.

Репозиторий `embedded-learning` должен существовать минимум в двух местах:

```text
рабочая машина
+
резервная копия / другой диск
```

GitHub не должен быть единственной копией.

---

# 21. Контрольные точки

Раз в месяц ответить себе без книги.

## C

- Чем массив отличается от указателя?
- Что такое object lifetime?
- Что делает `static` на file scope?
- Что такое UB?
- Какие integer promotions происходят в выражении?
- Что возвращает `sizeof`?

## Toolchain

- Что происходит от `.c` до ELF?
- Что такое symbol?
- Что такое relocation?
- Чем `.data` отличается от `.bss`?
- Для чего linker script?

## CPU

- Как вызывается функция?
- Где return address?
- Что происходит при exception entry Cortex-M?
- Что такое cache line?

## RTOS

- Чем mutex отличается от semaphore?
- Что такое priority inversion?
- Почему `volatile` не исправляет race condition?

## Linux

- Что такое file descriptor?
- Чем `fork()` отличается от `exec()`?
- Что такое `mmap()`?
- Чем process отличается от thread?
- Как работает blocking socket?

Если ответ получается расплывчатым — тема возвращается в backlog.

---

# 22. Приоритет на ближайшее время

Не пытаться одновременно изучать всё.

```text
1. K&R
2. Effective C
3. embedded-learning experiments
4. CS:APP
5. Cortex-M / architecture
6. RTOS
7. Linux system programming
8. C++
9. Embedded Linux
```

Петцольда и другие обзорные книги можно читать параллельно для удовольствия.

---

# 23. Что не является целью

Не требуется:

- помнить весь стандарт C;
- решить каждое упражнение K&R без подсказки;
- прочитать всю библиотеку;
- знать все регистры STM32 наизусть;
- самостоятельно написать компилятор;
- знать Linux kernel целиком;
- стать одновременно специалистом по FPGA, DSP, Linux, C++, сетям и электронике.

Цель:

> Иметь прочный фундамент и уметь быстро разбираться в новой конкретной системе.

---

# 24. Итоговая стратегия

Главный путь:

```text
глубокий C
    ↓
понимание toolchain
    ↓
понимание CPU и памяти
    ↓
RTOS / concurrency
    ↓
Linux system programming
    ↓
modern C++
    ↓
Embedded Linux / kernel / drivers
```

Главный метод:

```text
книга
→ вопрос
→ код
→ эксперимент
→ инструментальная проверка
→ README
→ повторение
```

Главный критерий прогресса:

> Не количество прочитанных страниц, а количество вещей, которые можно точно объяснить, проверить и применить.
