Team04 SPA

VS2017 is used to compile and run the programme in Release configuration.

Solution to be built: StartupSPASolution.sln
Path to executable (in Release configuration): Release/AutoTester.exe
Path to executable (in Debug configuration): Debug/AutoTester.exe

To test, run following command in current directory.
Single test command: .\Release\AutoTester.exe .\Tests\Iteration1Tests\[nest_level]\[source_file].txt .\Tests\Iteration1Tests\[nest_level]\[num_of_clauses]\[type_of_clauses].txt .\Tests\Iteration1Tests\[nest_level]\[num_of_clauses]\[type_of_clauses].xml
Batch test command: python .\Tests\Iteration1Tests\iter1tests.py

