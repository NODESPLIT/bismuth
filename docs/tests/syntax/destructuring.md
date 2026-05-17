# Destructuring
Destructuring works similarly to ES6 with a splatter of Ruby.

---

##### Object and array arguments can be destructured into variables from the arguments list

###### Example:
```

		block = [{ one, two }, [ three, four ]] {
			{
				one,
				two,
				three,
				four
			}
		}

		block(
			{ one: 1, two: 2 },
			[ 3, 4 ]
		)
	
```
```
=> {
  one: 1,
  two: 2,
  three: 3,
  four: 4
}
```

---

##### Object and array values can be destructured into individual variables

###### Example:
```

		[ one, two ] = [ 1, 2, 3, 4, 5, 6 ]
		{ three, four } = { one: 1, two: 2, three: 3, four: 4, five: 5, six: 6 }
		{ one, two, three, four }
	
```
```
=> {
  one: 1,
  two: 2,
  three: 3,
  four: 4
}
```

---

##### Using `*variable` at the beginning of an argument list captures the beginning of the destructured array

###### Example:
```

		block = [[ *beginning, three ]] { beginning }
		block([ 1, 2, 3 ])
	
```
```
=> [
  1,
  2
]
```

---

##### Using `*variable` at the beginning of a destructured assignment captures the beginning of the destructured array

###### Example:
```

		[ *beginning, three, four ] = [ 1, 2, 3, 4 ]
		beginning
	
```
```
=> [
  1,
  2
]
```

---

##### Using `*variable` in the middle of an argument list captures the middle of the destructured array

###### Example:
```

		array = [[ one, *middle, six ]] { middle }
		array([ 1, 2, 3, 4, 5, 6 ])
	
```
```
=> [
  2,
  3,
  4,
  5
]
```

---

##### Using `*variable` in the middle of a destructured assignment captures the beginning of the destructured array

###### Example:
```

		[ one, *middle, six ] = [ 1, 2, 3, 4, 5, 6 ]
		middle
	
```
```
=> [
  2,
  3,
  4,
  5
]
```

---

##### Using `*variable` at the end of a destructured object or array in an argument list captures the rest of the object or array

###### Example:
```

		object = [{ one, *after }] { after }
		array = [[ one, *after ]] { after }

		{
			object: object({ one: 1, two: 2, three: 3 }),
			array: array([ 1, 2, 3 ])
		}
	
```
```
=> {
  object: {
    two: 2,
    three: 3
  },
  array: [
    2,
    3
  ]
}
```

---

##### Using `*variable` at the end of a destructured assignment captures the rest of the destructured object or array

###### Example:
```

		[ one, *after ] = [ 1, 2, 3, 4 ]
		after
	
```
```
=> [
  2,
  3,
  4
]
```

