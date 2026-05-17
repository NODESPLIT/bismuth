/*
	Logical Operators
*/

#.test(
	'Logical `&&` AND between values compares their `isTrue()` results',
	[]{
		[
			true && true && 'Nice!',
			true && false && 'Nice!',
			1 && 1 && 'Nice!',
			0 && 1 && 'Nice!'
		]
	},
	[
		'Nice!',
		false,
		'Nice!',
		false
	]
)

#.test(
	'Logical `||` OR between values compares their `isTrue()` results',
	[]{
		[
			true || true && 'Nice!',
			true || false && 'Nice!',
			false || false && 'Nice!',
			1 || 1 && 'Nice!',
			0 || 1 && 'Nice!',
			0 || 0 && 'Nice!'
		]
	},
	[
		'Nice!',
		'Nice!',
		false,
		'Nice!',
		'Nice!',
		false
	]
)

#.test(
	'Logical `!` before a value flips its boolean value',
	[]{
		[
			!true,
			!false
		]
	},
	[
		false,
		true
	]
)