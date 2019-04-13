import os
import sys
from subprocess import check_output, TimeoutExpired


fail_cases = []
error_cases = []
timeout_cases = []

current_path = os.getcwd()

cmd = r"..\..\Debug\AutoTester.exe"
args = sys.argv
if len(args) != 1:
	if args[1].lower() == "r":
		cmd = r"..\..\Release\AutoTester.exe"
		print("Running System Test on Release Mode...\n")
	if args[1].lower() == "d":
		cmd = r"..\..\Debug\AutoTester.exe"
		print("Running System Test on Debug Mode...\n")
for root, dirs, files in os.walk(current_path):
	for nest in dirs:
		if "special" in nest:
			for root2, dirs2, files2 in os.walk(nest):
				for file in files2:
					if "_source.txt" in file:
						try:
							filename = file.split("_source.txt")[0]
							source = root + '\\' + root2 + '\\' + filename + "_source.txt"
							query = root + '\\' + root2 + '\\' + filename + "_queries.txt"
							output = root + '\\' + root2 + '\\' + filename + "_queries.xml"
							if os.path.exists(output):
								os.remove(output)
							out = check_output([cmd, source, query, output], timeout=60).strip().decode()
							print(out)
							if 'Missing' in out or 'Additional' in out:
								fail_cases.append(root2 + '\\' + filename + "_queries.txt")
							if 'TIMEOUT' in out:
								timeout_cases.append(root2 + '\\' + filename + "_queries.txt")	
						except TimeoutExpired as e:
							print("Query File Timeout: " + root2 + '\\' + filename + "_queries.txt")
						except Exception as e:
							print(e)
							error_cases.append(root2 + '\\' + filename + "_queries.txt")
		else:
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