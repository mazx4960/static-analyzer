#!/bin/bash

# run system tests
mkdir -p test_results

test_names=("pql_error" "basic" "compute_ave" "compute_centroid" "extreme_nesting" "patterns")
num_failed=0

for test_name in "${test_names[@]}"; do
    echo "=================================================="
    echo "Running test '$test_name'"
    build/src/autotester/autotester Team14/Tests14/system/"$test_name"_source.txt Team14/Tests14/system/"$test_name"_queries.txt test_results/"$test_name".xml > test_results/"$test_name".log
    if grep -qiE "missing|additional|exception|timeout" test_results/"$test_name".xml; then
        echo "Test '$test_name' failed"
        echo "--------------------------------------------------"
        grep -iE -B 8 -A 1 "missing|additional|exception|timeout" test_results/"$test_name".log
        num_failed=$((num_failed+1))
    else
        echo "Test '$test_name' passed"
    fi
    echo "=================================================="
done
if [ $num_failed -eq 0 ]; then
    echo "=================================================="
    echo "ALL TESTS PASSED"
    echo "=================================================="
else
    echo "=================================================="
    echo "${num_failed} TEST(S) FAILED"
    echo "=================================================="
    exit 1
fi
exit 0
