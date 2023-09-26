fib = [ n ] {
	if n <= 1 { return 1 }
	fib( n - 1 ) + fib( n - 2 )
}

number = 46
result = 0

duration = time.while([]{ result = fib(number) })
'recursive fib(' + number + '): ' + result + ' ( duration: ' + duration + 'ms )'