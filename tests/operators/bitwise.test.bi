/*
	Bitwise Operators
*/

#.test('Bitwise & AND between numbers', []{ 7 & 4 }, 4)
#.test('Bitwise | OR between numbers', []{ 7 | 4 }, 7)
#.test('Bitwise ^ XOR between numbers', []{ 7 ^ 4 }, 3)
#.test('Bitwise ~ NOT on a number', []{ ~4 }, -5)
#.test('Bitwise << LEFT SHIFT between numbers', []{ 5 << 2 }, 20)
#.test('Bitwise >> RIGHT SHIFT between numbers', []{ 16 >> 2 }, 4)