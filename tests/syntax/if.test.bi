/*
	If statements
	If statements in bismuth are expressions and can be woven into other expressions. Like blocks each block of the if statements last expression is the resolved value. This is optional behaviour and they can be used like normal statements with the return value being unused.
*/

#.test(
	'If can be used inside expressions and are resolved before further operation',
	[]{
		test = [ value ] {
			'result: ' + if value == 10 { value + ' Equals 10!' } else value == 2 { value + ' Equals 2!' } else { value + ' is neither!' } + '!!'
		}

		[
			test(10),
			test(2),
			test(5)
		]
	},
	[
		'result: 10 Equals 10!!!',
		'result: 2 Equals 2!!!',
		'result: 5 is neither!!!'
	]
)