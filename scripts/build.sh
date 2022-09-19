#!/bin/bash

mkdir -p build

# Generate the build files
cmake -S Team14/Code14 -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Release

# Build the project
cmake --build build/ --target unit_testing --config Release
cmake --build build/ --target integration_testing --config Release
cmake --build build/ --target autotester --config Release
cmake --build build/ --target runner --config Release