# ==================================================================================================
# Copyright 2015 Adobe
# All Rights Reserved.
#
# NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
# of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
# ==================================================================================================

# --------------------------------------------------------------------
# Make sure the stage (debug/release) and mode (dll/static) are known.

TargetStage = ${stage}
ifneq "${TargetStage}" "debug"
	ifneq "${TargetStage}" "release"
		Error += Invalid target stage "${TargetStage}"
	endif
endif

TargetMode = ${mode}
ifneq "${TargetMode}" "dll"
	ifneq "${TargetMode}" "static"
		Error += Invalid target mode "${TargetMode}"
	endif
endif

# architecture (x86, x64)
TargetArch = ${arch}
ifeq "${TargetArch}" ""
	Error += Unspecified target architecture "${TargetArch}"
else
	ifneq "${TargetArch}" "x86"
		ifneq "${TargetArch}" "x64"
			ifneq "${TargetArch}" "ia64"
				Error += Invalid target architecture "${TargetArch}"
			endif
		endif
	endif
endif

TargetPaths = ${paths}
ifeq "${TargetPaths}" ""
	TargetPaths = ct
else
	ifneq "${TargetPaths}" "ct"
		ifneq "${TargetPaths}" "local"
			Error += Invalid paths choice "${TargetPaths}"
		endif
	endif
endif

# ensure that in local mode, libpath is set:
LibraryPath = ${ctlibpath}
ifeq "${TargetPaths}" "local"
	ifeq "${LibraryPath}" ""
		Error += In local mode, must specify ctlibpath.
	endif
endif

# ------------------------------------------------------------
# Define variables for input and output directories and files.

ProjectRoot = ../..
ProductRoot = ../../..

# 64-bit specific additions: (added in every 32-bit case, remain empty if not 64 bit mode)
x64_ext =
x64_compileFlags =
x64_linkFlags =

ifeq "${TargetArch}" "x64"
	x64_ext = _x64
	x64_compileFlags =-m64
	x64_linkFlags =-m64
endif

LibRoot    = ${ProductRoot}/public/libraries/${TargetOS}${x64_ext}/${TargetStage}
TempRoot   = ${ProductRoot}/temp/XMPCore/${TargetOS}${x64_ext}/${TargetStage}/${TargetMode}
TargetRoot = ${ProductRoot}/target/${TargetOS}${x64_ext}/${TargetStage}/bin

ExpatRoot  = ${ProjectRoot}/third-party/expat/public
ZUIDRoot   = ${ProductRoot}/third-party/zuid

ifeq "${TargetMode}" "static"
	LibName = ${LibRoot}/staticXMPCore${x64_ext}.ar
	ModeOpt = -DXMP_StaticBuild=1
else
	LibName = ${LibRoot}/libAdobeXMP.so
	ifeq "${TargetOS}" "hppahpux"
		LibName = ${LibRoot}/libAdobeXMP.sl
	endif
	ifeq "${TargetOS}" "ia64hpux"
		LibName = ${LibRoot}/libAdobeXMP.sl
	endif
endif

# ==================================================================================================

# ---------------------------------------------------------------------------------------------
# Define compile and link tools and options. We've moved to gcc on all platforms.

ToolName = gcc
ifneq "${TargetPaths}" "ct"
	ToolPath = ${PATH}
else
	ifeq "${TargetOS}-${TargetArch}" "i80386linux-x64"
		ToolBase = /user/unicore/compiler/gnu/gcc4.1.0/i80386linux_64
		LibPath  = ${ToolBase}/lib64
	else
		ToolBase = /user/unicore/${TargetOS}/compiler/${GCCVer}/${OSVer}
		LibPath  = ${ToolBase}/lib
	endif
	ToolPath  = ${ToolBase}/bin
endif

