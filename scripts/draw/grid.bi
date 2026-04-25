Render = import('crystals/draw/draw.crystal')
{ Window, Draw } = Render

width = 480
height = 480

tx = 16
ty = 16
t = tx * ty

cell = 16
cx = (width / 2) - ((tx * cell) / 2)
cy = (height / 2) - ((ty * cell) / 2)

Window({
	title: 'Bismuth - Line Grid',
	width, height,
	render: []{
		tx : [ y ]{
			ty : [ x ]{
				now = time.now() * 0.0008

				ox = cx + ( x * cell )
				oy = cy + ( y * cell )

				spinning = time.now() * 0.001
				spin = math.sin(spinning) * 0.4

				mx = math.cos(now) * ( spin )
				my = math.sin(now) * ( spin )

				Draw.Line(
					[ ox, oy ],
					[
						ox + (y * x * mx),
						oy + (y * x * my)
					],
					{ color: 16xFFFFFF }
				)
			}
		}
	}
})