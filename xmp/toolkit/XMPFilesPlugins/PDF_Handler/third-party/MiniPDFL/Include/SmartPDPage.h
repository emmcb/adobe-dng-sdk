/*********************************************************************************
	File: 		SmartPDPage.h
	Created:	June 16, 2003
	Purpose:	This class contains a thin wrapper for PDPage objects that
				ensures the reference count is decremented when the object
				is destroyed.
*
* ___________________
*
*  (c) Copyright 2002-2006 Adobe Systems, Inc.
*  All Rights Reserved.
*
* NOTICE: Adobe permits you to use, modify, and distribute this file
* in accordance with the terms of the Adobe license agreement
* accompanying it. If you have received this file from a source other
* than Adobe, then your use, modification, or distribution of it
* requires the prior written permission of Adobe.
************************************************************************************/
#ifndef _SMARTPDPAGE_H
#define _SMARTPDPAGE_H

#if defined (__cplusplus)

/* Include common macros */
#include "PDClassDefs.h"

#include "CorCalls.h"

#if ACROBAT_LIBRARY || PDFL_EXTENSION
#include "PDCalls.h"
#endif //#if ACROBAT_LIBRARY || PDFL_EXTENSION

/* Set ENABLE_SMARTPDPAGETESTS to 1 to enable unit tests for the class  */
#define ENABLE_SMARTPDPAGETESTS 0

/**
   This class ensures that PDPage objects are released even in case 
   of exceptions and <code>RAISE</code>. There are many ways to acquire a Page that 
   we need to be careful about, and those APIs are listed below:
   <ul>
   <li>PDDocAcquirePage</li> 
   <li>PDPageAcquireFromCosPage</li> 
   <li>PDCosAcquirePage</li>
   <li>PDBeadAcquirePage</li> 
   </ul>
   
   <p>Note that PDPage objects retrieved using AVPageViewGetPage() should not be released.</p>

   <p>This class is modeled based on a <code>SmartPointer</code> class. It does not
   have a <code>release()</code> API, as there was a clash in the meaning of the API
   in the <code>auto_ptr</code> domain and the <code>PD</code> API domain.</p>
   
   <p>This class is designed to be a <code>Final</code> class, and hence does not
   have a virtual destructor. Classes that need this functionality
   should encapsulate it using containment.</p>
*/

#if CPP_EXCEPTIONS
#define ERRFILE "SmartPDPage.h"
#endif

