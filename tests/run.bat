if ""=="%1" goto erro

set files = %CD%;
for /f %%f in ('dir /b ..\input\test') do %1_test.exe <%%f> ..\logs\%1\%%~nf.log
goto sai

:erro
echo on
REM "Entrada errada!"

:sai