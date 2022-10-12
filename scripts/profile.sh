#!/bin/bash

# run stress tests
test_folder="Team14/Tests14/system"
result_folder="Team14/Code14/tests"
test_names=(
  "qps_stress_1"
  "sp_stress_1"
  "stress_1"
)

for test_name in "${test_names[@]}"; do
    echo "=============================================================="
    echo "Profiling '$test_name'"
    python3 run_with_mprof.py -c build/src/autotester/autotester $test_folder/"$test_name"_source.txt $test_folder/"$test_name"_queries.txt $result_folder/"$test_name".xml -t 0.001
    echo "[PARSE TIME]" $(sed -n -E 's/.*<parsing_time_taken>(.+)<\/parsing_time_taken>/\1/p' $result_folder/"$test_name".xml)
    echo "=============================================================="
done
