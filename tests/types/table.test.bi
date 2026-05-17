/*
	Table
	Tables in Bismuth have the same base syntax as ES6 objects.
*/

#.test(
	'Tables are defined between curly brackets `{}` with pairs of `key: value` separated by commas.',
	[]{
		table = {
			one: 1,
			two: 2
		}

		table.one
	},
	1
)

#.test(
	'You can access members of objects with `.` dot syntax or `[]` square brackets.',
	[]{
		table = {
			one: 1,
			two: 2
		}

		[
			table.one,
			table['two']
		]
	},
	[
		1,
		2
	]
)

#.test(
	'You can provide an `@` table in a table to define special functionality akin to a Lua metatable',
	[]{
		count = {
			@: {
				call: []{ ++@.total },
				index: [key]{
					if key == 'override' { return 'override: ' + @.string }
				}
			},
			total: 0,
			string: 'Nice!'
		}

		10 : []{ count() }

		[
			count.total,
			count.string,
			count.override,
			count.unset
		]
	},
	[
		10,
		'Nice!',
		'override: Nice!',
		void
	]
)