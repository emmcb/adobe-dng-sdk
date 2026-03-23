#!/usr/bin/python

import sys
import os
import subprocess
import shutil
import platform
import pickle
import json
from os import environ
from subprocess import Popen, PIPE

def build_web(build_bit, build_mode, build_type, emsdk_path, flags):
    if ((not build_type.lower() == 'release' and not build_type.lower() == 'debug') or emsdk_path == ''):
        usage_string()
        exit(1)

    if not os.path.exists(emsdk_path):
        print 'emsdk must point to valid emsdk directory'
        exit(1)
        
    for x in range(len(flags)):
        if flags[x] == '-DCLEAN_BUILD=1':
            if (os.path.exists("cmake_install.cmake")):
                os.remove("cmake_install.cmake")
            if (os.path.exists("CMakeCache.txt")):
                os.remove("CMakeCache.txt")
            if (os.path.exists("Makefile")):
                os.remove("Makefile")
            if (os.path.exists("CMakeFiles")):
                shutil.rmtree("CMakeFiles")
            break;

    activate_emsdk_and_build(build_bit, build_mode, build_type, emsdk_path, flags)


def activate_emsdk_and_build(build_bit, build_mode, build_type, emsdk_path, flags):
    os.chdir(emsdk_path)
    
    system_name = os_name()
    if system_name == 'Win':
        activate_emsdk_win(emsdk_path, build_type, build_bit, build_mode, flags)
    else:
        activate_emsdk_mac(emsdk_path, build_type, build_bit, build_mode, flags)

def activate_emsdk_mac(emsdk_path, build_type, build_bit, build_mode, flags):

	#setting enviroment variables
    set_env();

    #change to current directory
    os.chdir(sys.path[0])

    print "configuring: "+build_type + " " + str(build_bit) + " " + build_mode;

    build_static = 'On';
    if build_mode.lower() == 'dynamic':
        build_static = 'Off'

    build_64 = 'Off';
    if build_bit == 64:
        build_64 = 'On';

    if not os.path.exists('./../../testtools/third-party/cmake/bin'):
        print 'cmake must be present at toolkit/testtools/third-party/cmake'
        exit(1)

    os.environ['PATH'] += ':./../../testtools/third-party/cmake/bin/cmake.app/Contents/bin/';

    #configure the emscripten cmake tool
    #emconfigure cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DXMP_BUILD_STATIC=On -DXMPWASM=1 -DCMAKE_TOOLCHAIN_FILE=/Users/angupt/Documents/git/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake
    args=['emcmake','cmake','../.','-G','Unix Makefiles','-DCMAKE_CL_64='+build_64,'-DXMP_BUILD_STATIC='+build_static,'-DXMPWASM=1','-DCMAKE_BUILD_TYPE='+build_type,'-DCMAKE_TOOLCHAIN_FILE='+emsdk_path+'/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake']
    for x in range(len(flags)):
        args.append(flags[x])
    
    return_code = subprocess.call(args)
    if return_code != 0:
        exit(1)

    #build using emmake make
    print "calling emmake make command"
    return_code = subprocess.call(['emmake','make', '-j'])
    if return_code != 0:
        print 'Please make sure that cmake is installed.'
        exit(1)


    
def set_env():

    source = 'source ./emsdk_env.sh'
    dump = '/usr/bin/python -c "import os, json;print (json.dumps(dict(os.environ)))"'
    pipe = subprocess.Popen(['/bin/bash', '-c', '%s && %s' %(source,dump)], stdout=subprocess.PIPE)
    env = json.loads(pipe.stdout.read())
    os.environ = env
       
    return env

def activate_emsdk_win(emsdk_path, build_type, build_bit, build_mode, flags):

    #setting enviroment variables
    set_env();

    #change to current directory
    os.chdir(sys.path[0])

    print "configuring: "+build_type + " " + str(build_bit) + " " + build_mode;

    build_static = 'On';
    if build_mode.lower() == 'dynamic':
        build_static = 'Off'

    build_64 = 'Off';
    if build_bit == 64:
        build_64 = 'On';

    if not os.path.exists('./../../testtools/third-party/cmake/bin'):
        print 'cmake must be present at toolkit/testtools/third-party/cmake'
        exit(1)

    #configure the emscripten cmake tool
    #emconfigure cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DXMP_BUILD_STATIC=On -DXMPWASM=1 -DCMAKE_TOOLCHAIN_FILE=/Users/angupt/Documents/git/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake
        args=['emcmake','./../../testtools/third-party/cmake/bin/cmake.exe','../.','-G','MinGW Makefiles','-DCMAKE_CL_64='+build_64,'-DXMP_BUILD_STATIC='+build_static,'-DXMPWASM=1','-DCMAKE_BUILD_TYPE='+build_type,'-DCMAKE_TOOLCHAIN_FILE='+emsdk_path+'/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake']
    for x in range(len(flags)):
        args.append(flags[x])
    
    return_code = subprocess.call(args,shell=True)
   
    if return_code != 0:
        print 'Please make sure that cmake is installed.'
        exit(1)

    #build using emmake make
    print "calling emmake make command"
    return_code = subprocess.call(['emmake','make'], shell=True)
    if return_code != 0:
        exit(1)

def os_name():
    uname = platform.platform()
    if 'Windows' in uname:
        return 'Win'
    elif 'Darwin' in uname:
        return 'Mac'
    else:
        print "Not supported platform"
        exit(1)   

def usage_string() : 
    print 'usage : build_web.py \n build_bit - 32/64 \n build_type - Static/Dynamic \n build_mode - Release/Debug \n path where emsdk is present [Additonal flags] '

# Execute `main()` function 
if __name__ == '__main__':
    os_name()
    print len(sys.argv)
    if len(sys.argv) < 5:
        usage_string()
        exit(1)
    build_web(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5:len(sys.argv)])
