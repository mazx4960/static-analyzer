#!/bin/bash

# run unit tests
build/src/unit_testing/unit_testing

# run integration tests
build/src/integration_testing/integration_testing

# run system tests
mkdir -p test_results
build/src/autotester/autotester Team14/Tests14/system/Sample_source.txt Team14/Tests14/system/Sample_queries.txt test_results/Sample.xml
build/src/autotester/autotester Team14/Tests14/system/basic_source.txt Team14/Tests14/system/basic_queries.txt test_results/basic.xml
