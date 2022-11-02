@ECHO off

SET test_folder=..\..\Team14\Tests14\system
SET result_folder=..\..\Team14\Code14\tests
SET test_names=(^
	affects_edge^
	basic^
	bug_catch_1^
	bug_catch_2^
	code_6^
	compute_ave^
	compute_centroid^
	demo^
	extreme_nesting^
	intertwined_procedures^
	patterns^
	pql_error^
	sp_error^
	select_attributes^
)
SET /A num_failed=0
SET /A num_tests=0
SET /A num_errors=0

FOR %%i IN %test_names% DO (
	SET /A num_tests+=1
	ECHO ==============================================================
    ECHO Running test %%i
    ..\..\Team14\Code14\build\src\autotester\release\autotester %test_folder%\%%i_source.txt %test_folder%\%%i_queries.txt %result_folder%\%%i.xml > %result_folder%\%%i.log
    SETLOCAL ENABLEDELAYEDEXPANSION
    SET /A is_printed=0
    FOR /F "delims=:" %%j IN ('FINDSTR /i /n "Additional Exception Timeout" %result_folder%\%%i.log') DO (
        IF !is_printed! EQU 0 (
            ECHO Test %%i failed
            ECHO --------------------------------------------------------------
            ENDLOCAL
            SET /A num_failed+=1
            SETLOCAL ENABLEDELAYEDEXPANSION
            SET /A is_printed+=1
        )
        SET /A num_errors+=1
        SET /A error_line_number=%%j
        SET /A upper_bound=!error_line_number!
        SET /A lower_bound=!error_line_number!-6
        SET /A current_line=1
        FOR /F "tokens=*" %%k IN (%result_folder%\%%i.log) DO (
            IF !current_line! GEQ !lower_bound! (
                IF !current_line! LEQ !upper_bound! (
                    ECHO %%k
                )
            )
            SET /A current_line+=1
        )
    )
	IF !num_errors! EQU 0 (
	    ECHO Test %%i passed
    )
    ENDLOCAL
    ECHO ==============================================================
)
IF %num_failed%==0 (
	ECHO ==============================================================
    ECHO ALL TESTS PASSED ^(%num_tests% TESTS TOTAL^)
    ECHO ==============================================================
) ELSE (
	ECHO %num_failed% TEST^(S^) FAILED ^(%num_tests% TEST TOTAL^)
	EXIT 1
)
EXIT