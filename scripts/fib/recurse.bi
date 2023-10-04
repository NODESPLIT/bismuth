fib = [ n ] {
	if n <= 1 { return n }
	fib( n - 1 ) + fib( n - 2 )
}

number = 35

duration = time.while([]{ result = fib(number) })
'recursive fib(' + number + '): ' + result + ' ( duration: ' + duration + 'ms )'
