#!/bin/bash

# $1: folder that contains the project file
# $2: folder for log files

echo "Building ..."

echo "...XMPCore 64"

xcodebuild -project ../XMPCore/build/$1/XMPCore.xcodeproj -target 'Build DLLs 64' > $2/1_build_corefiles_64_xcode.log 2>$2/1_build_corefiles_64_xcode_err.log

if [ $? == 0 ]; then
  echo "XMPCore Success."
else
  echo "XMPCore Failed."
  exit 1
fi

echo "...XMPFiles 64"

xcodebuild -project ../XMPFiles/build/$1/XMPFiles.xcodeproj -target 'Build DLLs 64' > $2/1_build_corefiles_64_xcode.log 2>$2/1_build_corefiles_64_xcode_err.log

if [ $? == 0 ]; then
  echo "XMPFiles Success."
else
  echo "XMPFiles Failed."
  exit 1
fi

echo "...XMPCore 32"

xcodebuild -project ../XMPCore/build/$1/XMPCore.xcodeproj -target 'Build DLLs 32' > $2/1_build_corefiles_32_xcode.log 2>$2/1_build_corefiles_32_xcode_err.log

if [ $? == 0 ]; then
  echo "XMPCore Success."
else
  echo "XMPCore Failed."
  exit 1
fi

echo "...XMPFiles 32"

xcodebuild -project ../XMPFiles/build/$1/XMPFiles.xcodeproj -target 'Build DLLs 32' > $2/1_build_corefiles_32_xcode.log 2>$2/1_build_corefiles_32_xcode_err.log

if [ $? == 0 ]; then
  echo "XMPFiles Success."
else
  echo "XMPFiles Failed."
  exit 1
fi