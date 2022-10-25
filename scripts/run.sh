#!/bin/bash

test_folder="Team14/Tests14/playground"
result_folder="Team14/Code14/tests"

for test_name in {0..7}; do
    echo "=============================================================="
    echo "Running test '$test_name'"
    python3 -m mprof run -M -T 0.001 build/src/runner/runner $test_folder/"$test_name".txt $test_folder/"$test_name"_1.txt > $result_folder/"$test_name".log
    grep "Time taken to load source file" $result_folder/"$test_name".log
    grep "Time taken to fetch results from pkb" $result_folder/"$test_name".log
    grep "Time taken to project results" $result_folder/"$test_name".log
    grep "Time taken to execute query" $result_folder/"$test_name".log
    python3 -m mprof peak
    echo "=============================================================="
done