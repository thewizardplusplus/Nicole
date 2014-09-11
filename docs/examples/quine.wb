note Quine.

string code = "note Quine.\n\nstring code = \"\"\n\nlet INSERT_POSITION = 28\nlet NEW_LINE_CODE =   10\nlet TAB_CODE =        9\nlet QUOTE_CODE =      34\nlet N_LETTER_CODE =   110\nlet T_LETTER_CODE =   116\nlet SLASH_CODE =      92\n\nlet code_copy = ArrayCreate(0) $ code\nlet source_index = ArrayGetSize(code) - 1\nlabel start\n\tif source_index < 0 then\n\t\tgo to end\n\n\tlet symbol = ArrayItemGet(code, source_index)\n\tif symbol = NEW_LINE_CODE then\n\t\tgo to insert_n_letter\n\tif symbol = TAB_CODE then\n\t\tgo to insert_t_letter\n\tif symbol = QUOTE_CODE then\n\t\tgo to insert_quote\n\tArrayItemInsert(code_copy, INSERT_POSITION, symbol)\n\tgo to continue\n\n\tlabel insert_n_letter\n\tArrayItemInsert(code_copy, INSERT_POSITION, N_LETTER_CODE)\n\tgo to insert_slash\n\n\tlabel insert_t_letter\n\tArrayItemInsert(code_copy, INSERT_POSITION, T_LETTER_CODE)\n\tgo to insert_slash\n\n\tlabel insert_quote\n\tArrayItemInsert(code_copy, INSERT_POSITION, QUOTE_CODE)\n\n\tlabel insert_slash\n\tArrayItemInsert(code_copy, INSERT_POSITION, SLASH_CODE)\n\n\tlabel continue\n\tlet source_index = source_index - 1\n\tgo to start\n\nlabel end\nShow(code_copy)\n"

let INSERT_POSITION = 28
let NEW_LINE_CODE =   10
let TAB_CODE =        9
let QUOTE_CODE =      34
let N_LETTER_CODE =   110
let T_LETTER_CODE =   116
let SLASH_CODE =      92

let code_copy = ArrayCreate(0) $ code
let source_index = ArrayGetSize(code) - 1
label start
	if source_index < 0 then
		go to end

	let symbol = ArrayItemGet(code, source_index)
	if symbol = NEW_LINE_CODE then
		go to insert_n_letter
	if symbol = TAB_CODE then
		go to insert_t_letter
	if symbol = QUOTE_CODE then
		go to insert_quote
	ArrayItemInsert(code_copy, INSERT_POSITION, symbol)
	go to continue

	label insert_n_letter
	ArrayItemInsert(code_copy, INSERT_POSITION, N_LETTER_CODE)
	go to insert_slash

	label insert_t_letter
	ArrayItemInsert(code_copy, INSERT_POSITION, T_LETTER_CODE)
	go to insert_slash

	label insert_quote
	ArrayItemInsert(code_copy, INSERT_POSITION, QUOTE_CODE)

	label insert_slash
	ArrayItemInsert(code_copy, INSERT_POSITION, SLASH_CODE)

	label continue
	let source_index = source_index - 1
	go to start

label end
Show(code_copy)
