/*
  File
  Simple API for interacting with the filesystem.
*/

#.test(
  'file.tree deeply recurses through the filesystem tree starting at the given path, returning the path list',
  []{ file.tree('files.imported') },
  [
    'files.imported/1',
    'files.imported/2',
    'files.imported/3',
    'files.imported/4',
    'files.imported/4/5'
  ]
)

#.test(
  'file.list shallowly lists the files and folders at the given path',
  []{ file.list('files.imported') },
  [
    'files.imported/1',
    'files.imported/2',
    'files.imported/3',
    'files.imported/4'
  ]
)