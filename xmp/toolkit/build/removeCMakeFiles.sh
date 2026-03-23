#!/bin/bash -e
#
#
#

echo "Clean up CMake files"
 find ./../../  -type d '(' -name 'CMakeDebug' -o -name 'CMake64Debug' -o -name 'CMake' -o -name 'CMake64'  -o -name 'CMake64Release' -o -name 'CMakeRelease' -o -name 'CMakeFiles' ')' -exec rm -rifv {} \; -prune
