#!/bin/bash

LOGFILE="4_testSuite.log"
CWD=`pwd`
TARGETDIR="../../public/libraries/macintosh/intel_64_libcpp/release"


# copy files into working directory
cp -f ../samples/JSUnit.js $TARGETDIR
rm -rf $TARGETDIR/tests
mkdir $TARGETDIR/tests
mkdir $TARGETDIR/bin
mkdir $TARGETDIR/Frameworks
cp -fR ../samples/tests/ $TARGETDIR/tests

#cp -rfp ../third-party/extendscript/extendscript/public/libraries/macintosh/intel_64/release/AdobeExtendScript.framework $TARGETDIR/Frameworks
#cp -rfp ../third-party/extendscript/sccore/public/libraries/macintosh/intel_64/release/AdobeScCore.framework $TARGETDIR/Frameworks

cp -f ../third-party/extendscript/extendscript/public/libraries/macintosh/intel_64/release/shell $TARGETDIR/bin

pushd $TARGETDIR
# create simplic links the frameworks
ln -fs ../../../../../../XMPScript/third-party/extendscript/extendscript/public/libraries/macintosh/intel_64/release/AdobeExtendScript.framework ./Frameworks
ln -fs ../../../../../../XMPScript/third-party/extendscript/sccore/public/libraries/macintosh/intel_64/release/AdobeScCore.framework ./Frameworks

ln -fs ../AdobeXMP.framework ./Frameworks
ln -fs ../AdobeXMPFiles.framework ./Frameworks
ln -fs ../AdobeXMPScript.framework ./Frameworks

#ls -la
./bin/shell -u -s -l -B -c "load('JSUnit.js');runTests('tests','"$LOGFILE"');"

popd

if [ $? -ne 0 ]; then
	echo "Failed."
	cp -f $TARGETDIR/$LOGFILE "../../$1"
	cd $CWD
	exit 1
	
else
	echo "Success."
	cp -f $TARGETDIR/$LOGFILE "../../$1"
	cd $CWD
	exit 0
fi