ifneq "${OldToolSuffix}" ""
	# These allow build with SunStudio
	ifeq "${OldToolSuffix}" "sun"
		ToolPath = /SS12/SUNWspro/bin
		ToolName = CC
		# Special case hack for way the Monza (PDF Print Engine) team set things up without asking us.
		TargetRoot = ${ProductRoot}/public/libraries/${TargetOS}/sunstudio12/${TargetStage}
		TempRoot   = ${ProjectRoot}/build/common-unix/intermediate/${TargetOS}/sunstudio12/${TargetStage}
	endif
	# These allow to build with AIX
	ifeq "${OldToolSuffix}" "vacpp"
		ifeq "${TargetArch}" "x64"
			x64_compileFlags =-q64
			x64_linkFlags	 =-q64
			x64_stripFlags	 =-X 64
		else
			x64_compileFlags =-q32
			x64_linkFlags	 =-q32
			x64_stripFlags	 =-X 32
		endif
		ToolPath = /usr/vacpp/bin
		ToolName = xlC_r
	endif
endif

LibPathVar = LD_LIBRARY_PATH
ifeq "${TargetOS}" "hppahpux"
	LibPathVar = SHLIB_PATH
endif
ifeq "${TargetOS}" "ia64hpux"
	LibPathVar = SHLIB_PATH
endif

ifeq "${TargetPaths}" "ct"
	Compile = PATH=${ToolPath} ${ToolName}
	Link    = PATH=${ToolPath} ${LibPathVar}=${LibPath} ${ToolName}
else
	Compile = PATH=${ToolPath} ${ToolName}
	Link    = PATH=${ToolPath} ${LibPathVar}=${LibraryPath} ${ToolName}
endif

# Do something innocuous by default for Strip.
Strip = ls

ifeq "${TargetStage}" "release"
	Strip = PATH=${ToolPath} strip
	ifeq "${TargetOS}" "rs6000aix"
		Strip = PATH=/usr/bin strip
	endif
	ifeq "${TargetOS}" "hppahpux"
		# Don't have a good gcc strip on HPUX yet.
		Strip = ls
	endif
	ifeq "${TargetOS}" "ia64hpux"
		Strip = ls
	endif
endif

CPPFlags =

XMPDefines = -DUNIX_ENV=1 -DAdobePrivate=1 -DHAVE_EXPAT_CONFIG_H=1 -DXML_STATIC=1 ${ModeOpt}

ifeq "${TargetArch}" "x86"
	XMPDefines += -DXMP_64=0
endif
ifeq "${TargetArch}" "x64"
	XMPDefines += -DXMP_64=1 -D__x86_64__=1 
endif

ifeq "${TargetOS}" "sparcsolaris"
	XMPDefines += -DHaveAtomicIncrDecr=0
endif
ifeq "${TargetOS}" "hppahpux"
	XMPDefines += -DHaveAtomicIncrDecr=0
endif
ifeq "${TargetOS}" "ia64hpux"
	XMPDefines += -DHaveAtomicIncrDecr=0
endif

DashG = -g
ifeq "${TargetOS}" "rs6000aix"
ifneq "${OldToolSuffix}" "vacpp"
	DashG = -gxcoff
endif
endif

ifeq "${TargetStage}" "debug"
	CompileFlags = ${DashG} -O0 -DDEBUG=1 -D_DEBUG=1
endif

OptFlags = -O2 -Os

ifeq "${TargetStage}" "release"
	CompileFlags = ${OptFlags} -DNDEBUG=1
endif

Includes = \
   -I${ProductRoot}/public/include \
   -I${ProjectRoot}/source \
   -I${ProductRoot}/source \
   -I${ProductRoot}/build \
   -I${ProjectRoot}/build/common-unix \
   -I${ExpatRoot}/lib \
   -I${ZUIDRoot}/interfaces \
   -I${ProductRoot} \
   -I${ProjectRoot}/third-party/boost/ \

# --------------------------------
# Define settings specific to gcc.

CPPFlags = -x c++ -Wno-ctor-dtor-privacy

CompileFlags += -fPIC -funsigned-char -fexceptions -D_FILE_OFFSET_BITS=64
CompileFlags += -Wno-multichar -Wno-implicit

LinkFlags   = -shared -fPIC -z defs -Xlinker -Bsymbolic -Xlinker --version-script -Xlinker XMPCore.exp
RuntimeLibs = -lc -lm -lpthread -L${LibPath} -Bdynamic -lstdc++ -shared-libgcc

