for /d %%f in (*) do (
for /d %%g in (%%f\*) do (
for /r %%i in (%%g\*.txt) do (
start /w /b ..\..\Debug\AutoTester.exe %%f\source.txt %%i %%~pi%%~ni.xml )))