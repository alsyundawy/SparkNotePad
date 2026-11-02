# SparkNotePad Workspace Instructions for AI Agents

## Mandatory Build Rules

1. **PERMANENT PROHIBITION OF NINJA**:
   - DO NOT use `ninja`, `-G Ninja`, or `cmake-ninja` under any circumstances.
   - Ninja causes severe CPU thermal spikes and laptop overheating.
   - All builds must use standard CMake with default generators (Unix Makefiles).

2. **STANDARD CMAKE USAGE**:
   - Configuration: `cmake -S . -B build -DCMAKE_BUILD_TYPE=Release`
   - Build: `cmake --build build` (or controlled jobs `cmake --build build -j2`)
   - Test: `ctest --test-dir build --output-on-failure` or `./build/tests/SparkNotePadTests`
