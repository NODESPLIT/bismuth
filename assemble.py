import sys, os, json

root = sys.argv[1]

def Save(path, content):
	with open(path, 'w') as file:
		file.write(content)

def Operators():
	global ops, root

	types = [
		'Void',
		'Boolean',
		'Number',
		'Range',
		'String',
		'Array',
		'Table',
		'Block'
	]

	count = len(types)
	pairs = count * count

	invalid = '{ return Value::Empty(); }'
	folder = root + '/src/runtime/operators'
	operators = {}
	indexes = {}
	ops = 0

	def Variable(i, left, right):
		return left + right + str(i)

	def Insert(transform):
		global ops

		if transform['operator'] not in operators:
			operators[transform['operator']] = {}
			indexes[transform['operator']] = ops
			ops += 1

		if transform['left'] not in operators[transform['operator']]:
			operators[transform['operator']][transform['left']] = {}

		operators[transform['operator']][transform['left']][transform['right']] = transform['body']

	def Parse(descriptor):
		class Focus:
			Nothing = 0
			Name = 1
			Type = 2
			Body = 3

		focus = Focus.Nothing

		transform = {}
		content = ''

		for character in descriptor:
			if focus == Focus.Nothing:
				if character == '[':
					content = ''
					focus = Focus.Name
				elif character == '(':
					content = ''
					focus = Focus.Type
				elif content[-2:] == ':{':
					content = '{'
					focus = Focus.Body

			elif focus == Focus.Name:
				if character == ']':
					transform['operator'] = content[1:].strip()
					focus = Focus.Nothing

			elif focus == Focus.Type:
				if character == ')':
					types = content[1:].split(',')
					transform['left'] = types[0].strip()
					transform['right'] = types[1].strip()
					focus = Focus.Nothing

			elif focus == Focus.Body:
				if content[-2:] == '}:':
					transform['body'] = content[:-1]
					focus = Focus.Nothing
					Insert(transform)

			content += character

	def Load(path):
		file = open(folder + '/' + path, 'r')
		Parse(file.read() + '\n')

	for path in os.listdir(folder):
		if path.endswith('.opp'):
			Load(path)

	document = 'namespace Operate {\n  Reference Invalid(Reference left, Reference right) ' + invalid

	for operator in operators:
		transforms = operators[operator]
		for left in transforms:
			for right in transforms[left]:
				document += '\n\n  Reference ' + Variable(indexes[operator], left, right) + '(Reference left, Reference right) ' + transforms[left][right].replace('\n', '\n' + '  ') + ';'

	document += '\n\n  Reference (*List[])(Reference, Reference) = { '

	for operator in operators:
		transforms = operators[operator]
		for x in range(0, count):
			left = types[x]
			for y in range(0, count):
				right = types[y]
				pointer = 'Invalid'

				if left in transforms and right in transforms[left]:
					pointer = Variable(indexes[operator], left, right)
				elif left in transforms and 'Any' in transforms[left]:
					pointer = Variable(indexes[operator], left, 'Any')
				elif 'Any' in transforms and right in transforms['Any']:
					pointer = Variable(indexes[operator], 'Any', right)
				elif 'Any' in transforms and 'Any' in transforms['Any']:
					pointer = Variable(indexes[operator], 'Any', 'Any')

				document += '&{name}, '.format( name = pointer )

	document += '};\n\n'

	document += '  int Count = ' + str(count) + ';\n';
	document += '  int Pairs = ' + str(pairs) + ';\n\n';

	document += '  unordered_map<string, int> Reduced = {\n'

	for operator in indexes:
		document += '    { "' + operator + '", ' + str(indexes[operator] * pairs) + ' },\n'

	document += '  };\n'

	document += '}\n\n'

	document += 'namespace Names {\n'
	document += '  unordered_map<int, string> Operators = {\n'

	for operator in indexes:
		document += '    { ' + str(indexes[operator] * pairs) + ', "' + operator + '" },\n'

	document += '  };\n'
	document += '}'

	return document

Save(root + '/src/runtime/operators.cpp', Operators())