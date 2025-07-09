#!/bin/bash

# Format all C++ files in the project according to .clang-format
# Run this script from the project root directory

echo "Formatting C++ files with clang-format..."

# Check if clang-format is installed
if ! command -v clang-format &> /dev/null; then
    echo "Error: clang-format not found. Please install it first."
    echo "You can install it with: sudo apt install clang-format"
    exit 1
fi

# Format all C++ source and header files
find ./src -type f \( -name "*.cpp" -o -name "*.h" -o -name "*.hpp" \) -print0 | xargs -0 clang-format -i

echo "Formatting complete!"
