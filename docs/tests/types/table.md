# Table
Tables in Bismuth have the same base syntax as ES6 objects.

---

##### Tables are defined between curly brackets `{}` with pairs of `key: value` separated by commas.

###### Example:
```

		table = {
			one: 1,
			two: 2
		}

		table.one
	
```
```
=> 1
```

---

##### You can access members of objects with `.` dot syntax or `[]` square brackets.

###### Example:
```

		table = {
			one: 1,
			two: 2
		}

		[
			table.one,
			table['two']
		]
	
```
```
=> [
  1,
  2
]
```

---

##### You can provide an `@` table in a table to define special functionality akin to a Lua metatable

###### Example:
```

		count = {
			@: {
				call: []{ ++@.total },
				index: [key]{
					if key == 'override' { return 'override: ' + @.string }
				}
			},
			total: 0,
			string: 'Nice!'
		}

		10 : []{ count() }

		[
			count.total,
			count.string,
			count.override,
			count.unset
		]
	
```
```
=> [
  10,
  'Nice!',
  'override: Nice!',
  void
]
```

