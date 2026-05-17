# Inside Operators
The inside operators are an experimental set of operators to provide looping, mapping, and reduction with shallow and deep options among other things. You can do some cool stuff with these chained together.

---

##### `~:` operator shallow reduces the left side with the block on the right

###### Example:
```

		[
			10 ~: [result=0, value]{ result + value },
			(10 -> 20) ~: [result=0, value]{ result + value },
			'Nice!' ~: [result='', value]{ value + result },
			[ 1, 2, 3 ] ~: [result=[], value]{ [ value ] + result },
			{ one: 1, two: 2, three: 3 } ~: [result={}, value, key]{ result[key] = value * 2; result }
		]
	
```
```
=> [
  45,
  165,
  '!eciN',
  [
    3,
    2,
    1
  ],
  {
    one: 2,
    two: 4,
    three: 6
  }
]
```

---

##### `-:` operator returns a new instance of left side with elements removed if keys match the right side or block returns true

###### Example:
```

		[
			{ one: 1, two: 2, three: 3, four: 4, five: 5, six: 6 } -: [value, key]{ value > 3 },
			[ 1, 2, 3, 4, 5, 6 ] -: [value]{ value > 3 },
			'Nice!' -: [value]{ value == 'i' || value == '!' },
			{ one: 1, two: 2, three: 3, four: 4, five: 5, six: 6 } -: [ 'two', 'three', 'four' ],
			[ 1, 2, 3, 4, 5, 6 ] -: [ 0, 1, 2 ],
			{ one: 1, two: 2, three: 3, four: 4, five: 5, six: 6 } -: 'two',
			[ 1, 2, 3, 4, 5, 6 ] -: 0,
			[ 1, 2, 3, 4, 5, 6 ] -: (2 -> 4)
		]
	
```
```
=> [
  {
    one: 1,
    two: 2,
    three: 3
  },
  [
    1,
    2,
    3
  ],
  'Nce',
  {
    one: 1,
    five: 5,
    six: 6
  },
  [
    4,
    5,
    6
  ],
  {
    one: 1,
    three: 3,
    four: 4,
    five: 5,
    six: 6
  },
  [
    2,
    3,
    4,
    5,
    6
  ],
  [
    1,
    2,
    6
  ]
]
```

---

##### `:-` operator returns a new instance of left side with elements removed if values match the right side

###### Example:
```

		[
			{ one: 1, two: 2, three: 3 } :- [ 1, 2 ],
			[ 1, 2, 3, 4, 5, 6, 7, 8 ] :- [ 4, 5, 6 ],
			{ one: 0, two: 1, three: 0, four: 1, five: 1 } :- 1,
			[ 0, 1, 0, 1, 1, 0, 1, 0 ] :- 1
		]
	
```
```
=> [
  {
    three: 3
  },
  [
    1,
    2,
    3,
    7,
    8
  ],
  {
    one: 0,
    three: 0
  },
  [
    0,
    0,
    0,
    0
  ]
]
```

---

##### `+:` operator returns a new instance of left side with elements kept if block returns true

###### Example:
```

		[
			{ one: 1, two: 2, three: 3 } +: [value]{ value >= 2 },
			[ 1, 2, 3, 4, 5, 6 ] +: [value]{ value > 3 },
			'Nice!' +: [value]{ value == 'i' || value == '!' }
		]
	
```
```
=> [
  {
    two: 2,
    three: 3
  },
  [
    4,
    5,
    6
  ],
  'i!'
]
```

---

##### `%:` between operator places the resolved value on the right between every element in the container on the left

###### Example:
```

		[
			'Nice!' %: ',',
			[ 1, 2, 3, 4, 5, 6 ] %: '!',
			[ 1, 2, 3, 4, 5, 6 ] %: [ 0 ],
			[ 1, 2, 3, 4, 5, 6 ] %: [left, right]{ left + right },
			[ 1, 2, 3, 4, 5, 6 ] %: (1 -> 4)
		]
	
```
```
=> [
  'N,i,c,e,!',
  '1!2!3!4!5!6',
  [
    1,
    0,
    2,
    0,
    3,
    0,
    4,
    0,
    5,
    0,
    6
  ],
  [
    1,
    3,
    2,
    5,
    3,
    7,
    4,
    9,
    5,
    11,
    6
  ],
  [
    1,
    ( 1 -> 4 ),
    2,
    ( 1 -> 4 ),
    3,
    ( 1 -> 4 ),
    4,
    ( 1 -> 4 ),
    5,
    ( 1 -> 4 ),
    6
  ]
]
```

---

##### `^>` operator returns a descending sorted array when used against an array

###### Example:
```
^>[ 5, 2, 3, 6, 1, 4 ]
```
```
=> [
  6,
  5,
  4,
  3,
  2,
  1
]
```

---

##### `^<` operator returns an ascending sorted array when used against an array

###### Example:
```
^<[ 5, 2, 3, 6, 1, 4 ]
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

##### `^:` operator returns an array sorted by the block on the right when used against an array

###### Example:
```
[ 5, 2, 3, 6, 1, 4 ] ^: [left, right]{ left > right }
```
```
=> [
  6,
  5,
  4,
  3,
  2,
  1
]
```

---

##### `:` the within operator maps through a container with the given block

###### Example:
```

		[
			4 : [value]{ value * 2 },
			(2 -> 8) : [value]{ value * 2 },
			'Nice!' : [value]{ value + '.' },
			[ 1, 2, 3, 4 ] : [value]{ value * 2 },
			{ one: 1, two: 2, three: 3, four: 4 } : [value, key]{ value * 2 }
		]
	
```
```
=> [
  [
    0,
    2,
    4,
    6
  ],
  [
    4,
    6,
    8,
    10,
    12,
    14,
    16
  ],
  'N.i.c.e.!.',
  [
    2,
    4,
    6,
    8
  ],
  {
    one: 2,
    two: 4,
    three: 6,
    four: 8
  }
]
```

