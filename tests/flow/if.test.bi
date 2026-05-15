/*
	If statements
	If statements in bismuth are expressions and can be woven into other expressions. Like blocks each block of the if statements last expression is the resolved value. This is optional behaviour and they can be used like normal statements with the return value being unused.
*/

#.test(
	'If statements can be used to tree binary decisions with if, elseif and else',
	[]{
		test = [number, boolean] {
			if boolean { return 'nice!' }

			if number <= 10 {
				return 'Under or equal to 10'
			} else number == 20 {
				return 'Is 20'
			} else {
				return 'Not 20'
			}
		}

		[
			test(10, true),
			test(10, false),
			test(20, false),
			test(30, false)
		]
	},
	[
		'nice!',
		'Under or equal to 10',
		'Is 20',
		'Not 20'
	]
)

#.test(
	'Ifs can be used inside expressions and are resolved before further operation',
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