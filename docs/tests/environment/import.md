# Import
The `import` function loads a .bi file or a .crystal file and calls it with the given arguments, returning the resolved return value.

---

##### `import` loads and evaluates a .bi file

###### Example:
```
import('import.imported.bi')
```
```
=> {
  test: 100
}
```

---

##### `import` loads and evaluates a .bi file providing an argument value

###### Example:
```
import('import.imported.bi', 20)
```
```
=> {
  test: 20
}
```

---

##### `import` loads and runs a .crystal dynamic library

###### Example:
```

		crystal = import('../crystal/build/example.crystal')
		[ crystal.string, crystal.test() ]
	
```
```
=> [
  'Nice!',
  {
    subtest: 200
  }
]
```

