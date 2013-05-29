note Число пи (алгоритм Брента - Саламина).

string MESSAGE_BEGIN = "Pi = "
string MESSAGE_END =   ".\n"
let    ERROR =         0.000001

let a = 1
let b = 1 / MathSquareRoot(2)
let t = 1 / 4
let p = 1

label loop
	let new_a = (a + b) / 2
	let new_b = MathSquareRoot(a * b)
	let new_t = t - p * (a - new_a) * (a - new_a)
	let new_p = 2 * p

	let a = new_a
	let b = new_b
	let t = new_t
	let p = new_p
if MathModulus(a - b) > ERROR then
	go to loop

let pi = (a + b) * (a + b) / (4 * t)
Show(MESSAGE_BEGIN $ ToString(pi) $ MESSAGE_END)
