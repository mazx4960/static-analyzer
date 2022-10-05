#!/bin/bash

# run system tests
test_folder="Team14/Tests14/system"
result_folder="Team14/Code14/tests"
test_names=(
  "pql_error"
  "basic"
  "compute_ave"
  "compute_centroid"
  "extreme_nesting"
  "patterns"
  "intertwined_procedures"
  "qps_stress_1"
  "sp_stress_1"
  "sp_stress_2"
)
num_failed=0

for test_name in "${test_names[@]}"; do
    echo "=============================================================="
    echo "Running test '$test_name'"
    build/src/autotester/autotester $test_folder/"$test_name"_source.txt $test_folder/"$test_name"_queries.txt $result_folder/"$test_name".xml > $result_folder/"$test_name".log
    if grep -qiE "missing|additional|exception|timeout" $result_folder/"$test_name".xml; then
        echo "Test '$test_name' failed"
        echo "--------------------------------------------------------------"
        grep -iE -B 8 -A 1 "missing|additional|exception|timeout" $result_folder/"$test_name".log
        num_failed=$((num_failed+1))
    else
        echo "Test '$test_name' passed"
    fi
    echo "=============================================================="
done
if [ $num_failed -eq 0 ]; then
    echo "=============================================================="
    echo "ALL TESTS PASSED (${#test_names[@]} TESTS TOTAL)"
    echo "=============================================================="
else
    echo "=============================================================="
    echo "${num_failed} TEST(S) FAILED (${#test_names[@]} TESTS TOTAL)"
    echo "=============================================================="
    exit 1
fi
exit 0
