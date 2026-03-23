/*************************************************************************
* CAVAlert.h
* ___________________
*
*  Copyright 2001-2006 Adobe Systems Incorporated
*  All Rights Reserved.
*
* NOTICE: Adobe permits you to use, modify, and distribute this file
* in accordance with the terms of the Adobe license agreement
* accompanying it. If you have received this file from a source other
* than Adobe, then your use, modification, or distribution of it
* requires the prior written permission of Adobe.
**************************************************************************/

#ifndef __CAVALERT_H__
#define __CAVALERT_H__

#if defined(PI_ACROVIEW_VERSION) && (PI_ACROVIEW_VERSION >= AcroViewHFT_VERSION_5)
#if defined(PI_ASEXTRA_VERSION) && (PI_ASEXTRA_VERSION >= ASExtraHFT_VERSION_5)

#include "AVCalls.h"
#include "ASExtraCalls.h"

#if WIN_ENV
#pragma warning( disable : 4611 ) /* interaction between '_setjmp' and C++ object destruction is non-portable. */
/* Later we should remove the DURING/HANDLER/END_HANDLER instances from this file */
#endif

// Typedefs for enumeration used within the class
typedef ASEnum16 AVAlertResult;
// enumeration for the return values
enum 
{
	kAVAlertError = 0,
	kAVAlertOk,
	kAVAlertCancel,
	kAVAlertYes,
	kAVAlertNo
};

