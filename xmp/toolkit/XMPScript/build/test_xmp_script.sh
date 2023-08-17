#!/bin/bash

LOGFILE="4_testSuite.log"
CWD=`pwd`
TARGETDIR="../../target/macintosh/intel_64/release"
FRAMEWORKDIR="../../target/macintosh/intel_64/release/Frameworks"

# copy files into working directory
cp -f ../samples/JSUnit.js $TARGETDIR
rm -rf $TARGETDIR/tests
mkdir $TARGETDIR/tests
cp -fR ../samples/tests/ $TARGETDIR/tests


cp -rfp ../third-party/extendscript/extendscript/public/libraries/macintosh/intel_64/release/AdobeExtendScript.framework $FRAMEWORKDIR
cp -rfp ../third-party/extendscript/sccore/public/libraries/macintosh/intel_64/release/AdobeScCore.framework $FRAMEWORKDIR
cp -f ../third-party/extendscript/extendscript/public/libraries/macintosh/intel_64/release/shell $TARGETDIR/bin


cd $TARGETDIR
ls -la
./bin/shell -u -s -l -B -c "load('JSUnit.js');runTests('tests','"$LOGFILE"');"

if [ $? -ne 0 ]; then
	echo "Failed."
	cp -f $LOGFILE "../../$1"
	cd $CWD
	exit 1

else
	echo "Success."
	cp -f $LOGFILE "../../$1"
	cd $CWD
	exit 0
fi
