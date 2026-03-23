// =================================================================================================
// Copyright Adobe
// Copyright 2014 Adobe
// All Rights Reserved
//
// NOTICE: Adobe permits you to use, modify, and distribute this file in accordance with the terms
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
//	AB  Amit Bhatti
//
// mm-dd-yy who Description of changes, most recent on top
//
// 01-05-15	AB	5.6-f122 Provide more functionalities to Plugin( Existing XMP packet, PacketInfo, OpenFlags, Error Callback and progress notification),
//						 more standard handler access API getFileModDate,IsMetadataWritable,putXMP,getAssociatedResources.
//						 New plugin handler for MPEG4 with Exif support.
//
// =================================================================================================
#endif // AdobePrivate

#include "XMP_Const.h"
#include "source/XMP_LibUtils.hpp"
#include "PluginUtils.h"

namespace XMP_PLUGIN
{
	// =================================================================================================
	// ErrorCallbackInfo::CanNotify()
	// ====================================
	bool ErrorCallbackInfo::CanNotify() const
	{
		XMP_Assert ( (this->clientProc == 0) || (this->wrapperProc != 0) );
		return ( this->clientProc != 0 );
	}	// ErrorCallbackInfo::CanNotify()

	// =================================================================================================
	// ErrorCallbackInfo::ClientCallbackWrapper()
	// ====================================
	bool ErrorCallbackInfo::ClientCallbackWrapper ( XMP_StringPtr _filePath,
															 XMP_ErrorSeverity severity,
															 XMP_Int32 cause,
															 XMP_StringPtr messsage ) const
	{
	
		XMP_StringPtr filePathPtr = _filePath;
		if ( filePathPtr == 0 ) {
			filePathPtr = this->filePath.c_str();
		}

		XMP_Bool retValue = (*this->wrapperProc) ( this->clientProc, this->context, filePathPtr, severity, cause, messsage );
		return ConvertXMP_BoolToBool(retValue);
	}	// ErrorCallbackInfo::ClientCallbackWrapper()

	// =================================================================================================
	// ErrorNotifyClient
	// ===================================
	// Plugin method to throw exception
	void ErrorNotifyClient( GenericErrorCallback * errCBptr, XMP_ErrorSeverity severity, XMP_Error & error )
	{	
		if (errCBptr)
			errCBptr->NotifyClient( severity, error );
		else {
			if ( severity != kXMPErrSev_Recoverable )
				throw error;
		}
	}	// ErrorNotifyClient()
	
}	//  namespace XMP_PLUGIN