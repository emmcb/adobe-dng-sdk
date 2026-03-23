/*
** MiniPDFLConfig.h
**
** Configuration file CoreTech MiniPDFL
**
**
** (c) Copyright 2012 Adobe Systems Inc.  All Rights Reserved.
*/

#ifndef _H_MINIPDFLCONFIG
#define _H_MINIPDFLCONFIG

#ifndef PDFEDIT_IN_CORE
#if UNIX_ENV
#define PDFEDIT_IN_CORE 1
#else
#define PDFEDIT_IN_CORE 1
#endif
#endif

#ifndef ADD_FILTERS
#define ADD_FILTERS     1
#endif

#ifndef ADD_ANNOTS
#define ADD_ANNOTS      1
#endif

#ifndef ADD_OCG
#define ADD_OCG         1
#endif

#ifndef ADD_ASFIXED
#define ADD_ASFIXED     0
#endif

#ifndef ADD_BSAFE
#define ADD_BSAFE       1
#endif

#ifndef CAN_EXPORT_HFTS
#if ANDROID
#define CAN_EXPORT_HFTS 0
#else
#define CAN_EXPORT_HFTS 1
#endif
#endif


// Fix invalid combinations
#if !PDFEDIT_IN_CORE && ADD_OCG
#undef ADD_OCG
#define ADD_OCG 0
#endif


#endif //_H_MINIPDFLCONFIG
