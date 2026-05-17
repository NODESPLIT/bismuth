# Logical Operators

---

##### Logical `&&` AND between values compares their `isTrue()` results

###### Example:
```

		[
			true && true && 'Nice!',
			true && false && 'Nice!',
			1 && 1 && 'Nice!',
			0 && 1 && 'Nice!'
		]
	
```
```
=> [
  'Nice!',
  false,
  'Nice!',
  false
]
```

---

##### Logical `||` OR between values compares their `isTrue()` results

###### Example:
```

		[
			true || true && 'Nice!',
			true || false && 'Nice!',
			false || false && 'Nice!',
			1 || 1 && 'Nice!',
			0 || 1 && 'Nice!',
			0 || 0 && 'Nice!'
		]
	
```
```
=> [
  'Nice!',
  'Nice!',
  false,
  'Nice!',
  'Nice!',
  false
]
```

---

##### Logical `!` before a value flips its boolean value

###### Example:
```

		[
			!true,
			!false
		]
	
```
```
=> [
  false,
  true
]
```

