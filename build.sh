#!/bin/bash

show_help() {
    echo "Usage: ./build.sh [target]"
    echo ""
    echo "Options:"
    echo "  -h, --help      Show this help message"
    echo "  <target>        Name of the example or target to build and run (e.g., level3_smart_pointers)"
    echo ""
    echo "If no target is specified, all targets will be built."
}

# Check for help flag
if [[ "$1" == "-h" || "$1" == "--help" ]]; then
    show_help
    exit 0
fi

# Check for build directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/build"

if [ ! -d "$BUILD_DIR" ]; then
    echo "[ERROR] Build directory does not exist."
    exit 1
fi

cd "$BUILD_DIR" || exit 1

if [ -z "$1" ]; then
    echo "[INFO] Building all targets..."
    cmake --build . --config Release --verbose
else
    TARGET="$1"
    echo "[INFO] Building target: $TARGET"
    cmake --build . --config Release --target "$TARGET" --verbose

    # Try to run the executable if it exists
    # Search in src/ and examples/ subfolders
    EXEC_PATH=""
    if [ -f "src/$TARGET" ]; then
        EXEC_PATH="src/$TARGET"
    elif [ -f "examples/$TARGET" ]; then
        EXEC_PATH="examples/$TARGET"
    elif [ -f "$TARGET" ]; then
        EXEC_PATH="$TARGET"
    fi

    if [ -n "$EXEC_PATH" ]; then
        echo "[INFO] Running $EXEC_PATH"
        chmod +x "$EXEC_PATH"
        "./$EXEC_PATH"
    else
        echo "[WARN] Executable for target '$TARGET' not found."
    fi
fi