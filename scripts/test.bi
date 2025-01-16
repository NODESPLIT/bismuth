block = [ n ]{
	log(n)
	if n >= 10 {
		return 10
	} else {
		block(n + 1)
	}
}

block(0)