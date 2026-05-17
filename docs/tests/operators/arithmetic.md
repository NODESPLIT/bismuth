# Arithmetic Operators

---

##### Addition of numbers adds the numbers together

###### Example:
```
1 + 1
```
```
=> 2
```

---

##### Addition of anything non string to a string on the right side describes the left side and concatenates the strings

###### Example:
```

		[
			1 + 'string',
			void + 'string',
			true + 'string',
			(1 -> 4) + 'string',
			[]{} + 'string'
		]
	
```
```
=> [
  '1string',
  'voidstring',
  'truestring',
  '( 1 -> 4 )string',
  '[]{}string'
]
```

---

##### Addition of strings concatenates the strings

###### Example:
```
'left' + 'right'
```
```
=> 'leftright'
```

---

##### Addition of arrays concatenates the arrays

###### Example:
```
[ 1, 2, 3 ] + [ 4, 5, 6 ]
```
```
=> [
  1,
  2,
  3,
  4,
  5,
  6
]
```

---

##### Addition of an array with a table flattens the keys and values into the array

###### Example:
```
[ 1, 2, 3 ] + { four: 4, five: 5, six: 6 }
```
```
=> [
  1,
  2,
  3,
  'four',
  4,
  'five',
  5,
  'six',
  6
]
```

---

##### Addition of an array with a string breaks up the string into characters and concatenates to the array

###### Example:
```
[ 1, 2, 3 ] + 'Nice!'
```
```
=> [
  1,
  2,
  3,
  'N',
  'i',
  'c',
  'e',
  '!'
]
```

---

##### Additions of tables concatenates the tables overwriting the existing fields on the left with the rights

###### Example:
```
{ one: 1, two: 2 } + { two: 'two', three: 3, four: 4 }
```
```
=> {
  one: 1,
  two: 'two',
  three: 3,
  four: 4
}
```

---

##### Addition of a table with an array expands the table into the array with indexes as keys

###### Example:
```
{ one: 1 } + [ 1, 2, 3, 4 ]
```
```
=> {
  one: 1,
  0: 1,
  1: 2,
  2: 3,
  3: 4
}
```

---

##### Addition of a table with a string breaks up the string into characters and concatenates to the table with indexes as keys

###### Example:
```
{ one: 1 } + 'Nice!'
```
```
=> {
  one: 1,
  0: 'N',
  1: 'i',
  2: 'c',
  3: 'e',
  4: '!'
}
```

---

##### Subtraction of numbers deducts one number from the other

###### Example:
```
10 - 5
```
```
=> 5
```

---

##### Multiplication of numbers multiplies them together

###### Example:
```
2 * 4
```
```
=> 8
```

---

##### Multiplication of a string with a number repeats that string by the amount on the right

###### Example:
```
'*' * 10
```
```
=> '**********'
```

---

##### Division of numbers divides them

###### Example:
```
10 / 2
```
```
=> 5
```

---

##### Division of strings splits the left string with the right as a delimiter

###### Example:
```
'1,2,3,4,5,6' / ','
```
```
=> [
  '1',
  '2',
  '3',
  '4',
  '5',
  '6'
]
```

---

##### Modulus between numbers applies an fmod between the numbers

###### Example:
```
[ 11 % 2, 10 % 2 ]
```
```
=> [
  1,
  0
]
```

---

##### Unary negative on a number makes that number negative

###### Example:
```
-( 12 - 2 )
```
```
=> -10
```

---

##### Increment operator used on the right side of a number increases it by one in place, resolving to the original value

###### Example:
```

		value = 10
		[ value++, value ]
	
```
```
=> [
  10,
  11
]
```

---

##### Decrement operator used on the right side of a number decreases it by one in place, resolving to the original value

###### Example:
```

		value = 10
		[ value--, value ]
	
```
```
=> [
  10,
  9
]
```

---

##### Increment operator used on the left side of a number increases it by one in place, resolving to the new value

###### Example:
```

		value = 10
		[ ++value, value ]
	
```
```
=> [
  11,
  11
]
```

---

##### Decrement operator used on the left side of a number decreases it by one in place, resolving to the new value

###### Example:
```

		value = 10
		[ --value, value ]
	
```
```
=> [
  9,
  9
]
```