ifeq "${TargetOS}" "i80386linux"
ifeq "${TargetArch}" "x86"
	CompileFlags += -mtune=i686
endif
	LinkFlags += -Wl,-rpath,'$$ORIGIN'
endif

ifeq "${TargetOS}" "sparcsolaris"
	CompileFlags += -mcpu=ultrasparc
endif

# We need special link options on AIX and HPUX because gcc uses the native linker on them.

ifeq "${TargetOS}" "hppahpux"
	CompileFlags += -D_REENTRANT
	LinkFlags     = -shared -fPIC -Xlinker -Bsymbolic -Xlinker -c -Xlinker XMPCore-acc.exp
	RuntimeLibs   = -lc -lm -L${LibPath} -Bdynamic -lstdc++ -shared-libgcc
endif		

ifeq "${TargetOS}" "ia64hpux"
	CompileFlags += -D_REENTRANT
	LinkFlags     = -shared -fPIC -Xlinker -Bsymbolic -Xlinker -c -Xlinker XMPCore-acc.exp
	RuntimeLibs   = -lc -lm -L${LibPath} -Bdynamic -lstdc++ -shared-libgcc
endif		

ifeq "${TargetOS}" "rs6000aix"
	CompileFlags += -D_LARGE_FILE_API=1 -D_REENTRANT -pthread -fno-gcse
	LinkFlags     = -shared -fPIC -Bsymbolic -Xlinker -bE:XMPCore-vacpp.exp -pthread
	RuntimeLibs  += -Xlinker -brtl
endif

# --------------------------------------------------
# Define settings specific to Sun Studio on Solaris.

ifeq "${OldToolSuffix}" "sun"

	CPPFlags =

	CompileFlags = -KPIC -D_FILE_OFFSET_BITS=64 -DSunOS -library=stlport4
	CompileFlags += -errtags=yes  
	CompileFlags += -erroff=wbadinit,wbadasg,badargtype2w

	CompileFlags += -xO5

	LinkFlags   = -G -KPIC -library=stlport4 -M XMPCore.exp

	Strip = /usr/ccs/bin/strip
	RuntimeLibs = -lc -lm -lpthread -L${ToolPath}/lib -Bdynamic 

endif

ifeq "${OldToolSuffix}" "vacpp"
	CPPFlags =
	CompileFlags = -qpic=large -D_LARGE_FILE_API=1 -D_REENTRANT -qeh=v6
	
	LinkFlags    = -qmkshrobj -qrtti -qeh=v6 -bE:XMPCore.exp -qstaticinline -bhalt:8  -lpthread 
	Strip = /usr/bin/strip ${x64_stripFlags}
	RuntimeLibs = -L/opt/freeware/lib -lc -lm -lpthread -lrt -ldl -L${ToolPath}/lib  
endif

# add 64 bit flags (empty if n/a)
CompileFlags += ${x64_compileFlags}
LinkFlags += ${x64_linkFlags}

# ==================================================================================================
# Define the set of source files.

vpath %.incl_cpp\
    ${ProductRoot}/public/include:\
    ${ProductRoot}/public/include/client-glue:\	

vpath %.cpp\
    ${ProjectRoot}/source:\
    ${ProductRoot}/source:\
    ${ProductRoot}/public/include:\
    ${ProductRoot}/public/include/client-glue:\
    ${ZUIDRoot}/interfaces:\
    ${ZUIDRoot}/sources:\

vpath %.c\
    ${ExpatRoot}/lib:\

CSources = \
    xmlparse.c \
    xmlrole.c \
	loadlibrary.c \
    xmltok.c

