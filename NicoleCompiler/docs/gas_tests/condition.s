	// вывод числа в stdout, вызов функций из библиотеки
	.text
	.global main
main:
	// загружаем число в формате с плавающей запятой на стек
	subl $4, %esp
	fld NUMBER
	fstps (%esp)

	// загружаем значение с вершины стека в первую переменную
	pop number

	// загружаем указатель на первую строку на стек
	push $CONSTANT_STRING0

	// вызываем функцию из библиотеки, очищаем стек от её параметров, кладём на стек её результат
	call ArrayCreateFromString
	add $4, %esp
	subl $4, %esp
	fstps (%esp)

	// загружаем значение с вершины стека во вторую переменную
	pop message_true

	// загружаем указатель на вторую строку на стек
	push $CONSTANT_STRING1

	// вызываем функцию из библиотеки, очищаем стек от её параметров, кладём на стек её результат
	call ArrayCreateFromString
	add $4, %esp
	subl $4, %esp
	fstps (%esp)

	// загружаем значение с вершины стека в третью переменную
	pop message_false

	// загружаем значение из первой переменной на вершину стека
	push number

	// сравниваем значение на верине стека с нулём
	cmp $0, (%esp)

	// удаляем с вершины стека значение, использовавшееся при сравнении
	pop %ebx

	// переходим на ветку else, если сравнение было ложно
	jz else

	// загружаем значение из второй переменной на вершину стека
	push message_true

	// переходим в конец ветки else
	jmp end

else:
	// загружаем значение из третьей переменной на вершину стека
	push message_false

end:
	// вызываем функцию из библиотеки, очищаем стек от её параметров
	call Show
	add $4, %esp

	// выходим из программы, возвращая код успешного завершения
	mov $0, %eax
	ret

	.data
NUMBER:
	.float   1.0
CONSTANT_STRING0:
	.string "True.\n"
CONSTANT_STRING1:
	.string "False.\n"
number:
	.float   0
message_true:
	.float   0
message_false:
	.float   0
