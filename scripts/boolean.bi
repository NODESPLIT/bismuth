test = [ value ] { 'result: ' + if value == 10 { value + ' DOES equal 10!!!' } else value == 2 { value + ' DOES equal 2 !!!!' } else { value + ' IS SOMETHING ELSE !!!!!!!!' } + '!!' }

log('test(10): ', test(10))
log('test(2): ', test(2))
log('test(5): ', test(5))