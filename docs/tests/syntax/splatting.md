# Splatting
When using the `*variable` syntax within an expression with an existing container value the value is splatted into the container.

---

##### An array can be spread into another array with `*variable`

###### Example:
```

		test = [ 1, 2, 3, 4 ]
		[ 0, *test, 5, 6, 7 ]
	
```
```
=> [
  0,
  1,
  2,
  3,
  4,
  5,
  6,
  7
]
```

---

##### An array can be spread into an argument list with `*variable`

###### Example:
```

		spread = []{ #.args }
		spread('before', *[ 1, 2, 3, 4 ], *'nice', 'after')
	
```
```
=> [
  'before',
  1,
  2,
  3,
  4,
  'n',
  'i',
  'c',
  'e',
  'after'
]
```

---

##### A string can be spread into an array with `*variable`

###### Example:
```
[*'Nice!']
```
```
=> [
  'N',
  'i',
  'c',
  'e',
  '!'
]
```

