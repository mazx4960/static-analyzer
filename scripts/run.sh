#!/bin/bash

test_folder="Team14/Tests14/playground"
result_folder="Team14/Code14/tests"

for test_name in {1..2}; do
    echo "=============================================================="
    echo "Running test '$test_name'"
    python3 -m mprof run -M -T 0.001 build/src/runner/runner $test_folder/0.txt $test_folder/0_"$test_name".txt > $result_folder/"$test_name".log
    grep "Time taken to load source file" $result_folder/"$test_name".log
    grep "Time taken to fetch results from pkb" $result_folder/"$test_name".log
    grep "Number of queries" $result_folder/"$test_name".log
    grep "Number of cache hits" $result_folder/"$test_name".log
    grep "Number of cache misses" $result_folder/"$test_name".log
    grep "Total context size:" $result_folder/"$test_name".log
    grep "Time taken to project results" $result_folder/"$test_name".log
    grep "Time taken to execute query" $result_folder/"$test_name".log
    python3 -m mprof peak
    echo "=============================================================="
done