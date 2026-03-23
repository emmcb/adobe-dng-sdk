#!/bin/bash
# -e: Exit immediately if a command exits with a non-zero status.
# -u: Treat unset variables as an error when substituting.
trap catch_errors  ERR;

# 
function catch_errors () {
echo "cmake.command failed";
exit 1;
}


# Get the absolut path of the script
abspath=$(cd ${0%/*} && echo $PWD/${0##*/})

# to get the path only - not the script name
path_only=`dirname "$abspath"`

#Set XMPROOT for further reference
XMPROOT=$path_only/../../..

# defines some handy function used over in several scripts
# TODO:@aman: remove the next line if not required
# source $XMPROOT/toolkit/build/RelengUtils.txt

# generate projects in above file defined function
GenerateBuildProjects $1 $2 $3 $4 $5

