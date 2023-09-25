test = 10
log('=', test)

test %= 6
log('%= 6 =', test)

test++
log('++ =', test)

test--
log('-- =', test)

test += 5
log('+= 5 =', test)

test -= 1
log('-= 1 =', test)

test *= 10
log('*= 10 =', test)

test /= 2
log('/= 2 =', test)

test := [ i ] { i * 4 }
log(':=', test)