class CAVAlert
{
public:
	// Constructor and Destructor
	CAVAlert(AVDoc pDoc = NULL)
	{
		SetDoc(pDoc);
		// Zero out the AVAlertWithParams structure
		memset( &m_cAlertParams, 0, sizeof(m_cAlertParams) );
	};
	virtual ~CAVAlert()
	{
	}
	// Exports
	static AVAlertResult Display(ASInt32 iconType, AVAlertType alertType, ASConstText pMsg, ASBool beep = false, AVDoc pDoc = NULL)
	{
		CAVAlert cAlert;
		cAlert.SetDoc(pDoc);
		return cAlert.IDisplay(iconType, alertType, pMsg, beep);
	}
	AVAlertResult IDisplay(ASInt32 iconType, AVAlertType alertType, ASConstText pMsg, ASBool beep = false)
	{
		return IDisplay(iconType, alertType, pMsg, NULL, NULL, NULL, beep);
	}
	// Exported APIs for easy migration
	static void DisplayNote( ASConstText pMsg, ASBool beep = false)
	{
		CAVAlert cAlert;
		if (beep) beep = false; // Eliminate warning
		cAlert.IDisplay(ALERT_NOTE, kAVAlertTypeOk, pMsg, NULL, NULL, NULL, false);
	}
	static ASBool DisplayOkCancel( ASConstText pMsg, ASBool beep = false)
	{
		CAVAlert cAlert;
		if (beep) beep = false; // Eliminate warning
		return (cAlert.IDisplay(ALERT_CAUTION, kAVAlertTypeOkCancel, pMsg, NULL, NULL, NULL, false) == kAVAlertOk);
	}
	/* unix_newport_changes:sdash: if type is bool it better be set to false for C++ */
	static AVAlertResult DisplayYesNo(ASInt32 IconType, ASConstText pMsg, bool bCancel = false, ASBool beep = false)
	{
		CAVAlert cAlert;
		if (beep) beep = false; // Eliminate warning
		return cAlert.IDisplay(IconType, (AVAlertType)((bCancel)? kAVAlertTypeYesNoCancel: kAVAlertTypeYesNo), pMsg, NULL, NULL, NULL, false);
	}
	AVAlertResult IDisplay(ASInt32 iconType, AVAlertType alertType, ASConstText pMsg
		, ASConstText aButton1, ASConstText aButton2, ASConstText aButton3
		, ASBool beep = false)
	{
		AVAlertResult alertResult = kAVAlertError;
		ASInt32 nResult = IDisplayOld(iconType, alertType, pMsg, aButton1, aButton2, aButton3, beep);
		switch (alertType)
		{
		case kAVAlertTypeOk:
			alertResult =  (AVAlertType)(((nResult == 1)? kAVAlertOk: kAVAlertError));
			break;
		case kAVAlertTypeOkCancel:
			alertResult = (AVAlertType)((nResult == 1)? kAVAlertOk: ((nResult == 2)? kAVAlertCancel: kAVAlertError));
			break;
		case kAVAlertTypeYesNo:
			alertResult = (AVAlertType)((nResult == 1)? kAVAlertYes: ((nResult == 2)? kAVAlertNo: kAVAlertError));
			break;
		case kAVAlertTypeYesNoCancel:
			alertResult = (AVAlertType)((nResult == 1)? kAVAlertYes: ((nResult == 2)? kAVAlertNo: ((nResult == 3)? kAVAlertCancel: kAVAlertError)));
			break;
		default:
			break;
		}
		return alertResult;
	}
	ASInt32 IDisplayOld(ASInt32 iconType, AVAlertType alertType, ASConstText pMsg
		, ASConstText aButton1 = NULL, ASConstText aButton2 = NULL, ASConstText aButton3 = NULL
		, ASBool beep = false)
	{
		ASInt32 nResult = 0;
		DURING
		{
			/* Don't memset m_cAlertParams here, we just stomp over the AVAlertCheckBoxInfo data.
			 * The expectation is that if you are using the checkbox, you manage that state through
			 * CAVAlert::ShowCheckBox. */
//			memset( &m_cAlertParams, 0, sizeof(m_cAlertParams) );
			m_cAlertParams.size = sizeof(m_cAlertParams);
			m_cAlertParams.iconType = iconType;
			m_cAlertParams.alertType = alertType;
			m_cAlertParams.beep = beep;
			m_cAlertParams.message = const_cast< ASText >( pMsg );
			m_cAlertParams.parentDoc = m_pDoc;
			m_cAlertParams.button1.title = const_cast< ASText >( aButton1 );
			m_cAlertParams.button2.title = const_cast< ASText >( aButton2 );
			m_cAlertParams.button3.title = const_cast< ASText >( aButton3 );
			#ifndef PDFLPI
			nResult = AVAlertWithParams( &m_cAlertParams );
			#endif

		}
		HANDLER 
		{
		}
		END_HANDLER;
		return nResult;
	}
	void ShowCheckBox(ASBool bCheckbox = true, ASBool bValue = false, ASConstText aTitle = NULL)
	{
		m_cAlertParams.checkbox.show = bCheckbox;
		m_cAlertParams.checkbox.value = bValue;
		m_cAlertParams.checkbox.title = const_cast< ASText >( aTitle );
	};
	ASBool IsChecked()
	{
		return m_cAlertParams.checkbox.value;
	}
	void SetDoc(AVDoc pDoc)
	{
		m_pDoc = pDoc;
	}
	// APIs that should be used for debug purposes or places where
	// translation is not an issue
	static void DisplayNote(const char* pMsg, ASBool beep = false)
	{
		DURING
		{
			#ifndef PDFLPI
			ASText pText = ASTextFromEncoded(pMsg, AVAppGetLanguageEncoding());
			DisplayNote(pText, beep);
			ASTextDestroy(pText);
			#endif

		}
		HANDLER 
		{
		}
		END_HANDLER;
	}
protected:
	AVDoc m_pDoc;
	// Using AVAlertWithParams to enable the right type of icon
	AVAlertParamsRec m_cAlertParams;
};

#endif // defined(PI_ASEXTRA_VERSION) && (PI_ASEXTRA_VERSION >= ASExtraHFT_VERSION_5)
#endif // defined(PI_ACROVIEW_VERSION) && (PI_ACROVIEW_VERSION >= AcroViewHFT_VERSION_5)
#endif // __CAVALERT_H__
