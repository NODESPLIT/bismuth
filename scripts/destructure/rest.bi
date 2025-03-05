[ one, *rest ] = [ 1, 2, 3, 4 ]

log('one:', one)
log('rest:', rest)


block = [ first, *arguments ]{
	log('first:', first)
	log('arguments:', arguments)
}

log('')
log('rest arguments call:')
block('argument!', 1, 2, 3, 4, 5)


spread = []{ log('args:', #.args) }

log('')
log('spread call:')

test = [ 1, 2, 3, 4 ]
spread('before', *[ 0, *test, 5, 6, 7 ], 'after')


log('')
log('args:')
[ first, second, *middle, penultimate, ultimate ] {
	log('first:', first)
	log('second:', second)
	log('middle:', middle)
	log('penultimate:', penultimate)
	log('ultimate:', ultimate)
}(1, 2, 3, 4, 5, 6, 7, 8, 9)


[ first, second, *middle, penultimate, ultimate ] = [ 1, 2, 3, 4, 5, 6, 7, 8, 9 ]

log('')
log('destructure:')
log('first:', first)
log('second:', second)
log('middle:', middle)
log('penultimate:', penultimate)
log('ultimate:', ultimate)