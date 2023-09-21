test = import('tests/imported')

if number > 2 {
  log('number is higher than 2');
} else number < 2 {
  log('number is smaller than 2');
} else {
  log('number is 2');
}

log( 'Log: ' + if number > 2 { 'higher than 2' } else number < 2 { 'lower than 2' } else { 'is 2' } );

when number {
  2: 'is two',
  1: 'is one',
  -: 'is number'
}

log( 'Log: ' + when number { 2: 'is two', 1: 'is one', -: 'is number' } );

variable = 'text';
number = 10.4;
n1 = 'string';

table = {
  one: 'one',
  two: 'two'
};

list = [ 1, 2, 'Hello, world!', 3, 4 ];
list[3];

character = ([ test ]{ number + test })('text')[2];


100 : [index] {
  index * 2
}