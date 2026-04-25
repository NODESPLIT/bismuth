Render = import('crystals/draw/draw.crystal')
{ Window, Draw } = Render

width = 480
height = 480

Window({
	title: 'Bismuth - Text',
	width, height,
	render: []{
		Draw.Text([ 10, 10 ], 'chicago.ttf', 'Hello, world!')
	}
})