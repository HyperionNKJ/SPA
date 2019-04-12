import os
import sys
from subprocess import check_output, TimeoutExpired


fail_cases = []
error_cases = []
timeout_cases = []

current_path = os.getcwd()

cmd = r"..\..\Debug\AutoTester.exe"
for root, dirs, files in os.walk(current_path):
	for nest in dirs:
		source = root + '\\' + nest + '\\' + nest + "_source.txt"
		for root2, dirs2, files2 in os.walk(nest):
			for file in files2:
				if 'txt' in file and 'source' not in file:
					try:
						query = root2 + '\\' + file
						output = root2 + '\\' + file.split('.')[0] + '.xml'
						if os.path.exists(output):
							os.remove(output)
						out = check_output([cmd, source, query, output], timeout=60).strip().decode()
						print(out)

						if 'Missing' in out or 'Additional' in out:
							fail_cases.append(root2 + '\\' + file)
						if 'TIMEOUT' in out:
							timeout_cases.append(root2 + '\\' + file)	
					except TimeoutExpired as e:
						print("Query File Timeout: " + root2 + '\\' + file)
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
print('Timeout Test Cases')
print('-----------------------------------------------')
for file in timeout_cases:
	print(file)

print()
print('-----------------------------------------------')
print('Error Test Cases')
print('-----------------------------------------------')
for file in error_cases:
	print(file)
print()