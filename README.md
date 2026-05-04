# Bismuth
Bismuth is a completely custom dynamically typed programming language and virtual machine stack built from scratch in C++. It's bindable to any C++ project and extendable through dynamic libraries (crystals). There's a lot of experimental features that I just thought were interesting ideas and implemented. This is for fun.

Also, this is barely an alpha version, please use at your own risk and be prepared for bugs.

You can check out the scripts folder for a collection of examples of the language, you can run them after install just by running `bismuth path/to/script`.

## Quick Start

Make sure you have following dependencies installed:

- `tcmalloc_minimal` (https://github.com/google/tcmalloc - there's often a package for tcmalloc_minimal, like debian has `libtcmalloc-minimal4` or for mac os `brew install gperftools`, this is optional but drastically improves performance)
- `tsl::ordered_map` (https://github.com/Tessil/ordered-map - clone and do `mkdir build && cd build && cmake .. && make && sudo make install` in the root directory)
- `textflowcpp` (https://github.com/catchorg/textflowcpp - clone this repo and install the file TextFlow.hpp somewhere in your path like `/usr/local/include`)
- `linenoise` (https://github.com/arangodb/linenoise-ng - follow the cmake install instructions)
- `boost (with filesystem)` >= 1.74.0

Then simply clone the repo and do the CMake shuffle:
```
mkdir build
cd build
cmake ..
sudo make install
```

Then you can run the bismuth repl by running `bismuth`, you can also open a .bi file with `bismuth path/to/file.bi`.

Some flags you can give to the runtime:
- `-r` / `--repl` : force load the repl
- `-q` / `--quiet` : silence all terminal output
- `-v` / `--verbose` : describe all steps of loading and running of the given file
- `-t` / `--test` : run all tests in given script or every script deep in given path

## Fundementals

- Functions are "blocks" `[args]{ code }`
- Blocks can be pointers to C/C++ functions to attach the bismuth runtime to C++.
- Blocks can be used with special operators to map, reduce and loop through data or ranges with a focus on covering as many type pairs as possible.
- Files are blocks with inputs and outputs, they can be called with arguments and can return values. This also means this supports a JavaScript ES6 style module system by simply providing an exports object at the bottom to expose your functionality to the imported variable.
- Final expressions are return values.
- Data -> Code interoperability has been a focus where all objects can be serialised into their bismuth representations (simply through logging) which can then be called through bismuth to deserialise. This combined with all files being callable functions makes for a powerful way to pipeline modular code with data.
- There is a simple DSL translated by Python into C++ prior to compilation that allows for writing operators code in a simple and comfortable context without sacrificing optimisation.
- Dynamic libraries are loaded through boost::dll, they are compiled dynamic libraries interfacing using the class in `src/crystal/api.hpp`. Within the construct override method of a subclass of the API class you can put together your external functionality and build Bismuth values and return them to the runtime with blocks to control your external code. Potential memory management issues here.

## Features

I will eventually have this fully documented. There's a lot to document with the different operators but it's worth just opening bismuth and having a play in the repl to see what happens when you do stuff. The following is some cool features and how to use them.


#### If statements as expressions

If statements in bismuth are expressions and can be woven into other expressions. Like blocks each block of the if statements last expression is the resolved value. This is optional behaviour and they can be used like normal statements with the return value being unused.

```
test = [ value ] {
	'result: ' + if value == 10 { value + ' Equals 10!' } else value == 2 { value + ' Equals 2!' } else { value + ' is neither!' } + '!!'
}

log('test(10):', test(10))
log('test(2):', test(2))
log('test(5):', test(5))
```
```
=> test(10): result: 10 Equals 10!!!
=> test(2): result: 2 Equals 2!!!
=> test(5): result: 5 is neither!!!
```

#### For loops as mapped ranges

The Range type e.g `(1 -> 10)` or a number can be used with one of the inside operators to be looped through, this is the way bismuth supports for loops.
```
10 : [n]{ log(n) } // prints 0 to 9
(10 -> 20) : [n]{ log(n) } // prints 10 to 20
```

#### Context

The `#` table provides details about the current scope.

##### On the base scope it just has the current files path and input arguments:
```log(#) => { path: './', in: void }```

##### Within the scope of a block it also provides details on the call like the arguments list:
```[]{ log(#) }(1, 2, 3) => { path: './', in: void, args: [ 1, 2, 3 ] }```


#### Imports

The `import` function loads a .bi file or a .crystal file and calls it with the given arguments, returning the resolved return value.
```
import('path/to/file.bi') // simple import of another .bi file
import('path/to/file.bi', { nice: true }) // provides the second value as the #.in value when running the file
import('path/to/file.crystal') // imports and constructs the dynamic library, providing the resolved returned value
```


#### Instance

The `@` symbol points to the current called blocks container, this allows for something akin to minimal OOP where a block can return an object containing instance methods and values where the methods can read and write the object easily.
```
Class = [value] {
	{
		value,
		method: []{ log(@.value) }
	}
}

instance = Class('nice!')
instance.method()
```
`=> nice!`


#### Destructuring

Destructuring works similarly to ES6 with a splash of Ruby.

##### Object and array arguments can be destructured into variables from the arguments list: 
```
test = [{ one, two }, [ three, four ]] {
	log(
		'one:', one,
		'two:', two,
		'three:', three,
		'four:', four
	)
}

test(
	{ one: 1, two: 2 },
	[ 3, 4 ]
)
```
`=> one: 1 two: 2 three: 3 four: 4`

##### Using `*variable` captures rest of the object or array: 
```
test = [{ one, two, *restobj }, [ three, four, *restarr ], *restargs] {
	log(
		'restobj:', restobj,
		'restarr:', restarr,
		'restargs:', restargs
	)
}

test(
	{ one: 1, two: 2, three: 3, four: 4 },
	[ 5, 6, 7, 8 ],
	9, 10, 11, 12
)
```
`=> restobj: { three: 3, four: 4 } restarr: [ 7, 8 ] restargs: [ 9, 10, 11, 12 ]`

##### It can also be used during assignment: 
```
[ one, *rest ] = [ 1, 2, 3, 4 ]
rest
```
`=> [ 2, 3, 4 ]`

##### Used at the start gets everything before the last values: 
```
[ *rest, four ] = [ 1, 2, 3, 4 ]
rest
```
`=> [ 1, 2, 3 ]`

##### Used in the middle gets everything in between: 
```
[ one, *rest, five ] = [ 1, 2, 3, 4, 5 ]
rest
```
`=> [ 2, 3, 4 ]`


#### Splatting

When using the `*variable` syntax within an expression with an existing container value the value is splatted into the container.

##### You can spread variables out into containers:
```
test = [ 1, 2, 3, 4 ]
[ 0, *test, 5, 6, 7 ]
```
`=> [ 0, 1, 2, 3, 4, 5, 6, 7 ]`

##### You can spread variables out into argument lists:
```
spread = []{ log('args:', #.args) }
test = [ 1, 2, 3, 4 ]

spread('before', *test, 'after')
```
`=> args: [ 'before', 1, 2, 3, 4, 'after' ]`

##### You can spread strings too, as they're just containers of characters:
```
[*'Nice!']
```
`=> [ 'N', 'i', 'c', 'e', '!' ]`
