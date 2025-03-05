block = [ [ one ], two ] { one + two }

block([ 1 ], 4)



[ one, two ] = [ 0, 1 ]


[] {
	{
		nice: 'Nice!'
		test: []{ log('Nice value: ', @.nice) }
	}
}