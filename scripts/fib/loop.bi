fib = [ n ] {
  one = 0
  two = 1
  next = one

  n : [] {
    next = one + two
    one = two
    two = next
  }
}

number = 4000
result = 0

duration = time.while([]{ result = fib(number, 0) })
'loop fib(' + number + '): ' + result + ' ( duration: ' + duration + 'ms )'