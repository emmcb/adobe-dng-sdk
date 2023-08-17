#!/bin/bash

# $1: folder that contains the project file
# $2: folder for log files

echo "Building..."

echo "...XMPCore"

xcodebuild -project ../../XMPCore/build/$1/XMPCore.xcodeproj -target 'Build DLLs 32' > $2/2_xcode_libs.log 2>$2/2_xcode_libs_err.log

if [ $? == 0 ]; then
  echo "XMPCore Success."
else
  echo "XMPCore Failed."
  exit 1
fi

echo "...XMPFiles"

xcodebuild -project ../../XMPFiles/build/$1/XMPFiles.xcodeproj -target 'Build DLLs 32' > $2/2_xcode_libs.log 2> $2/2_xcode_libs_err.log

if [ $? == 0 ]; then
  echo "XMPFiles Success."
else
  echo "XMPFiles Failed."
  exit 1
fi

echo "...XMPScript"

xcodebuild -project ./$1/XMPScript.xcodeproj -target 'Build DLLs 32' > $2/2_xcode_script.log 2> $2/2_xcode_script_err.log

if [ $? == 0 ]; then
  echo "XMPScript Success."
else
  echo "XMPScript Failed."
  exit 1
fi

