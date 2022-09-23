#!/bin/bash

src=$(pwd)/Team14/Code14
lcov --capture --directory . --output-file coverage.info
lcov --extract coverage.info "$src/*" --output-file coverage.info
genhtml coverage.info -o coverage
