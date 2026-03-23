# importing shutil module
import shutil
import time
import os
import sys

#You need to edit these for your library
stripped_native_lib_names_list = ['libAdobeXMP.so', 'libAdobeXMPCompareAndMerge.so', 'libAdobeXMPFiles.so']
destinationRootPath = '../../../public/libraries/android/%s/%s/'

# Optional Edit- do it only if you want some non-standard name for your extracted symbols
stripped_native_lib_symble_file_names_list = ['libAdobeXMP.so.sym', 'libAdobeXMPCompareAndMerge.so.sym', 'libAdobeXMPFiles.so.sym']

#No need to edit anything here onwards...
rootNativeLibPath = 'build/intermediates/stripped_native_libs/'
rootSymTablePath = 'build/intermediates/native_symbol_tables/'

name = sys.argv[1] # getting the name of the build flavor
#print(name)
abi = ''
debugOrRelease = ''
nativeLibFinalPath = ''
symTableFinalPath = ''

if name == 'arm64Debug' :
    abi = 'arm64-v8a'
    debugOrRelease = 'Debug'
if name == 'arm64Release' :
    abi = 'arm64-v8a'
    debugOrRelease = 'Release'
if name == 'armv7aDebug' :
    abi = 'armeabi-v7a'
    debugOrRelease = 'Debug'
if name == 'armv7aRelease' :
    abi = 'armeabi-v7a'
    debugOrRelease = 'Release'
if name == 'intel32Debug' :
    abi = 'x86'
    debugOrRelease = 'Debug'
if name == 'intel32Release' :
    abi = 'x86'
    debugOrRelease = 'Release'
if name == 'intel64Debug' :
    abi = 'x86_64'
    debugOrRelease = 'Debug'
if name == 'intel64Release' :
    abi = 'x86_64'
    debugOrRelease = 'Release'

finalLibDstPath = destinationRootPath % (abi, debugOrRelease)
finalSymDstPath = destinationRootPath % (abi, debugOrRelease+'Symbols')

if not os.path.exists(finalLibDstPath):
    os.makedirs(finalLibDstPath)

if not os.path.exists(finalSymDstPath):
    os.makedirs(finalSymDstPath)

for x in stripped_native_lib_names_list:
    nativeLibFinalPath = os.path.join( rootNativeLibPath, name, 'out/lib', abi, x)
    print "Copying from %s to %s " %(nativeLibFinalPath, finalLibDstPath)
    shutil.copy2( nativeLibFinalPath, finalLibDstPath )    

for x in stripped_native_lib_symble_file_names_list:    
    symTableFinalPath = os.path.join( rootSymTablePath, name, 'out', abi, x)
    print "Copying from %s to %s " %(symTableFinalPath, finalSymDstPath)
    shutil.copy2( symTableFinalPath, finalSymDstPath )