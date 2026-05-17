# Context
The `#` table provides details about the current scope.

---

##### On the base scope of a script the context provides the scripts path, the incoming arguments and utility functions

###### Example:
```

		{
			withoutArguments: import('context.imported.bi'),
			withArguments: import('context.imported.bi', { one: 1, two: 2, three: 3 })
		}
	
```
```
=> {
  withoutArguments: {
    path: 'tests/environment/context.imported.bi',
    in: void,
    test: '{ Block }'
  },
  withArguments: {
    path: 'tests/environment/context.imported.bi',
    in: {
      one: 1,
      two: 2,
      three: 3
    },
    test: '{ Block }'
  }
}
```

---

##### Within a call of a block the context provides extra details about the current scope including the arguments it was called with

###### Example:
```

		{
			withoutArguments: contextBlock(),
			withArguments: contextBlock(1, 2, 3)
		}
	
```
```
=> [{ withoutArguments, withArguments }]{
		withoutArguments.args == [] &&
		withoutArguments.called == contextBlock &&
		withArguments.args == [ 1, 2, 3 ] &&
		withArguments.called == contextBlock
	}
```

