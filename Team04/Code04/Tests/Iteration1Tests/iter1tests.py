import os
import sys
from subprocess import check_output


fail_cases = []
error_cases = []

cmd = r"..\..\Debug\AutoTester.exe"
for root, dirs, files in os.walk("."):
	for nest in dirs:
		source = nest + "\\source.txt"
		for root2, dirs2, files2 in os.walk(nest):
			for file in files2:
				if 'txt' in file and 'source' not in file:
					try:
						query = root2 + '\\' + file
						output = root2 + '\\' + file.split('.')[0] + '.xml'
						out = check_output([cmd, source, query, output], shell=True).strip().decode()
						print(out)

						if 'Missing' in out or 'Additional' in out:
							fail_cases.append(root2 + '\\' + file)
					except Exception as e:
						print(e)
						error_cases.append(root2 + '\\' + file)
						
					
						
print()
print('-----------------------------------------------')
print('Failed Test Cases')
print('-----------------------------------------------')
for file in fail_cases:
	print(file)

print()
print('-----------------------------------------------')
print('Error Test Cases')
print('-----------------------------------------------')
for file in error_cases:
	print(file)
print()