class CSmartPDPage
#ifdef BASECLASS
: public BASECLASS
#endif
{
public:

	/**
		Constructor.
		<p>This is the default constructor. It is used along with
		AssignAndTakeOwnership() or the AcquirePage() method.</p>
	*/
	CSmartPDPage(): m_pdPage(NULL)
	{
	}

	/**
		Constructor.
		<p>This is a constructor for the most common PDPage creation APIs. Modules using this
		class should never call the PDDocAcquirePage() directly.</p>

		@param pdDoc			The PDDoc containing the page that is requested. This
								cannot be <code>NULL</code>.
		@param nPageNum			The page number of the page that is requested. This cannot
								be negative and should be in the page range of the document.
	*/
	CSmartPDPage(PDDoc pdDoc, ASInt32 nPageNum): m_pdPage(NULL)
	{
		AssignPage(pdDoc, nPageNum);
	}

	/**
		Constructor.
		<p>This constructor takes a PDPage pointer as parameter. It increases the
		reference count of the object before taking ownership of the PDPage 
		pointer passed in. It is to be used in cases where the PDPage pointer
		is acquired by APIs that do not increase the reference count (for example, 	AVPageViewGetPage()).</p>

		@param pdPage	The Page object. This should not be <code>NULL</code>.
		@example		<code>CSmartPDPage cPDPage(AVPageViewGetPage(avPageView));</code>
	*/
	explicit CSmartPDPage( PDPage pdPage): m_pdPage(NULL)
	{
		AssignPage(pdPage);
	}

	/**
		Copy Constructor.
		<p>It increases the reference count of the contained PDPage object in case
		it is not <code>NULL</code>.</p>

		@param cAutoPage	The CSmartPDPage object.
	*/
	CSmartPDPage (const CSmartPDPage &cAutoPage): m_pdPage(NULL)
	{
		/* Increase the refcount for the page. */
		if (cAutoPage.m_pdPage != NULL)
			AssignPage(cAutoPage.m_pdPage);
	}

	/**
		Destructor.
		<p>If the PDPage object is not empty, then it 
		releases the PDPage object. This method is not a virtual method, 
		since the class is supposed to be a <code>Final</code> class.</p>
	*/
	~CSmartPDPage ()
	{
		if (m_pdPage != NULL)
		{
			/* Destructor should not Raise and hence is different from Reset */
			PDPage tmpPage = m_pdPage;
			m_pdPage = NULL;
			DURING
				PDPageRelease(tmpPage);
			HANDLER
			END_HANDLER
		}
	}

	/**
		The PDPage object is released. It then acquires a new PDPage
		pointer using PDDocAcquirePage(). Modules using this class should never call 
		PDDocAcquirePage() directly.

		<p>In the case of an exception, the object will be set to the empty state.</p>

		@param pdDoc			The PDDoc containing the page that is requested. This
								cannot be <code>NULL</code>.
		@param nPageNum			The page number of the page that is requested. This cannot
								be negative and should be in the page range of the document.
	*/
	void AcquirePage(PDDoc pdDoc, ASInt32 nPageNum)
	{
		/* Release the current PDPage object. */
		Reset();
		/* Acquire a new page */
		AssignPage(pdDoc, nPageNum);
	}

	/**
		The old PDPage pointer contained within is released. It then takes ownership 
		of the PDPage pointer passed in as parameter. The method does not increase the
		reference count of the passed in PDPage object. This method is used in scenarios 
		where the <code>Smart</code> pointer needs to be constructed before
		the page object becomes available. It is also used with APIs like 
		PDPageAcquireFromCosPage() that have already increased the reference count for
		the PDPage object but are not encapsulated by the class methods.

		<p>In the case of an exception, the object will be set to the empty state.</p>

		@param pdPage			The Page object. It cannot be <code>NULL</code>.
	*/
	void AssignAndTakeOwnership(PDPage pdPage)
	{
		ACROASSERT(pdPage != NULL);
		/* Release the current PDPage object. */
		Reset();
		/* Take ownership of the new PDPage object. */
		m_pdPage = pdPage;
	}

	/**
		PDPage operator.
		<p>This is an operator to access the PDPage pointer contained within. This gives access to 
		the raw pointer within and hence should not be released. The lifetime of the
		PDPage pointer returned is bound to the CSmartPDPage object that returned it.</p>

		<p>This does not raise exceptions.</p>

		@return The PDPage contained in this object. It may be <code>NULL</code>.
	*/
	inline operator PDPage ( void ) const
	{
		return m_pdPage;
	}

	/**
		Assignment operator.
		<p>This copies the PDPage pointer contained within the CSmartPDPage object passed 
		in as parameter. It increases the reference count in case the PDPage pointer
		contained within the passed in object is not <code>NULL</code>.</p>

		<p>In the case of an exception, the object will be set to the empty state.</p>

		@param hRhs				The Page object.
		@return A reference to <code>this</code>.
	*/
	CSmartPDPage& operator=( const CSmartPDPage& hRhs )
	{
		if( this != &hRhs )
		{
			/* Release the current PDPage object. */
			Reset();
			/* Assign and increase the refcount for the page. */
			if (hRhs.m_pdPage != NULL)
				AssignPage(hRhs.m_pdPage);
		}
		return *this;
	}

	/**
		This method releases ownership of the internal PDPage object. The caller
		will be responsible for releasing the PDPage object. The PDPage object 
		contained within will be empty once this API is called, and hence should not 
		be used.

		<p>This does not raise exceptions.</p>

		@return the PDPage that was contained in this object. This can be <code>NULL</code>.
	*/
	PDPage GiveOwnershipToCaller()
	{
		PDPage pdRetVal = m_pdPage;
		m_pdPage = NULL;
		return pdRetVal;
	}

	/**
		Releases the PDPage object. In case the object
		is empty, this method does not do anything. The PDPage object will 
		no longer be valid once this method is called, and hence should not be used.

		<p>In the case of an exception, the object will be set to the empty state.</p>
	*/
	void Reset()
	{
		if (m_pdPage != NULL)
		{
			// The method 'PDPageRelease' may raise and hence we need to reset the value before this
			PDPage tmpPage = m_pdPage;
			m_pdPage = NULL;
			ASTRY
			{
				PDPageRelease(tmpPage);
			}
			END_ASTRY
		}
	}
/* Run the following tests when changing the class */
#if (ENABLE_SMARTPDPAGETESTS)
	static void UnitTestSmartPDPage(PDPage pdPageOwnedByCaller, PDDoc pdDoc, ASInt32 nPageNum)
	{
		//////////////////////////////////////
		// Test Code to test CSmartPDPage
		// ASRaise test
		DURING
		{
			CSmartPDPage pdPage(pdPageOwnedByCaller);
			ASRaise(0);
		}
		HANDLER
		END_HANDLER
		// Method tests
		{
			// Constructor
			CSmartPDPage pdPage(pdDoc, nPageNum);
			// PDPage operator
			CSmartPDPage pdCopy = pdPage;
			// Copy constructor
			CSmartPDPage pdCopy2;
			CSmartPDPage pdCopy3(pdCopy2);
			CSmartPDPage pdCopy4(pdCopy);
			// Release
			PDPageRelease(pdCopy.GiveOwnershipToCaller());
			//Assignment operator
			pdPage = pdCopy4;
			pdPage = pdCopy;
			//Assign and take ownership
			pdPage.AssignAndTakeOwnership(pdCopy4.GiveOwnershipToCaller());
			// Acquire page
			pdPage.AcquirePage(pdDoc, nPageNum);
			// Reset
			pdPage.Reset();
			// Destructor
			{
				CSmartPDPage pdPageDesTest, nullPDPageTest;
				pdPageDesTest.AcquirePage(pdDoc, nPageNum);
			}
		}
	}
#endif /* ENABLE_SMARTPDPAGETESTS */

private:

#if (NEEDS_RAISEAWARE)
	/* The volatile keyword is needed in a RAISE scenario */
	PDPage volatile m_pdPage;
#else
	PDPage m_pdPage;
#endif

	/**
		PDPage assignment operator.
		<p>There is no clear way to handle this operator. It could be used as
		<code>AssignAndTakeOwnership()</code> (the reference count is unchanged) or AcquirePage(),
		where the reference count is increased by <code>1</code>. Hence we have made this
		private so that the unambiguous APIs are explicitly used.</p>
		@param hRhs				The Page object.
	*/
	CSmartPDPage& operator=( const PDPage& hRhs );

	/**
		Wrapper for PDPageAcquirePage(). This API assumes that there is no
		PDPage object.

		<p>In the case of an exception, the object will be set to the empty state.</p>

		@param pdPage			The Page object. This should not be <code>NULL</code>.
	*/
	void AssignPage(PDPage pdPage)
	{
		ACROASSERT((m_pdPage == NULL) && (pdPage != NULL));
		/* Assign and increase the refcount for the page. */
		ASTRY 
		{
			PDPageAcquirePage(pdPage);
			m_pdPage = pdPage;
		}
		END_ASTRY;
	}
	/**
		Wrapper for PDDocAcquirePage(). This API assumes that there is no
		PDPage object.

		<p>In the case of an exception, the object will be set to the empty state.</p>

		@param pdDoc			The PDDoc containing the page that is requested. This
								cannot be <code>NULL</code>.
		@param nPageNum			The page number of the page that is requested. This cannot
								be negative and should be in the page range of the document.
	*/
	void AssignPage(PDDoc pdDoc, ASInt32 nPageNum)
	{
		ACROASSERT((m_pdPage == NULL) && (pdDoc != NULL) && (nPageNum >= 0));
		/* Acquire the page. */
		ASTRY 
		{
			m_pdPage = PDDocAcquirePage(pdDoc, nPageNum);
		}
		END_ASTRY;
	}
};

#if CPP_EXCEPTIONS
#undef ERRFILE
#endif

#endif /* __cplusplus */

#endif /* _SMARTPDPAGE_H */
