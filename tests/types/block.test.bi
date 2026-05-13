/*
	Blocks
	Functions in Bismuth have a single syntax and are called Blocks.
	Square brackets wrap the argument list like a C++ lambda, then curly brackets wrap the blocks code.
	They can be called by variable or anonymously with normal brackets.
*/

#.test(
	'Blocks can be defined and called with arguments',
	[]{
		test = [one, two] {
			result = one * two
			result / 2
		}

		test(10, 4)
	},
	20
)

#.test(
	'Blocks can be called anonymously',
	[]{
		[one, two]{
			result = one * two
			result / 2
		}(10, 4)
	},
	20
)

#.test(
	'The `return` keyword can be used to pass back a value and stop execution',
	[]{
		[]{
			test = 10
			return test
			20
		}()
	},
	10
)