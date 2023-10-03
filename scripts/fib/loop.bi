fib = [ n ] {
  one = 0
  two = 1
  next = one

  n : [i] {
    next = one + two
    one = two
    two = next
  }
}

number = 46
result = 0

duration = time.while([]{ result = fib(number) })
'loop fib(' + number + '): ' + result + ' ( duration: ' + duration + 'ms )'