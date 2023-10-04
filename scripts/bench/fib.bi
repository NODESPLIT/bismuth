fib = [ n ] {
	if n <= 1 { return n }
	fib( n - 1 ) + fib( n - 2 )
}

fib(20)