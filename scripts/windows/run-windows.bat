@ECHO off

SET test_folder=..\..\Team14\Tests14\playground
SET result_folder=..\..\Team14\Code14\tests

SET /A counter=0
SETLOCAL ENABLEDELAYEDEXPANSION
:loop
    SET /A counter+=1
    IF !counter! EQU 3 (
        GOTO done
    ) ELSE (
        ECHO ==============================================================
        ECHO Running test !counter!
        python3 -m mprof run -M -T 0.001 ..\..\Team14\Code14\build\src\runner\release\runner %test_folder%\0.txt %test_folder%\0_!counter!.txt > %result_folder%\!counter!.log
        FINDSTR /i /c:"Time taken to load source file" %result_folder%\!counter!.log
        FINDSTR /i /c:"Time taken to fetch results from pkb" %result_folder%\!counter!.log
        FINDSTR /i /c:"Number of queries" %result_folder%\!counter!.log
        FINDSTR /i /c:"Number of cache hits" %result_folder%\!counter!.log
        FINDSTR /i /c:"Number of cache misses" %result_folder%\!counter!.log
        FINDSTR /i /c:"Total context size:" %result_folder%\!counter!.log
        FINDSTR /i /c:"Time taken to project results" %result_folder%\!counter!.log
        FINDSTR /i /c:"Time taken to execute query" %result_folder%\!counter!.log
        python3 -m mprof peak
        GOTO loop
    )
:done
EXIT