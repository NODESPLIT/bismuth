# Comparison Operators

---

##### Lesser than between two booleans

###### Example:
```
[ true < false, false < false, false < true ]
```
```
=> [
  false,
  false,
  true
]
```

---

##### Lesser than or equal to between two booleans

###### Example:
```
[ true <= false, false <= false, false <= true ]
```
```
=> [
  false,
  true,
  true
]
```

---

##### Greater than between two booleans

###### Example:
```
[ true > false, false > false, false > true ]
```
```
=> [
  true,
  false,
  false
]
```

---

##### Greater than or equal to between two booleans

###### Example:
```
[ true >= false, false >= false, false >= true ]
```
```
=> [
  true,
  true,
  false
]
```

---

##### Lesser than between two numbers

###### Example:
```
[ 10 < 5, 5 < 10 ]
```
```
=> [
  false,
  true
]
```

---

##### Lesser than or equal to between two numbers

###### Example:
```
[ 10 <= 5, 5 <= 10, 10 <= 10 ]
```
```
=> [
  false,
  true,
  true
]
```

---

##### Greater than between two numbers

###### Example:
```
[ 10 > 5, 5 > 10 ]
```
```
=> [
  true,
  false
]
```

---

##### Greater than or equal to between two numbers

###### Example:
```
[ 10 >= 5, 5 >= 10, 10 >= 10 ]
```
```
=> [
  true,
  false,
  true
]
```

---

##### Lesser than between two strings

###### Example:
```
[ 'a/b/c' < 'a/b/d', '10/test' < '20/test', '100' < '100' ]
```
```
=> [
  true,
  true,
  false
]
```

---

##### Lesser than or equal to between two strings

###### Example:
```
[ 'a/b/c' <= 'a/b/d', '10/test' <= '20/test', '100' <= '100' ]
```
```
=> [
  true,
  true,
  true
]
```

---

##### Greater than between two strings

###### Example:
```
[ 'a/b/c' > 'a/b/d', '10/test' > '20/test', '100' > '100' ]
```
```
=> [
  false,
  false,
  false
]
```

---

##### Greater than or equal to between two strings

###### Example:
```
[ 'a/b/c' >= 'a/b/d', '10/test' >= '20/test', '100' >= '100' ]
```
```
=> [
  false,
  false,
  true
]
```

---

##### Equality operator

###### Example:
```

		[
			void == true,
			void == false,
			true == true,
			true == false,
			1 == 1,
			1 == 2,
			'test' == 'test',
			'test' == 'no',
			'test' == 0,
			0 == '0',
			(1 -> 20) == (1 -> 20),
			(1 -> 20) == (1 -> 19)
		]
	
```
```
=> [
  false,
  true,
  true,
  false,
  true,
  false,
  true,
  false,
  false,
  false,
  true,
  false
]
```

---

##### Inequality operator

###### Example:
```

		[
			void != true,
			void != false,
			true != true,
			true != false,
			1 != 1,
			1 != 2,
			'test' != 'test',
			'test' != 'no',
			'test' != 0,
			0 != '0',
			(1 -> 20) != (1 -> 20),
			(1 -> 20) != (1 -> 19)
		]
	
```
```
=> [
  true,
  false,
  false,
  true,
  false,
  true,
  false,
  true,
  true,
  true,
  false,
  true
]
```

