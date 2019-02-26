for /d %%f in (*) do (
for /d %%g in (%%f\*) do (
for /r %%i in (%%g\*) do (
..\..\Debug\AutoTester.exe %%f\source.txt %%i %%i.xml 
pause )))