# Filesystem
Simple API for interacting with the filesystem.

---

##### `filesystem.tree` deeply recurses through the filesystem tree starting at the given path, returning the path list

###### Example:
```
filesystem.tree('filesystem.imported')
```
```
=> [
  'filesystem.imported/1',
  'filesystem.imported/2',
  'filesystem.imported/3',
  'filesystem.imported/4',
  'filesystem.imported/4/5'
]
```

---

##### `filesystem.list` shallowly lists the filesystem and folders at the given path

###### Example:
```
filesystem.list('filesystem.imported')
```
```
=> [
  'filesystem.imported/1',
  'filesystem.imported/2',
  'filesystem.imported/3',
  'filesystem.imported/4'
]
```

---

##### `filesystem.folder('path/to/folder')` can be used to check if a path exists as a folder

###### Example:
```

    [
      filesystem.folder('filesystem.imported'),
      filesystem.folder('filesystem.imported/2'),
      filesystem.folder('filesystem.imported/2/nothing')
    ]
  
```
```
=> [
  true,
  false,
  false
]
```

---

##### `filesystem.file('path/to/folder')` can be used to check if a path exists as a file

###### Example:
```

    [
      filesystem.file('filesystem.imported/1'),
      filesystem.file('filesystem.imported/4'),
      filesystem.file('filesystem.imported/nothing')
    ]
  
```
```
=> [
  true,
  false,
  false
]
```

---

##### `filesystem.path('path/to/folder')` can be used to check if a path exists

###### Example:
```

    [
      filesystem.path('filesystem.imported/1'),
      filesystem.path('filesystem.imported/4'),
      filesystem.path('filesystem.imported/nothing')
    ]
  
```
```
=> [
  true,
  true,
  false
]
```

---

##### `filesystem.build('path/to/folders')` can be used to create a folder structure for a given path

###### Example:
```

    [
      filesystem.build('filesystem.imported/5/test/folder'),
      filesystem.folder('filesystem.imported/5/test/folder'),
      filesystem.folder('filesystem.imported/5/test'),
      filesystem.folder('filesystem.imported/5'),
      filesystem.file('filesystem.imported/5/test')
    ]
  
```
```
=> [
  true,
  true,
  true,
  true,
  false
]
```

---

##### `filesystem.delete('path/to/folders')` can be used to delete the given path

###### Example:
```

    [
      filesystem.delete('filesystem.imported/5'),
      filesystem.folder('filesystem.imported/5'),
      filesystem.folder('filesystem.imported/5/test')
    ]
  
```
```
=> [
  true,
  false,
  false
]
```

---

##### `filesystem.read('path/to/folders')` can be used to read a file at the given path, by default it reads the file as text but a custom driver can be used as the second argument to read different formats

###### Example:
```
filesystem.read('filesystem.imported/1')
```
```
=> '1'
```

---

##### `filesystem.write('path/to/folders', content)` can be used to write to a given path, by default it writes text but a custom driver can be used as the third argument to write different formats

###### Example:
```

    [
      filesystem.write('filesystem.imported/1', 'Nice!'),
      filesystem.read('filesystem.imported/1'),
      filesystem.write('filesystem.imported/1', '1'),
      filesystem.read('filesystem.imported/1')
    ]
  
```
```
=> [
  true,
  'Nice!',
  true,
  '1'
]
```

