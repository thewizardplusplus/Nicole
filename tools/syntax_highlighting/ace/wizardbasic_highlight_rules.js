/* The MIT License (MIT)
 *
 * Copyright (c) 2014 thewizardplusplus <thewizardplusplus@yandex.ru>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

define(
	function(require, exports, module) {
		"use strict";

		var oop = require("../lib/oop");
		var TextHighlightRules =
			require("./text_highlight_rules")
			.TextHighlightRules;

		var WizardBasicHighlightRules = function() {
			this.$rules = {
				start: [
				{
					// labels
					token: [
						'keyword.control.wizard-basic',
						'support.constant.wizard-basic'
					],
					regex: '(\\s*(?:label|go to) )([A-Za-z_]\\w*\\b)',
				},
				{
					// keywords
					token: 'keyword.control.wizard-basic',
					regex: '\\b(?:'
							+ 'function'
							+ '|go'
							+ '|if'
							+ '|label'
							+ '|let'
							+ '|procedure'
							+ '|string'
							+ '|then'
							+ '|to'
							+ '|use'
						+ ')\\b',
				},
				{
					// subprograms
					token: 'entity.name.function.wizard-basic',
					regex: '\\b(?:'
						+ 'ArrayDelete'
						+ '|ArrayItemSet'
						+ '|ArrayItemInsert'
						+ '|ArrayItemAppend'
						+ '|ArrayItemRemove'
						+ '|ArrayCreate'
						+ '|ToString'
						+ '|ToNumber'
						+ '|c_string'
						+ '|ArrayGetSize'
						+ '|ArrayItemGet'
						+ '|Show'
						+ '|FileClose'
						+ '|FileRemove'
						+ '|FileWrite'
						+ '|FileOpen'
						+ '|FileRead'
						+ '|FileReadAll'
						+ '|MathsSin'
						+ '|MathsCos'
						+ '|MathsTg'
						+ '|MathsArcsin'
						+ '|MathsArccos'
						+ '|MathsArctg'
						+ '|MathsExp'
						+ '|MathsLn'
						+ '|MathsLg'
						+ '|MathsModulus'
						+ '|MathsPower'
						+ '|MathsIntegral'
						+ '|MathsSquareRoot'
						+ '|MathsAngle'
						+ '|MathsRandom'
						+ '|TimerStart'
						+ '|Exit'
						+ '|GetOs'
						+ '|TimerGetElapsedTimeInUs'
					+ ')\\b',
				},
				{
					// constants
					token: 'constant.language.wizard-basic',
					regex: '\\b(?:'
						+ 'FILE_OPEN_MODE_READ'
						+ '|FILE_OPEN_MODE_APPEND'
						+ '|FILE_OPEN_MODE_REWRITE'
						+ '|FILE_OPEN_ERROR'
						+ '|APP_PATH'
						+ '|NEW_LINE'
						+ '|PATH_SEPARATOR'
						+ '|FALSE'
						+ '|TRUE'
						+ '|PI'
						+ '|E'
						+ '|OS_LINUX'
						+ '|OS_WINDOWS'
					+ ')\\b',
				},
				{
					// numbers
					token: 'constant.numeric.wizard-basic',
					regex: '\\d+(?:\\.\\d+)?',
				},
				{
					// strings
					token: 'string.quoted.double.wizard-basic',
					regex: '"(?:\\\\.|[^"])*"',
				},
				{
					// comments
					token: 'comment.line.wizard-basic',
					regex: '^\\s*note\\b.*$',
				}
				]
			};

			this.normalizeRules();
		};
		oop.inherits(WizardBasicHighlightRules, TextHighlightRules);

		exports.WizardBasicHighlightRules = WizardBasicHighlightRules;
	}
);
