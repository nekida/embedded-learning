# Аудит знаний и план развития embedded-инженера

Версия: 28 августа 2026 года  
Профиль: Никита Шестаков, STM32 / C / FreeRTOS / Linux / Jetson

## 1. Что именно было проанализировано

Аудит составлен по доступной истории этого проекта, ответам на диагностические вопросы, реальным рабочим разборам и приложенным материалам:

- резюме с опытом STM32F373/F407/F746/G4, FreeRTOS, LwIP, CAN/UART/SPI, DMA, bootloader, Linux kernel modules, Jetson и аппаратной отладкой;
- ответы по языку C, модулям, компиляции, ELF, startup, undefined behavior, `volatile`, прерываниям, RTOS и Cortex-M;
- рабочие вопросы по `const`, `static`, `extern`, `__common__`, массивам, `sizeof`, `memcpy`, DMA, SPI, PLL, VTOR, HardFault, FreeRTOS и организации модулей;
- прежние планы `embedded_architect_working_plan`, `embedded_architect_master_learning_plan` и `embedded_architect_roadmap`;
- roadmap Embedded Systems Engineering и ссылки на GitHub/блог;
- опыт, уже подтверждённый реальными результатами: DMA-библиотека, FIFO с IRQ/DMA и тестами, linker scripts, bootloader, FSM, LwIP, BLE/LoRa, Linux kernel module и Jetson bring-up.

Это не перечень всего, что существует в embedded. Тема попала в основной аудит только при наличии одного из признаков:

1. в диагностике был неверный или неполный ответ;
2. один и тот же вопрос возникал в реальном коде несколько раз;
3. технология уже применялась, но знания выглядят фрагментарными;
4. тема нужна для заявленной цели — более сильной и лучше оплачиваемой embedded/firmware-роли.

## 2. Главный вывод

Текущий уровень нельзя описывать как «не знает embedded». Есть большой практический опыт и несколько результатов уровня сильного middle: аппаратный bring-up, DMA/FIFO, тесты, bootloader, протоколы, RTOS, Linux kernel space, схемотехника и работа с измерительными приборами.

Проблема другая: знания неравномерны. Практический навык часто есть, но базовая модель под ним вспоминается неточно. Поэтому знакомая задача решается, а объяснение механизма, перенос решения на другую платформу или диагностика редкого сбоя требуют лишнего времени.

Наиболее важные четыре узла:

1. **C как системный язык:** время жизни, владение памятью, `const`, UB, `volatile`, атомарность.
2. **Путь от исходника до исполнения:** toolchain, ABI, ELF, linker, startup, секции памяти.
3. **Cortex-M + FreeRTOS:** исключения, HardFault, планировщик, ISR, приоритеты, совместные данные.
4. **Архитектура и проверяемость:** границы модулей, владение ресурсом, FSM, error model, host tests.

Это и есть обязательный ближайший маршрут. Остальное — направления развития, а не срочные «дыры».

## 3. Обозначения

| Метка | Значение |
|---|---|
| **П0 — подтвердить базу** | Был неправильный ответ или заметная путаница. Нужны теория и маленькая лабораторная работа. |
| **П1 — систематизировать** | Есть практический опыт, но модель неполная или знание нестабильно. |
| **П2 — развить** | Не мешает нынешней работе, но нужно для следующей роли или специализации. |
| **Не приоритет** | Полезная область, но сейчас отдача ниже, чем затраты времени. |

## 4. Что уже является сильной стороной

- практическая работа со STM32 нескольких семейств и периферией;
- UART, CAN, SPI, I2C, ADC/DAC, PWM, timers, Ethernet/LwIP;
- аппаратная отладка осциллографом и логическим анализатором;
- разработка загрузчика, CRC, обновление по Ethernet;
- DMA и FIFO с IRQ/DMA, включая тестирование;
- FreeRTOS в реальных проектах;
- протоколы и конечные автоматы в изделиях;
- BLE HID, LoRa, Qt-инструменты для управления устройствами;
- Linux user space, kernel modules, cross-compilation, Netlink;
- Jetson/MIPI CSI/device-tree bring-up;
- командная разработка, code review, GitLab, gtest/gcov/lcov, Docker, Doxygen;
- схемотехника, монтаж, КД и взаимодействие с изготовителем плат.

Поэтому не нужны общие курсы «STM32 с нуля», полный повтор GPIO/UART/SPI или очередная большая книга обо всех микроконтроллерах.

## 5. Полная карта пробелов

### 5.1. Язык C и модель памяти

