#!/usr/bin/env bash
set -euo pipefail

cd "$(dirname "$0")"

BUILD_DIR="build"
mkdir -p "$BUILD_DIR"

javac -encoding UTF-8 -d "$BUILD_DIR" Lab3.java
java -Dfile.encoding=UTF-8 -Dstdout.encoding=UTF-8 -cp "$BUILD_DIR" Lab3 "$@"
