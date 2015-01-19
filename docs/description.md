## Wizard BASIC

Wizard BASIC &mdash; компилируемый высокоуровневый язык программирования.

### Исходники

Файлы исходного кода должны быть в кодировке ASCII.

### Ключевые слова

10 ключевых слов:

	function go if label let procedure string then to use

### Типы

#### Числа

Тип: с плавающей запятой, 4 байт.

Копирование: по значению.

Хранение: на стеке.

Определение: `/\d+(\.\d+)?/i`.

#### Логические значения

В качестве ложного логического значения принимается число 0.

В качестве истинного логического значения принимаются числа, отличные от 0.

#### Массивы

Тип: динамический массив; доступ по хэндлеру; хэндлер представляет собой число
(см. выше).

Копирование: хендлера &mdash; по значению.

Хранение:

* хэндлера &mdash; на стеке;
* массива &mdash; в куче.

#### Строки

Строками являются массивы, хранящие коды символов. Используется кодировка ASCII.

Определение: оператор
`"string", identifier, "=", ? /"(\\.|[^"])*"/ ?, line break`.

#### Файлы

Тип: файлы ОС; доступ по хэндлеру; хэндлер представляет собой число (см. выше).

Копирование: хендлера &mdash; по значению.

Хранение: хендлера &mdash; на стеке.

### Операции

Перечислены в порядке убывания приоритета.

Приоритет | Операция | Описание | Ассоциативность
--- | --- | --- | ---
1 | `!` | логическое отрицание | правая
2 | `*` | умножение | левая
2 | `/` | деление | левая
3 | `+` | сложение | левая
3 | `-` | вычитание | левая
3 | `$` | конкатенация массивов | левая
4 | `<` | меньше | левая
4 | `>` | больше | левая
5 | `=` | равенство | левая
6 | `&` | конъюнкция | левая
7 | <code>&#124;</code> | дизъюнкция | левая

### Вызов функции

	identifier, "(", [expression, {",", expression}], ")"

Является выражением.

### Объявление переменных и присваивание

	"let", identifier, "=", expression, line break

Оператор.

### Управляющие конструкции

#### Метки

Объявление: оператор `"label", identifier, line break`.

Переход: оператор `"go", "to", identifier, line break`.

#### Условие

	"if", expression, "then", line break,
		statement, line break

Оператор.

### Внешние библиотеки

Подключение: оператор `"use", string, line break`, где `string` &mdash;
относительный путь к библиотеке. Если библиотека расположена по стандартному
пути, его можно опустить. Путь может быть предварён строкой `linux:` или
`windows:`, в этом случае данная библиотека будет подключена только при сборке
на соответствующей ОС. Имя библиотеки может указываться в сокращённом варианте
(`Name` вместо `libName.a`). Стандартные библиотеки языков C и C++ подключаются
автоматически.

Импорт процедур: оператор `"procedure", identifier, "/", number, line break`;
`number` &mdash; число параметров.

Импорт функций: оператор `"function", identifier, "/", number, line break`;
`number` &mdash; число параметров.

_**Внимание!** Из-за особенностей компилятора при вызове внешней процедуры или
функции параметры ей передаются **в обратном порядке**!_

### Стандартная библиотека

#### Модуль Array

##### Процедуры

* `ArrayDelete(array_id)` &mdash; удаляет массив из памяти;
* `ArrayItemSet(array_id, index, value)`;
* `ArrayItemInsert(array_id, index, value)`;
* `ArrayItemAppend(array_id, value)`;
* `ArrayItemRemove(array_id, index)`.

##### Функции

* `ArrayCreate(size)`;
* `ToString(number)` &mdash; преобразует число в строку;
* `ToNumber(array_id)` &mdash; парсит число из строки;
* `c_string(array_id)` &mdash; преобразует строку в строку языка C; данная
функция должна использоваться **только** для преобразования строк при передаче
их во внешние функции; сохранять результат этой функции в переменной **нельзя**;
* `ArrayGetSize(array_id)`;
* `ArrayItemGet(array_id, index)`.

#### Модуль IO

##### Константы

* `FILE_OPEN_MODE_READ = 0`;
* `FILE_OPEN_MODE_APPEND = 1`;
* `FILE_OPEN_MODE_REWRITE = 2`;
* `FILE_OPEN_ERROR = -1`;
* `APP_PATH` &mdash; путь к исполняемому файлу;
* `NEW_LINE = "\n"`;
* `PATH_SEPARATOR`:
	* `"/"` на Linux;
	* `"\\"` на Windows.

##### Процедуры

* `Show(string_id)` &mdash; выводит строку в консоль;
* `FileClose(file_id)`;
* `FileRemove(filename_id)` &mdash; удаляет файл с жёсткого диска;
* `FileWrite(file_id, array_id)`.

##### Функции

* `FileOpen(filename_id, mode)`;
* `FileRead(file_id, number_of_bytes)`;
* `FileReadAll(file_id)`.

#### Модуль Maths

##### Константы

* `FALSE = 0`;
* `TRUE = 1`;
* `PI = 3.14159`;
* `E = 2.71828`.

##### Функции

* `MathsSin(number)`;
* `MathsCos(number)`;
* `MathsTg(number)`;
* `MathsArcsin(number)`;
* `MathsArccos(number)`;
* `MathsArctg(number)`;
* `MathsExp(number)`;
* `MathsLn(number)`;
* `MathsLg(number)`;
* `MathsModulus(number)`;
* `MathsPower(base, exponent)`;
* `MathsIntegral(number)` &mdash; возвращает целую часть числа;
* `MathsSquareRoot(number)`;
* `MathsAngle(x, y)` &mdash; `atan2`;
* `MathsRandom(minimum, maximum)`.

#### Модуль System

##### Константы

* `OS_LINUX = 1`;
* `OS_WINDOWS = 0`.

##### Процедуры

* `TimerStart()`;
* `Exit()`.

##### Функции

* `GetOs()`;
* `TimerGetElapsedTimeInUs()`.
