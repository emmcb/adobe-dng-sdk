#!/bin/bash 


# usage
# $1: Architecure 32 || 64
# $2: Static || Dynamic
# $3: Release || Debug
# $4: Path to emsdk directory

ARCH=$1
TARGET=$2

# Got script dir
cd "$(dirname $0)" >/dev/null

echo "Make Target $ARCH $TARGET"

python ./web/build.py $1 $2 $3 $4

if [ $? == 0 ]; then
  echo -e "  Make $ARCH $TARGET Success \n."
  exit 0
else
  echo -e "  Make $ARCH $TARGET Failed \n."
  exit 1
fi
