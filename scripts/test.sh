#!/bin/bash

# run system tests
mkdir -p test_results
build/src/autotester/autotester Team14/Tests14/system/Sample_source.txt Team14/Tests14/system/Sample_queries.txt test_results/Sample.xml
build/src/autotester/autotester Team14/Tests14/system/basic_source.txt Team14/Tests14/system/basic_queries.txt test_results/basic.xml
build/src/autotester/autotester Team14/Tests14/system/basic_source.txt Team14/Tests14/system/basic_queries.txt test_results/basic.xml
build/src/autotester/autotester Team14/Tests14/system/compute_ave_source.txt Team14/Tests14/system/compute_ave_queries.txt test_results/compute_ave.xml
build/src/autotester/autotester Team14/Tests14/system/compute_centroid_source.txt Team14/Tests14/system/compute_centroid_queries.txt test_results/compute_centroid.xml
build/src/autotester/autotester Team14/Tests14/system/extreme_nesting_source.txt Team14/Tests14/system/extreme_nesting_queries.txt test_results/extreme_nesting.xml
