/*
	Instance
	The `@` symbol points to the current called blocks container, this allows for something akin to minimal OOP where a block can return an object containing instance methods and values where the methods can read and write the object easily.
*/

#.test(
	'The `@` symbol refers to the called methods container',
	[]{
		Class = [value] {
			{
				value,
				method: []{ @.value }
			}
		}

		instance = Class('nice!')
		instance.method()
	},
	'nice!'
)