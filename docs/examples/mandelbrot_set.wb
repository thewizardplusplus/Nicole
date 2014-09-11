note Множество Мандельброта в ASCII-арте.
note Основано на http://people.sc.fsu.edu/~jburkardt/c_src/mandelbrot_ascii/mandelbrot_ascii.html

string MESSAGE = "Mandelbrot Set:\n"
Show(MESSAGE)

let symbol = ArrayCreate(1)

let i = 0 - 1
label loop1
	if !(i < 1) then
		go to end_loop1

	let r = 0 - 2
	label loop2
		let I = i
		let R = r
		if !(R < 1) then
			go to end_loop2

		let n = 0
		label loop3
			let b = I * I
			let last_n = n
			let n = n + 1
			if !(last_n < 26 & R * R + b < 4) then
				go to end_loop3

			let I = 2 * R * I + i
			let R = R * R - b + r
		go to loop3
		label end_loop3

		ArrayItemRemove(symbol, 0)
		ArrayItemAppend(symbol, n + 31)
		Show(symbol)
		let r = r + 0.03
	go to loop2
	label end_loop2

	Show(NEW_LINE)
	let i = i + 0.06
go to loop1
label end_loop1
