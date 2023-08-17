/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1994-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 CosExpT.h

 - Types, macros, structures, etc. required to use the Cos HFT.

*********************************************************************/

#ifndef _H_CosExpT
#define _H_CosExpT

#include "ASExpT.h"

#ifdef __cplusplus
extern "C" {
#endif

/** */
typedef ASInt32 CosCryptVersion;
/** */
typedef ASUns16 CosGeneration;
/** */
typedef ASUns32 CosID;
/** <code>0</code> is not valid. */
typedef ASUns32 CosHashCode;
/** */
typedef ASInt32 CosStreamStartAndCode;
/** <code>-1</code> for none, error, or other special meaning */
typedef ASInt32 CosByteMax;
/**
	Used for an array of bytes in CosDocGetID(). 
	@see CosDocGetID
*/
typedef ASUns8  CosByte;  /* used for arrays of bytes */

/* Type codes for Cos objects */

enum {
	/** A <code>NULL</code> object or an invalid object. */
	CosNull		= 0,
	/** An integer object. */
	CosInteger	= 1,
	/** A fixed number object. It is a deprecated type-name for real numbers. */
	CosFixed	= 2,	
	/** A preferred type-name for real numbers. */
	CosReal		= 2,
	/** An ASBool object. */
	CosBoolean	= 3,
	/** A name object. */
	CosName		= 4,
	/** A string object. */
	CosString	= 5,
	/** A dictionary object. */
	CosDict		= 6,
	/** An array object. */
	CosArray	= 7,
	/** A stream object. */
	CosStream	= 8
};

/**
	Constants that specify a Cos object's type (string, number, 
	dictionary, and so on). 

	@see CosObjGetType 
*/
typedef ASInt32 CosType;

/* Opaque. It represents an Acrobat document. */
/** */
typedef struct _t_CosDoc *CosDoc;

typedef OPAQUE_64_BITS CosObj;
typedef OPAQUE_64_BITS CosObjCollection;


/**
	A callback for CosObjEnum(), CosDocEnumIndirect(), and PDDocEnumOCGs(). 
	It is called once for each component of a composite Cos object 
	(dictionary, array, and stream). 
	@param obj Possible values:

	<TABLE rules="all" cellspacing="1">
	<TR><TH>Value</TH><TH>Description</TH></TR>
	<TR><TD><code>Dictionary</code></TD><TD>A key.</TD></TR>
	<TR><TD><code>Array</code></TD><TD>An array element.</TD></TR>
	<TR><TD><code>Stream</code></TD><TD>The stream's dictionary (the whole thing, not one key at a time).</TD></TR>
	</TABLE>

	@param value Possible values:

	<TABLE rules="all" cellspacing="1">
	<TR><TH>Value</TH><TH>Description</TH></TR>
	<TR><TD><code>Dictionary</code></TD><TD>The value associated with the Key.</TD></TR>
	<TR><TD><code>Array</code></TD><TD>A <code>NULL</code> Cos object.</TD></TR>
	<TR><TD><code>Stream</code></TD><TD>A <code>NULL</code> Cos object.</TD></TR>
	</TABLE>

	<p>For CosDocEnumIndirect() and PDDocEnumOCGs(), this is always 
	the <code>NULL</code> Cos object. </p>

	@param clientData User-supplied data that was passed in 
	the call to CosObjEnum(), CosDocEnumIndirect(), or PDDocEnumOCGs().
	@return <code>true</code> to continue enumeration, <code>false</code> to halt enumeration. 
	
	@see CosObjEnum 
	@see CosDocEnumIndirect 
	@see PDDocEnumOCGs 
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *CosObjEnumProc)(
			CosObj obj, CosObj value, void *clientData);

	/*	The data in a CosStream may be filtered and encrypted.
	If CosOpenRaw is specified to CosStreamOpenStm, the data will
	be delivered by the returned ASStm as it is in the source ASStm,
	neither filtered nor decrypted.
	If CosOpenUnfiltered is specified, the data will be decrypted
	but not filtered.
	If CosOpenFiltered is specified, the data will be both
	decrypted and filtered.  (This is the usual case.)
 */
enum {
	/**	The data will be decrypted but not filtered. 
	
*/
	cosOpenRaw,
	/**
*/
	cosOpenUnfiltered,
	/**	The data will be both decrypted and filtered. (This is the 
	usual case.) */
	cosOpenFiltered,
	/** */
	cosOpenRawIgnoreFKey,
	/** */
	cosOpenUnfilteredIgnoreFKey,
	/** */
	cosOpenFilteredIgnoreFKey
};
/**
	Constants that specify whether filters and decryption should 
	be applied to the stream's data. 
	@see CosStreamOpenStm 
*/
typedef ASEnum8 CosStreamOpenMode;

/* CosDocOpenParams.openFlags bitfield values */
/** */
#define kCosDocOpenDoRepair	0x0001

/* CosDocOpenParams
** Used in the call to CosDocOpenWithParams.
*/
/**
	Parameters used when saving a file using CosDocOpenWithParams(). 
	
	@see CosDocOpenWithParams 
*/
typedef struct _t_CosDocOpenParams {
	/** The size of this <code>struct</code>. */
	ASSize_t	size;	
	/** A bitfield of kCosDocOpen flags. */
	ASFlagBits	openFlags;	
	/** May be <code>NULL</code> if using the default file system. */
	ASFileSys	fileSys;	
	/** Must be provided. */
	ASPathName	pathName;	
	/** An expected header string (for example, <code>"%ADF-"</code>). If it is <code>NULL</code>, <code>"%PDF-"</code> is the assumed value. */
	const char *headerString;	
	} CosDocOpenParamsRec, *CosDocOpenParams;

/* Flags for CosDocCreate createFlags parameter */
/** */
#define cosDocCreateInfoDict	0x01

/* Flags for CosDocSave saveFlags parameter */
/** Delete unreferenced objects before save. */
#define cosSaveGarbageCollect	0x01	
/** Write all objects, not just changes. */
#define cosSaveFullSave			0x02
/**	Do NOT use the newly saved file as new store, stay with the current one */
#define	cosSaveCopy			0x04
/** It is ok to store binary data in the file. */
#define cosSaveBinaryOK			0x08	
/** If there are any object streams, write them
	in a way that is hidden from PDF 1.4 
	(and earlier) viewers. This is used for
	hybrid files, for example. */
#define cosSaveConcealObjStreams	0x10	
/** */
typedef ASFlagBits CosDocSaveFlags;

/* CosDocSaveParams
** Used in call to CosDocSaveToFile */
/**
	Parameters used when saving a file using CosDocSaveToFile() 
	and CosDocSaveWithParams(). 
	@see CosDocSaveToFile 
	@see CosDocSaveWithParams 
*/
typedef struct _t_CosDocSaveParams {
	/** The size of this <code>struct</code>. */
	ASSize_t		size;
	/** A complete header string, such as <code>"%ADF-1.0"</code>. */
	const char		*header;
	/** The encryption key to pass into the PDCryptHandler 
	if security has been set on the document. */
	char			*cryptData;
	/** The length of the encryption key in bytes. 
	Cannot be greater than 5. */
	ASTArraySize	cryptDataLen;
	/** The progress monitor. Use AVAppGetDocProgressMonitor() 
	to obtain the default progress monitor. */
	ASProgressMonitor mon;	
	/** A pointer to user-supplied data to 
	pass to <code>mon</code> each time it is called. */
	void			*monClientData;	
	/** The Cos cryptographic version - the 
	version of the algorithm that is used to encrypt and decrypt 
	document data. <code>cryptVersion</code> equal to <code>0</code> is treated as <code>cryptVersion</code> 
	equal to <code>1</code>, to maintain backward compatibility. */
 	CosCryptVersion	cryptVersion;
} CosDocSaveParamsRec, *CosDocSaveParams;


/**
	A callback for CosDocEnumEOFs(). It is called once for each position in a CosDoc
	after a <code>%%EOF</code> keyword that marks the end of either a main cross-reference 
	section, or an update cross-reference section that corresponds 
	to an incremental save. See CosDocEnumEOFs() for more details.
	@param cosDoc The CosDoc in which the EOF is found. 
	@param fileOffset The 31-bit offset into the file directly following 
	the <code>%%EOF</code> keyword. If the procedure is called more than 
	once, the file positions passed to it are in decreasing 
	order (that is, the EOF positions are treated as <i>rollback 
	points</i>). 
	@param clientData User-supplied data that was passed in 
	the call to CosDocEnumEOFs().
	@return <code>true</code> to continue enumeration, <code>false</code> to halt the enumeration. 
	
	@see CosDocEnumEOFs 
	@see CosDocEnumEOFsProc64

	@note The precise value passed to the procedure 
	is not defined. It is at least one byte past the <code>%%EOF</code> keyword, 
	but may include one or more white space characters. When 
	the procedure is called only once, there is no guarantee 
	that <code>fileOffset</code> is the same as the length of the file.
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *CosDocEnumEOFsProc)(CosDoc cosDoc,
	ASFileOffset fileOffset, void *clientData);

/**
	A callback for CosDocEnumEOFs64(). It is called once for each position in a CosDoc
	after a <code>%%EOF</code> keyword that marks the end of either a main cross-reference 
	section, or an update cross-reference section that corresponds 
	to an incremental save. See CosDocEnumEOFs() for more details.
	This is similar to CosDocEnumEOFsProc(), except that the
	<code>fileOffset</code> parameter is a 64-bit value instead of a 31-bit value.
	@param cosDoc The CosDoc in which the EOF is found. 
	@param fileOffset The 64-bit offset into the file directly following 
	the <code>%%EOF</code> keyword.  
	@param clientData User-supplied data that was passed in 
	the call to CosDocEnumEOFs64().
	@return <code>true</code> to continue enumeration, <code>false</code> to halt the enumeration. 
	
	@see CosDocEnumEOFs64
	@see CosDocEnumEOFsProc
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *CosDocEnumEOFsProc64)(CosDoc cosDoc,
	ASFileOffset64 fileOffset, void *clientData);

/**
	A callback for PDDocSaveParams() used by PDDocSaveWithParams(). 
	Use this to get information about Cos objects of interest 
	while a PDDoc is being saved. 
	@param obj IN/OUT The CosObj found. 
	@param fileOffset IN/OUT The offset of <code>obj</code> into the PDF file. 
	
	@param length IN/OUT The length of <code>obj</code>. 
	@param clientData IN/OUT A pointer to user-supplied data passed 
	in the <code>offsetProcClientData</code> parameter of the PDDocSaveParams 
	structure. 
	@see PDDocSaveWithParams 
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *CosObjOffsetProc)(
	CosObj obj, ASFilePos fileOffset, ASArraySize length, void *clientData);

typedef ACCBPROTO1 void (ACCBPROTO2 *CosObjOffsetProc64)(
	CosObj obj, ASFilePos64 fileOffset, ASUns64 length, void *clientData);

/**
	A callback in PDDocPreSaveInfo(), which is used by the PDDocPreSaveProc() 
	callback. Use this callback to set a flag in each CosObj 
	that you care about, so that your callback will be called back during 
	the PDDoc's save and will be given the Cos object's offset and length. 
	After a PDF file is saved, the Cos objects previously obtained 
	are no longer valid. 
	@param obj IN/OUT The CosObj marked. 
	@param set IN/OUT <code>true</code> to set the flag to be called back during 
	the save, <code>false</code> otherwise. 
	@see PDDocSaveWithParams 
*/
typedef ACCBPROTO1 ASBool  (ACCBPROTO2 *CosObjSetCallbackFlagProc)(
	CosObj obj, ASBool set);

/** A prototype for the string encryption/decryption callback. This is part of the Crypt Filter mechanism. */
typedef ACCBPROTO1 ASInt32 (ACCBPROTO2 *CosCryptStringProc)(CosDoc dP, ASAtom filterName, char *dest, char *src, 
		ASInt32 dstSize, ASInt32 srcLength, ASUns32 genNumber, ASUns32 objNumber);
		
/*	The following constants discribe various versions of PDF files.
	The form is 0x00 Major Minor ExtLevel with each being 2 hex digits
	All calls that set a single version parameter should be deprecated
	and replaced with calls that use AdobePDFVersion.
	MRB 07/15/16 Adding support for PDF version 2.X  	*/
		  
typedef enum AdobePDFVersion {
	kNullPDFVersion 			= 0x00000000,	// 	We use this to test for uninitialized versions
	kMinPDFVersion 				= 0x00010000,	//	Anything less than this malformed.
	kAdobeAcrobat4Version		= 0x00010300,
	kAdobeAcrobat5Version		= 0x00010400,
	kAdobeAcrobat6Version		= 0x00010500,
	kAdobeAcrobat7Version		= 0x00010600,
	kAdobeAcrobat8Version		= 0x00010700,
	kAdobeAcrobat9Version 		= 0x00010703,
	kAdobeAcrobat9_1Version 	= 0x00010705,
	kAdobeAcrobat10Version 		= 0x00010708,
	kAdobeAcrobat11Version 		= 0x0001070B,

	kMinSaveVersion	 			= kAdobeAcrobat4Version,
	kMinXRefStreamVersion		= kAdobeAcrobat6Version,
	
/* 	MRB kDefaultPDFVersion is the default version when we do a saveAs. 
    Clients should use this rather than a specific version unless a 
    higher version is required.
    Please update this as required in future Acrobat versions  */	
	kDefaultPDFVersion 			= kAdobeAcrobat7Version,
	kLastAdobe1XVersionWithoutExt	= 0x00010700, // MRB left numeric intentionally 
	kLastAdobe1XVersionWithExt	= kAdobeAcrobat11Version,

	kMinPDFNextVersion 			= 0x00020000,  // MRB Will want change this before GM

/*	This should always be defined as the highest
	PDF version we support								*/
	kCurrentPDFVersion 		= 0x00020000
} AdobePDFVersion;


#ifdef __cplusplus
}
#endif

#endif /* _H_CosExpT */
