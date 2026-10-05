#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")"
out="$(mktemp -d)/frame_test"
c++ -std=c++17 -Wall -Wextra -Werror -I ../components/comfoair frame_test.cpp -o "$out"
"$out"
