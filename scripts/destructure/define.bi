[
	one,
	two,
	{
		three,
		four: [ five ]
	}
] = [
	10,
	20,
	{
		three: 30,
		four: [ 100 ]
	}
]

log('one:', one)
log('two:', two)
log('three:', three)
log('five:', five)