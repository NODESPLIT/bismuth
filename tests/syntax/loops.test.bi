/*
	Loops
	The Range type e.g (1 -> 10) or a number can be used with one of the inside operators to be looped through, this is the way bismuth supports for loops.
*/

#.test(
	'The inside operator `:` can be used between a number and a block to loop the block that amount of times',
	[]{
		results = []
		10 : [n]{ results += [ n * 10 ] }
		results
	},
	[ 0, 10, 20, 30, 40, 50, 60, 70, 80, 90 ]
)

#.test(
	'The inside operator `:` can be used between a range and a block to loop the block through the numbers in that range',
	[]{
		results = []
		(10 -> 20) : [n]{ results += [ n * 2 ] }
		results
	},
	[ 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40 ]
)