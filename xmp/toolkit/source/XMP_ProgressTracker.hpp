#ifndef __XMP_ProgressTracker_hpp__
#define __XMP_ProgressTracker_hpp__ 1

// =================================================================================================
// Copyright Adobe
// Copyright 2012 Adobe
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================

#if AdobePrivate
// =================================================================================================
// Change history
// ==============
//
// Writers:
//  AWL Alan Lillich
//	AB  Amit Bhatti
//
// mm-dd-yy who Description of changes, most recent first.
//
// 01-05-15	AB	5.6-f122 Provide more functionalities to Plugin( Existing XMP packet, PacketInfo, OpenFlags, Error Callback and progress notification),
//						 more standard handler access API getFileModDate,IsMetadataWritable,putXMP,getAssociatedResources.
//						 New plugin handler for MPEG4 with Exif support.
//
// 06-22-12 AWL 5.5-f019 Add file update progress tracking to the MPEG-4 handler.
// 06-20-12 AWL 5.5-f018 Add outer layers for XMPFiles progress notifications, no handlers use it yet.
//
// =================================================================================================
#endif	// AdobePrivate

#include "public/include/XMP_Environment.h"	// ! XMP_Environment.h must be the first included header.

#include "public/include/XMP_Const.h"

#include "source/PerfUtils.hpp"

// =================================================================================================

class XMP_ProgressTracker {
public:
	
	struct CallbackInfo {

		XMP_ProgressReportWrapper wrapperProc;
		XMP_ProgressReportProc clientProc;
		void * context;
		float  interval;
		bool   sendStartStop;

		void Clear() { this->wrapperProc = 0; this->clientProc = 0;
					   this->context = 0; this->interval = 1.0; this->sendStartStop = false; };
		CallbackInfo() { this->Clear(); };
		CallbackInfo ( XMP_ProgressReportWrapper _wrapperProc, XMP_ProgressReportProc _clientProc,
					   void * _context, float _interval, bool _sendStartStop )
			: wrapperProc(_wrapperProc), clientProc(_clientProc),
			  context(_context), interval(_interval), sendStartStop(_sendStartStop) {};

	};

	XMP_ProgressTracker ( const CallbackInfo & _cbInfo );

	void BeginWork ( float _totalWork = 0.0 );
	void AddTotalWork ( float workIncrement );
	void AddWorkDone ( float workIncrement );
	void WorkComplete();
	CallbackInfo * GetCallbackInfo() {
		//return ( (&cbInfo) ? (&cbInfo) : NULL); //cbInfo is an object so its address will always be available
        return (&cbInfo);
	}

	bool WorkInProgress() { return this->workInProgress; };

	~XMP_ProgressTracker() {};

private:

	XMP_ProgressTracker() { this->Clear(); };	// Hidden on purpose.

	void Clear();
	void NotifyClient ( bool isStartStop = false );
	
	CallbackInfo cbInfo;
	bool workInProgress;
	float totalWork, workDone;
	PerfUtils::MomentValue startTime, prevTime;

};	// XMP_ProgressTracker

// =================================================================================================

#endif	// __XMP_ProgressTracker_hpp__
