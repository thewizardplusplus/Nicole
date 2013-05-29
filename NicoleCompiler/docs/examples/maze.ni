note 10 PRINT CHR$(205.5+RND(1));: GOTO 10

string SYMBOL1 = "/"
string SYMBOL2 = "\\"
let    DELAY =   10

TimerStart()
label loop
	if MathRandom(0, 1) < 0.5 then
		go to symbol1
	Show(SYMBOL2)
	go to delay_loop

	label symbol1
	Show(SYMBOL1)

	label delay_loop
	if TimerGetElapsedTime() < DELAY then
		go to delay_loop
	TimerStart()
label continue_loop
go to loop