| Приоритет | Русский термин | English term | Что именно нужно закрыть |
|---|---|---|---|
| **П0** | Время жизни объекта / длительность хранения | Object lifetime / storage duration | Автоматические, статические и динамические объекты; почему нельзя возвращать указатель на локальный массив; висячий указатель; отличие времени жизни от области видимости. |
| **П0** | Владение и заимствование памяти | Ownership / borrowing | Кто владеет буфером, кто только временно использует его; срок действия переданного указателя; API вида `pointer + length`; выходной параметр. |
| **П0** | Константность | Const-correctness | `const T *`, `T * const`, `const T * const`; `const` в интерфейсе модуля; копирование конфигурации во внутреннее `static`-состояние. |
| **П1** | Область видимости, связывание и длительность хранения | Scope, linkage, storage duration | `static`, `extern`, tentative definition, common symbol; внутреннее и внешнее связывание; почему `__common__` не является нормальным способом проектирования API. |
| **П0** | Неопределённое поведение | Undefined behavior, UB | Выход за границы, use-after-lifetime, uninitialized read, signed overflow, неверный format string, нарушение выравнивания и strict aliasing; почему `-O0` и `-O2` ведут себя по-разному. |
| **П0** | Порядок вычислений | Evaluation order / sequencing | Не только старые «sequence points», но sequenced/unsequenced; почему `func(x++, x += 2)` содержит не просто неопределённый порядок аргументов, а конфликтующие неупорядоченные изменения. |
| **П0** | Изменяемая извне переменная | `volatile` | Что гарантирует observable access и чего не гарантирует: атомарность, взаимное исключение, порядок между потоками, целостность сложного объекта. |
| **П0** | Атомарность и модель памяти | Atomicity / memory model | Атомарная операция, data race, critical section, C11 atomics на host; связь с main/ISR/RTOS. |
| **П1** | Барьеры памяти и переупорядочивание | Memory barriers / reordering | Compiler barrier, DMB, DSB, ISB; когда они нужны для MMIO, DMA и смены системной конфигурации; не применять «на всякий случай». |
| **П1** | Представление данных | Data representation | Integer promotions, signed/unsigned comparison, сдвиги, overflow, `uint32_t`, padding, alignment, endianness. |
| **П1** | Битовые поля, объединения и упаковка | Bit-fields, unions, packing | Маски и сдвиги как основной переносимый вариант; ограничения bit-fields и `packed`; type punning и strict aliasing; сериализация отдельно от layout структуры. |
| **П1** | Массивы и указатели | Arrays and pointers | Array-to-pointer decay, `sizeof(array)` против `sizeof(pointer)`, многомерные массивы, передача длины. |
| **П1** | Библиотечные функции памяти и форматирования | Memory/string/formatted I/O functions | `memcpy`, `memset`, `snprintf`, `sscanf`, `inttypes.h`, NULL-контракты, размеры буферов. |
| **П1** | Контракт функции | Function contract | Допустимые аргументы, предусловия, постусловия, владение, коды ошибок; сигнатура функции — только часть контракта. |

**Основание оценки:** хорошо распознаны `static/extern/.bss` и пример UB, но были вопросы по комбинациям `const`, возврату локального массива, `__common__`, `sizeof`, `memcpy`; описание `volatile` было неполным, а барьеры памяти оставались неизвестными.

### 5.2. Компиляция, ABI, ELF, linker и startup

| Приоритет | Русский термин | English term | Что именно нужно закрыть |
|---|---|---|---|
| **П0** | Этапы сборки | Build pipeline | Препроцессор → компилятор → ассемблер → компоновщик; отдельная компиляция translation units. В диагностике ассемблер и linker были переставлены местами. |
| **П0** | Объектный файл, исполняемый файл, бинарный образ | Object file, ELF executable, binary image | `.o`, `.elf`, `.hex`, `.bin`; sections, symbol table, relocations, debug info; `objcopy`. Общая модель уже есть, нужны детали и инструменты. |
| **П0** | Секции программы | Program sections | `.text`, `.rodata`, `.data`, `.bss`, stack, heap, custom sections; `.data` копируется из Flash в RAM, `.bss` не копируется, а обнуляется. |
| **П0** | Стартовый код | Startup code | Vector table, initial SP, `Reset_Handler`, копирование `.data`, обнуление `.bss`, `SystemInit`, constructors C++, `main`. |
| **П1** | Скрипт компоновщика и карта памяти | Linker script / memory map | `MEMORY`, `SECTIONS`, VMA/LMA, linker symbols, `KEEP`, alignment; чтение `.map`; собственная секция metadata. Скрипты уже писались — требуется восстановить модель. |
| **П0** | Двоичный интерфейс приложений | ABI — Application Binary Interface | Calling convention, регистры аргументов/результата, caller-saved/callee-saved, stack alignment, structure passing, name mangling. Не путать с C-сигнатурой функции. |
| **П0** | Обычный вызов функции | Function call | `BL`, `LR`, prologue/epilogue, stack frame; обычный вызов функции не является переключением контекста. |
| **П1** | Символы и разрешение имён | Symbols and symbol resolution | Local/global/weak/common/undefined symbols; relocations; duplicate definition; static library linking order. |
| **П1** | Оптимизация и межмодульная оптимизация | Optimization / LTO | Влияние `-O0/-Og/-O2`, inlining, dead code elimination, debug/release difference; читать disassembly, а не угадывать. |
| **П1** | Система сборки и кросс-компиляция | Build system / cross-compilation | Include paths вместо длинных `../../..`; Make/CMake targets; toolchain file; различие host tests и firmware build. |

### 5.3. Cortex-M, прерывания, fault handling и загрузка

