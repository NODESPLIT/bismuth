/*
	Destructuring
	Destructuring works similarly to ES6 with a splatter of Ruby.
*/

#.test(
	'Object and array arguments can be destructured into variables from the arguments list',
	[]{
		block = [{ one, two }, [ three, four ]] {
			{
				one,
				two,
				three,
				four
			}
		}

		block(
			{ one: 1, two: 2 },
			[ 3, 4 ]
		)
	},
	{
		one: 1,
		two: 2,
		three: 3,
		four: 4
	}
)

#.test(
	'Object and array values can be destructured into individual variables',
	[]{
		[ one, two ] = [ 1, 2, 3, 4, 5, 6 ]
		{ three, four } = { one: 1, two: 2, three: 3, four: 4, five: 5, six: 6 }
		{ one, two, three, four }
	},
	{
		one: 1,
		two: 2,
		three: 3,
		four: 4
	}
)


#.test(
	'Using `*variable` at the beginning of an argument list captures the beginning of the destructured array',
	[]{
		block = [[ *beginning, three ]] { beginning }
		block([ 1, 2, 3 ])
	},
	[ 1, 2 ]
)

#.test(
	'Using `*variable` at the beginning of a destructured assignment captures the beginning of the destructured array',
	[]{
		[ *beginning, three, four ] = [ 1, 2, 3, 4 ]
		beginning
	},
	[ 1, 2 ]
)


#.test(
	'Using `*variable` in the middle of an argument list captures the middle of the destructured array',
	[]{
		array = [[ one, *middle, six ]] { middle }
		array([ 1, 2, 3, 4, 5, 6 ])
	},
	[ 2, 3, 4, 5 ]
)

#.test(
	'Using `*variable` in the middle of a destructured assignment captures the beginning of the destructured array',
	[]{
		[ one, *middle, six ] = [ 1, 2, 3, 4, 5, 6 ]
		middle
	},
	[ 2, 3, 4, 5 ]
)


#.test(
	'Using `*variable` at the end of a destructured object or array in an argument list captures the rest of the object or array',
	[]{
		object = [{ one, *after }] { after }
		array = [[ one, *after ]] { after }

		{
			object: object({ one: 1, two: 2, three: 3 }),
			array: array([ 1, 2, 3 ])
		}
	},
	{
		object: { two: 2, three: 3 },
		array: [ 2, 3 ]
	}
)

#.test(
	'Using `*variable` at the end of a destructured assignment captures the rest of the destructured object or array',
	[]{
		[ one, *after ] = [ 1, 2, 3, 4 ]
		after
	},
	[ 2, 3, 4 ]
)