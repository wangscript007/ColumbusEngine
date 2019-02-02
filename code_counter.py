import os, sys

cpp_lines = int(0)
cpp_header_lines = int(0)
glsl_lines = int(0)

for root, dirs, files in os.walk(os.path.dirname(sys.argv[0])):
	dirs[:] = [d for d in dirs if d not in 'Lib']

	for file in files:
		name, ext = os.path.splitext(file)

		if ext == '.cpp':
			cpp_lines += sum(1 for line in open(root + '/' + file))

		if ext == '.h':
			cpp_header_lines += sum(1 for line in open(root + '/' + file))

		if ext == '.glsl':
			glsl_lines += sum(1 for line in open(root + '/' + file))

print('C/C++ code lines: ', cpp_lines)
print('C/C++ headers lines: ', cpp_header_lines)
print('GLSL code lines: ', glsl_lines)
print('')
print('Total: ', cpp_lines + cpp_header_lines + glsl_lines)