| Приоритет | Русский термин | English term | Что именно нужно закрыть |
|---|---|---|---|
| **П0** | Режим потока и режим обработчика | Thread mode / Handler mode | Чем обычный код отличается от ISR; привилегии; MSP/PSP; регистр `CONTROL`. |
| **П0** | Вход и выход из исключения | Exception entry / return | Автоматически сохраняемые `R0-R3`, `R12`, `LR`, `PC`, `xPSR`; `EXC_RETURN`; где искать stacked PC. |
| **П0** | Контроллер вложенных векторных прерываний | NVIC — Nested Vectored Interrupt Controller | Priority grouping, preemption priority, subpriority, nesting, tail chaining, late arrival; численно меньшее значение означает более высокий приоритет. |
| **П1** | Маскирование прерываний | Interrupt masking | `PRIMASK`, `BASEPRI`, `FAULTMASK`; короткая critical section; почему глобальный `__disable_irq()` не всегда лучший ответ. |
| **П0** | Исключения ошибок | Fault exceptions | MemManage, BusFault, UsageFault, HardFault escalation; precise/imprecise bus fault. |
| **П0** | Регистры диагностики аварий | Fault status registers | `CFSR`, `HFSR`, `MMFAR`, `BFAR`, stacked PC/LR; поиск строки через `.elf`, `addr2line` и disassembly. |
| **П1** | Таблица векторов и её перемещение | Vector table / VTOR relocation | Bootloader → application, корректный MSP, VTOR, отключение IRQ/DMA/SysTick и очистка pending state перед jump. |
| **П1** | Система тактирования | Clock tree | HSI/HSE/PLL/VCO, PLLM/N/P/Q/R, bus prescalers, peripheral clocks, Flash wait states; проверка диапазонов по reference manual. |
| **П1** | Защита памяти | MPU — Memory Protection Unit | Что означает принудительная изоляция модулей: права и области памяти, privileged/unprivileged execution. Для F4 — понять концепцию, не строить сейчас полноценную sandbox-систему. |

**Основание оценки:** прерывание было верно названо асинхронным, но механика exception frame, fault registers и различие function call/context switch были неполными. В практических разборах оставались вопросы по регистрам HardFault, PLL/VCO, VTOR и переходу загрузчика.

### 5.4. FreeRTOS, конкурентность и реальное время

| Приоритет | Русский термин | English term | Что именно нужно закрыть |
|---|---|---|---|
| **П0** | Состояния задачи | Task states | Running, Ready, Blocked, Suspended; почему delay переводит задачу в Blocked, а yield только отдаёт текущий квант. |
| **П0** | Планирование и переключение контекста | Scheduling / context switch | Приоритеты, time slicing, tick, PendSV, сохранение контекста задачи; `taskYIELD()` не гарантирует запуск конкретной другой задачи. |
| **П0** | API из обработчика | FromISR API | `xQueueSendFromISR`, `vTaskNotifyGiveFromISR`, `portYIELD_FROM_ISR`; флаг `higherPriorityTaskWoken`; ограничения приоритетов IRQ относительно `configMAX_SYSCALL_INTERRUPT_PRIORITY`. |
| **П0** | Очередь, семафор, мьютекс, уведомление | Queue, semaphore, mutex, task notification | Выбор примитива по смыслу: данные, событие, владение ресурсом; mutex имеет priority inheritance, binary semaphore — нет. |
| **П0** | Инверсия приоритетов | Priority inversion | Low/medium/high scenario, priority inheritance, bounded blocking; отличие от deadlock и starvation. |
| **П0** | Совместные данные main/ISR/task/DMA | Shared data | Atomicity, race condition, critical section, snapshot, single owner, queue, double buffer; `volatile` недостаточно. |
| **П1** | Владение устройством одной задачей | Single-owner task | Один device task владеет UART/SPI-сессией; остальные посылают команды; сериализация запроса и ответа. |
| **П1** | Контроль ресурсов RTOS | Runtime resource diagnostics | Stack high-water mark, stack overflow hook, heap statistics, queue-full counters, timer task, idle task, watchdog feeding model. |
| **П1** | Анализ реального времени | Real-time analysis | Deadline, latency, jitter, WCET, blocking time, missed deadline; измерение GPIO/trace вместо ощущения «быстро/медленно». |

### 5.5. DMA, буферы и периферийные транзакции

| Приоритет | Русский термин | English term | Что именно нужно закрыть |
|---|---|---|---|
| **П0** | Жизненный цикл DMA-передачи | DMA transfer lifecycle | Настройка source/destination/length, start, half-transfer, transfer-complete, error, abort; почему без IRQ асинхронный HAL API не завершает программный протокол. |
| **П0** | Кольцевой и двойной буфер | Ring buffer / double buffering | Producer/consumer indexes, overflow policy, snapshot, ownership; ISR/DMA пишет один буфер, parser обрабатывает другой. |
| **П1** | Согласованность кэша и DMA | Cache coherency | На M4 обычно нет D-cache, но на M7/Jetson проблема обязательна: clean перед DMA-read, invalidate после DMA-write, cache-line alignment. |
| **П1** | Транзакция SPI | SPI transaction | CS boundaries, command/address/data phases, full-duplex RX discard, prescaler, blocking/interrupt/DMA modes, timeout and error recovery. |
| **П1** | Пропускная способность и задержка | Throughput / latency | Измерять время и количество байт отдельно; учитывать частоту SPI, overhead HAL, переключение CS, размер чанка и паузы flash/FPGA. |
| **П1** | UART RX DMA с обнаружением простоя | UART RX DMA with IDLE line | Circular DMA, current write position, IDLE event, ring buffer, overflow counter, parser outside ISR. |

