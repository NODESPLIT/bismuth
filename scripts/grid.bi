width = 100
height = 40

blocks = [ '▁', '▂', '▃', '▄', '▅', '▆', '█', '▆', '▅', '▄', '▃', '▂', '▁' ]

height : [ y ]{ log( width ~: [ line='', x ]{ line + blocks[ y * x % 13 ] } ) }