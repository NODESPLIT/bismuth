Dog = [ face, name, breed, says ] {
	{
		face, name, breed, says,
		message: name + ' the ' + breed + ' says:  ' + face + ' -=( ' + says + ' )',
		bark: []{ log(@.message) }
	}
}

Dogs = [
	Dog(
		'໒( ̿･ ᴥ ̿･ )ʋ',
		'Steve',
		'Labrador',
		'Woof!'
	),
	Dog(
		'૮ ˶′ﻌ ‵˶ ა',
		'Bella',
		'Dachshund',
		'Damn... I am a dog that speaks English... erm, kinda cringe!'
	),
	Dog(
		'( ͡° ᴥ ͡° ʋ)',
		'Jeremiah',
		'Rottweiler',
		'Bork!'
	),
	Dog(
		'૮ ˙ ﻌ˙ ა',
		'Timothy',
		'Golden Retriever',
		'Le epic doggo'
	),
	Dog(
		'໒( ̿❍ ᴥ ̿❍)ʋ',
		'Antonio Banderas',
		'Basset Hound',
		'I am spanish'
	)
]

Dogs : [ dog ] { dog.bark() }