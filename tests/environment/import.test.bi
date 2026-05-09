/*
	Import
	The `import` function loads a .bi file or a .crystal file and calls it with the given arguments, returning the resolved return value.
*/

#.test(
	'Import loads and evaluates a .bi file',
	[]{ import('import.imported.bi') },
	{ test: 100 }
)

#.test(
	'Import loads and evaluates a .bi file providing an argument value',
	[]{ import('import.imported.bi', 20) },
	{ test: 20 }
)

#.test(
	'Import loads and runs a .crystal dynamic library',
	[]{
		crystal = import('../crystal/build/example.crystal')
		[ crystal.string, crystal.test() ]
	},
	[
		'Nice!',
		{ subtest: 200 }
	]
)