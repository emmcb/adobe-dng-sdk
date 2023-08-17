#!/bin/bash 


# usage
# $1: Architecure 32 || 64
# $2: Release || Debug
ARCH=$1
TARGET=$2

#needed for the cmake binary called on RelEngMachines
export LD_LIBRARY_PATH=$PWD/../../resources/tools/CMakeApp/linux/bin:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/user/unicore/i80386linux_x64/compiler/gcc4.8.2/linux3.10_64/lib64:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/user/unicore/i80386linux/compiler/gcc4.8.2/linux3.10_32/lib:$LD_LIBRARY_PATH
export PATH=/usr/local/bin:/bin:/usr/bin:/usr/ucb:/etc:.:/opt/bin

# get available cores
AVAILABLE_CORES=`grep -c ^processor /proc/cpuinfo`
echo Available cores to use by build: $AVAILABLE_CORES

# Got script dir
cd "$(dirname $0)" >/dev/null

if [ $ARCH == "32" ]; then
  export MACHTYPE=i386
  cd  ./CMake"$TARGET"
  /user/unicore/i80386linux/compiler/gcc4.8.2/linux3.10_32/bin/make -k VERBOSE=1 
else
  export MACHTYPE=x86_64
  cd ./CMake"$ARCH""$TARGET"
  /user/unicore/i80386linux_x64/compiler/gcc4.8.2/linux3.10_64/bin/make -k VERBOSE=1 
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
