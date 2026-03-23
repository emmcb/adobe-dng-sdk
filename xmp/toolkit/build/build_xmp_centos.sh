#!/bin/bash 


# usage
# $1: Architecure 32 || 64
# $2: Release || Debug
ARCH=$1
TARGET=$2

#needed for the cmake binary called on RelEngMachines
export LD_LIBRARY_PATH=/opt/llvm/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/usr/lib64:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/usr/lib:$LD_LIBRARY_PATH
export PATH=/usr/local/bin:/bin:/usr/bin:/usr/ucb:/etc:.:/opt/bin
export toolchain=ToolchainRelEng_centos_clang.cmake

# get available cores
AVAILABLE_CORES=`grep -c ^processor /proc/cpuinfo`
echo Available cores to use by build: $AVAILABLE_CORES

# Got script dir
cd "$(dirname $0)" >/dev/null

if [ $ARCH == "32" ]; then
  export MACHTYPE=i386
  cd  ./CMake"$TARGET"
  /usr/bin/make -k VERBOSE=1 
else
  export MACHTYPE=x86_64
  cd ./CMake"$ARCH""$TARGET"
  /usr/bin/make -k VERBOSE=1 
fi

echo $(env)
echo "Make Target $ARCH $TARGET"


if [ $? == 0 ]; then
  echo -e "  Make $ARCH $TARGET Success \n."
  exit 0
else
  echo -e "  Make $ARCH $TARGET Failed \n."
  exit 1
fi