Периферия GPIO/ADC/DAC/PWM/timers, базовый SPI/UART/CAN и работа с приборами не являются общими пробелами. Нужна не очередная вводная лекция, а точечные лаборатории по транзакциям и конкурентности.

### 5.6. Архитектура, модули и моделирование поведения

| Приоритет | Русский термин | English term | Что именно нужно закрыть |
|---|---|---|---|
| **П0** | Модуль и сокрытие информации | Module / information hiding | Публичный контракт против приватного состояния; заголовочный файл не должен раскрывать ненужную структуру реализации. |
| **П0** | Интерфейс и реализация | Interface / implementation | Интерфейс — не только прототипы, но типы, ownership, thread-safety, допустимые состояния, ошибки и timing guarantees. |
| **П1** | Связанность и сцепление | Coupling / cohesion | Высокая внутренняя связность модуля и низкая зависимость между модулями; избегать `global.h` и общего изменяемого состояния. |
| **П1** | Инверсия/внедрение зависимостей | Dependency inversion / dependency injection | `transport_if`, mock transport, platform adapter; бизнес-логика компилируется без HAL. |
| **П1** | Разделение слоёв | Layering / separation of concerns | Application → device API → device task → driver → protocol → transport → platform; не превращать схему в догму, применять к одному сложному устройству. |
| **П0** | Конечный автомат | FSM — Finite-State Machine | State, event, transition, guard, action, entry/exit action; явная таблица переходов и тесты недопустимых переходов. |
| **П2** | Иерархический автомат | HSM — Hierarchical State Machine | Superstate, общие переходы, entry/exit; нужен только когда плоская FSM действительно разрастается. |
| **П1** | Модель ошибок | Error model | Recoverable/non-recoverable error, warning/alarm/fault, retry policy, safe state, counters, сохранение причины reset. |
| **П1** | Модель потоков | Threading model | Кто владеет ресурсом, откуда приходят события, где допустима блокировка, какой контекст вызывает API. |
| **П2** | Архитектурное решение | ADR — Architecture Decision Record | Контекст, варианты, решение, последствия; фиксировать только важные trade-offs, а не каждый `if`. |
| **П2** | Принудительная изоляция | Enforced isolation | MPU/MMU, processes, address spaces, privilege boundaries; это объясняет, почему модульность в C сама по себе не защищает память. |

**Основание оценки:** интуитивное понимание модуля, интерфейса и абстракции есть. Формальные элементы FSM, enforced isolation, ABI и системные гарантии были неполными. Значит, не нужна большая теория архитектуры — нужны точные термины и применение к одному рабочему модулю.

### 5.7. Тестирование, отладка и инженерная инфраструктура

| Приоритет | Русский термин | English term | Что именно нужно закрыть |
|---|---|---|---|
| **П1** | Модульный тест на хосте | Host unit test | Parser, command builder, state machine и error conversion собирать на PC без STM32/HAL. Этот опыт уже был — нужно вернуть его в текущую практику. |
| **П1** | Заглушка, имитация, шпион | Stub, fake, mock, spy | Различать по назначению; создавать test seam на границе transport/time/GPIO. |
| **П1** | Регрессионное тестирование и покрытие | Regression testing / coverage | Coverage — индикатор, не цель; важнее ошибки границ, malformed input, timeout, overflow, invalid transition. |
| **П0** | Отладка GDB/OpenOCD | GDB/OpenOCD debugging | Registers, memory, disassembly, breakpoint/watchpoint, backtrace, remote target; `addr2line`, `objdump`, `readelf`, `nm`, `size`. |
| **П1** | Статический и динамический анализ | Static/dynamic analysis | `-Wall -Wextra -Wconversion -Wshadow`, cppcheck/clang-tidy; ASan/UBSan на host tests. Не обещать, что анализатор докажет корректность firmware. |
| **П1** | Непрерывная интеграция | CI — Continuous Integration | Firmware build, host tests, warnings/static analysis, сохранение `.elf/.hex/.map`, git hash. |
| **П1** | CMake и цели сборки | CMake targets | Target-based include/definitions/options, host target и cross target, toolchain file; не переписывать CubeIDE-проект целиком в первую неделю. |
| **П1** | Эксплуатационная диагностика | Runtime diagnostics / observability | HardFault dump, reset cause, queue-full, parser/timeout counters, stack/heap metrics, version/git hash, debug command. |

### 5.8. Направления развития, а не срочные пробелы

#### Современный C++17

Статус: **П2**, после основной 16-недельной программы.

