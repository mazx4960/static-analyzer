@ECHO off

REM run stress tests
SET test_folder=..\..\Team14\Tests14\system
SET result_folder=..\..\Team14\Code14\tests
SET test_names=(^
  compute_centroid^
  qps_stress_1^
  sp_stress_1^
  stress_1^
  stress_2^
)
SET /A is_failure=0

FOR %%i IN %test_names% DO (
    ECHO ==============================================================
    ECHO Profiling %%i
    python3 ..\..\run_with_mprof.py -t 0.001 -c ..\..\Team14\Code14\build\src\autotester\release\autotester %test_folder%\%%i_source.txt %test_folder%\%%i_queries.txt %result_folder%\%%i.xml
    FOR /F %%s IN ('FINDSTR /i "additional exception timeout" %result_folder%\%%i.xml') DO (
        ECHO Test %%i failed
        SET /A is_failure=1
    )
    IF %is_failure% NEQ 1 (
        SETLOCAL ENABLEDELAYEDEXPANSION
        SET /A parse_time=0
        SET /A query_time=0
        FOR /F "tokens=4 delims=>." %%j IN ('FINDSTR /i /c:"parsing_time_taken" %result_folder%\%%i.xml') DO (
            SET /A parse_time=%%j
        )
        FOR /F "tokens=2 delims=>." %%k IN ('FINDSTR /i /c:"<time_taken" %result_folder%\%%i.xml') DO (
            SET /A query_time+=%%k
        )
        ECHO ^[PARSE TIME^] !parse_time!ms
        ECHO ^[MAX QUERY TIME^] !query_time!ms
        ENDLOCAL
    )
    ECHO ==============================================================
)
EXIT