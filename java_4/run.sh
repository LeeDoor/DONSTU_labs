#!/usr/bin/env bash
set -euo pipefail

cd "$(dirname "$0")"

BUILD_DIR="build"
mkdir -p "$BUILD_DIR"

find src -name '*.java' -print0 | xargs -0 javac -encoding UTF-8 -d "$BUILD_DIR"
java -Dfile.encoding=UTF-8 -Dstdout.encoding=UTF-8 -cp "$BUILD_DIR" lab4.Lab4 "$@"
