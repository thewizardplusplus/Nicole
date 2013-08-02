	// вывод числа в stdout, вызов функций из библиотеки
	.text
	.global main
main:
	// загружаем число в формате с плавающей запятой на стек
	subl $4, %esp
	fld NUMBER
	fstps (%esp)

	// вызываем функцию из библиотеки, очищаем стек от её параметров, кладём на стек её результат
	call ArrayCreateFromNumber
	addl $4, %esp
	subl $4, %esp
	fstps (%esp)

	// вызываем функцию из библиотеки, очищаем стек от её параметров
	call Show
	add $4, %esp

	// выходим из программы, возвращая код успешного завершения
	mov $0, %eax
	ret

	.data
NUMBER:
	.float  0.23