CPPSources = \
    WXMPDocOps.cpp \
    WXMPIterator.cpp \
    WXMPMeta.cpp \
    WXMPUtils.cpp \
    XMPDocOps-Utils.cpp \
    XMPDocOps.cpp \
    XMPIterator.cpp \
    XMPMeta-GetSet.cpp \
    XMPMeta-Parse.cpp \
    XMPMeta-Serialize.cpp \
    XMPMeta.cpp \
    XMPUtils-FileInfo.cpp \
    XMPUtils.cpp \
    ExpatAdapter.cpp \
    ParseRDF.cpp \
    XMPCore_Impl.cpp \
    XML_Node.cpp \
    UnicodeConversions.cpp \
    ZUIDUUID.cpp \
    MD5.cpp \
    ZUIDSysDep.cpp \
    XMP_LibUtils.cpp

CObjs   = $(foreach objs,${CSources:.c=.o},${TempRoot}/$(objs))
CPPObjs = $(foreach objs,${CPPSources:.cpp=.o},${TempRoot}/$(objs))

.SUFFIXES:                # Delete the default suffixes
.SUFFIXES: .o .c .cpp     # Define our suffix list

# ==================================================================================================

# ---------------------------------------
# Define the generic compile build rules.

${TempRoot}/%.o : %.c
	@echo "------------------------------"
	@echo "source= $<"
	@echo "target= $@"
	@echo "TargetPaths= ${TargetPaths}"
	
	@echo x64_ext = ${x64_ext}
	@echo x64_compileFlags = ${x64_compileFlags}
	@echo x64_linkFlags = ${x64_linkFlags}
	
	rm -f $@
	${Compile} ${CompileFlags} ${XMPDefines} ${Includes} -c $< -o $@
	@touch $@

${TempRoot}/%.o : %.cpp
	@echo "------------------------------"
	@echo "source= $<"
	@echo "target= $@"
	@echo "TargetPaths= ${TargetPaths}"

	@echo x64_ext = ${x64_ext}
	@echo x64_compileFlags = ${x64_compileFlags}
	@echo x64_linkFlags = ${x64_linkFlags}
	@echo CompileFlags = ${CompileFlags}

	rm -f $@
	${Compile} ${CPPFlags} ${CompileFlags} ${XMPDefines} ${Includes} -c $< -o $@
	@touch $@

# ==================================================================================================

.PHONY: all rebuild msg create_dirs

# full re-build (with prior cleaning)
all : msg clean create_dirs cleanTemp ${LibName} listOutput

# currently a forward, soon to be the real thing ('all' only outputting usage info)
rebuild: all

# lazy build
build: msg create_dirs ${LibName} listOutput
	@echo "lazy build done."

msg :
ifeq "${Error}" ""
	@echo "----------------------------------------------------------------------------------"
	@echo Building XMPCore for ${TargetOS} ${TargetStage} ${TargetMode} --------------------
else
	@echo ""
	@echo "Error: ${Error}"
	@echo ""
	@echo "# To build the Adobe XMP Core DLL:"
	@echo "#   make -f XMPCore.mak [os=<os>] [stage=<stage>] [mode=<mode>] [arch=<arch>]"
	@echo "# where"
	@echo "#   os    = i80386linux | sparcsolaris | rs6000aix | hppahpux"
	@echo "#   stage = debug | release"
	@echo "#   mode  = static | dll"
	@echo "#   arch  = x86 | x64  (just say x86 for anything 32 bit)"
	@echo "#"
	@echo ""
	@exit 1
endif

create_dirs :
	@mkdir -p ${LibRoot} ${TempRoot} ${TargetRoot}

${LibName} : ${CObjs} ${CPPObjs}
	@echo ""
	@echo "Linking $@"
	rm -f $@
ifeq "${TargetMode}" "dll"
	${Link} ${LinkFlags} $? ${RuntimeLibs} -o $@
	${Strip} $@
	cp $@ ${TargetRoot}/
else
	ar -qsc $@ ${CObjs} ${CPPObjs}
endif
	@#touch file to set actual, full date
	@touch $@

clean: cleanTemp cleanLib
	
cleanTemp:
	rm -rf ${TempRoot}/*
	
cleanLib:
	rm -f  ${LibName}
	rm -rf ${ProjectRoot}/build/common-unix/intermediate/${TargetOS}/

listOutput:
	@echo "output directory: ${LibRoot}"
	@ls -l ${LibRoot}
