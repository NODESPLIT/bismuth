data = import('imports/data')

data : [ value, key ] {
	log('Type:', key)
	value : [ value ] {
		log(value.type, 'is', value.mood)
		value
	}
}