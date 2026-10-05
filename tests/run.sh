#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")"
dir="$(mktemp -d)"
trap 'rm -rf "$dir"' EXIT
c++ -std=c++17 -Wall -Wextra -Werror -I ../components/comfoair frame_test.cpp -o "$dir/frame_test"
"$dir/frame_test"
