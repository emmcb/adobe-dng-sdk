#!/bin/bash -e
# -e: Exit immediately if a command exits with a non-zero status.
# -u: Treat unset variables as an error when substituting.
#$1 64 | 32
#$2 Release | Debug
#$3 Dynamic | Static
#$4 libcpp|libstdc++
#$5 toolchain filename include path if desired where basedir = toolkit/build  e.g toolchains/ToolchainSastatine.cmake
#$6 Clean optional
#$7 Simulator optional

OS_NAME=`uname -s`
if [ $OS_NAME != 'SunOS' ]; then
    trap catch_errors  ERR;
fi

function catch_errors () {
echo "cmake_all.command failed";
exit 1;
}


# Linux only:
# Install necessary packages for cross compile on local machine:
# sudo apt-get install g++-multilib

# Change to the directory where this script lives.
cd "$(dirname $0)" >/dev/null

# list of all projects to build
# XMP build being last to ensure certain projects being excluded
./cmake.command $1 $2 $3 $4 $5 $6 $7

