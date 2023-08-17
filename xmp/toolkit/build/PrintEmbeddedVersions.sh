# UNIX shell script to print the XMP embedded version strings for all DLLs on all platforms.
# To use, first cd to .../public/libraries.

echo
echo "========================================="
echo "Embedded version info for Macintosh debug"
echo "========================================="

cd macintosh/debug

echo
strings AdobeXMP.framework/Versions/A/AdobeXMP | grep "Adobe XMP"
strings AdobeXMP.framework/Versions/A/AdobeXMP | grep "AdobeIP"
strings AdobeXMP.framework/Versions/A/AdobeXMP | grep BUILD
strings AdobeXMP.framework/Versions/A/AdobeXMP | grep FILEVERSION
strings AdobeXMP.framework/Versions/A/AdobeXMP | grep COPYRIGHT

echo
strings AdobeXMPFiles.framework/Versions/A/AdobeXMPFiles | grep "Adobe XMP"
strings AdobeXMPFiles.framework/Versions/A/AdobeXMPFiles | grep "AdobeIP"
strings AdobeXMPFiles.framework/Versions/A/AdobeXMPFiles | grep BUILD
strings AdobeXMPFiles.framework/Versions/A/AdobeXMPFiles | grep FILEVERSION
strings AdobeXMPFiles.framework/Versions/A/AdobeXMPFiles | grep COPYRIGHT

echo
strings AdobeXMPScript.framework/Versions/A/AdobeXMPScript | grep "Adobe XMP"
strings AdobeXMPScript.framework/Versions/A/AdobeXMPScript | grep "AdobeIP"
strings AdobeXMPScript.framework/Versions/A/AdobeXMPScript | grep BUILD
strings AdobeXMPScript.framework/Versions/A/AdobeXMPScript | grep FILEVERSION
strings AdobeXMPScript.framework/Versions/A/AdobeXMPScript | grep COPYRIGHT

cd ../..

echo
echo "==========================================="
echo "Embedded version info for Macintosh release"
echo "==========================================="

cd macintosh/release

echo
strings AdobeXMP.framework/Versions/A/AdobeXMP | grep "Adobe XMP"
strings AdobeXMP.framework/Versions/A/AdobeXMP | grep "AdobeIP"
strings AdobeXMP.framework/Versions/A/AdobeXMP | grep BUILD
strings AdobeXMP.framework/Versions/A/AdobeXMP | grep FILEVERSION
strings AdobeXMP.framework/Versions/A/AdobeXMP | grep COPYRIGHT

echo
strings AdobeXMPFiles.framework/Versions/A/AdobeXMPFiles | grep "Adobe XMP"
strings AdobeXMPFiles.framework/Versions/A/AdobeXMPFiles | grep "AdobeIP"
strings AdobeXMPFiles.framework/Versions/A/AdobeXMPFiles | grep BUILD
strings AdobeXMPFiles.framework/Versions/A/AdobeXMPFiles | grep FILEVERSION
strings AdobeXMPFiles.framework/Versions/A/AdobeXMPFiles | grep COPYRIGHT

echo
strings AdobeXMPScript.framework/Versions/A/AdobeXMPScript | grep "Adobe XMP"
strings AdobeXMPScript.framework/Versions/A/AdobeXMPScript | grep "AdobeIP"
strings AdobeXMPScript.framework/Versions/A/AdobeXMPScript | grep BUILD
strings AdobeXMPScript.framework/Versions/A/AdobeXMPScript | grep FILEVERSION
strings AdobeXMPScript.framework/Versions/A/AdobeXMPScript | grep COPYRIGHT

cd ../..

echo
echo "======================================="
echo "Embedded version info for Windows debug"
echo "======================================="

cd windows/debug

echo
strings AdobeXMP.dll | grep "Adobe XMP"
strings AdobeXMP.dll | grep "AdobeIP"
strings AdobeXMP.dll | grep BUILD
strings AdobeXMP.dll | grep FILEVERSION
strings AdobeXMP.dll | grep COPYRIGHT

echo
strings AdobeXMPFiles.dll | grep "Adobe XMP"
strings AdobeXMPFiles.dll | grep "AdobeIP"
strings AdobeXMPFiles.dll | grep BUILD
strings AdobeXMPFiles.dll | grep FILEVERSION
strings AdobeXMPFiles.dll | grep COPYRIGHT

echo
strings AdobeXMPScript.dll | grep "Adobe XMP"
strings AdobeXMPScript.dll | grep "AdobeIP"
strings AdobeXMPScript.dll | grep BUILD
strings AdobeXMPScript.dll | grep FILEVERSION
strings AdobeXMPScript.dll | grep COPYRIGHT