- ссылки и константность — references and const-correctness;
- жизненный цикл объекта — object lifetime;
- конструкторы/деструкторы — constructors/destructors;
- RAII — захват ресурса при инициализации;
- правило нуля/пяти — Rule of Zero/Five;
- стандартные контейнеры и алгоритмы — STL containers/algorithms;
- `enum class`, `constexpr`, templates;
- умные указатели и явное владение — smart pointers/ownership;
- cost model: allocation, exceptions, RTTI, code size;
- C/C++ boundaries и name mangling.

#### Embedded Linux / Jetson / Yocto

Статус: **П2**, перспективная специализация после восстановления базы.

- процессы, потоки, виртуальная память, системные вызовы;
- cross toolchain, root filesystem, packages;
- Buildroot и Yocto Project;
- driver model, kernel modules, device tree;
- V4L2, media controller graph, MIPI CSI;
- `dmesg`, ftrace/trace-cmd, live device tree;
- воспроизводимый bring-up guide.

#### Технический английский

Статус: **П2**, фоновая привычка, а не отдельный тяжёлый курс. Уровень A2 ограничивает самостоятельное чтение ARM/ST/FreeRTOS/Linux documentation. Цель — не разговорная беглость, а чтение datasheet/reference manual без полного перевода.

Режим: 10–15 новых терминов в неделю из реально прочитанного документа; определение по-русски, оригинальная фраза и пример использования. ИИ можно использовать для перевода абзаца, но рядом сохранять оригинал.

## 6. Русскоязычные источники

Ниже не «библиотека на всю жизнь», а короткий набор. Один основной источник на блок и несколько точечных материалов.

### 6.1. C, toolchain, архитектура компьютера

