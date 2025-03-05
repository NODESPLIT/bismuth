one = [{ one, two, *resttable }, three, [ four, { five, five: [ six, *rest ] } ]]{
	log(
		'one:', one,
		'two:', two,
		'resttable:', resttable,
		'three:', three,
		'four:', four,
		'five:', five,
		'six:', six,
		'rest:', rest
	)
}

one(
	{
		one: 1,
		two: 2,
		three: 3,
		four: 4
	},
	3,
	[
		4,
		{ five: [ 6, 7, 8, 9 ] }
	]
)