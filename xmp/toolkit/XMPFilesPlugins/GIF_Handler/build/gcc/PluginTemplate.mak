# ==================================================================================================
# Copyright 2011 Adobe
# All Rights Reserved.
#
# NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
# of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
# ==================================================================================================

# -----------------------------------------------
# All of the real processing is in a common file.

PluginName = PluginTemplate

CPPSources = \
	Template_Handler.cpp \
	Host_IO-POSIX.cpp \
	XMPFiles_IO.cpp \
	XIO.cpp

include PluginCommon.mak
