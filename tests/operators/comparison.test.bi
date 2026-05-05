/*
	Comparison Operators
*/

#.test('Lesser than between two numbers', []{[ 10 < 5, 5 < 10 ]}, [ false, true ])
#.test('Lesser than or equal to between two numbers', []{[ 10 <= 5, 5 <= 10, 10 <= 10 ]}, [ false, true, true ])
#.test('Greater than between two numbers', []{[ 10 > 5, 5 > 10 ]}, [ true, false ])
#.test('Greater than or equal to between two numbers', []{[ 10 >= 5, 5 >= 10, 10 >= 10 ]}, [ true, false, true ])

// found bug in equality where range comparison was ending the array

#.test(
	'Equality operator',
	[]{
		[
			void == true,
			void == false,
			true == true,
			true == false,
			1 == 1,
			1 == 2,
			'test' == 'test',
			'test' == 'no',
			'test' == 0,
			0 == '0',
			(1 -> 20) == (1 -> 20),
			(1 -> 20) == (1 -> 19)
		]
	},
	[
		false,
		true,
		true,
		false,
		true,
		false,
		true,
		false,
		false,
		false,
		true,
		false
	]
)

#.test(
	'Inequality operator',
	[]{
		[
			void != true,
			void != false,
			true != true,
			true != false,
			1 != 1,
			1 != 2,
			'test' != 'test',
			'test' != 'no',
			'test' != 0,
			0 != '0',
			(1 -> 20) != (1 -> 20),
			(1 -> 20) != (1 -> 19)
		]
	},
	[
		true,
		false,
		false,
		true,
		false,
		true,
		false,
		true,
		true,
		true,
		false,
		true
	]
)