cd ../..

echo
echo "========================================="
echo "Embedded version info for Windows release"
echo "========================================="

cd windows/release

echo
strings AdobeXMP.dll | grep "Adobe XMP"
strings AdobeXMP.dll | grep "AdobeIP"
strings AdobeXMP.dll | grep BUILD
strings AdobeXMP.dll | grep FILEVERSION
strings AdobeXMP.dll | grep COPYRIGHT

echo
strings AdobeXMPFiles.dll | grep "Adobe XMP"
strings AdobeXMPFiles.dll | grep "AdobeIP"
strings AdobeXMPFiles.dll | grep BUILD
strings AdobeXMPFiles.dll | grep FILEVERSION
strings AdobeXMPFiles.dll | grep COPYRIGHT

echo
strings AdobeXMPScript.dll | grep "Adobe XMP"
strings AdobeXMPScript.dll | grep "AdobeIP"
strings AdobeXMPScript.dll | grep BUILD
strings AdobeXMPScript.dll | grep FILEVERSION
strings AdobeXMPScript.dll | grep COPYRIGHT

cd ../..

echo
echo "====================================="
echo "Embedded version info for Linux debug"
echo "====================================="

cd i80386linux/debug

echo
strings libAdobeXMP.so | grep "Adobe XMP"
strings libAdobeXMP.so | grep "AdobeIP"
strings libAdobeXMP.so | grep BUILD
strings libAdobeXMP.so | grep FILEVERSION
strings libAdobeXMP.so | grep COPYRIGHT

cd ../..

echo
echo "======================================="
echo "Embedded version info for Linux release"
echo "======================================="

cd i80386linux/release

echo
strings libAdobeXMP.so | grep "Adobe XMP"
strings libAdobeXMP.so | grep "AdobeIP"
strings libAdobeXMP.so | grep BUILD
strings libAdobeXMP.so | grep FILEVERSION
strings libAdobeXMP.so | grep COPYRIGHT

cd ../..

echo
echo "======================================="
echo "Embedded version info for Solaris debug"
echo "======================================="

cd sparcsolaris/debug

echo
strings libAdobeXMP.so | grep "Adobe XMP"
strings libAdobeXMP.so | grep "AdobeIP"
strings libAdobeXMP.so | grep BUILD
strings libAdobeXMP.so | grep FILEVERSION
strings libAdobeXMP.so | grep COPYRIGHT

cd ../..

echo
echo "========================================="
echo "Embedded version info for Solaris release"
echo "========================================="

cd sparcsolaris/release

echo
strings libAdobeXMP.so | grep "Adobe XMP"
strings libAdobeXMP.so | grep "AdobeIP"
strings libAdobeXMP.so | grep BUILD
strings libAdobeXMP.so | grep FILEVERSION
strings libAdobeXMP.so | grep COPYRIGHT

cd ../..

echo
echo "==================================="
echo "Embedded version info for AIX debug"
echo "==================================="

cd rs6000aix/debug

echo
strings libAdobeXMP.so | grep "Adobe XMP"
strings libAdobeXMP.so | grep "AdobeIP"
strings libAdobeXMP.so | grep BUILD
strings libAdobeXMP.so | grep FILEVERSION
strings libAdobeXMP.so | grep COPYRIGHT

cd ../..

echo
echo "====================================="
echo "Embedded version info for AIX release"
echo "====================================="

cd rs6000aix/release

echo
strings libAdobeXMP.so | grep "Adobe XMP"
strings libAdobeXMP.so | grep "AdobeIP"
strings libAdobeXMP.so | grep BUILD
strings libAdobeXMP.so | grep FILEVERSION
strings libAdobeXMP.so | grep COPYRIGHT

cd ../..

echo
echo "===================================="
echo "Embedded version info for HPUX debug"
echo "===================================="

cd hppahpux/debug

echo
strings libAdobeXMP.sl | grep "Adobe XMP"
strings libAdobeXMP.sl | grep "AdobeIP"
strings libAdobeXMP.sl | grep BUILD
strings libAdobeXMP.sl | grep FILEVERSION
strings libAdobeXMP.sl | grep COPYRIGHT

cd ../..

echo
echo "======================================"
echo "Embedded version info for HPUX release"
echo "======================================"

cd hppahpux/release

echo
strings libAdobeXMP.sl | grep "Adobe XMP"
strings libAdobeXMP.sl | grep "AdobeIP"
strings libAdobeXMP.sl | grep BUILD
strings libAdobeXMP.sl | grep FILEVERSION
strings libAdobeXMP.sl | grep COPYRIGHT

cd ../..
