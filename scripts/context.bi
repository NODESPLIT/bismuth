plog('#:', #)

Class = [ arg1 ] {
	{
		changes: 0,
		arg1: arg1,
		nice: 'Nice!'
		test: [] {
			plog('@:', @)
			plog('#:', #)
			log('Nice value:', @.nice)
			log('arg1 value:', @.arg1)
			@.changes += 1
			@.fresh = 'working'
			log('#.args:', #.args ~: [sum=0, number] {
				log('inside @:', @)
				log('inside #:', #)
				sum + number
			})
		}
	}
}

log('Creating instance')
instance = Class('It works!')

log('Instance:', instance)

instance.test(1,2,3)

log('Instance:', instance)