	// возврат c-строки из функции, передача c-строки в функцию
	.text
	.global main
main:
	/* С использованием тестовой библиотеки CStringPassingTest.
	 */
	// вызываем функцию из библиотеки, кладём на стек её результат
	call ReturningFunction
	push %eax

	// вызываем функцию из библиотеки, очищаем стек от её параметров
	call GettingFunction
	add $4, %esp

	/* С использованием библиотеки NicoleFramework.
	 */
	// кладём на стек указатель на строку
	push $test_string

	// вызываем функцию из библиотеки, очищаем стек от её параметров, кладём на стек её результат
	call ArrayCreateFromString
	add $4, %esp
	subl $4, %esp
	fstps (%esp)

	// вызываем функцию из библиотеки, очищаем стек от её параметров, кладём на стек её результат
	call ArrayConvertToString
	add $4, %esp
	push %eax

	// вызываем функцию из стандартной библиотеки, очищаем стек от её параметров
	call printf
	add $4, %esp

	// выходим из программы, возвращая код успешного завершения
	mov $0, %eax
	ret

	.data
test_string:
	.string "Test string.\n"