1. **Б. Керниган, Д. Ритчи — «Язык программирования C»**. Использовать выборочно: главы 2, 5, 6, 7 и 8; обязательно выполнять маленькие упражнения. [Страница русского издания](https://www.williamspublishing.com/Books/978-5-8459-0891-9.html).
2. **Игорь Жирков — «Низкоуровневое программирование»**. Основной видеокурс по памяти, ассемблеру, toolchain и выполнению программ: [плейлист](https://www.youtube.com/playlist?list=PL4PvyDyhNGskyixnavM27jEgkVF9iZgwR).
3. **Игорь Жирков — «Языки программирования и архитектура систем»**. Модули, интерфейсы, абстракции, виртуализация, модели памяти: [плейлист](https://www.youtube.com/playlist?list=PL4PvyDyhNGskozbFW-q6nq48AeaQf_F9K) и [канал](https://www.youtube.com/@prog-lang-sys-edu/).
4. **Конспекты к курсу Жиркова**: [GitHub mirror](https://github.com/deniskirbaba/low-level-programming/tree/main/notes). Использовать для поиска ответа, а не переписывать целиком.
5. Точечно: [семинар 13 — оптимизации компилятора](https://www.youtube.com/watch?v=Irzgx7Nu7JE) и [семинар 16 — модели памяти, барьеры, реордеринги](https://www.youtube.com/watch?v=rk_SfSVt3eY).
6. **Р. Брайант, Д. О’Халларон — «Компьютерные системы: архитектура и программирование»**. Не читать 994 страницы подряд. Нужны главы о представлении данных, машинном коде, linking и exceptional control flow. [Страница издательства](https://www.dmkpress.com/catalog/computer/programming/978-5-97060-492-2/).
7. Статья **«STM32. Процесс компиляции и сборки прошивки»** — связка toolchain, startup, linker, ELF и загрузки: [Habr](https://habr.com/ru/companies/timeweb/articles/793152/).

### 6.2. Cortex-M, прерывания и HardFault

1. **Джозеф Ю — «Ядро Cortex-M3 компании ARM. Полное руководство»**. Для STM32F4 ядро M4 отличается расширениями, но модель регистров, exceptions и NVIC применима. Читать разделы programmer model, exceptions, NVIC, memory system и fault handling. [Страница автора и книги у ДМК Пресс](https://www.dmkpress.com/author/7231/).
2. Статья **«Начинаем изучать Cortex-M на примере STM32, часть 2»** — русское объяснение exception entry, stacking, tail-chaining и приоритетов: [Habr](https://habr.com/ru/articles/218825/).
3. Официальная проверка после русского объяснения: [документация STM32F4, включая PM0214](https://www.st.com/en/microcontrollers-microprocessors/stm32f4-series/documentation.html) и [CMSIS-Core/NVIC](https://arm-software.github.io/CMSIS_6/latest/Core/index.html). Они на английском, но это нормативный источник; читать через переводчик по 2–3 страницы.

### 6.3. FreeRTOS, конкурентность и реальное время

1. **Владимир Мединцев — полный курс FreeRTOS**: [YouTube-плейлист](https://www.youtube.com/playlist?list=PLiu4O2eIDAdUz50gTS33a2qozzDSFMouI). Смотреть только нужные части: задачи/планировщик, очереди, notifications, semaphores/mutexes, ISR API.
2. **В. Мединцев — «Операционные системы микроконтроллеров»**, примеры на FreeRTOS: [страница книги и примеров](https://electronics-krd.ru/magazin/product/kniga-freertos).
3. Официальная книга FreeRTOS — английская, но с точными примерами API: [Mastering the FreeRTOS Real Time Kernel](https://www.freertos.org/Documentation/02-Kernel/07-Books-and-manual/01-RTOS_book). Использовать как справочник после русского материала.

### 6.4. Конечные автоматы и архитектура модулей

1. В курсе Жиркова — лекции о системах, модулях, интерфейсах, виртуализации и сложности: [плейлист](https://www.youtube.com/playlist?list=PL4PvyDyhNGskozbFW-q6nq48AeaQf_F9K).
2. Для полного набора терминов FSM — state/event/guard/action/transition: [«Достучаться до небес, или FSM на шаблонах»](https://habr.com/ru/articles/540984/). Реализацию на шаблонах повторять не требуется; взять модель и терминологию.
3. Короткое практическое видео: [«Программная реализация конечных автоматов»](https://www.youtube.com/watch?v=B4qWkdEus9w).

### 6.5. Тесты, CMake, Git и отладка

1. Серия **«TDD для микроконтроллеров»**: [часть 1 и ссылки на продолжения](https://habr.com/ru/companies/ntc-vulkan/articles/487966/). Особенно полезны test seams и отделение аппаратных зависимостей.
2. **«Модульное тестирование в Embedded»**: [Habr](https://habr.com/ru/articles/698092/).
3. **«CMake для C++. Современный подход»** — использовать как справочник по target-based CMake, не читать подряд: [ДМК Пресс](https://www.dmkpress.com/catalog/computer/programming/c/978-5-93700-326-3/).
4. Русская официальная книга **Pro Git**: [git-scm.com/book/ru/v2](https://git-scm.com/book/ru/v2). Достаточно глав 2, 3 и точечно 7.
5. Введение в GDB: [учебный материал Technosphere](https://github.com/miyuki/technosphere/blob/master/gdb-intro.md) и [русский перевод руководства GNU GDB](https://www.opennet.ru/docs/RUS/gdb/). Команды закреплять только практикой с `.elf`.
6. Для host-лабораторий по UB: compiler warnings плюс AddressSanitizer/UndefinedBehaviorSanitizer. Обзор возможностей sanitizers: [Habr](https://habr.com/ru/companies/bizone/articles/570312/).

### 6.6. C++17 и Embedded Linux — после основной программы

1. **Яндекс Образование — «Основы C++»**: [хендбук](https://education.yandex.ru/handbook/cpp). Начать не с «Hello world», а с функций, ссылок/указателей/const, контейнеров/алгоритмов, классов, жизненного цикла и RAII.
2. **Майя Пош — «Программирование встроенных систем на C++17»**: [ДМК Пресс](https://www.dmkpress.com/catalog/computer/programming/c/978-5-97060-785-5/). Читать после базового C++ и проверять cost model на Godbolt/objdump.
3. **«Введение в Linux» Института биоинформатики**: [Stepik](https://stepik.org/course/73/promo). Курс уже пройден, поэтому использовать для выборочного повторения shell/processes/permissions, не проходить заново целиком.
4. **Крис Симмондс — «Встраиваемые системы на основе Linux»**: Buildroot, Yocto и embedded Linux workflow: [ДМК Пресс](https://www.dmkpress.com/catalog/computer/freeware/978-5-97060-483-0/).
5. Фоновый технический английский: [«Английский язык для инженеров и технических специалистов» на Stepik](https://stepik.org/course/93843/promo). Это дополнительный вариант; основной метод — словарь из реальных ARM/ST/Linux документов.

## 7. Методика, которая не даст снова забыть курс

### 7.1. Не конспект лекции, а ответ на вопрос

Плохой конспект:

> `volatile` — ключевое слово языка C...

Рабочий конспект:

```markdown
# volatile

## Вопрос
Что именно гарантирует volatile и почему volatile counter++ не защищает от гонки?

## Короткий ответ своими словами
...

## Гарантирует
- реальное чтение/запись volatile-объекта в точках, требуемых абстрактной машиной C;

## Не гарантирует
- атомарность;
- mutex/critical section;
- целостный snapshot структуры;
- межпоточный happens-before.

## Код, который ломается
...

## Как проверил
compiler output / test / disassembly / oscilloscope

## Где это встретилось в работе
main + ISR, DMA buffer, MMIO register
```

На одну тему — одна страница. Не переписывать видео и книги.

### 7.2. Цикл одного занятия

1. **Воспроизведение — 5–10 минут.** До источника записать всё, что помнишь, и ответить на 3 вопроса.
2. **Точечное изучение — 30–40 минут.** Один раздел книги или фрагмент видео, а не двухчасовая лекция целиком.
3. **Закрыть источник — 10 минут.** Объяснить тему вслух или письменно без подсказки.
4. **Эксперимент — 30–60 минут.** Сначала воспроизвести ошибку, затем исправить.
5. **Пять карточек — 5 минут.** Только вопросы, требующие воспроизведения, а не узнавания.
6. **Повторение через 1, 7 и 30 дней.** Если ответ не воспроизводится — вернуться к эксперименту, а не пересматривать весь курс.

### 7.3. Критерий усвоения темы

Тема закрыта, если выполнены четыре пункта:

- можешь объяснить её за 5 минут без поиска;
- написал минимальный работающий пример;
- воспроизвёл типичную ошибку и объяснил симптом;
- через неделю ответил минимум на 4 из 5 контрольных вопросов.

Не требуется помнить адреса регистров, все параметры HAL/FreeRTOS или точные команды linker script. Их нормально искать. Требуется помнить модель и распознавать опасную ситуацию.

## 8. Репозиторий обучения

Название: `embedded-learning` или `firmware-engineering-lab`. Сначала лучше private, потому что часть примеров может быть основана на рабочих проблемах.

```text
embedded-learning/
├── README.md
├── roadmap.md
├── learning_log.md
├── glossary.md
├── questions/
│   ├── c.md
│   ├── toolchain.md
│   ├── cortex_m.md
│   ├── freertos.md
│   └── architecture.md
├── notes/
│   ├── c/
│   ├── toolchain/
│   ├── cortex_m/
│   ├── freertos/
│   └── architecture/
├── labs/
│   ├── host_c_ub/
│   ├── linker_startup_f407/
│   ├── cortex_m_faults/
│   ├── freertos_scheduling/
│   ├── dma_double_buffer/
│   └── device_module/
├── tests/
├── docs/
│   ├── architecture.md
│   ├── threading_model.md
│   ├── error_model.md
│   └── debug_guide.md
└── cmake/
```

### `learning_log.md`

```markdown
## 2026-09-01 — object lifetime

- До занятия помнил:
- Ошибся в:
- Понял:
- Написал:
- Сломал и диагностировал:
- Применение в работе:
- Повторить: 02.09 / 08.09 / 01.10
```

### Правило коммита

Один коммит должен доказывать одно изменение знания:

- `c: add lifetime failure examples`
- `toolchain: document data and bss startup`
- `cortex-m: capture hardfault stacked pc`
- `freertos: compare delay and yield`
- `tests: add malformed SPI frame cases`

## 9. Реалистичный план на 16 недель

Нагрузка: **3–4 часа в неделю**. Не 5–7 часов: при текущей работе, здоровье и финансовой нагрузке важнее не сорвать ритм. Формат недели: два занятия по 60 минут, одна лаборатория 90 минут и короткие повторения.

### Спринт 1. C и toolchain — недели 1–4

| Неделя | Теория | Лабораторная работа | Результат |
|---|---|---|---|
| 1 | Lifetime, scope, linkage, ownership, `const` | Вернуть локальный буфер, поймать ASan; переписать API через output buffer | `notes/c/lifetime.md`, 5 тестов |
| 2 | UB, sequencing, integer rules, `volatile` | Примеры `-O0/-O2`, UBSan, main/ISR shared data model | `notes/c/ub_volatile.md` |
| 3 | Preprocess/compile/assemble/link, symbols, ABI | `gcc -E/-S/-c`, `nm`, `readelf`, `objdump`; разобрать один function call | `notes/toolchain/pipeline_abi.md` |
| 4 | ELF, sections, startup | Найти `.text/.data/.bss` в рабочем `.map`; доказать, кто копирует/обнуляет секции | `notes/toolchain/startup.md` |

**Зачёт:** без подсказки объяснить lifetime локального массива, три формы `const`, отличие signature/ABI, порядок сборки и `.data/.bss`.

### Спринт 2. Cortex-M и fault handling — недели 5–8

| Неделя | Теория | Лабораторная работа | Результат |
|---|---|---|---|
| 5 | Registers, stack, function call, MSP/PSP | Сопоставить C-функцию с disassembly; отметить caller/callee saved | `notes/cortex_m/call_stack.md` |
| 6 | Exception entry, NVIC, priorities | GPIO pulse в ISR; nested IRQ или модель приоритетов; записать exception frame | `notes/cortex_m/exceptions.md` |
| 7 | HardFault, CFSR/HFSR/BFAR/MMFAR | NULL, invalid function pointer; сохранить PC/LR и найти строку через `.elf` | `labs/cortex_m_faults/` |
| 8 | Clock tree, VTOR, boot jump | Нарисовать clock tree текущего F4; checklist корректного jump в application | `docs/debug_guide.md` |

**Зачёт:** получить HardFault и определить причину без догадки и без GUI CubeIDE.

### Спринт 3. FreeRTOS, ISR и DMA — недели 9–12

| Неделя | Теория | Лабораторная работа | Результат |
|---|---|---|---|
| 9 | Task states, delay, yield, scheduling | Три задачи одинакового/разного priority; наблюдать Ready/Blocked | `notes/freertos/scheduler.md` |
| 10 | Queue, mutex, semaphore, notification, FromISR | Передать событие из ISR и корректно вызвать `portYIELD_FROM_ISR` | `labs/freertos_scheduling/` |
| 11 | Race, atomicity, critical section, priority inversion | Испорченный shared state; single owner/queue; сценарий priority inversion | `docs/threading_model.md` |
| 12 | DMA lifecycle, ring/double buffer, throughput | DMA producer + parser consumer; overflow counter; измерить bytes/s и latency | `labs/dma_double_buffer/` |

**Зачёт:** по сценарию выбрать queue/mutex/notification/critical section и объяснить, почему `volatile` не решает задачу.

### Спринт 4. Архитектура и тестируемость — недели 13–16

| Неделя | Теория | Лабораторная работа | Результат |
|---|---|---|---|
| 13 | Module/interface, coupling/cohesion, ownership | Выбрать один небольшой модуль: MCP4921, FPGA config или IMX304; описать контракт | `docs/architecture.md` |
| 14 | FSM, error model, timeout/retry/safe state | Диаграмма state/event/guard/action и таблица ошибок | `docs/error_model.md` |
| 15 | Host tests, mock/fake/stub | Вынести protocol/command builder без HAL; 10–15 GoogleTest cases | `tests/device_module/` |
| 16 | CMake, warnings, static analysis, CI | Одна команда build+test; сохранить `.elf/.map`; итоговый self-review | `README.md`, CI job |

**Зачёт:** выбранный модуль компилируется на host, имеет ясный контракт и тестируемую логику без HAL.

## 10. Что делать после 16 недель

Выбирать только один следующий трек.

### Вариант A — C++17, 8 недель

1. References, pointers, `const`, object lifetime.
2. Classes, constructors/destructors, Rule of Zero.
3. RAII и управление ресурсами.
4. Containers и algorithms.
5. Templates, `constexpr`, `enum class`.
6. Smart pointers и ownership.
7. Embedded cost model, exceptions/RTTI/allocation.
8. Маленький драйвер или protocol library на C++17 с тестами.

### Вариант B — Embedded Linux/Jetson, 8–12 недель

1. Process/thread/syscall/memory/file model.
2. Cross-compilation и rootfs.
3. Device tree и driver model.
4. V4L2/media graph.
5. MIPI CSI path и clock/lane/pixel-format calculations.
6. `dmesg`, ftrace, trace-cmd, live DT.
7. Buildroot.
8. Yocto recipe/layer/image.
9–12. Воспроизводимый camera bring-up project.

C++ лучше выбрать первым, если ближайшие вакансии требуют язык. Linux/Jetson — если появится реальный проект или целевая вакансия с Yocto/device drivers.

## 11. Что сейчас не изучать

- Rust, TinyML, embedded security и functional safety — пока нет целевой задачи;
- все алгоритмы и структуры данных «на всякий случай»;
- всю схемотехнику заново — практическая база уже есть;
- все исходники `tasks.c` FreeRTOS построчно;
- все STM32-периферийные блоки;
- FPGA как отдельную специализацию;
- полный Linux kernel source;
- все паттерны GoF;
- одновременно C++, Yocto, RTOS и Cortex-M.

## 12. Как использовать ИИ в обучении и работе

ИИ полезен как наставник, генератор тестов и оппонент, но не как источник окончательной истины.

Правильный цикл:

1. Сам сформулировать контракт и ограничения.
2. Попросить ИИ предложить решение и перечислить assumptions.
3. Попросить найти UB, race, lifetime и error-path risks.
4. Скомпилировать с warnings/sanitizers, запустить tests.
5. Для embedded посмотреть `.map`/disassembly и проверить timing на железе.
6. Критичное утверждение сверить со стандартом, ARM/ST/FreeRTOS documentation.

Полезные запросы к ИИ:

- «Дай пять фрагментов C с ошибками lifetime, не показывай ответы до моей попытки».
- «Сыграй code reviewer: проверь API на ownership, thread-safety и error handling».
- «Придумай негативные тесты для этого parser/FSM».
- «Сравни мой ответ с ABI Cortex-M и укажи только конкретные неточности».
- «Составь вопросы на повторение через 7 дней по этому конспекту».

Неполезный режим: получить длинный пересказ, согласиться с ним и перейти к следующей теме. Именно так знания снова растворяются.

## 13. Минимальная версия плана, если сил мало

Если неделя тяжёлая, обязательный минимум — **20 минут**:

1. ответить на три старых вопроса;
2. прочитать одну страницу или посмотреть 10 минут видео;
3. сделать один маленький commit в конспект или lab.

Нулевая неделя хуже маленькой. Но пропущенная неделя не обнуляет план: не нужно «догонять», нужно продолжить с текущего пункта.

## 14. Итоговый ориентир

Цель программы — не запомнить весь стандарт C и FreeRTOS API. Через 16 недель должно измениться следующее:

- при чужом коде с локальным массивом, `volatile`, DMA или ISR сразу виден класс риска;
- при HardFault есть алгоритм и данные, а не угадывание;
- при споре о модуле можно назвать контракт, владельца состояния и границу зависимости;
- ответ ИИ проверяется тестом, warning, `.map`, disassembly или измерением;
- знания остаются в репозитории как код, вопросы и диагностические примеры, а не как потерянный линейный конспект.

Полная карта выше — справочник. Реальная текущая задача — только первый 16-недельный цикл.
