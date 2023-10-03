recurse = [ i ] {
	if i >= 2000 { i } else {
		log('recurse: ' + i)
		recurse(i + 1)
	}
}

recurse(0)