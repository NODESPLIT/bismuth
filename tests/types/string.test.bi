/*
	String
	There's multiple ways to define and use strings in bismuth as they're often the way data / markup / other languages come in to most runtimes. I've tried to make it easy to template and interweave markup and other languages.
*/

#.test(
	'Simple strings can be defined with "" or \'\' and escaped with backslash',
	[]{
		[
			[
				'Nice! "part of string" !',
				"Nice! 'part of string' !",
				'\'one\' \'two\' \'three\'',
			] : [string] {
				string / ' '
			},
			'\\',
			'New Line\nHere!!'
		]
	},
	[
		[
			[ 'Nice!', '"part', 'of', 'string"', '!' ],
			[ "Nice!", "'part", "of", "string'", "!" ],
			[ "'one'", "'two'", "'three'" ]
		],
		'\\',
		'New Line
Here!!'
	]
)