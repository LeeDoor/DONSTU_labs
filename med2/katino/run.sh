#!/usr/bin/env bash
set -euo pipefail

cd "$(dirname "$0")"

if [[ ! -x venv/bin/python ]]; then
    echo "Error: venv/bin/python not found. Create the virtual environment first." >&2
    exit 1
fi

export QT_QPA_PLATFORM="${QT_QPA_PLATFORM:-wayland}"
exec venv/bin/python main.py
