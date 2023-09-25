count = 0

block = [ number = if count == 3 { count } else { count++ }, other = 20 - number ] {
	log('count:', count, other)
}

block()
block()
block()
block()
block()
block()
block()