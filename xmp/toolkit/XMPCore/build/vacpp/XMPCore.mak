# ==================================================================================================
# Copyright 2015 Adobe
# All Rights Reserved.
#
# NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
# of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
# ==================================================================================================

# ---------------------------------
# Make sure the target OS is known.

Error =

TargetOS = ${os}

ifeq "${TargetOS}" ""
	TargetOS = ${OS}
endif


ifeq "${TargetOS}" ""
	Error = Undefined TargetOS
endif

OSVer =
ifeq "${TargetOS}" "solaris86"
	OSVer = solaris10
endif

ifeq "${TargetOS}" "rs6000aix"
	OSVer = aix6.1
endif

ifeq "${OSVer}" ""
	Error = Undefined OSVer
endif

ifeq "${MODE}" "64"
	arch = x64
endif

ifeq "${MODE}" "32"
	arch = x86
endif

ifeq "${arch}" ""
	arch = x86
endif

ifeq "${mode}" ""
	mode = dll
endif

ifeq "${stage}" ""
	stage = ${STAGE}
endif

ifeq "${paths}" ""
	paths = local
endif

ifeq "${ctlibpath}" ""
	ctlibpath = /usr/vacpp/lib
endif

OldToolSuffix = vacpp

# -----------------------------------------------
# All of the real processing is in a common file.

include ../common-unix/XMPCore.mak
