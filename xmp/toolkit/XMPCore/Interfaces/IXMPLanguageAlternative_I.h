#ifndef __IXMPLanguageAlternative_I_h__
#define __IXMPLanguageAlternative_I_h__ 1

// =================================================================================================
// Copyright Adobe
// Copyright 2014 Adobe
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
//  HK	Honey Kansal
//
// mm-dd-yy who Description of changes, most recent first.
//
// 05-19-14 ADC 5.6-c012 Refactoring XMPCommon code and appropriate changes in XMPCore and XMPCompareAndMerge.
// 02-24-14 HK  5.6-c004 Added new internal XMP DOM interfaces + fixed memory leaks + implemented qualifiers for new XMP DOM.
//
// =================================================================================================
#endif	// AdobePrivate

#include "XMPCore/XMPCore_Defines.h"
#include "XMPCommon/XMPCommonFwdDeclarations.h"

#include "XMPCore/Interfaces/IXMPStructNode.h"
#include "XMPCore/Interfaces/IXMPLanguageAlternative.h"

namespace AdobeXMPCore_Int{

	class IXMPLanguageAlternative
		: public virtual IXMPLanguageAlternative_latest
	{

		

	};
};

#endif  // __IXMPLanguageAlternative_I_h__
