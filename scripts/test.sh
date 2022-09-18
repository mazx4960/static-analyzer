#!/bin/bash

# run system tests
mkdir -p test_results

test_names=("basic" "compute_ave" "compute_centroid")

for test_name in "${test_names[@]}"; do
    echo "Running test $test_name"
    build/src/autotester/autotester Team14/Tests14/system/"$test_name"_source.txt Team14/Tests14/system/"$test_name"_queries.txt test_results/"$test_name".xml > test_results/"$test_name".log
    if grep -qiE "missing|additional|exception" test_results/"$test_name".xml; then
        echo "Test $test_name failed"
        exit 1
    else
        echo "Test $test_name passed"
    fi
done