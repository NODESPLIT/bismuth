/*
	Arithmetic Operators
*/

#.test('Addition of numbers adds the numbers together', []{ 1 + 1 }, 2)

#.test(
	'Addition of anything non string to a string on the right side describes the left side and concatenates the strings',
	[]{
		[
			1 + 'string',
			void + 'string',
			true + 'string',
			(1 -> 4) + 'string',
			[]{} + 'string'
		]
	},
	[
		'1string',
		'voidstring',
		'truestring',
		'( 1 -> 4 )string',
		'[]{}string'
	]
)

// bug on last string, can't mix quote types or escape

#.test('Addition of strings concatenates the strings', []{ 'left' + 'right' }, 'leftright')
#.test('Addition of arrays concatenates the arrays', []{ [ 1, 2, 3 ] + [ 4, 5, 6 ] }, [ 1, 2, 3, 4, 5, 6 ])

#.test(
	'Addition of an array with a table flattens the keys and values into the array',
	[]{ [ 1, 2, 3 ] + { four: 4, five: 5, six: 6 } },
	[ 1, 2, 3, 'four', 4, 'five', 5, 'six', 6 ]
)

#.test(
	'Addition of an array with a string breaks up the string into characters and concatenates to the array',
	[]{ [ 1, 2, 3 ] + 'Nice!' },
	[ 1, 2, 3, 'N', 'i', 'c', 'e', '!' ]
)

#.test(
	'Additions of tables concatenates the tables overwriting the existing fields on the left with the rights',
	[]{ { one: 1, two: 2 } + { two: 'two', three: 3, four: 4 } },
	{ one: 1, two: 'two', three: 3, four: 4 }
)

#.test(
	'Addition of a table with an array expands the table into the array with indexes as keys',
	[]{ { one: 1 } + [ 1, 2, 3, 4 ] },
	{ one: 1, 0: 1, 1: 2, 2: 3, 3: 4 }
)

#.test(
	'Addition of a table with a string breaks up the string into characters and concatenates to the table with indexes as keys',
	[]{ { one: 1 } + 'Nice!' },
	{ one: 1, 0: 'N', 1: 'i', 2: 'c', 3: 'e', 4: '!' }
)

#.test(
	'Subtraction of numbers deducts one number from the other',
	[]{ 10 - 5 },
	5
)

#.test(
	'Multiplication of numbers multiplies them together',
	[]{ 2 * 4 },
	8
)

#.test(
	'Multiplication of a string with a number repeats that string by the amount on the right',
	[]{ '*' * 10 },
	'**********'
)

#.test(
	'Division of numbers divides them',
	[]{ 10 / 2 },
	5
)

#.test(
	'Division of strings splits the left string with the right as a delimiter',
	[]{ '1,2,3,4,5,6' / ',' },
	[ '1', '2', '3', '4', '5', '6' ]
)

#.test(
	'Modulus between numbers applies an fmod between the numbers',
	[]{ [ 11 % 2, 10 % 2 ] },
	[ 1, 0 ]
)

#.test(
	'Unary negative on a number makes that number negative',
	[]{ -( 12 - 2 ) },
	-10
)

#.test(
	'Increment operator used on the right side of a number increases it by one in place, resolving to the original value',
	[]{
		value = 10
		[ value++, value ]
	},
	[ 10, 11 ]
)

#.test(
	'Decrement operator used on the right side of a number decreases it by one in place, resolving to the original value',
	[]{
		value = 10
		[ value--, value ]
	},
	[ 10, 9 ]
)

#.test(
	'Increment operator used on the left side of a number increases it by one in place, resolving to the new value',
	[]{
		value = 10
		[ ++value, value ]
	},
	[ 11, 11 ]
)

#.test(
	'Decrement operator used on the left side of a number decreases it by one in place, resolving to the new value',
	[]{
		value = 10
		[ --value, value ]
	},
	[ 9, 9 ]
)
