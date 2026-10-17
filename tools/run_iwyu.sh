#!/bin/sh -e

sed -e 's#-include-pch godot4-cpp-ext/src/dry.h.gch##' compile_commands.json > compile_commands_no_pch.json
iwyu_tool -p compile_commands_no_pch.json "$@" -- -Xiwyu --pch_in_code -Xiwyu --prefix_header_includes=remove
