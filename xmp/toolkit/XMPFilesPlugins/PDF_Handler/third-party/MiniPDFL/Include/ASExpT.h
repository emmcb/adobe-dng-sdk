/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1994-2007 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 ASExpT.h

 - Types, macros, structures, etc. required to use the AcroSupport HFT.

*********************************************************************/

#ifndef _H_ASExpT
#define _H_ASExpT

#include "CoreExpT.h"
#include "acroassert.h"

#if defined(__GNUC__)
#define AS_UNUSED_PARAM(type, name)	type name __attribute__ ((unused))
#else
#define AS_UNUSED_PARAM(type, name) type name
#endif

#if defined(__GNUC__)
#define AS_UNUSED_VAR(type, name)	type __attribute__ ((unused)) name
#else
#define AS_UNUSED_VAR(type, name)	type name; name
#endif

#if defined(IA64HPUX)
#if defined(open64)
#define _open64_ open64
#undef open64
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* define types based on SDK_LEVEL */

/** File access modes used to specify how a file can be used when it is open. Not all modes can
	be specified individually: ASFILE_CREATE can be used only in conjunction with
	ASFILE_READ or ASFILE_WRITE. In addition, it is acceptable to specify
	ASFILE_READ and ASFILE_WRITE together by <code>OR</code>-ing the two constants.
	ASFILE_SERIAL and ASFILE_LOCAL (present only in version 3.0 or later) are hints that
	help the Acrobat viewer optimize access to the file; they must be <code>OR</code>-ed with one or more
	of the other constants:
	<TABLE rules="all" cellspacing="1">
	<TR><TH>Value</TH><TH>Description</TH></TR>
	<TR><TD>ASFILE_READ</TD><TD>Open the file for reading.</TD></TR>
	<TR><TD>ASFILE_WRITE</TD><TD>Open the file for writing.</TD></TR>
	<TR><TD>ASFILE_CREATE</TD><TD>Create the file if it does not exist.</TD></TR>
	<TR><TD>ASFILE_SERIAL</TD><TD>A hint indicating that the file will be accessed sequentially.</TD></TR>
	<TR><TD>ASFILE_LOCAL</TD><TD>A hint indicating that a local copy of the file will be needed.</TD></TR>
	</TABLE>
	@see ASFileSysOpenFile
	@see ASFileReopen
*/
typedef ASUns16 ASFileMode;

/**
	A file position value for use in callback procedures. This value cannot exceed 2 GB.
	@see CosObjOffsetProc
*/
typedef ASUns32 ASFilePos;

/**
	A file offset value for use in callback procedures.
	@see CosDocEnumEOFsProc
*/
typedef ASInt32 ASFileOffset;
typedef ASInt64 ASFileOffset64;

/** The absolute position within a file. This value can exceed 2 GB. */
typedef ASUns64 ASFilePos64;

/**
	An error code value for use in <code>ASFile</code> and <code>ASFileSys</code> methods
	and callbacks. Introduced in Acrobat 6.0.
*/
typedef ASInt32 ASErrorCode;

/**
	A byte count value for use in ASProcStmRdExHandler and ASFileSysItemProps.
	@see ASUUIDGenFromName
*/
typedef ASUns32 ASByteCount;

/**
	An array size value for use in callback procedures.
	@see AVDocSelectionAcquireQuadsProc
	@see CosObjOffsetProc
*/
typedef ASUns32 ASArraySize;

/**
	A flag-bits value.
	@see ASFileSetMode
	@see CosDocCreate
	@see CosDocSaveToFile
	@see CosDocSaveWithParams
	@see HFTReplaceEntry
	@see HFTReplaceEntryEx
	@see PDAnnotInfo
	@see ASFileSysGetFileFlags
	@see ASFileSysGetStatusProc
	@see PDAnnotHandlerGetAnnotInfoFlagsProc
*/
typedef ASUns32 ASFlagBits;

/** Can only contain values up to 4 GB. */
typedef ASUns32 ASDiskSpace;

/** */
typedef ASUns64 ASDiskSpace64;

/** */
typedef ASUns32 ASlFileMode;

/** */
typedef ASUns32 ASMaskBits;

/** */
typedef ASInt32 ASDuration;

/**
	An HFT version number.
	@see ASExtensionMgrGetHFT
	@see HFTServerProvideHFTProc
	@see HFTGetVersion
*/
typedef ASUns32 ASVersion;

/** May not be larger than <code>int16</code>. */
typedef ASUns16 ASSmallBufferSize;

/**
	A numeric count value.
	@see ASGetSecs
	@see ASIsValidUTF8
	@see AVAppCreateIconBundle6
	@see AVDocGetNthPageView
	@see AVDocGetNumPageViews
*/
typedef ASUns32 ASCount;

/** */
typedef ASUns8  ASByte;

/* types that may transition to unsigned types */

/** A signed <code>int</code> value. Negative values are never used. */
typedef ASInt16 ASSmallCount;

/**
	A numeric count value for use in I/O methods and data structures.

	@see ASFileGetEOF
	@see ASFileGetPos
	@see ASFilePushData
	@see ASFileSetPos
	@see ASFileCompletionProc
	@see ASFileSysMReadRequestProc
*/
typedef ASInt32 ASTFilePos;

/**
	A numeric array size value for use in AS and Cos-level I/O
	methods and data structures.
	@see numerous
	@see ASFileCompletionProc
	@see ASFileSysGetNameProc
	@see ASFileSysMReadRequestProc
	@see ASStmProc
*/
typedef ASInt32 ASTArraySize;

/**
	A cryptographic version number.
	@see CosCryptGetVersion
	@see CosDecryptGetMaxKeyBytes
	@see CosEncryptGetMaxKeyBytes
*/
typedef ASInt32 ASTVersion;

/**
	A numeric count value for use in stream methods.
	@see ASIsValidUTF8
	@see ASStmFlush
	@see ASStmRead
	@see ASStmWrite
	@see CosCopyStringValue
	@see CosDocGetID
	@see CosStreamPos
	@see CosStringValue
	@see CosStringValueSafe
	@see HFTNew
	@see ASStmProc
*/
typedef ASInt32 ASTCount;

#if (!defined(ACRO_SDK_LEVEL)  || (ACRO_SDK_LEVEL < 0x00090000)) && defined(_MSC_VER)
/* #pragma message is supported by the MS VC++ compiler (but not supported by the GCC 4.0.1 compiler) */
#pragma message("Warning: Using older Acrobat SDK.  Define ACRO_SDK_LEVEL to 0x00090000")
#endif

#if !defined(ACRO_SDK_LEVEL)  || (ACRO_SDK_LEVEL < 2)
/**
	A coordinate for a point in device space, for use in mouse click
	callbacks. Values are conditionally compiled as 16-bit or
	32-bit integers, depending on the Acrobat version.

	@see ASGetSecs
	@see ASIsValidUTF8
	@see AVAppCreateIconBundle6
	@see AVDocGetNthPageView
	@see AVDocGetNumPageViews
*/
typedef ASInt16 ASCoord;
#else

/**
	A coordinate for a point in device space, for use in mouse click
	callbacks. Values are conditionally compiled as 16-bit or
	32-bit integers, depending on the Acrobat version.

	@see ASGetSecs
	@see ASIsValidUTF8
	@see AVAppCreateIconBundle6
	@see AVDocGetNthPageView
	@see AVDocGetNumPageViews
*/
typedef ASInt32 ASCoord;


#if 0
 /* potential future transitions */
typedef ASUns16 ASSmallCount;
typedef ASUns32 ASTFilePos;
typedef ASUns32 ASTArraySize; /*elements in array */
typedef ASUns32 ASTVersion;
typedef ASUns32 ASTCount;
#endif

#endif /* ...ACRO_SDK_LEVEL < 2 */


/** */
typedef ASEnum8 ASErrSeverity;


/** A stream object definition (see ASStream.h).
	It is a data stream that may be a buffer in memory, a file, or an arbitrary user-written
	procedure. It is typically used to extract data from a PDF file. When writing or extracting
	data streams, the ASStm must be connected to a Cos stream.
	@see ASFileStmRdOpen
	@see ASFileStmWrOpen
	@see ASMemStmRdOpen
	@see ASProcStmRdOpen
	@see CosStreamOpenStm
	@see ASStmClose
*/
typedef struct _t_ASStmRec ASStmRec, *ASStm;
#define ASCRYPTSTM_EOF (-1)

/* ASCryptStm is essentially ASStm object for security handler plug-in.
** ASCryptStm callbacks are trapped by PD layer and converted into ASStm.
** See PDRegisterCryptFilter() definition on how ASCryptStm is used.
**
** These #define are used for setting modeFlag in ASCryptStmRec structure. For decryption, read flag,
** for encryption, write flag should be set. EOF and error flag should also be set as appropriate.
*/
#define	ASCryptStmModeRead	0x0001
#define	ASCryptStmModeWrite	0x0002
#define	ASCryptStmModeEOF	0x0004
#define	ASCryptStmModeError	0x0008

/**
	An ASStm object cover used for a cryptographic filter's
	stream callbacks.
	@see PDCryptFilterStreamProc
*/
typedef struct _t_ASCryptStmRec *ASCryptStm;

/* Prototypes for ASCryptStm callback routines */

/**
	A callback for ASCryptStm. This is called by <code>getc</code> when the buffer is empty.
	It is called only during decryption (when reading from the stream, not when writing).
	@param pistm The security stream to fill.
	@return <code>0</code> when successful, a non-zero error code otherwise.
	@see ASCryptStmFCloseProc
	@see ASCryptStmFFlushProc
	@see ASCryptStmFlsBufProc
	@see ASCryptStmFPutEOFProc
	@see ASCryptStmFResetProc
	@see ASCryptStmUnGetcProc
*/
typedef ASInt32	(*ASCryptStmFilBufProc)(ASCryptStm pistm);

/**
	A callback for ASCryptStm. This is called by <code>putc</code> when the buffer is full.
	It is called only during encryption (when writing to the stream, not when reading).
	@param ch The character being written to the full stream.

	@param stm The security stream that is full.
	@return <code>0</code> when successful, a non-zero error code otherwise.
	@see ASCryptStmFCloseProc
	@see ASCryptStmFFlushProc
	@see ASCryptStmFilBufProc
	@see ASCryptStmFPutEOFProc
	@see ASCryptStmFResetProc
	@see ASCryptStmUnGetcProc
*/
typedef ASInt32	(*ASCryptStmFlsBufProc)(ASInt32 ch, ASCryptStm stm);

/**
	A callback for ASCryptStm. It goes back one character in the
	input stream, undoing a character <code>get</code> operation. It is called
	only during decryption (when reading from the stream, not when writing).
	@param ch The character being written to the stream.
	@param stm The security stream to which the character
	is written.
	@return <code>0</code> when successful, a non-zero error code otherwise.
	@see ASCryptStmFCloseProc
	@see ASCryptStmFFlushProc
	@see ASCryptStmFilBufProc
	@see ASCryptStmFlsBufProc
	@see ASCryptStmFPutEOFProc
	@see ASCryptStmFResetProc
*/
typedef ASInt32	(*ASCryptStmUnGetcProc)(ASInt32 ch, ASCryptStm stm);

/**
	A callback for ASCryptStm. This flushes a dirty buffer if necessary.

	@param stm The security stream to be flushed.
	@return <code>0</code> when successful, a non-zero error code otherwise.
	@see ASCryptStmFCloseProc
	@see ASCryptStmFilBufProc
	@see ASCryptStmFlsBufProc
	@see ASCryptStmFPutEOFProc
	@see ASCryptStmFResetProc
	@see ASCryptStmUnGetcProc
*/
typedef ASInt32	(*ASCryptStmFFlushProc)(ASCryptStm stm);

/**
	A callback for ASCryptStm. This closes a security stream.
	@param stm The security stream to be closed.
	@return <code>0</code> when successful, a non-zero error code otherwise.
	@see ASCryptStmFFlushProc
	@see ASCryptStmFilBufProc
	@see ASCryptStmFlsBufProc
	@see ASCryptStmFPutEOFProc
	@see ASCryptStmFResetProc
	@see ASCryptStmUnGetcProc
*/
typedef ASInt32	(*ASCryptStmFCloseProc)(ASCryptStm stm);

/**
	A callback for ASCryptStm. This resets a security stream, discarding
	any buffered data. It is called only during encryption (when writing to the stream, not when reading).
	@param stm The security stream to be reset.
	@return <code>0</code> when successful, a non-zero error code otherwise.
	@see ASCryptStmFCloseProc
	@see ASCryptStmFFlushProc
	@see ASCryptStmFilBufProc
	@see ASCryptStmFlsBufProc
	@see ASCryptStmFPutEOFProc
	@see ASCryptStmUnGetcProc
*/
typedef ASInt32	(*ASCryptStmFResetProc)(ASCryptStm stm);

/**
	A callback for ASCryptStm. This puts an end-of-file (EOF) marker to a
	security stream.
	@param stm The security stream to receive the EOF.
	@return <code>0</code> when successful, a non-zero error code otherwise.
	@see ASCryptStmFCloseProc
	@see ASCryptStmFFlushProc
	@see ASCryptStmFilBufProc
	@see ASCryptStmFlsBufProc
	@see ASCryptStmFResetProc
	@see ASCryptStmUnGetcProc
*/
typedef ASInt32	(*ASCryptStmFPutEOFProc)(ASCryptStm stm);

/** Callback procs for ASCryptStm */
typedef struct {
	/** Called by <code>getc</code> when the buffer is empty. */
	ASCryptStmFilBufProc EmptyBuff;
	/** Called by <code>putc</code> when the buffer is full. */
	ASCryptStmFlsBufProc FullBuff;
	/** Backs up an input stream. */
	ASCryptStmUnGetcProc UnGetCh;
	/** Flushes a dirty buffer if necessary. */
	ASCryptStmFFlushProc FlushBuff;
	/** Closes a stream. */
	ASCryptStmFCloseProc Close;
	/** Discards any buffered data. */
	ASCryptStmFResetProc Reset;
	/** Puts an EOF marker. */
	ASCryptStmFPutEOFProc PutEOF;
} ASCryptStmProcs;

/**
	An ASStm object cover used for a cryptographic filter's
	stream callbacks.
	@see PDCryptFilterStreamProc
*/
typedef struct _t_ASCryptStmRec {

	/** The number of characters remaining in the buffer.  */
	ASInt32 count;

	/** The next character to get or put.  */
	char *currentPointer;

	/** The base of the buffer, if any. */
	char *basePointer;

	/** Flag to indicate mode:
	<TABLE rules="all" cellspacing="1">
	<TR><TH>Flag</TH><TH>Value</TH></TR>
	<TR><TD>ASCryptStmModeRead</TD><TD><code>0x0001</code> (decryption)</TD></TR>
	<TR><TD>ASCryptStmModeWrite</TD><TD><code>0x0002</code> (encryption)</TD></TR>
	<TR><TD>ASCryptStmModeEOF</TD><TD><code>0x0004</code></TD></TR>
	<TR><TD>ASCryptStmModeError</TD><TD><code>0x0008</code></TD></TR>
	</TABLE>
	*/
	ASUns32 modeFlag;

	/** Handlers for security stream access. */
	ASCryptStmProcs *procs;

	/** The base ASStm object. */
	ASStm	baseStm;

	/** The number of bytes requested for decryption.  */
	ASInt32 nBytesWanted;

	/** A pointer to arbitrary user-defined data.*/
	void *clientData;
} ASCryptStmRec;

/**
	A callback for use by ASProcStmRdOpenEx() and ASProcStmWrOpen().
	This should place data in the buffer specified by the
	parameter data.

	<p>If your procedure reads data from a file, it is generally
	quite inefficient to open the file, read the bytes, and
	close the file each time bytes are requested. Instead, consider
	opening the file the first time bytes are requested from
	it, reading the entire file into a secondary buffer, and
	closing the file. When subsequent requests for data from
	the file are received, simply copy data from the secondary
	buffer, rather than reopening the file.</p>

	@param data (Filled by the callback) The buffer into which
	your procedure must place the number of bytes specified
	by <code>nData</code>.
	@param nData The number of bytes to read from the stream and
	place into <code>data</code>.
	@param clientData User-supplied data that was passed in
	the call to ASProcStmRdOpenEx() or ASProcStmWrOpen().
	@return The number of bytes actually read or written. This should be
	equal to the <code>nData</code> parameter unless the end of file marker (EOF) has been reached, an error
	has occurred, or this is short block of data just before EOF. If
	EOF has been reached, a zero should be returned. If an error has
	occurred, a negative value should be returned.
	@see ASProcStmDestroyProc
	@see ASProcStmRdOpenEx
	@see ASProcStmWrOpen
*/
typedef ACCBPROTO1 ASTCount (ACCBPROTO2 *ASStmProc)(char *data,
	ASTArraySize nData, void *clientData);


/**
	A callback for use by ASProcStmWrOpen() and ASProcStmRdOpenEx().

	<p>This is called at the end of the stream so you can do clean up and free
	allocated memory.</p>

	@param clientData IN/OUT User-supplied data that was passed in
	the call to ASProcStmWrOpen() or ASProcStmRdOpenEx().
	@see ASStmProc
	@see ASProcStmWrOpen
	@see ASProcStmRdOpenEx
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *ASProcStmDestroyProc)(void *clientData);

/**
	A callback for use by ASProcStmRdOpenEx().

	<p>This is called to set the stream position to a new location, which may be <code>NULL</code> if the stream
	cannot be set to a new position. ASProcStmSeekProc() and ASProcStmGetLength() must
	be provided together. If either is <code>NULL</code>, the stream will not be set to a new
	position.</p>

	@param newPos IN
	@param clientData IN/OUT User-supplied data that was passed in
	the call to ASProcStmRdOpenEx().
	@see ASStmProc
	@see ASProcStmRdOpenEx
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *ASProcStmSeekProc)(ASFilePos64 newPos, void *clientData);

/**
	A callback for use by ASProcStmRdOpenEx().

	<p>This is called to get the length of the stream, which may be <code>NULL</code> if the stream cannot be
	set to a new position. ASProcStmSeekProc() and ASProcStmGetLength() must be
	provided together. If either is <code>NULL</code>, the stream will not be set to a new
	position.</p>

	@param clientData IN/OUT User-supplied data that was passed in
	the call to ASProcStmRdOpenEx().
	@return The length of the stream in bytes.
	@see ASStmProc
	@see ASProcStmRdOpenEx
*/
typedef ACCBPROTO1 ASFilePos64 (ACCBPROTO2 *ASProcStmGetLength)(void *clientData);

/** For use by ASProcStmRdOpenEx(). */
typedef struct _s_ASProcStmRdExHandler
{
	/** Set to <code>sizeof(ASProcStmRdExHandlerRec)</code>. */
	ASByteCount size;
	/** */
	ASStmProc readProc;
	/** */
	ASProcStmDestroyProc destroyProc;
	/** */
	ASProcStmSeekProc seekProc;
	/** */
	ASProcStmGetLength getLengthProc;
	/** The size of the buffer to use for the stream. If this field
		is missing, the default is <code>65535</code>. If this field is present and
		has a value of <code>0</code>, then the default is implementation-specific.
	*/
	ASByteCount bufSize;
} ASProcStmRdExHandlerRec, *ASProcStmRdExHandler;

/** */
#define HFT_ERROR_NO_VERSION (0xFFFFFFFF)

/**
	A data structure to pass to an HFT server to create a new
	HFT. New in Acrobat 6.0.
	@see HFTNewEx
*/
typedef struct _t_HFTData
{
	/** Set to <code>sizeof(HFTDataRec)</code> */
	ASUns32 size;
	/** The number of entries in the new HFT. This determines
		the number of methods that the HFT can contain;
		each method occupies one entry.
	*/
	ASCount numSelectors;
	/** The version number. */
	ASVersion version;

	/** Optional. This should point to an HFT array of function pointers. */
	const void *hftProcs;
} HFTDataRec;
typedef const HFTDataRec *HFTData;

/** Each HFT is serviced by an HFT server. The HFT server is responsible for handling
	requests to obtain or destroy its HFT. An <code>HFTServer</code> is an object that manages several versions of an
	HFT for different clients which may have been compiled with different versions of the HFT's API.
	@see HFTServerNew
	@see HFTServerDestroy
*/
typedef struct _t_HFTServer *HFTServer;

/**
	A callback for an HFT server. This returns an HFT with the specified
	version number. If the HFT has not yet been created, create
	and return it. If the HFT already exists, do not create
	a new copy of it; simply return the existing copy.

	@param hftServer The HFT server associated with this proc.
	@param version The HFT version being requested.
	@param rock User-supplied data passed in the call
	to HFTServerNew().
	@return The requested version of the HFT.
	@see HFTServerNew

	@note The version numeric type has changed in Acrobat 6.0.
*/
typedef ACCBPROTO1 HFT (ACCBPROTO2 *HFTServerProvideHFTProc)(HFTServer hftServer, ASVersion version, void *rock);


/**
	A callback for an HFT server. This destroys the specified HFT (for example, by calling HFTServerDestroy()).
	@param hftServer IN/OUT The HFT server associated with this destroy proc.
	@param rock IN/OUT User-supplied data that was passed in the call to HFTServerNew().
	@see HFTServerNew
	@see HFTDestroy
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *HFTServerDestroyProc)(HFTServer hftServer, void *rock);

/**
	The ASFixed type is a 32-bit quantity representing a rational
	number with the high (low on little-endian machines) 16
	bits representing the number's mantissa and the low (high on little-endian machines)
	16 bits representing the fractional part. The definition
	is platform-dependent.

	<p>ASFixedP is a pointer to an ASFixed object. </p>

	<p>Addition, subtraction, and negation with ASFixed types can
	be done with <code>+</code> and <code>-</code> operators, unless you are concerned with overflow. Overflow
	in ASFixed-value operations is indicated by the values <code>fixedPositiveInfinity</code>
	and <code>fixedNegativeInfinity</code>.</p>

	@see ASFixedDiv
	@see ASFixedMatrixConcat
	@see ASFixedMatrixInvert
	@see ASFixedMatrixTransform
	@see ASFixedMatrixTransformRect
	@see ASFixedMul
	@see ASFixedToCString
*/
#if ADD_ASFIXED //PKG
typedef ASInt32 ASFixed, *ASFixedP;


/*
** CONVERSION to/from standard C types and ASFixed
*/

/** */
#define ASFixedPosInf ASMAXInt32

/** */
#define ASFixedNegInf ASMINInt32

/*#define ASInt32ToFixed(i)	((ASFixed)(i) << 16)
 note that an expression argument get evaluated several times and this should be avoided */
/**
	Converts an ASInt32 to a fixed point number.
	@param i The ASInt32 to convert.

	@see ASFixedRoundToInt16
	@see ASFixedRoundToInt32
	@see ASFixedToFloat
	@see ASFixedTruncToInt16
	@see ASFixedTruncToInt32
	@see ASFloatToFixed
	@see ASFixedToFloat
	@see ASInt16ToFixed
*/
#define ASInt32ToFixed(i)     ((((ASInt32)i) < (-32767)) ? ASFixedNegInf : (((ASInt32)i) > 32767) ? ASFixedPosInf : ((ASFixed)(i) << 16))

/**
	Converts a fixed point number to an ASInt32, rounding the
	result.
	@param f The fixed point number to convert.

	@see ASFixedRoundToInt16
	@see ASFixedToFloat
	@see ASFixedTruncToInt16
	@see ASFixedTruncToInt32
	@see ASFloatToFixed
	@see ASInt16ToFixed
	@see ASInt32ToFixed
*/
#define ASFixedRoundToInt32(f)	((ASInt32) (((f) + fixedHalf) >> 16))

/**
	Converts a fixed point number to an ASInt32, truncating
	the result.
	@param f The fixed point number to convert.

	@see ASFixedRoundToInt16
	@see ASFixedRoundToInt32
	@see ASFixedToFloat
	@see ASFixedTruncToInt16
	@see ASFloatToFixed
	@see ASInt16ToFixed
	@see ASInt32ToFixed
*/
#define ASFixedTruncToInt32(f)	((ASInt32) ((f) >> 16))

/**
	Converts an ASInt16 to a fixed point number.
	@param i The ASInt16 to convert.

	@see ASFixedRoundToInt16
	@see ASFixedRoundToInt32
	@see ASFixedToFloat
	@see ASFixedTruncToInt16
	@see ASFixedTruncToInt32
	@see ASFloatToFixed
	@see ASInt32ToFixed
*/
#define ASInt16ToFixed(i)	((ASFixed)(i) << 16)
#define ASUns16ToFixed(i)	(((i) > 32767) ? ASFixedPosInf : ((ASFixed)(i) << 16))

/**
	Converts a fixed point number to an ASInt16, rounding the
	result.
	@param f The fixed point number to convert.

	@see ASFixedRoundToInt32
	@see ASFixedToFloat
	@see ASFixedTruncToInt16
	@see ASFixedTruncToInt32
	@see ASFloatToFixed
	@see ASInt16ToFixed
	@see ASInt32ToFixed
*/
#define ASFixedRoundToInt16(f)	((ASInt16) (((f) + fixedHalf) >> 16))

/**
	Converts a fixed point number to an ASInt16, truncating
	the result.
	@param f The fixed point number to convert.

	@see ASFixedRoundToInt16
	@see ASFixedRoundToInt32
	@see ASFixedToFloat
	@see ASFixedTruncToInt32
	@see ASFloatToFixed
	@see ASInt16ToFixed
	@see ASInt32ToFixed
*/
#define ASFixedTruncToInt16(f)	((ASInt16) ((f) >> 16))

/* Float <-> ASFixed */
#ifdef __cplusplus

/**
	The ASFixed type is a 32 bit quantity representing a rational
	number with the high (low on little-endian machines) 16
	bits representing the number's mantissa and the low (high on little-endian machines)
	16 bits representing the fractional part. The definition
	is platform-dependent.

	<p>ASFixedP is a pointer to an ASFixed object. </p>

	<p>Addition, subtraction, and negation with ASFixed types can
	be done with <code>+</code> and <code>-</code> operators, unless you are concerned with overflow. Overflow
	in ASFixed-value operations is indicated by the values <code>fixedPositiveInfinity</code>
	and <code>fixedNegativeInfinity</code>. </p>

	@see ASFixedDiv
	@see ASFixedMatrixConcat
	@see ASFixedMatrixInvert
	@see ASFixedMatrixTransform
	@see ASFixedMatrixTransformRect
	@see ASFixedMul
	@see ASFixedToCString
*/
inline ASFixed FloatNToFixed(const double& x) {return (((x)>32767)?ASFixedPosInf:(ASFixed)(((x)*65536.0f +0.5f))); }
/**
	The ASFixed type is a 32 bit quantity representing a rational
	number with the high (low on little-endian machines) 16
	bits representing the number's mantissa and the low (high on little-endian machines)
	16 bits representing the fractional part. The definition
	is platform-dependent.

	<p>ASFixedP is a pointer to an ASFixed object. </p>

	<p>Addition, subtraction, and negation with ASFixed types can
	be done with <code>+</code> and <code>-</code> operators, unless you are concerned with overflow. Overflow
	in ASFixed-value operations is indicated by the values <code>fixedPositiveInfinity</code>
	and <code>fixedNegativeInfinity</code>. </p>

	@see ASFixedDiv
	@see ASFixedMatrixConcat
	@see ASFixedMatrixInvert
	@see ASFixedMatrixTransform
	@see ASFixedMatrixTransformRect
	@see ASFixedMul
	@see ASFixedToCString
*/
inline ASFixed FloatIToFixed(const double& x) {return (((x)<(-32767))?ASFixedNegInf:((x)>32767)?ASFixedPosInf:((ASFixed)x)<<16); }
#else

/** FloatN to ASFixed (for use when you know that <code>float</code> numbers are non-negative). */
#define FloatNToFixed(x) ((x)<(-32767))?ASFixedNegInf:((x)>32767)?ASFixedPosInf:(((ASFixed)(((x)*65536.0f +0.5f)))

/** FloatI to ASFixed (for use when you know that <code>float</code> numbers are integer values). */
#define FloatIToFixed(x) ((x)>32767)?ASFixedPosInf:(((ASFixed)x)<<16)


#endif /* __cplusplus */

/* Backward compatibility #defines. DO NOT use these in new code */
#define Int32ToFixed ASInt32ToFixed
#define FixedRoundToInt32 ASFixedRoundToInt32
#define FixedTruncToInt32 ASFixedTruncToInt32
#define Int16ToFixed ASInt16ToFixed
#define FixedRoundToInt16 ASFixedRoundToInt16
#define FixedTruncToInt16 ASFixedTruncToInt16


/**
	@ingroup ASFixedConstants
*/
#define fixedZero					((ASFixed) 0x00000000L)

/**
	@ingroup ASFixedConstants
*/
#define fixedHundredth				((ASFixed) 0x0000028FL)

/**
	@ingroup ASFixedConstants
*/
#define fixedSixteenth				((ASFixed) 0x00001000L)

/**
	@ingroup ASFixedConstants
*/
#define fixedTwelfth				((ASFixed) 0x00001555L)

/**
	@ingroup ASFixedConstants
*/
#define fixedTenth					((ASFixed) 0x00001999L)

/**
	@ingroup ASFixedConstants
*/
#define fixedEighth					((ASFixed) 0x00002000L)

/**
	@ingroup ASFixedConstants
*/
#define fixedQuarter 				((ASFixed) 0x00004000L)

/**
	@ingroup ASFixedConstants
*/
#define fixedThird					((ASFixed) 0x00005555L)

/**
	@ingroup ASFixedConstants
*/
#define fixedHalf 					((ASFixed) 0x00008000L)

/**
	@ingroup ASFixedConstants
*/
#define fixedTwoThirds				((ASFixed) 0x0000AAAAL)

/**
	@ingroup ASFixedConstants
*/
#define fixedThreeQuarters 			((ASFixed) 0x0000C000L)

/**
	@ingroup ASFixedConstants
*/
#define fixedPi4 					((ASFixed) 0x0000c910L)

/**
	@ingroup ASFixedConstants
*/
#define fixedSevenEighths			((ASFixed) 0x0000E000L)

/**
	@ingroup ASFixedConstants
*/
#define fixedOne1					((ASFixed) 0x0000ffffL)

/**
	@ingroup ASFixedConstants
*/
#define fixedOne 					((ASFixed) 0x00010000L)

/**
	@ingroup ASFixedConstants
*/
#define fixedOneAndQuarter			((ASFixed) 0x00014000L)

/**
	@ingroup ASFixedConstants
*/
#define fixedFourThirds				((ASFixed) 0x00015555L)

/**
	@ingroup ASFixedConstants
*/
#define fixedSqrtTwo				((ASFixed) 0x00016A0AL)

/**
	@ingroup ASFixedConstants
*/
#define fixedThreeHalves			((ASFixed) 0x00018000L)

/**
	@ingroup ASFixedConstants
*/
#define fixedOneAnd3Qtr				((ASFixed) 0x0001C000L)

/**
	@ingroup ASFixedConstants
*/
#define fixedPi2 					((ASFixed) 0x00019220L)

/**
	@ingroup ASFixedConstants
*/
#define fixedGolden 				((ASFixed) 0x00019e37L)

/**
	@ingroup ASFixedConstants
*/
#define fixedTwo					((ASFixed) 0x00020000L)

/**
	@ingroup ASFixedConstants
*/
#define fixedThree					((ASFixed) 0x00030000L)

/**
	@ingroup ASFixedConstants
*/
#define fixedFour 					((ASFixed) 0x00040000L)

/**
	@ingroup ASFixedConstants
*/
#define fixedFive					((ASFixed) 0x00050000L)

/**
	@ingroup ASFixedConstants
*/
#define fixedSix					((ASFixed) 0x00060000L)

/**
	@ingroup ASFixedConstants
*/
#define fixedSeven					((ASFixed) 0x00070000L)

/**
	@ingroup ASFixedConstants
*/
#define fixedEight					((ASFixed) 0x00080000L)

/**
	@ingroup ASFixedConstants
*/
#define fixedNine					((ASFixed) 0x00090000L)

/**
	@ingroup ASFixedConstants
*/
#define fixedTen					((ASFixed) 0x000A0000L)

/**
	@ingroup ASFixedConstants
*/
#define fixedEleven					((ASFixed) 0x000B0000L)

/**
	@ingroup ASFixedConstants
*/
#define fixedTwelve					((ASFixed) 0x000C0000L)

/**
	@ingroup ASFixedConstants
*/
#define fixedSixteen				((ASFixed) 0x00100000L)

/**
	@ingroup ASFixedConstants
*/
#define fixedThirtyTwo				((ASFixed) 0x00200000L)

/**
	@ingroup ASFixedConstants
*/
#define fixedFifty 					((ASFixed) 0x00320000L)

/**
	@ingroup ASFixedConstants
*/
#define fixedSeventyTwo				((ASFixed) 0x00480000L)

/**
	@ingroup ASFixedConstants
*/
#define fixedNinety 				((ASFixed) 0x005a0000L)

/**
	@ingroup ASFixedConstants
*/
#define fixedHundred				((ASFixed) 0x00640000L)

/**
	@ingroup ASFixedConstants
*/
#define fixedHundredFifty			((ASFixed) 0x00960000L)

/**
	@ingroup ASFixedConstants
*/
#define fixedOneEighty				((ASFixed) 0x00b40000L)

/**
	@ingroup ASFixedConstants
*/
#define fixedTwoSeventy				((ASFixed) 0x010e0000L)

/**
	@ingroup ASFixedConstants
*/
#define fixedFiveHundred			((ASFixed) 0x01F40000L)

/**
	@ingroup ASFixedConstants
*/
#define fixedThousand				((ASFixed) 0x03E80000L)

/**
	@ingroup ASFixedConstants
*/
#define fixedTenThousand			((ASFixed) 0x27100000L)

/**
	@ingroup ASFixedConstants
*/
#define fixedNegativeInfinity		ASFixedNegInf

/**
	@ingroup ASFixedConstants
*/
#define fixedPositiveInfinity 		ASFixedPosInf


#ifndef __ASTypes__ /* Newer ASTypes.h defines these types */
/**
	Matrix containing fixed numbers.
*/
typedef struct _t_ASFixedMatrix
{

	/** */
	ASFixed a;

	/** */
	ASFixed b;

	/** */
	ASFixed c;

	/** */
	ASFixed d;

	/** */
	ASFixed h;

	/** */
	ASFixed v;
}
ASFixedMatrix, *ASFixedMatrixP;


/**
	A point (in two-dimensional space) represented by two fixed
	numbers.
*/
typedef struct _t_ASFixedPoint
{

	/** */
	ASFixed h;

	/** */
	ASFixed v;
}
ASFixedPoint, *ASFixedPointP;


/**
	A rectangle represented by the coordinates of its four sides.
	In the Acrobat viewer, a rectangle differs from a quadrilateral
	in that a rectangle must always have horizontal and vertical
	sides, and opposite sides must be parallel.
*/
typedef struct _t_ASFixedRect
{

	/** */
	ASFixed left;

	/** */
	ASFixed top;

	/** */
	ASFixed right;

	/** */
	ASFixed bottom;
}
ASFixedRect, *ASFixedRectP;
#endif /* ndef __ASTypes */


/**
	A structure that represents a quadrilateral defined by four
	corner points.

	@see AVPageViewInvertQuad

	@note The coordinate numeric types changed in Acrobat 6.
*/
typedef struct _t_Quad {
	/** */
	ASCoord tlh, tlv;
	/** */
	ASCoord trh, trv;
	/** */
	ASCoord blh, blv;
	/** */
	ASCoord brh, brv;
} Quad, *QuadP;


/**
	A quadrilateral represented by four fixed points (one at each
	corner). In the Acrobat viewer, a quadrilateral differs
	from a rectangle in that a rectangle must always have horizontal
	and vertical sides, and opposite sides must be parallel.
*/
typedef struct _t_ASFixedQuad {
	/** */
	ASFixedPoint tl, tr, bl, br;
} ASFixedQuad, *ASFixedQuadP;

#endif //ADD_ASFIXED //PKG

/** Definition of ASFract. */
typedef ASInt32 ASFract;

/** Definition of ASReal. */
typedef float ASReal;

#ifndef __ASTypes__ /* Newer ASTypes.h defines these types */
/** */
typedef struct _t_ASRealPoint {
	/** */
	ASReal h, v;
} ASRealPoint;

/** */
typedef struct _t_ASRealRect {
	/** */
	ASReal left, top, right, bottom;
} ASRealRect;

/** */
typedef struct _t_ASRealMatrix {
	/** */
	ASReal a, b, c, d, tx, ty;
} ASRealMatrix;
#endif /* ndef __ASTypes.h__ */


/**
	The ASDouble type is a 64-bit type representing a floating number

	<p>ASDoubleP is a pointer to an ASDouble object. </p>

	@see ASDoubleMatrixConcat
	@see ASDoubleMatrixInvert
	@see ASDoubleMatrixTransform
	@see ASDoubleMatrixTransformRect
*/
typedef double ASDouble, *ASDoubleP;


/**
	Matrix containing double numbers.
*/
typedef struct _t_ASDoubleMatrix
{

	/** */
	ASDouble a;

	/** */
	ASDouble b;

	/** */
	ASDouble c;

	/** */
	ASDouble d;

	/** */
	ASDouble h;

	/** */
	ASDouble v;
}
ASDoubleMatrix, *ASDoubleMatrixP;


/**
	A point (in two-dimensional space) represented by two double
	numbers.
*/
typedef struct _t_ASDoublePoint
{

	/** */
	ASDouble h;

	/** */
	ASDouble v;
}
ASDoublePoint, *ASDoublePointP;

typedef struct _t_ASDoubleQuad {
	/** */
	ASDoublePoint tl, tr, bl, br;
} ASDoubleQuad, *ASDoubleQuadP;

/**
	A rectangle represented by the coordinates of its four sides.
	In the Acrobat viewer, a rectangle differs from a quadrilateral
	in that a rectangle must always have horizontal and vertical
	sides, and opposite sides must be parallel.
*/
typedef struct _t_ASDoubleRect
{

	/** */
	ASDouble left;

	/** */
	ASDouble top;

	/** */
	ASDouble right;

	/** */
	ASDouble bottom;
}
ASDoubleRect, *ASDoubleRectP;

#if !ADD_ASFIXED //PKG
#define ASFixedRect			ASDoubleRect
#define ASFixedRectP		ASDoubleRectP

#define ASFixedPoint		ASDoublePoint
#define ASFixedPointP		ASDoublePointP

#define ASFixedMatrix		ASDoubleMatrix
#define ASFixedMatrixP		ASDoubleMatrixP

#define ASFixedQuad			ASDoubleQuad
#define ASFixedQuadP		ASDoubleQuadP

typedef ASDouble ASFixed, *ASFixedP;
#define CosNewFixed			CosNewDouble
#define ToASFixed			ToFloat
#define ASFixedToCString	DoubleToCString

#define ASFixedRectUnion	ASDoubleRectUnion
#define ASFixedRectUpright	ASDoubleRectUpright
#define ASFixedRectEqual	ASDoubleRectEqual
#define ASFixedRectIntersect	ASDoubleRectIntersect
#define ASFixedMatrixTransformRect	ASDoubleMatrixTransformRect
#define ASFixedMatrixConcat	ASDoubleMatrixConcat
#define ASFixedMatrixIdentity ASDoubleMatrixIdentity

#define PDEElementSetMatrix			PDEElementSetMatrixEx
#define PDETextItemSetTextMatrix	PDETextItemSetTextMatrixEx

#define fixedZero			0.f
#define fixedOne 			1.f
#define fixedTen			10.f
#define fixedHundred		100.f
#define fixedNegativeInfinity		ASMINInt32
#define ASInt16ToFixed(i)	(float)(i)
#define ASFixedToFloat(i)	(float)(i)
#define FloatToASFixed(i)	(i)
#define ASDoubleToFixed(i)	(i)
#define ASFixedToDouble(i)	(i)
#define ASFixedRoundToInt16(i)		(int)(i)
static const ASDoubleRect emptyFixedRect = { +1.0E30, -1.0E30, -1.0E30, +1.0E30 };
#endif //ADD_ASFIXED //PKG



/**
	A time/date structure.

	<p>The millisecond field is currently unused. </p>
	@see ASDateGetLocalTime
	@see ASDateGetUTCTime
	@see ASDateSetTimeFromRec
	@see PDAnnotGetDate
	@see PDAnnotSetDate
*/
typedef struct _t_ASTimeRec
{

	/**  Only common era years are displayed appropriately.
	*/
	ASInt16 year;

	/** Jan = <code>1</code>, ..., Dec = <code>12</code>.
	*/
	ASInt16 month;

	/** <code>1..31</code>
	*/
	ASInt16 date;

	/** <code>0..23 </code>
	*/
	ASInt16 hour;

	/** <code>0..59</code>
	*/
	ASInt16 minute;

	/** <code>0..59</code>
	*/
	ASInt16 second;

	/** <code>0..999</code>
	*/
	ASInt16 millisecond;

	/** Sun = <code>0</code>, ... Sat = <code>6</code>.
	*/
	ASInt16 day;

	/** GMT offset in half hours east of Greenwich.
		<code>-48</code> signifies an unknown value.
	*/
	ASInt16 gmtOffset;
}
ASTimeRec, *ASTimeRecP;

/* ASCab
**
** An opaque type.
*/

/** ASCab objects (<i>cabinets</i>) can be used to store arbitrary key/value pairs. The keys
	are always <code>NULL</code>-terminated strings containing only low ASCII alphanumeric characters
	and spaces (ASCII character <code>32</code>). Key names cannot begin or end with a space.

	<p>Every time you place a non-scalar value inside a cabinet, you are handing that value to
	the ASCab implementation to manage. Putting a value in a cabinet is always a
	handoff operation. For example, if you create an ASText object and add it as a value
	in an ASCab, the ASText object is no longer managed by you; it is managed by the
	ASCab. The ASCab will destroy the ASText object when its associated key is
	removed or the key's value is overwritten. Pointer values are a special case
	discussed in more detail below.</p>

	<p>The routine naming convention is as follows:</p>

	<TABLE rules="all" cellspacing="1">
	<TR><TH>Name</TH><TH>Description</TH></TR>
	<TR><TD>Get</TD><TD><code>Get</code> routines return a value. These objects are owned by the ASCab and should
	not be destroyed by the caller of <code>Get</code>.</TD></TR>
	<TR><TD>GetCopy</TD><TD><code>GetCopy</code> routines make a copy of the data; the <code>GetCopy</code> client owns the
	resulting information and can modify it at will; it is also responsible for destroying
	it.</TD></TR>
	<TR><TD>Detach</TD><TD><code>Detach</code> routines work the same way as <code>Get</code> routines, but the key is removed from
	the ASCab without destroying the associated value that is passed back to the client
	of <code>Detach</code>. The client is responsible for destroying the returned object.</TD></TR>
	</TABLE>


	<p>Normally, pointers are treated the same way as scalars; the ASCab passes the pointer
	value back and forth but does not manage the data to which it points. This all changes
	if the pointer has an associated <code>destroyProc</code>. If the <code>destroyProc</code> is set, the ASCab
	will reference count the pointer to track how many times the pointer is referenced from
	any ASCab. For example, the reference count will be bumped up whenever the pointer is
	copied via ASCabCopy() or added to another ASCab via ASCabPutPointer(), and will
	destroy the data associated with the pointer when the reference count goes to <code>0</code>. The data is
	destroyed by calling the <code>destroyProc</code>. Detaching a pointer removes one reference to
	the pointer without ever destroying the information to which it points.
	ASCabDetachPointer() returns a separate value indicating whether the pointer can
	safely be destroyed by the client or is still referred to by other key/value pairs inside
	any ASCab objects (for example, whether the reference count went to zero when the pointer was
	detached from the ASCab).</p>

	<p>Any of the ASCab API's can take a compound name: a string consisting of multiple
	keys separated by the colon (:) character. For example, <code>"Grandparent:Parent:Child:Key"</code> can be such a compound name. The
	implementation will burrow down through such a compound string until it reaches the
	most deeply nested cabinet. Also, any of the <code>Put</code> routines can take a <code>NULL</code> key
	name. If the key name is <code>NULL</code>, the routine creates a new numeric key name. If the
	cabinet is empty, the first generated key name will be <code>"0"</code> and subsequent names will
	increase in ascending order. This is useful when treating an ASCab as a bag of
	unnamed items, or when adding an ordered list of items to an empty ASCab.</p>
	@see ASCabNew
	@see ASCabDestroy
 */
typedef struct _t_ASCabinet *ASCab;
typedef const struct _t_ASCabinet *ASConstCab;

/*------------------------------------------------------------------------
	typedefs for ASText
------------------------------------------------------------------------*/

/** An opaque object holding encoded text.

	<p>An ASText object represents a Unicode string. ASText objects can also be used to
	convert between Unicode and various platform-specific text encodings, as well as
	conversions between various Unicode formats such as UTF-16 or UTF-8. Since it is
	common for a Unicode string to be repeatedly converted to or from the same
	platform-specific text encoding, ASText objects are optimized for this operation.
	For example, they can cache both the Unicode and platform-specific text strings.</p>

	<p>There are several ways of creating an ASText object depending on the type and
	format of the original text data. The following terminology is used throughout this API
	to describe the various text formats:</p>

	<TABLE rules="all" cellspacing="1">
	<TR><TH>Text Format</TH><TH>Description</TH></TR>
	<TR><TD>Encoded</TD><TD>A multi-byte string terminated with a single <code>0</code> character and coupled
	with a specific host encoding indicator. On Mac OS, the text encoding is
	specified using a script code. On Windows, the text encoding is specified
	using a <code>CHARSET</code> code. On UNIX the only valid host encoding indicator is <code>0</code>, which
	specifies text in the platform's default Roman encoding. On all platforms, Asian text
	is typically specified using multi-byte strings.</TD></TR>
	<TR><TD>ScriptText</TD><TD>A multi-byte string terminated with a single <code>0</code> character and coupled
	with an ASScript code. This is merely another way of specifying the Encoded
	case; the ASScript code is converted to a host encoding using ASScriptToHostEncoding().</TD></TR>
	<TR><TD>Unicode</TD><TD>Text specified using UTF-16 or UTF-8. In the UTF-16 case, the bytes can
	be in either big-endian format or the endian-ness that matches the platform, and
	are always terminated with a single ASUns16 <code>0</code> value. In the UTF-8 case, the text is
	always terminated with a trailing <code>0</code> byte. Unicode usage in this case is straight Unicode without
	the <code>0xFE 0xFF</code> prefix or language and country codes that can be encoded inside a
	PDF document.</TD></TR>
	<TR><TD>PDText</TD><TD>A string of text pulled out of a PDF document. This will either be a big-endian
	Unicode string pre-appended with the bytes <code>0xFE 0xFF</code>, or a string in
	PDFDocEncoding. In this case, the Unicode string may have embedded language
	and country identifiers. ASText objects strip language and country information out
	of the PDText string and track them separately. See below for more details.</TD></TR>
	</TABLE>


	<p>ASText objects can also be used to accomplish encoding and format conversions; you can
	request a string in any of the formats specified above.
	In all cases the ASText code attempts to preserve all characters. For example, if you
	attempt to concatenate two strings in separate host encodings, the implementation may
	convert both to Unicode and perform the concatenation in Unicode space.</p>

	<p>When creating a new ASText object or putting new data into an existing object, the
	implementation will always copy the supplied data into the ASText object. The
	original data is yours to do with as you wish (and release if necessary).</p>

	<p>The size of ASText data is always specified in bytes. For example, the <code>len</code> argument to
	ASTextFromSizedUnicode() specifies the number of bytes in the string, not the
	number of Unicode characters.</p>

	<p>Host encoding and Unicode strings are always terminated with a <code>NULL</code> character
	(which consists of one <code>NULL</code> byte for host encoded strings and two <code>NULL</code> bytes for
	Unicode strings). You cannot create a string with an embedded <code>NULL</code> character, even
	using the calls which take an explicit length parameter.</p>

	<p>The <code>Getxxx</code> calls return pointers to data held by the ASText object. You cannot free
	or manipulate this data directly. The <code>GetxxxCopy</code> calls return data you can
	manipulate and that you are responsible for freeing.</p>

	<p>An ASText object can have language and country codes associated with it. A
	language code is a 2-character ISO 639 language code. A country code is a 2-
	character ISO 3166 country code. In both cases the 2-character codes are packed
	into an ASUns16 value: the first character is packed in bits 8-15, and the second character is packed in
	bits 0-7. These language and country codes can be encoded into a UTF-16 variant of
	PDText encoding using an escape sequence; see Section 3.8 in the <i>PDF Reference</i>.
	The ASText calls will automatically parse the language and country codes
	embedded inside a UTF-16 PDText object, and will also author appropriate escape
	sequences to embed the language and country codes (if present) when generating a
	UTF-16 PDText object.</p>

	@see ASTextNew
	@see ASTextFromEncoded
	@see ASTextFromInt32
	@see ASTextFromPDText
*/
typedef struct _t_ASTextRec *ASText;

/**
	An opaque object holding constant encoded text.
	@see ASText
*/
typedef const struct _t_ASTextRec *ASConstText;

/*------------------------------------------------------------------------
	typedefs for ASFile.h
------------------------------------------------------------------------*/


/** Open the file for reading.
	@ingroup ASFileOpenModes
*/
#define ASFILE_READ		1

/** Open the file for writing.
	@ingroup ASFileOpenModes
*/
#define ASFILE_WRITE	2

/** Create the file if it does not exist.
	@ingroup ASFileOpenModes
*/
#define ASFILE_CREATE	4


/** A hint indicating that the file will be primarily accessed sequentially.
	@ingroup ASFileOpenModes
*/
#define ASFILE_SERIAL	8

/** A hint indicating that a local copy of the file will be needed.
	@ingroup ASFileOpenModes
*/
#define ASFILE_LOCAL	16

/** A hint indicating that the file will be primarily accessed randomly.
@ingroup ASFileOpenModes
*/
#define ASFILE_RANDOMACCESS 32


/** Set if the file's data transfer rate
	is generally slow. For example, the file is on a floppy
	disk or is being accessed via modem.

	@ingroup ASFileFlags
*/
#define kASFileSlowTransfer		0x00000001L

/**	Set if initiating each access to the file is slow. For example,
	access may be slow because the file is served by an HTTP server that spawns
	a new process for each request.

	@ingroup ASFileFlags
*/
#define kASFileSlowConnect		0x00000002L

/** Use multi-read commands to access the file.
	@ingroup ASFileFlags
*/
#define kASFileUseMRead	 		0x00000004L

/** Set if the file is to be cached (requires kASFileUseMRead to be set as well).
	@ingroup ASFileFlags
*/
#define kASFileDoCaching 		0x00000008L

/**	(Acrobat 5.0 and later) Set if media/access is a dial up connection.
	This flag is only fully implemented on Windows. On Mac OS,
	this flag is always conservatively set to <code>true</code>.
	@ingroup ASFileFlags
*/
#define kASFileDialUp			0x00000010L

/** Set if the file is still being loaded.
	@ingroup ASFileFlags
*/
#define kASFileStillFetching	0x00000020L

/** <code>true</code> if the file has outstanding MReads.
	@ingroup ASFileFlags
*/
#define kASFileHasOutstandingMReads	0x00000040L

/** <code>true</code> if the file is built with a Virtual EOF (Acrobat 10).
	@ingroup ASFileFlags
*/
#define kASFileHasVirtualEOF	0x00000080L


/* ASFileMode flags */

/**	If set, ASFileRead does not yield if bytes
	are not ready (which raises the fileErrBytesNotReady exception).
	@ingroup ASFileModeFlags
*/
#define kASFileModeDoNotYieldIfBytesNotReady		0x0001

/**	If set, the file will be read all at once regardless of
	multiple read requests.

	@ingroup ASFileModeFlags
*/
#define kASFileModeDisableExplicitMReadRequests		0x0002

/**	If set, ASFileRead will raise the fileErrBytesNotReady exception when
	trying to read from a file with a cache for which the requested
	bytes are not yet present.
	@ingroup ASFileModeFlags
*/
#define kASFileRaiseIfBytesNotReady					0x0004

/**	If set, no read requests are issued if bytes are not ready
	(that is, the bytes are not in the cache).
	@ingroup ASFileModeFlags
*/
#define kASFileNoRequestIfBytesNotReady				0x0008

/**	If set, <code>ASFileRead</code> will suspend the current thread  when
	trying to read from a file with a cache for which the requested
	bytes are not yet present.  Note that if <code>kASFileSuspendIfBytesNotReady</code>
	is set, the <code>kASFileRaiseIfBytesNotReady</code> is ignored.
	@ingroup ASFileModeFlags
*/


#define kASFileSuspendIfBytesNotReady				0x0010

/* Type/Creator codes for ASFileSysSetTypeAndCreator */
#if MAC_PLATFORM
#define ASFourCharCode(x)	(x)
#else
#define ASFourCharCode(x)	(0U)
#endif



/** Acrobat creator code.
	@ingroup Creators
*/
#define kAcrobatCreatorCode		ASFourCharCode('CARO')

/** Adobe Photoshop creator code.
	@ingroup Creators
*/
#define kPhotoshopCreatorCode	ASFourCharCode('8BIM')

/** Adobe ImageReady creator code.
	@ingroup Creators
*/
#define kImageReadyCreatorCode	ASFourCharCode('MeSa')

/** Adobe Illustrator creator code.
	@ingroup Creators
*/
#define kIllustratorCreatorCode	ASFourCharCode('ART5')



/** Portable document format (PDF).
	@ingroup AcrobatTypes
*/
#define kPDFTypeCode			ASFourCharCode('PDF ')

/** Forms data format.
	@ingroup AcrobatTypes
*/
#define kFDFTypeCode			ASFourCharCode('FDF ')

/** XML forms data format.
	@ingroup AcrobatTypes
*/
#define kXFDFTypeCode			ASFourCharCode('XFDF')

/** XML data package.
	@ingroup AcrobatTypes
*/
#define kXDPTypeCode			ASFourCharCode('XDP ')

/** XML PDF.
	@ingroup AcrobatTypes
*/
#define kPXDFTypeCode			ASFourCharCode('MARS')

/** Preferences file.
	@ingroup AcrobatTypes
*/
#define kPrefsTypeCode			ASFourCharCode('PREF')

/** Acrobat catalog index file.
	@ingroup AcrobatTypes
*/
#define kPDXTypeCode			ASFourCharCode('PDX ')

/** Adobe Web Buy rights management file.
	@ingroup AcrobatTypes
*/
#define kRMFTypeCode			ASFourCharCode('RMF ')

/** Acrobat profile format (PPKLite).
	@ingroup AcrobatTypes
*/
#define kAPFTypeCode			ASFourCharCode('APF ')

/** Acrobat sequence file.
	@ingroup AcrobatTypes
*/
#define kSequenceTypeCode		ASFourCharCode('SEQU')

/** Spelling dictionary file.
	@ingroup AcrobatTypes
*/
#define kDictionaryTypeCode		ASFourCharCode('DICT')

/** Web-hosted applications file.
	@ingroup AcrobatTypes
*/
#define kWHATypeCode			ASFourCharCode('WHA ')

/** Locale file.
	@ingroup AcrobatTypes
*/
#define kLocaleTypeCode			ASFourCharCode('LANG')

/** Plug-in file.
	@ingroup AcrobatTypes
*/
#define kPluginTypeCode			ASFourCharCode('XTND')

/** Plug-in file (preferred, supported by 5.0.5 and later).
	Using this file type will allows shipping
	a Carbonized plug-in without worrying that it will try to load
	and show an error when installed under Acrobat 4 or earlier.
	@ingroup AcrobatTypes
*/
#define kPluginNewTypeCode		ASFourCharCode('XTNc')

/** eBook Exchange Transfer Data (ETD) file.
	@ingroup AcrobatTypes
*/
#define kETDTypeCode			ASFourCharCode('fETD')

/** eBook EDN activation file.
	@ingroup AcrobatTypes
*/
#define kEDNTypeCode			ASFourCharCode('fEDN')



/** Adobe Photoshop PSD file.
	@ingroup PhotoshopTypes
*/
#define kPSDTypeCode			ASFourCharCode('8BIM')

/** Mac OS PICT file.
	@ingroup PhotoshopTypes
*/
#define kPICTTypeCode			ASFourCharCode('PICT')

/** TIFF file.
	@ingroup PhotoshopTypes
*/
#define kTIFFTypeCode			ASFourCharCode('TIFF')

/** GIF file.
	@ingroup PhotoshopTypes
*/
#define kGIFTypeCode			ASFourCharCode('GIFf')

/** JPEG file.
	@ingroup PhotoshopTypes
*/
#define kJPEGTypeCode			ASFourCharCode('JPEG')

/** PNG file.
	@ingroup PhotoshopTypes
*/
#define kPNGTypeCode			ASFourCharCode('PNGf')



/** Adobe Illustrator AI file.
	@ingroup IllustratorTypes
*/
#define KAITypeCode				ASFourCharCode('TEXT')

/** EPS file.
	@ingroup IllustratorTypes
*/
#define kEPSTypeCode			ASFourCharCode('EPSF')


/** Text file.
	@ingroup MiscCreatorTypes
*/
#define kTextTypeCode			ASFourCharCode('TEXT')

/** Text file.
	@ingroup MiscCreatorTypes
*/
#define kRTFTypeCode			ASFourCharCode('RTF ')

/** SimpleText.
	@ingroup MiscCreatorTypes
*/
#define kTextCreatorCode		ASFourCharCode('ttxt')

/** QuickTime file.
	@ingroup MiscCreatorTypes
*/
#define kQuickTimeTypeCode		ASFourCharCode('MooV')

/** QuickTime player.
	@ingroup MiscCreatorTypes
*/
#define kQuickTimeCreatorCode	ASFourCharCode('TVOD')

/** HTML file.
	@ingroup MiscCreatorTypes
*/
#define kHTMLTypeCode			ASFourCharCode('TEXT')

/** Microsoft Internet Explorer.
	@ingroup MiscCreatorTypes
*/
#define kHTMLCreatorCode		ASFourCharCode('MSIE')

/** XML file.
	@ingroup MiscCreatorTypes
*/
#define kXMLTypeCode			ASFourCharCode('TEXT')

/** Microsoft Excel.
	@ingroup MiscCreatorTypes
*/
#define kExcelCreatorCode		ASFourCharCode('XCEL')

/** Microsoft Word.
	@ingroup MiscCreatorTypes
*/
#define kWordCreatorCode		ASFourCharCode('W8BN')

/** Microsoft PowerPoint.
	@ingroup MiscCreatorTypes
*/
#define kPowerPointCreatorCode	ASFourCharCode('SLD8')

/** Unknown file.
	@ingroup MiscCreatorTypes
*/
#define kUnknownTypeCode		0x3f3f3f3f

/** Unknown application.
	@ingroup MiscCreatorTypes
*/
#define kUnknownCreatorCode		0x3f3f3f3f


/**
	A data structure containing callbacks that implement a file
	system.
	@see ASFileSys
*/
typedef struct _t_ASFileSysRec *ASFileSys;


/**
	@see ASFileAcquirePathName
	@see ASFileSysAcquireParent
	@see ASFileSysCreatePathName
	@see ASFileSysPathFromDIPath
	@see ASPathFromPlatformPath
	@see PDFileSpecAcquireASPath
	@see ASFileSysReleasePath
	@see ASFileSysDIPathFromPath
*/
typedef struct _t_ASPathNameRec *ASPathName;

/**
	An opaque representation of a particular open
	file. Each open file has a unique ASFile. The ASFile value
	has meaning only to the common ASFile implementation and bears
	no relationship to platform-specific file objects.
	@see PDDocGetFile
	@see ASFileSysOpenFile
	@see ASFileFromMDFile
*/
typedef	void * ASFile;

/**
	Called when an asynchronous read or write request has completed.

	@param aFile IN/OUT The ASFile for which data is read or written.

	@param p IN/OUT A pointer to the buffer provided by the client.
	It contains <code>nBytesRead</code> bytes of data if <code>error</code> is zero.
	@param fileOffsetRequested IN/OUT The file offset requested by the
	client.
	@param countRequested IN/OUT The number of bytes requested by the
	client.
	@param nBytesRead IN/OUT The number of bytes actually read or written.

	@param error IN/OUT The error condition if it is non-zero; see AcroErr.h.

	@param compProcClientData IN/OUT The client data parameter provided
	by client.
	@see ASFileSysAsyncAbortProc
	@see ASFileSysAsyncReadProc
	@see ASFileSysAsyncWriteProc
	@see ASFileSysYieldProc
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *ASFileCompletionProc)(ASFile aFile, const char *p,
				ASTFilePos fileOffsetRequested, ASTArraySize countRequested,
				ASTArraySize nBytesRead, ASErrorCode error, void *compProcClientData);

/*------------------------------------------------------------------------
	typedefs: see ASTypes.h for definitions of ASFileSys, ASPathName, and ASFileNum
	as well as flags for ASFileSysOpenFile mode.
------------------------------------------------------------------------*/


/**
	ASMDFile replaces MDFile. MDFile is an obsolete
	name for this data type for backward compatibility.

	<p>An MDFile is an opaque representation of a file instance
	for a particular file system. File system implementors may
	choose any convenient representation for an MDFile. In particular,
	file systems need not worry about MDFile space conflicts;
	the ASFile object exported by the common implementation
	is guaranteed to be unique across all open files, and the
	common implementation maps calls of ASFile methods to calls
	of ASFileSystem callbacks with the corresponding MDFile. </p>

	@see ASFileFromMDFile
	@see ASFileGetMDFile
	@see ASFileSysAsyncAbortProc
	@see ASFileSysGetFileFlags
	@see ASFileSysYieldProc
	@see ASFileSysMReadRequestProc
	@see ASFileSysClearOutstandingMReadsProc
	@see ASFileSysGetStatusProc
	@see ASFileSysOpenProc
	@see ASFileSysCloseProc
	@see ASFileSysFlushProc
	@see ASFileSysSetPosProc
	@see ASFileSysGetPosProc
	@see ASFileSysSetEofProc
	@see ASFileSysGetEofProc
	@see ASFileSysReadProc
	@see ASFileSysWriteProc
	@see ASFileSysRenameProc
	@see ASFileSysIsSameFileProc
*/
typedef void *ASMDFile;
/* Obsolete name for this data type, for backward compatibility */
#define MDFile ASMDFile


/**
	A data structure representing an I/O request.
	@see ASFileSysAsyncReadProc
	@see ASFileSysAsyncWriteProc
	@see ASIODoneProc
*/
typedef struct _t_ASIORequestRec *ASIORequest;


/**
	Values returned by ASFileSysGetStatusProc().
	@see ASFileRead
	@see ASFileSysGetStatusProc
*/
typedef enum {

	/** The MDFile is in a valid state.
	*/
	kASFileOkay				= 0x0000,

	/** The MDFile is being closed (for example,
	because the file is being displayed in a web browser's window
	and the user cancelled downloading).
	*/
	kASFileIsTerminating	= 0x0001
	} ASFileStatusFlags;


/**
	A callback in ASIORequest used by the asynchronous read/write
	ASFileSys implementation and provided by the ASFile implementation
	to the ASFileSys. The ASFileSys must call this method when
	an asynchronous request is completed:

	<ul>
	<li>
	When an I/O request has some or all of its data.
	</li>
	<li>
	If the request is successfully queued but an error prevents
	it from completing.
	</li>
	<li>
	If the request is aborted by calling ASFileSysAsyncAbortProc().
	In this case, <code>totalBytesCompleted = 0</code> and <code>pError = -1</code>.
	</li>
	</ul>

	<p>If the request fails, this method must still be called,
	with the error. It is not called, however, if there is an error queueing
	the read or write request. </p>

	@param req The I/O request for which data has been read/written.
	@see ASFileSysAsyncAbortProc
	@see ASFileSysAsyncReadProc
	@see ASFileSysAsyncWriteProc
	@see ASFileSysYieldProc
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *ASIODoneProc)(ASIORequest req);

/**
	The first five items in the ASIORequestSingleRec structure exactly
	match the parameters of an ASFileSys read or write call.

	<p><code>totalBytesRead</code> is filled in before the <code>IODoneProc</code> is called.
	If <code>totalBytesRead</code> is <code>0</code> or if <code>pError</code> is non-zero, the read was
	either terminated or did not complete.</p>

	<p>If <code>IODoneProc</code> is non-zero, it points to a procedure that must be
	called either when the request has terminated due to an error or other
	condition, or when all of the bytes have been received for this
	request.</p>
*/
typedef struct _t_ASIORequestRec
{

	/** The MDFile corresponding to the ASFile for which this request is intended.
	*/
	ASMDFile		mdFile;

	/** A pointer to data to write to or read from <code>mdFile</code>.
	*/
	void*			ptr;

	/** An offset (specified in bytes) into <code>mdFile</code> of the first byte to read or write.
	*/
	ASTFilePos		offset;

	/** The number of bytes to read or write. It must be filled
	in before <code>IODoneProc</code> is called. If is value is <code>0</code>, the read was either
	terminated or did not complete.
	*/
	ASTArraySize	count;


	/** The number of bytes actually read or written.
	*/
	ASTArraySize	totalBytesCompleted;

	/** An error code. This code is filled by the ASFileSys before <code>IODoneProc</code> is called.
		If its value is non-zero, the read was either terminated or did not complete.
	*/
	ASErrorCode		pError;

	/** User-supplied data that the ASFileSys can use for any purpose.
	*/
	void*			clientData;


	/** A user-supplied callback for use by the ASFileSys when the operation has completed. If it is non-<code>NULL</code>,
	it points to a procedure that must be called either when the request has terminated due to an
	error or other condition, or when all of the bytes have been received for this request.
	@note This callback may be called at interrupt time.
	*/
	ASIODoneProc	IODoneProc;

	/** User-supplied data that is available for <code>IODoneProc</code> to use.
	*/
	void*			IODoneProcData;
} ASIORequestRec;

/**
	An enumerated data type used to categorize an object associated
	with an ASPathName.
	@see ASFileSysGetItemPropsProc
	@see ASFileSysFirstFolderItemProc
	@see ASFileSysNextFolderItemProc
	@see ASFileSysGetItemProps
	@see ASFileSysFirstFolderItem
	@see ASFileSysNextFolderItem
*/
enum {

	/** The object is associated with a file.
	*/
	kASFileSysFile,

	/** The object is associated with a folder.
	*/
	kASFileSysFolder,

	/** The object type is unknown.
	*/
	kASFileSysUnknown = -1
};
typedef ASEnum16 ASFileSysItemType;


/**
	A list of properties for the object referenced by an ASPathName.
	It is used in ASFileSysGetItemProps() and the folder enumeration
	routines.
	@see ASFileSysGetItemProps
	@see ASFileSysFirstFolderItem
	@see ASFileSysNextFolderItem
*/
typedef struct _t_ASFileSysItemProps {

	/** The size of the data structure. It must be set to <code>sizeof(ASFileSysItemPropsRec)</code>.
	*/
	ASSize_t			size;

	/** <code>true</code> if the object exists. If it is <code>false</code>, none of the following fields are valid.
	*/
	ASBool				isThere;

	/** One of the ASFileSysItemType objects.
	*/
	ASFileSysItemType	type;

	/**  <code>true</code> if the object's hidden bit is set.
	*/
	ASBool				isHidden;

	/** <code>true</code> if the object is read-only.
	*/
	ASBool				isReadOnly;

	/** <code>true</code> if the file system could determine the creation date of the object.
	*/
	ASBool				creationDateKnown;

	/** The creation date of the object. It is valid only if <code>creationDateKnown</code> is <code>true</code>.
	*/
	ASTimeRec			creationDate;

	/** <code>true</code> if the file system could determine the last modification date of the object.
	*/
	ASBool				modDateKnown;

	/** The modification date of the object. It is valid only if <code>modDateKnown</code> is <code>true</code>.
	*/
	ASTimeRec			modDate;

	/** If the type is kASFileSysFile, this field holds the lower 32 bits of the size of the file (the upper 32
		bits are maintained by <code>fileSizeHigh</code>.
	*/
	ASByteCount			fileSize;

	/** If the type is kASFileSysFile, this field holds the upper 32 bits of the size of the file (the lower 32
		bits are maintained by <code>fileSize</code>.
	*/
	ASByteCount			fileSizeHigh;

	/** If the type is kASFileSysFolder, this field specifies how many items are in the folder. If this value
		is <code>-1</code>, the file system was unable to easily determine the number of objects. You will need to explicitly
		enumerate the objects to determine how many are in the folder.
	*/
	ASTCount			folderSize;

	/** The Mac OS creator code for the file.
		For other file systems, this will be zero.
	*/
	ASUns32				creatorCode;

	/** The Mac OS type code for the file.
		For	other file systems, this will be zero.
	*/
	ASUns32				typeCode;
} ASFileSysItemPropsRec, *ASFileSysItemProps;


/**
	An opaque object used to iterate through the contents of
	a folder. ASFileSysFirstFolderItem() returns the first item
	in the folder along with an ASFolderIterator object for
	iterating through the rest of the items in the folder. Call
	ASFileSysNextFolderItem() with this object to return the next
	object in the folder until the method returns <code>false</code>. To
	discard the ASFolderIterator object, call ASFileSysDestroyFolderIterator().

	@see ASFileSysFirstFolderItemProc
	@see ASFileSysNextFolderItemProc
	@see ASFileSysDestroyFolderIteratorProc
	@see ASFileSysFirstFolderItem
	@see ASFileSysNextFolderItem
	@see ASFileSysDestroyFolderIterator
*/
typedef struct _t_ASFolderIterator *ASFolderIterator;


/** An ASPlatformPath and associated platform path types.
	This is an opaque object used to retrieve a platform path object.
	ASFileSysAcquirePlatformPath() allocates and initializes this
	object. <code>ASPlatformPath*</code> calls are used to access its contents.
	To discard this object, call ASFileSysReleasePlatformPath().
*/
typedef struct _t_ASPlatformPath *ASPlatformPath;

/** A UNIX or Windows platform-specific path value.
	@see ASPlatformPathGetCstringPtr
*/
typedef char*							Cstring_Ptr;		/* generally for Win_K, Unix	*/
/**
	A C string containing a POSIX path (UTF-8 encoding).
	@see ASPlatformPathGetPOSIXPathPtr
*/
typedef char*							POSIXPath_Ptr;

#if MAC_PLATFORM

#if !AS_ARCH_64BIT
/**
	A pointer to a Mac OS platform-specific FSSpec.
	@see ASPlatformPathGetFSSpecPtr
*/
typedef struct FSSpec					*FSSpec_Ptr;		/* for Mac_K (here on down)	*/
#endif // !AS_ARCH_64BIT

/**
	A pointer to a Mac OS platform-specific FSRef.
	@see ASPlatformPathGetFSRefPtr
*/
typedef struct FSRef					*FSRef_Ptr;

/**
	A structure containing the equivalent of two Mac OS platform-specific types: a pointer to
	an FSRef and a CFStringRef.

	@see ASPlatformPathGetFSRefWithCFStringRefRecPtr
*/
typedef struct _t_FSRefWithCFStringRefRec {
	/** */
    struct FSRef						*ref;
	/** Definition of a CFStringRef. */
    const struct __CFString				*str;
} FSRefWithCFStringRefRec;

/** */
typedef	FSRefWithCFStringRefRec			*FSRefWithCFStringRefRec_Ptr;

/**
	A structure containing the equivalent of a Mac OS platform-specific CFURLRef.

	@see ASPlatformPathGetCFURLRefRecPtr
*/
typedef struct _t_CFURLRefRec {
	/** */
	const struct __CFURL				*url;				/* definition of a CFURLRef */
} CFURLRefRec;

/** */
typedef	CFURLRefRec						*CFURLRefRec_Ptr;

#else	/* this is to prevent type redefinitions, e.g. with PhotoshopSDK which defines FSSpec for Win */

typedef struct FSSpecPlacebo			*FSSpec_Ptr;
typedef struct FSRefPlacebo				*FSRef_Ptr;
typedef struct _t_FSRefWithCFStringRefRecPlacebo {
    struct FSRefPlacebo					*ref;
    const struct __CFStringPlacebo		*str;
} FSRefWithCFStringRefRecPlacebo;
typedef	FSRefWithCFStringRefRecPlacebo	*FSRefWithCFStringRefRec_Ptr;
typedef struct _t_CFURLRefRecPlacebo {
	const struct __CFURLPlacebo			*url;
} CFURLRefRecPlacebo;
typedef	CFURLRefRecPlacebo				*CFURLRefRec_Ptr;

#endif	/* #if MAC_PLATFORM */


/**
	A callback for ASFileSysRec that gets the flags for the specified
	file.
	@param f IN/OUT The file whose flags are obtained.
	@return A bit field using ASFile flags.
*/
typedef ACCBPROTO1 ASFlagBits (ACCBPROTO2 *ASFileSysGetFileFlags)(ASMDFile f);

/**
	A callback for ASFileSysRec that asynchronously reads the specified
	data, returning immediately after the request has been queued.
	The ASFileSys must call the ASIODoneProc() (if one was provided)
	when the specified data has been read.

	<p>This callback is similar to the ASFileSysMReadRequestProc(),
	except that this callback contains a caller-provided ASIODoneProc()
	and can only be used for a single byte range.</p>
	@param req A data structure specifying the data to read.
	It contains information about the request including the ASMDFile,
	the file offset, the buffer for the request, the number
	of bytes in the request, an ASIODoneProc(), and client data
	for the ASIODoneProc(). If the ASIODoneProc() in <code>req</code> is non-<code>NULL</code>
	and there is an error queueing the read request, the ASIODoneProc()
	must not be called.
	@return <code>0</code> if the request was successfully queued,
	a non-zero platform-dependent error code otherwise.
	@see ASFileSysAsyncAbortProc
	@see ASFileSysAsyncWriteProc
	@see ASFileSysYieldProc
*/
typedef ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysAsyncReadProc)(ASIORequest req);

/**
	A callback for ASFileSysRec that asynchronously writes the specified
	data, returning immediately after the request has been queued.
	The ASFileSys must call the ASIODoneProc() (if one was provided)
	when the specified data has been written.
	@param req A data structure specifying the data to write.
	It contains information about the request including the ASMDFile,
	the file offset, the buffer for the request, the number
	of bytes in the request, an ASIODoneProc(), and client data
	for the ASIODoneProc(). If the ASIODoneProc() in <code>req</code> is non-<code>NULL</code>
	and there is an error queueing the write request, the ASIODoneProc()
	must not be called.
	@return <code>0</code> if the request was successfully queued,
	a non-zero platform-dependent error code otherwise.
	@see ASFileSysAsyncAbortProc
	@see ASFileSysAsyncReadProc
	@see ASFileSysYieldProc
*/
typedef ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysAsyncWriteProc)(ASIORequest req);

/**
	A callback for ASFileSysRec that aborts all uncompleted asynchronous
	I/O requests for the specified file. This callback can
	be called at any time.

	<p>This callback calls each outstanding ASIORequest object's ASIODoneProc()
	to be called with <code>totalBytes = 0</code> and <code>error = -1</code>. </p>

	@param f IN/OUT The file for which all uncompleted asynchronous
	read and write requests are aborted.
	@see ASFileSysAsyncReadProc
	@see ASFileSysAsyncWriteProc
	@see ASFileSysYieldProc
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *ASFileSysAsyncAbortProc)(ASMDFile f);


/**
	A callback for ASFileSysRec that yields the asynchronous I/O requests
	for the specified file. This allows other processes
	to process events that may be required for a file
	read to complete. An ASFileSys should implement a yield
	mechanism to complement asynchronous read and write requests.

	<p>On Windows, this could be a normal PeekMessage-based yield.</p>

	<p>In UNIX, it could mean using <code>select</code> on a file descriptor.</p>

	@param f IN/OUT The file whose asynchronous I/O requests are yielded.
	@return <code>0</code> if the request was successful, a non-zero
	platform-dependent error code otherwise.
	@see ASFileSysAsyncAbortProc
	@see ASFileSysAsyncReadProc
	@see ASFileSysAsyncWriteProc
	@see ASFileRead
*/
typedef ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysYieldProc)(ASMDFile f);


/**
	A callback for ASFileSysRec that queues asynchronous requests
	for one or more byte ranges that the caller (usually the
	Acrobat viewer or library) will need in the near future.
	This callback is important for slow file systems, such as
	the web, to improve overall performance. It allows the
	file system to begin retrieving bytes before they are actually
	needed, while the Acrobat software continues processing
	as much as it can with the data that has already been downloaded.

	<p>This callback does not actually read the data, but merely
	queues the requests, starts the asynchronous code that reads
	the data, and returns. The asynchronous code that reads
	the data must use ASFilePushData() to push the data from each
	byte range to the Acrobat software as soon as the data is
	ready.</p>

	<p>This callback is similar to the ASFileSysAsyncReadProc(),
	except that this callback contains a caller-provided ASIODoneProc()
	and can only be used for a single byte range.</p>

	@param f The file whose data is read.
	@param aFile The corresponding ASFile, for use with ASFilePushData().

	@param blockPairs An array of file offsets and byte lengths.

	@param nBlockPairs The number of block pairs in <code>blockPairs</code>.
	@return <code>0</code> if the request was successfully queued,
	a non-zero platform-dependent error code otherwise.
	@see ASFileSysAsyncReadProc
	@see ASFileSysClearOutstandingMReadsProc
	@see ASFileRead
	@see ASFileHasOutstandingMReads
*/
typedef ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysMReadRequestProc)
							(ASMDFile f, ASFile aFile, ASTFilePos *blockPairs, ASTArraySize nBlockPairs);


/**
	A callback for ASFileSysRec that is used to advise a file system
	that the previous range of bytes requested to read are not
	needed, so that it may drop the read requests. The file
	system can continue pushing the bytes if it cannot stop
	the reads.
	@param f The file that was being read.
	@see ASFileSysMReadRequestProc
	@see ASFileRead
	@see ASFileHasOutstandingMReads
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *ASFileSysClearOutstandingMReadsProc)(ASMDFile f);

/**
	A callback for ASFileSysRec that gets the status of the specified
	file. This callback is used for asynchronous I/O. For example,
	it can indicate that an underlying file connection has been
	closed.
	@param f IN/OUT The file whose status is obtained.
	@return The file's status, which must be one of the ASFileStatusFlags
	values.
	@see ASFileRead
*/
typedef ACCBPROTO1 ASFlagBits (ACCBPROTO2 *ASFileSysGetStatusProc)(ASMDFile f);

/**
	A callback for ASFileSysRec that is used for non-local file systems.
	It returns an ASPathName on the new ASFileSys that refers to
	an image (which may be cached) of the remote file. Because of
	the possibility of cache flushing, you must hold a copy
	of the remote file's ASPathName for the duration of use
	of the local file.

	@param pathName IN/OUT The ASPathName for which an equivalent
	in <code>newFileSys</code> is obtained.
	@param newFileSys IN/OUT The file system in which an equivalent
	of <code>pathName</code> is obtained.
	@return The ASPathName (in <code>newFileSys</code>) for the specified file. It returns
	<code>NULL</code> if one can not be made.
	@see ASFileSysCreatePathNameProc
	@note Do not remove the local file copy, since the default
	file system does not know about the linkage to the remote
	file. The removal of this temporary file should be left to the file system.

	@note The ASPathName returned should be released with the
	ASFileSysReleasePath() method when it is no longer needed.
*/
typedef ACCBPROTO1 ASPathName (ACCBPROTO2 *ASFileSysAcquireFileSysPathProc)
							(ASPathName pathName, ASFileSys newFileSys);

/**
	A callback for ASFileSysRec that opens the specified file. It
	is called by ASFileSysOpenFile() and ASFileReopen(). This callback
	returns an error if the file is over 2 GB in length.

	@param pathName IN/OUT The path name for the file to open.
	@param mode IN/OUT The mode in which the file is opened, which must
	be an OR of the ASFile Open Modes.
	@param fP IN/OUT (Filled by the callback) The newly opened file.

	@return <code>0</code> if the request was successful, a non-zero
	platform-dependent error code otherwise.
	@see ASFileSysCloseProc
	@see ASFileSysOpenFile
	@see ASFileReopen
	@see ASFileClose
*/
typedef	ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysOpenProc)(ASPathName pathName, ASFileMode mode, ASMDFile *fP);

/**
	A callback for ASFileSysRec. This callback is responsible
	for closing the specified file. It is called by ASFileClose().

	@param f IN/OUT The file to close.
	@return <code>0</code> if the request was successful, a non-zero
	platform-dependent error code otherwise.
	@see ASFileSysOpenProc
	@see ASFileClose
*/
typedef	ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysCloseProc)(ASMDFile f);

/**
	A callback for ASFileSysRec that flushes data for the specified
	file. It is called by ASFileFlush().
	@param f IN/OUT The file to flush.
	@return <code>0</code> if the request was successful, a non-zero
	platform-dependent error code otherwise.
	@see ASFileSysFlushVolumeProc
	@see ASFileFlush
*/
typedef	ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysFlushProc)(ASMDFile f);

/**
	A callback for ASFileSysRec that sets the current position in
	a file (the point from which data will next be
	read). It is called by ASFileSetPos().
	@param f IN/OUT The file in which the position is set.
	@param pos IN/OUT The desired new position (specified in bytes
	from the beginning of the file).
	@return <code>0</code> if the request was successful, a non-zero
	platform-dependent error code otherwise.
	@see ASFileGetPos
	@see ASFileSetPos
*/
typedef	ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysSetPosProc)(ASMDFile f, ASFilePos pos);

/**
	A callback for ASFileSysRec that gets the current position for
	the specified file. It is called by ASFileGetPos(), and is not capable of handling file positions over 2 GB.
	@param f The file whose current position is obtained.
	@param pos (Must by filled by the callback) The current
	position.
	@return <code>0</code> if the request was successful, a non-zero
	platform-dependent error code otherwise.
	@see ASFileSysSetPosProc
	@see ASFileGetPos
	@see ASFileSetPos
*/
typedef	ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysGetPosProc)(ASMDFile f, ASFilePos *pos);

/**
	A callback for ASFileSysRec that increases or decreases the logical
	size of a file. It is called by ASFileSetEOF(). It returns an error
	if the current file position is over 2 GB.
	@param f The file to expand or shrink.
	@param pos The desired size in bytes.
	@return <code>0</code> if the request was successful, a non-zero
	platform-dependent error code otherwise.
	@see ASFileSysCanSetEofProc
	@see ASFileGetEOF
	@see ASFileSetEOF
*/
typedef	ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysSetEofProc)(ASMDFile f, ASFilePos pos);

/**
	A callback for ASFileSysRec that gets a file's current logical
	size. It is called by ASFileGetEOF(), and is not capable of handling file sizes over
	2 GB.
	@param f IN/OUT The file whose logical size is obtained.
	@param pos IN/OUT (Filled by the callback) The file's logical
	size in bytes.
	@return <code>0</code> if the request was successful, a non-zero
	platform-dependent error code otherwise.
	@see ASFileGetEOF
	@see ASFileSetEOF
*/
typedef	ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysGetEofProc)(ASMDFile f, ASFilePos *pos);

/**
	A callback for ASFileSysRec that reads data from the specified
	file. It is called by ASFileRead() and returns an error if the file size
	is over 2 GB.
	@param ptr IN/OUT (Filled by the callback) The data read from
	the file.
	@param size IN/OUT The size of each item to read.
	@param count IN/OUT The number of items to read.
	@param f IN/OUT The file from which data is read.
	@param pError IN/OUT (Filled by the callback) An error code. This
	value is filled only if an error occurs. In that case,
	it should contain an error code.
	@return The number of bytes read, or <code>0</code> if there was an error.
	@see ASFileSysWriteProc
	@see ASFileRead
*/
typedef	ACCBPROTO1 ASSize_t (ACCBPROTO2 *ASFileSysReadProc)(void *ptr, ASSize_t size, ASSize_t count, ASMDFile f, ASErrorCode *pError);

/**
	A callback for ASFileSysRec that writes data to the specified
	file.
	@param ptr IN/OUT A buffer containing data to write.
	@param size IN/OUT The size of each item to write.
	@param count IN/OUT The number of items to write.
	@param f IN/OUT The file into which data is written.
	@param pError IN/OUT (Filled by the callback) Error.
	@return The number of bytes written, or <code>0</code> if there was an error.

	@see ASFileSysReadProc
	@see ASFileWrite
*/
typedef	ACCBPROTO1 ASSize_t (ACCBPROTO2 *ASFileSysWriteProc)(void *ptr, ASSize_t size, ASSize_t count, ASMDFile f, ASErrorCode *pError);

/**
	A callback for ASFileSysRec that deletes a file. It is called
	by ASFileSysRemoveFile().
	@param pathName IN/OUT The file to delete.
	@return <code>0</code> if the request was successful, a non-zero
	platform-dependent error code otherwise.
	@see ASFileSysRemoveFile
*/
typedef	ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysRemoveProc)(ASPathName pathName);

/**
	A callback for ASFileSysRec that renames a file. It is not called
	directly by any method in the client API, but is used internally
	by the Acrobat viewer.
	@param f The file to rename.
	@param oldPath The file's old path name.
	@param newPath The file's new path name.
	@return <code>0</code> if the request was successful, a non-zero
	platform-dependent error code otherwise.
	@see ASFileSysGetNameProc
*/
typedef	ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysRenameProc)(ASMDFile* f, ASPathName oldPath, ASPathName newPath);

/**
	A callback for ASFileSysRec that tests whether two files are the
	same.
	@param f IN/OUT The ASFile of the first file to compare. In many implementations
	it may be unnecessary to use this information: <code>pathName</code>
	and <code>newPathName</code> may provide sufficient information.
	@param pathName IN/OUT The ASPathName of the first file to compare.

	@param newPathName IN/OUT The ASPathName of the second file to compare.

	@return <code>0</code> if the files are different, a non-zero value if they are the same.
*/
typedef	ACCBPROTO1 ASBool (ACCBPROTO2 *ASFileSysIsSameFileProc)(ASMDFile f, ASPathName pathName, ASPathName newPathName);

/**
	A callback for ASFileSysRec that returns a character string containing
	the file name for the specified ASPathName. The character
	string contains only the file name; it is not a complete
	path name.

	<p>This callback is not called directly from any plug-in API
	method. It is used internally by the Acrobat viewer.</p>

	@param pathName IN/OUT The ASPathName for which the file name is
	returned.
	@param name IN/OUT (Filled by the callback) A character string containing
	the file name for <code>pathName</code>.
	@param maxLength IN/OUT The maximum number of characters that <code>name</code>
	can hold.
	@return <code>0</code> if the request was successful, a non-zero
	platform-dependent error code otherwise.
	@see ASFileSysGetFileSysNameProc
*/
typedef	ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysGetNameProc)(ASPathName pathName, char *name, ASTArraySize maxLength);

/**
	A callback for ASFileSysRec that returns a unique path name suitable
	for use in creating temporary files.

	@param pathName IN/OUT If <code>pathName</code> is non-<code>NULL</code>, the temporary
	file must be stored such that a renaming of <code>pathName</code> will
	succeed. For example, the file is stored on the same volume if renaming across different
	volumes is illegal on the file system.
	@return The path name for a temporary file.
	@see ASFileSysCopyPathNameProc

	@note The ASPathName returned should be released by the
	ASFileSysReleasePath() method when it is no longer needed.
*/
typedef	ACCBPROTO1 ASPathName	(ACCBPROTO2 *ASFileSysGetTempPathNameProc)(ASPathName pathName);

/**
	A callback for ASFileSysRec that copies a path name (not the underlying
	file). It is called by ASFileSysCopyPath().

	<p>Copying a path name does not result in any file-level operations,
	and does not depend on the existence of an open
	file for the path name. </p>

	@param pathName IN/OUT The path name to copy.
	@return A new copy of the path name.
	@see ASFileSysCopyPath

	@note The ASPathName returned should be released by the
	ASFileSysReleasePath() method when it is no longer needed.
*/
typedef	ACCBPROTO1 ASPathName (ACCBPROTO2 *ASFileSysCopyPathNameProc)(ASPathName pathName);

/**
	A callback for ASFileSysRec that converts a path name to a device-
	independent path name. It is called by ASFileSysDIPathFromPath().

	@param path IN/OUT The pathname to convert to a device-independent
	path name.
	@param relativeToThisPath IN/OUT The path relative to which the
	device-independent path name is specified. Pass <code>NULL</code> if the
	device-independent path name  is an absolute path name (for example, <code>c:\\dir1\\dir2\\...dir3\\myfile.pdf </code>)
	instead of a relative path name (for example, <code>../../dir3/myfile.pdf</code>).

	@return The device-independent path name.
	@see ASFileSysDIPathFromPath

	@note The memory for the <code>char*</code> returned should be freed
	with the ASfree() method when it is no longer needed.
*/
typedef	ACCBPROTO1 char * (ACCBPROTO2 *ASFileSysDiPathFromPathProc)(
		ASPathName path,
		ASPathName relativeToThisPath
		);

/**
	A callback for ASFileSysRec that converts a device-independent
	path name to an ASPathName. It is called by ASFileSysPathFromDIPath().


	@param diPath IN/OUT A device-independent path name to convert to
	an ASPathName.
	@param relativeToThisPath IN/OUT If <code>diPath</code> is an absolute path name,
	the value of this parameter is <code>NULL</code>. If <code>diPath</code> is a relative path name,
	the parameter is the path name relative to which it is specified.
	@return The ASPathName corresponding to the specified device-independent
	path name.
	@see ASFileSysPathFromDIPath
	@see ASFileSysDIPathFromPath

	@note The ASPathName returned should be released by the
	ASFileSysReleasePath() method when it is no longer needed.
*/
typedef	ACCBPROTO1 ASPathName (ACCBPROTO2 *ASFileSysPathFromDIPathProc)(
		const char * diPath,
		ASPathName relativeToThisPath

		);
/**
	A callback for ASFileSysRec that is called by ASFileSysReleasePath().

	<p>This callback frees any memory occupied by <code>pathname</code>. It
	does not result in any file-level operations. </p>

	@param pathName The path name to release.
	@see ASFileSysReleasePath
*/
typedef	ACCBPROTO1 void (ACCBPROTO2 *ASFileSysDisposePathNameProc)(ASPathName pathName);

/**
	A callback for ASFileSysRec that gets this file system's name.

	<p>This callback is not called directly by any method in the
	client API, but is used internally by the Acrobat viewer. </p>

	@return The ASAtom containing the name of this file system.
	@see ASFileRegisterFileSys
*/
typedef	ACCBPROTO1 ASAtom (ACCBPROTO2 *ASFileSysGetFileSysNameProc)(void);

/**
	A callback for ASFileSysRec that gets the amount of free space
	on the volume containing the specified ASPathName.
	@param pathName The ASPathName for a file on the volume
	whose free space is obtained.
	@return The free space in bytes. Because the free space is returned
	as an ASInt32 object, it is limited to 4 GB.
*/
typedef	ACCBPROTO1 ASDiskSpace (ACCBPROTO2 *ASFileSysGetStorageFreeSpaceProc)(ASPathName pathName);

/**
	A callback for ASFileSysRec that flushes the volume on which the
	specified file resides. This ensures that any data written
	to the system for the volume containing <code>pathName</code> is flushed
	out to the physical volume (equivalent to the Mac OS
	FlushVol, or to the UNIX sync). Call this after you are finished
	writing a complete transaction to force a commit.

	<p>This callback is not called directly from any client API
	method, but is used internally by the Acrobat viewer. </p>

	@param pathName The path name for the file whose volume
	is flushed.
	@return <code>0</code> if the request was successful, a non-zero
	platform-dependent error code otherwise.
	@see ASFileSysFlushProc
*/
typedef	ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysFlushVolumeProc)(ASPathName pathName);

/**
	A callback for ASFileSysRec that creates an ASPathName based on
	the input type and PDFileSpec. Each ASFileSys implementation
	must publish the input types that it accepts. For example,
	the Mac OS ASFileSys may accept the type FSSpecPtr, and
	the MS-DOS ASFileSys may only accept types of <code>CString</code>.

	@param pathSpecType The type of the input PDFileSpec.
	@param pathSpec The file specification from which to create
	an ASPathName.
	@param mustBeZero Reserved for future use.
	@return The newly created path name.
	@see ASFileSysCreatePathName

	@note The ASPathName returned should be released by the
	ASFileSysReleasePath() method when it is no longer needed.
*/
typedef ACCBPROTO1 ASPathName (ACCBPROTO2 *ASFileSysCreatePathNameProc)(ASAtom pathSpecType, const void *pathSpec,
			const void *mustBeZero);


/**
	A callback for ASFileSysRec used to retrieve a full description
	of the file system object associated with the path.
	@param pathName The ASPathName associated with the object.

	@param props (Filled by the callback) A properties structure
	describing the object.
	@return <code>0</code> to denote success, an error code otherwise.
	@see ASFileSysGetItemProps
*/
typedef ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysGetItemPropsProc)
	(ASPathName pathName, ASFileSysItemProps props);


/**
	A callback for ASFileSysRec that begins the process of iterating
	through the contents of a folder.
	@param folderPath The path to the folder through which to iterate.

	@param props (Filled by the callback) A properties structure
	describing the object.
	@param itemPath (Filled by the callback) A newly allocated
	ASPathName associated with the object (which the client
	must free). It contains an absolute path.
	@return An ASFolderIterator object used for iterating through subsequent
	items. If there are no items in the folder, this callback
	returns <code>NULL</code>.
	@see ASFileSysNextFolderItemProc
	@see ASFileSysDestroyFolderIteratorProc
	@see ASFileSysFirstFolderItem
	@see ASFileSysNextFolderItem
	@see ASFileSysDestroyFolderIterator
*/
typedef ACCBPROTO1 ASFolderIterator (ACCBPROTO2 *ASFileSysFirstFolderItemProc)
	(ASPathName folderPath, ASFileSysItemProps props, ASPathName *itemPath);


/**
	A callback for ASFileSysRec used to continue the iteration
	process associated with the ASFolderIterator object. Both
	<code>itemPath</code> and <code>props</code> are optional and can be <code>NULL</code> if the caller
	is not interested in that information.
	@param folderIter An ASFolderIterator object returned
	from a previous call to ASFileSysFirstFolderItemProc().
	@param props (Filled by the callback) A properties structure
	describing the object.
	@param itemPath (Filled by the callback) A newly allocated
	ASPathName associated with the object (which the client
	must free). It contains an absolute path.
	@return <code>false</code> if no other objects were found, <code>true</code> otherwise.
	@see ASFileSysFirstFolderItemProc
	@see ASFileSysDestroyFolderIteratorProc
	@see ASFileSysFirstFolderItem
	@see ASFileSysNextFolderItem
	@see ASFileSysDestroyFolderIterator
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *ASFileSysNextFolderItemProc)
	(ASFolderIterator folderIter, ASFileSysItemProps props, ASPathName *itemPath);


/**
	A callback for ASFileSysRec used to release the resources
	associated with <code>folderIter</code>.
	@param folderIter An ASFolderIterator object returned
	from a previous call to ASFileSysFirstFolderItem().
	@see ASFileSysFirstFolderItemProc
	@see ASFileSysNextFolderItemProc
	@see ASFileSysFirstFolderItem
	@see ASFileSysNextFolderItem
	@see ASFileSysDestroyFolderIterator
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *ASFileSysDestroyFolderIteratorProc)
	(ASFolderIterator folderIter);


/**
	A callback for ASFileSysRec used to obtain the URL associated
	with the given ASPathName.
	@param path The ASPathName in question.
	@return The URL or <code>NULL</code> if it cannot be determined. It must be possible to release
	the allocated memory with ASfree().
	@see ASFileSysURLFromPath
*/
typedef ACCBPROTO1 char * (ACCBPROTO2 *ASFileSysURLFromPathProc)
	(ASPathName path);

/**
	ASFileSysSetMode() sets and gets parameters for the specified file.

	<p><b>Mode operations:</b></p>
	<TABLE rules="all" cellspacing="1">
	<TR><TH>Operation</TH><TH>Code</TH></TR>
	<TR><TD>Get the current mode</TD><TD><code>ASFileSetMode(aFile, 0, 0);</code></TD></TR>
	<TR><TD>Set the mode</TD><TD><code>ASFileSetMode( aFile, kASFileModeDoNotYieldIfBytesNotReady, kASFileModeDoNotYieldIfBytesNotReady );</code></TD></TR>
	<TR><TD>Clear the mode</TD><TD><code>ASFileSetMode( aFile, 0, kASFileModeDoNotYieldIfBytesNotReady );</code></TD></TR>
	</TABLE>

	<p><b>Setting parameters:</b></p>
	<TABLE rules="all" cellspacing="1">
	<TR><TH>Parameter</TH><TH>Effect</TH></TR>
	<TR><TD>kASFileModeDoNotYieldIfBytesNotReady</TD><TD>If set, then ASFileRead() will not perform a
               <code>fileSys->yield()</code> if <code>RaiseIfBytesNotReady</code> is <code>true</code>.
               Otherwise, it may call <code>yield</code> before raising the
               exception <code>fileErrBytesNotReady</code>.</TD></TR>
	<TR><TD>kASFileModeDisableExplicitMReadRequests</TD><TD>If set, <code>mread()</code> requests made via ASFileMReadRequest()
			   become NOPs.</TD></TR>
	<TR><TD>kASFileRaiseIfBytesNotReady</TD><TD>If set, ASFileRead() will raise <code>fileErrBytesNotReady</code>
			   when trying to read from a file with a cache for
			   which the requested bytes are not yet present.</TD></TR>
	</TABLE>

	@param asFile The file handle.
	@param modeValue The value of bits to be set or cleared.
    @param modeMask A mask indicating which bits are to be set or cleared.

*/
typedef ACCBPROTO1 ASlFileMode (ACCBPROTO2 *ASFileSysSetModeProc)
	(ASMDFile f, ASlFileMode modeValue, ASMaskBits modeMask);

/**
	A callback for ASFileSysRec used to obtain the parent of
	the input path.
	@param path The ASPathName in question.
	@return The parent path, or <code>NULL</code> if path is a root directory. It
	is the client's responsibility to free the returned ASPathName.

*/
typedef ACCBPROTO1 ASPathName (ACCBPROTO2 *ASFileSysGetParentProc)
	(ASPathName path);

/**
	A callback for ASFileSysRec used to create an empty folder
	at the specified path.
	@param path The path of the folder to create.
	@return <code>0</code> to denote success, an error code otherwise.
	@see ASFileSysRemoveFolderProc
	@see ASFileSysCreateFolder
*/
typedef ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysCreateFolderProc)
	(ASPathName path);

/**
	A callback for ASFileSysRec used to delete the folder at
	the specified path.
	@param path The path of the folder to remove.
	@return <code>0</code> to denote success, an error code otherwise.
	@see ASFileSysCreateFolderProc
	@see ASFileSysRemoveFolder
*/
typedef ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysRemoveFolderProc)
	(ASPathName path);

/**
	A callback for ASFileSysRec used to obtain a representation
	of a path that can be displayed by the user.
	@param path The ASPathName in question.
	@return The display string, or <code>NULL</code> if some error occurred. It must be possible to release
	its memory with ASfree().
	@see ASFileSysDisplayStringFromPath
*/
typedef	ACCBPROTO1 char * (ACCBPROTO2 *ASFileSysDisplayStringFromPathProc)
	(ASPathName path);

/**
	A callback for ASFileSysRec that sets the file type and creator
	for the file. This callback is currently only implemented on Mac OS. It does
	not raise an error.
	@param path Path to the file.
	@param type File type.
	@param creator File creator.
	@see ASFileSysGetTypeAndCreatorProc
	@see ASFileSysGetTypeAndCreator
	@see ASFileSysSetTypeAndCreator
*/
typedef	ACCBPROTO1 void (ACCBPROTO2 *ASFileSysSetTypeAndCreatorProc)
	(ASPathName path, ASUns32 type, ASUns32 creator);

/**
	A callback for ASFileSysRec that gets the file type and creator
	for the file. This callback is currently only implemented on Mac OS. It does
	not raise an error.
	@param path The path to the file.
	@param type (Filled by the callback) The file type.
	@param creator (Filled by the callback) The file creator.
	@see ASFileSysSetTypeAndCreatorProc
	@see ASFileSysGetTypeAndCreator
	@see ASFileSysSetTypeAndCreator
*/
typedef	ACCBPROTO1 void (ACCBPROTO2 *ASFileSysGetTypeAndCreatorProc)
	(ASPathName path, ASUns32 *type, ASUns32 *creator);

/**
	A callback for ASFileSysRec that reopens a file in the specified
	mode. ASFileReopen() calls this method if it is present. If
	this method is not present, or if it returns <code>NULL</code> and <code>error</code>
	is <code>0</code>, ASFileReopen() does a close followed by an open. If
	<code>error</code> is non-zero, ASFileReopen() ignores the return value
	and fails with that error.

	<p>On success, the old file should not need to be closed. On
	failure, the old file should remain unchanged. </p>

	@param f The file to reopen.
	@param newMode The mode for the new session.
	@param error The error code for the operation. <code>0</code> if the
	operation was successful, a non-zero error code otherwise. The error is platform and file-system specific.
	@return The newly reopened file or <code>NULL</code>.
	@see ASFileReopen
	@see ASFileSysOpenFile
*/
typedef ACCBPROTO1 ASMDFile (ACCBPROTO2 *ASFileSysReopenProc)
	(ASMDFile f, ASFileMode newMode, ASErrorCode *error);

/**
	Does a hard flush on the file.  A hard flush makes sure
	the data is flushed even if the file is remote.  This
	proc should succeed and do nothing if it is not supported.

	<p>This does not raise an error.</p>
*/
typedef ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysHardFlushProc)
	(ASMDFile f);

/**
	Returns a platform file system representation of the ASPathName
	passed according to the atom selector. It allocates memory for the
	returned structure, which the caller must release with ASfree().

	<p>This does not raise an error.</p>
*/
typedef ACCBPROTO1 void * (ACCBPROTO2 *ASFileSysGetPlatformThingProc)
	(ASPathName path, ASAtom thing);

/**
	A callback for ASFileSysRec that gets a full description of the
	file system object associated with <code>pathName</code>, returning the
	item properties in the ASCab format.

	<p>If the ASCab has no keys on entry, every known property
	is filled in. If it is not empty, only properties corresponding
	to keys in the ASCab are filled in. Keys that do not map
	to a property of the object are removed. The ASCab has the
	following potential entries: </p>

	<code>
	<p>ASBool isThere; </p>
	<p>ASInt32 type; </p>
	<p>ASBool isHidden; </p>
	<p>ASBool isReadOnly; </p>
	<p>char * creationDate; // PDF style date </p>
	<p>string char * modDate; 	// PDF style date string </p>
	<p>ASUns32 fileSizeHigh; </p>
	<p>ASUns32 fileSizeLow; </p>
	<p>ASInt32 folderSize; </p>
	<p>ASUns32 creatorCode; </p>
	<p>ASUns32 typeCode; </p>
	<p>ASUns32 versionMajor; </p>
	<p>ASUns32 versionMinor; </p>
	<p>ASBool isCheckedOut; </p>
	<p>ASBool isPublished; </p>
	</code>

	@param pathName The ASPathName associated with the object.

	@param theCab (Filled by the method) Properties describing
	the object, in cabinet format.
	@return <code>0</code> if no error was encountered; otherwise an error code is
	returned. If an error code is returned, <code>theCab</code> is not filled
	with valid values. If the path name does not point to an
	object on the file system, <code>asFileErrFNF</code> is returned and a valid
	ASCab with <code>isThere</code> is set to <code>false</code>.
	@see ASFileSysGetItemPropsAsCab
*/
typedef ACCBPROTO1 ASInt32 (ACCBPROTO2 *ASFileSysGetItemPropsAsCabProc)(ASPathName pathName, ASCab theCab);

/**
	A callback for ASFileSysRec that tests whether a specified operation
	can be performed on the file, which means that it tests whether a handler
	is defined for that operation in <code>ASFileSysPerformOpOnItemProc</code>.

	@param pathName The ASPathName of the file.
	@param op The name of the operation to test.
	@return <code>0</code> if the operation was successful,  a non-zero
	platform-dependent error code otherwise.
	@see ASFileSysPerformOpOnItemProc
	@see ASFileSysCanPerformOpOnItem
*/
typedef ACCBPROTO1 ASInt32 (ACCBPROTO2 *ASFileSysCanPerformOpOnItemProc)(ASPathName pathName, const char *op);

/**
	A callback for ASFileSysRec that performs the specified operation
	on a particular file.
	@param pathName The ASPathName of the file.
	@param op The name of the operation to perform (a file system-defined
	string).
	@param params An ASCab object containing parameters to
	pass to the operation.
	@return <code>0</code> if the operation was successful, a non-zero
	platform-dependent error code otherwise.
	@see ASFileSysCanPerformOpOnItemProc
	@see ASFileSysPerformOpOnItem
*/
typedef ACCBPROTO1 ASInt32 (ACCBPROTO2 *ASFileSysPerformOpOnItemProc)(ASPathName pathName, const char *op, ASCab params);

/**
	A callback for ASFileSysRec that acquires a platform-specific
	file system representation of the specified path, according
	to the specified type, wrapped in an allocated ASPlatformPath
	object. Use the <code>ASPlatformPath*</code> calls to get the actual
	platform object.
	@param path The ASPathName in the current file system.

	@param platformPathType The platform path type, which is one of
	the following constants:
	<ul>
	<li>FSSpec</li>
	<li>FSRef</li>
	<li>FSRefWithCFStringRefRec</li>
	<li>CFURLRefRec</li>
	</ul>
	@param platformPath (Filled by the method) The new platform path object.
	@return <code>0</code> if the operation was successful, a non-zero
	platform-dependent error code otherwise.
	@see ASFileSysAcquireFileSysPathProc
	@see ASFileSysAcquirePlatformPath
*/
typedef ACCBPROTO1 ASInt32 (ACCBPROTO2 *ASFileSysAcquirePlatformPathProc)
	(ASPathName path, ASAtom platformPathType, ASPlatformPath *platformPath);

/**
	A callback for ASFileSysRec that releases the specified platform
	path object when the client is done with it.
	@param platformPath The ASPathName of the file.
	@return <code>0</code> if the operation was successful, a non-zero
	platform-dependent error code otherwise.
	@see ASFileSysAcquirePlatformPathProc
	@see ASFileSysReleasePlatformPath
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *ASFileSysReleasePlatformPathProc)
	(ASPlatformPath platformPath);

/**
	A callback for ASFileSysRec that gets the file name for the specified
	ASPathName as an ASText object.

	@param pathName The ASPathName for which the file name
	is returned.
	@param name (Filled by the callback) An ASText object for
	the file name for <code>pathName</code>.
	@return <code>0</code> if the request was successful, a non-zero
	platform-dependent error code otherwise.
	@see ASFileSysGetFileSysNameProc
	@see ASFileSysGetNameProc
	@note This supersedes ASFileSysGetNameProc() for Acrobat 6.0.
*/
typedef	ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysGetNameAsASTextProc)
	(ASPathName pathName, ASText name);
/**
	<p>Places a representation that can be displayed to users of a path into <code>displayText</code>.</p>

	<p>This does not raise an error.</p>
*/
typedef	ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysDisplayASTextFromPathProc)
	(ASPathName path, ASText displayText);

/**
	A callback for ASFileSysRec used when a byte range has
	arrived during a file load operation.
	@param start The offset from the beginning of the file
	to the start of the byte range.
	@param length The number of bytes in the byte range.
	@param clientData A pointer to the data to pass to the callback.
*/
	typedef	ACCBPROTO1 void (ACCBPROTO2 *ASFileSysRangeArrivedProc)
			(ASInt32 start, ASInt32 length, void* clientData);

/**
	A callback for ASFileSysRec that determines whether ASFileSys
	can set the end of file marker (EOF) to a new offset for the specified file.
	@param f The file.
	@param pos The new EOF offset.
	@return <code>true</code> if the EOF can be set for the file.
	@see ASFileSysSetEofProc
	@see ASFileGetEOF
	@see ASFileSetEOF
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *ASFileSysCanSetEofProc)
	(ASMDFile f, ASFilePos pos);

/**
	A callback for ASFileSysRec that converts a path name to a device-independent
	path name, returned as an ASText object. It is called by
	ASFileSysDIPathFromPathEx().
	@param path The path name to convert to a device-independent
	path name.
	@param relativeToThisPath The path relative to which the
	device-independent path name is specified. Pass <code>NULL</code> if the
	device-independent path name
	is an absolute path name (for example, <code>c:\\dir1\\dir2\\dir3\\myfile.pdf</code>)
	instead of a relative pathname (for example, <code>../../dir3/myfile.pdf</code>).
	@param diPathText (Filled by the method) The ASText object
	to contain the device-independent path. Must be allocated
	and freed by the client.
	@return <code>0</code> if the operation was successful, a non-zero platform-independent error code otherwise.
	@see ASFileSysPathFromDIPathExProc
	@see ASFileSysDIPathFromPathEx
	@see ASFileSysPathFromDIPathEx
*/
typedef	ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysDIPathFromPathExProc)(
		ASPathName path,
		ASPathName relativeToThisPath,
		ASText diPathText
		);

/**
	A callback for ASFileSysRec that converts a device-independent
	path name from an ASText object to an ASPathName. It is called
	by ASFileSysPathFromDIPathEx().

	@param diPathText A device-independent path name to convert to
	an ASPathName, as an ASText object.
	@param relativeToThisPath If <code>diPath</code> is an absolute path name,
	this parameter's value is <code>NULL</code>. If <code>diPath</code> is a relative path name,
	its value is the path name relative to which it is specified.
	@return The ASPathName corresponding to the specified device-independent
	path name.
	@see ASFileSysDIPathFromPathExProc
	@see ASFileSysPathFromDIPathEx
	@see ASFileSysDIPathFromPathEx

	@note The ASPathName returned should be released by the
	ASFileSysReleasePath() method when it is no longer needed.
*/
typedef	ACCBPROTO1 ASPathName (ACCBPROTO2 *ASFileSysPathFromDIPathExProc)(
		ASConstText diPathText,
		ASPathName relativeToThisPath
		);

/**
	A callback for ASFileSysRec that opens the specified file. It
	is called by ASFileSysOpen64(). This callback must be used if the file
	is over 2 GB in length.
	@param pathName IN/OUT The path name for the file to open.
	@param mode IN/OUT The mode in which the file is opened. It must
	be an OR of the ASFile Open Modes.
	@param fP IN/OUT (Filled by the callback) The newly opened file.
	@return <code>0</code> if the request was successful, a non-zero
	platform-dependent error code otherwise.
	@see ASFileSysCloseProc
	@see ASFileSysOpenFile
	@see ASFileReopen
	@see ASFileClose
*/
typedef	ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysOpen64Proc)(ASPathName pathName, ASFileMode mode, ASMDFile *fP);

/**
	A callback for ASFileSysRec that returns the maximum file position that
	can be processed by this file system. This is not the maximum size file
	that can be created, but the maximum file position that can be handled
	by the arithmetic in the file system implementation. This will typically
	be <code>(2 ^ 31) - 1</code> or <code>(2 ^ 63) - 1</code>. If this entry is not present, a value of
	<code>(2 ^ 31) - 1</code> should be assumed.
	@param pos IN/OUT The maximum file position that can be handled.

	@return <code>0</code> if the request was successful, a non-zero
	platform-dependent error code otherwise.
	@see ASFileSysGetFilePosLimit
*/
typedef	ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysGetFilePositionLimitProc)(ASFilePos64 *pos);

/**
	A callback for ASFileSysRec that sets the current position in
	a file, which is the point from which data will next be
	read. It is called by ASFileSetPos() and is capable of handling file postions
	over 2 GB.
	@param f IN/OUT The file in which the position is set.
	@param pos IN/OUT The desired new position, specified in bytes
	from the beginning of the file.
	@return <code>0</code> if the request was successful, a non-zero
	platform-dependent error code otherwise.
	@see ASFileGetPos
	@see ASFileSetPos
*/
typedef	ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysSetPos64Proc)(ASMDFile f, ASFilePos64 pos);

/**
	A callback that gets the current position for the specified file. It is called
	by ASFileGetPos(), and is capable of handling file postions over 2 GB.
	@param f IN/OUT The file whose current position is obtained.
	@param pos IN/OUT (Must by filled by the callback) The current
	position.
	@return <code>0</code> if the request was successful, a non-zero
	platform-dependent error code otherwise.
	@see ASFileSysSetPosProc
	@see ASFileGetPos
	@see ASFileSetPos
*/
typedef	ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysGetPos64Proc)(ASMDFile f, ASFilePos64 *pos);

/**
	A callback for ASFileSysRec that increases or decreases the logical
	size of a file. It is called by ASFileSetEOF() and is capable of handling
	file sizes over 2 GB.
	@param f IN/OUT The file to expand or shrink.
	@param pos IN/OUT The desired size in bytes.
	@return <code>0</code> if the request was successful, a non-zero
	platform-dependent error code otherwise.
	@see ASFileGetEOF
	@see ASFileSetEOF
*/
typedef	ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysSetEof64Proc)(ASMDFile f, ASFilePos64 pos);

/**
	A callback for ASFileSysRec that gets a file's current logical
	size. It is called by ASFileGetEOF() and is capable of handling file sizes over
	2 GB.
	@param f IN/OUT The file whose logical size is obtained.
	@param pos IN/OUT (Filled by the callback) The file's logical
	size in bytes.
	@return <code>0</code> if the request was successful, a non-zero
	platform-dependent error code otherwise.
	@see ASFileGetEOF
	@see ASFileSetEOF
*/
typedef	ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysGetEof64Proc)(ASMDFile f, ASFilePos64 *pos);

/**
	A callback for ASFileSysRec that gets the Windows Explorer/Mac Finder
	representation for the specified ASPathName as an ASText object. This
	may be a localized and extension-stripped version of the filename.

	@param pathName The ASPathName for which the name
	is returned.
	@param nameForDisplay (Filled by the callback) An ASText object for
	the name for <code>pathName</code>.
	@return <code>0</code> if the request was successful, a non-zero
	platform-dependent error code otherwise.
*/
typedef	ACCBPROTO1 ASErrorCode (ACCBPROTO2 *ASFileSysGetNameForDisplayProc)
	(ASPathName pathName, ASText nameForDisplay);

/**
	A callback for ASFileSysRec that gets the amount of free space
	on the volume containing the specified ASPathName.
	It is similar to ASFileSysGetStorageFreeSpace(), except that the return
	value is not limited to 4 GB (with a 64-bit return value).
	@param pathName The ASPathName for a file on the volume
	whose free space is obtained.
	@return The free space in bytes.
*/
typedef	ACCBPROTO1 ASDiskSpace64 (ACCBPROTO2 *ASFileSysGetStorageFreeSpace64Proc)(ASPathName pathName);

/**
	A callback for <code>ASFileSysRec</code> that tests whether a file is in use by
	another process.
	@param pathName IN The ASPathName of the file to be tested.

	@return <code>0</code> if the file is not in use, a non-zero value if the file is in use.
*/
typedef	ACCBPROTO1 ASBool (ACCBPROTO2 *ASFileSysIsInUseProc)(ASPathName pathName);

/**
	A data structure containing callbacks that implement a file
	system.
	@see ASFileSys
*/
typedef struct _t_ASFileSysRec {

	/** The size of the data structure. It must be set to <code>sizeof( ASFileSysRec)</code>. */
	ASSize_t  size;

	/** Open file callback. */
	ASFileSysOpenProc					open;

	/** Close file callback. */
	ASFileSysCloseProc					close;

	/** Flush callback. */
	ASFileSysFlushProc					flush;

	/** Set position callback. */
	ASFileSysSetPosProc					setpos;

	/** Get position callback. */
	ASFileSysGetPosProc					getpos;

	/** Set a file's current logical size. */
	ASFileSysSetEofProc					seteof;

	/** Get a file's current logical size. */
	ASFileSysGetEofProc					geteof;

	/** Read file callback. */
	ASFileSysReadProc					read;

	/** Write file callback. */
	ASFileSysWriteProc					write;

	/** Remove file callback. */
	ASFileSysRemoveProc					remove;

	/** Rename file callback. */
	ASFileSysRenameProc					rename;

	/** Is the same file. */
	ASFileSysIsSameFileProc				isSameFile;

	/** Get the file name. */
	ASFileSysGetNameProc				getName;

	/** Get the temporary path name. */
	ASFileSysGetTempPathNameProc		getTempPathName;

	/** Copy the path name. */
	ASFileSysCopyPathNameProc			copyPathName;

	/** Get the device-independent path from the path. */
	ASFileSysDiPathFromPathProc			diPathFromPath;

	/** Get the path from the device-independent path. */
	ASFileSysPathFromDIPathProc			pathFromDIPath;

	/** Dispose the path name. */
	ASFileSysDisposePathNameProc		disposePathName;

	/** Get the file system name.*/
	ASFileSysGetFileSysNameProc			getFileSysName;

	/** Get the amount of free space on the volume. */
	ASFileSysGetStorageFreeSpaceProc	getStorageFreeSpace;

	/** Flush the volume. */
	ASFileSysFlushVolumeProc			flushVolume;
	/* Added for Acrobat 3.0) */

	/** Get the file flags. */
	ASFileSysGetFileFlags				getFileFlags;
	/*
	** These functions return zero on successful queue, and a non-zero
	** platform dependent error code on failure to queue.  If IODoneProc is
	** non-zero (meaning try to operate async), and Read or Write returns
	** non-zero (meaning failure to queue), IODoneProc MUST NOT BE CALLED.
	*/

	/** Asynchronous read. */
	ASFileSysAsyncReadProc				readAsync;

	/** Asynchronous write. */
	ASFileSysAsyncWriteProc				writeAsync;

	/** Asynchronous abort. */
	ASFileSysAsyncAbortProc				abortAsync;

	/** Yield callback. */
	ASFileSysYieldProc					yield;

	/** Asynchronous request for a byte range. */
	ASFileSysMReadRequestProc			mreadRequest;

	/** Retrieve the status. */
	ASFileSysGetStatusProc				getStatus;


	/** Create the path name. */
	ASFileSysCreatePathNameProc			createPathName;

	/** Acquire the path. */
	ASFileSysAcquireFileSysPathProc		acquireFileSysPath;

	/** Drop read request. */
	ASFileSysClearOutstandingMReadsProc	clearOutstandingMReads;
	/* END of Acrobat 3.0 ASFileSys definition */

	/* Added for Acrobat 5.0 */

	/** Get the file description. */
	ASFileSysGetItemPropsProc			getItemProps;

	/** Begin iterating through the folder. */
	ASFileSysFirstFolderItemProc		firstFolderItem;

	/** Get the next folder item. */
	ASFileSysNextFolderItemProc			nextFolderItem;

	/** Destroy the folder iterator. */
	ASFileSysDestroyFolderIteratorProc	destroyFolderIterator;

	/** Set the file mode. */
	ASFileSysSetModeProc				setFileMode;

	/** Get the URL from the path. */
	ASFileSysURLFromPathProc			urlFromPath;

	/** Get the parent of the input path. */
	ASFileSysGetParentProc				getParent;

	/** Create a folder. */
	ASFileSysCreateFolderProc			createFolder;

	/** Remove a folder. */
	ASFileSysRemoveFolderProc			removeFolder;

	/** Get a display string representing the path. */
	ASFileSysDisplayStringFromPathProc	displayStringFromPath;

	/** Set the type and creator. */
	ASFileSysSetTypeAndCreatorProc		setTypeAndCreator;

	/** Get the type and creator. */
	ASFileSysGetTypeAndCreatorProc		getTypeAndCreator;

	/** Reopen the file. */
	ASFileSysReopenProc					reopen;

	/** Perform a hard flush on the file. */
	ASFileSysHardFlushProc				hardFlush;

	/* Added for Acrobat 6.0 */

	/** Deprecated. Get the platform file system representation. */
	ASFileSysGetPlatformThingProc		getPlatformThing;	/* deprecated */

	/** Get the file item properties in <code>ASCab</code> format. */
	ASFileSysGetItemPropsAsCabProc		getItemPropsAsCab;

	/** Test whether an operation can be performed. */
	ASFileSysCanPerformOpOnItemProc		canPerformOpOnItem;

	/** Perform the operation. */
	ASFileSysPerformOpOnItemProc		performOpOnItem;

	/** Acquire the platform path. */
	ASFileSysAcquirePlatformPathProc	acquirePlatformPath;

	/** Release the platform path. */
	ASFileSysReleasePlatformPathProc	releasePlatformPath;

	/** Get the file name as an <code>ASText</code> object. */
	ASFileSysGetNameAsASTextProc		getNameAsASText;

	/** Get the path for display. */
	ASFileSysDisplayASTextFromPathProc	displayASTextFromPath;

	/** The byte range has arrived. */
	ASFileSysRangeArrivedProc			rangeArrived;

	/** Determine whether the <code>ASFileSys</code> can set the end of file marker (EOF) to a new offset for the specified file. */
	ASFileSysCanSetEofProc				canSetEof;

	/** Convert a path to a device-independent path. */
	ASFileSysDIPathFromPathExProc		diPathFromPathEx;

	/** Convert a device-independent path to a path. */
	ASFileSysPathFromDIPathExProc		pathFromDIPathEx;

	/** Get the maximum file position that can be processed by this file system. */
	ASFileSysGetFilePositionLimitProc	getfileposlimit;

	/** Open a file over 2 GB in length. */
	ASFileSysOpen64Proc					open64;

	/** Set the current position in a file over 2 GB in length. */
	ASFileSysSetPos64Proc				setpos64;

	/** Get the current position in a file over 2 GB in length. */
	ASFileSysGetPos64Proc				getpos64;

	/** Increase or decrease the logical size for a file over 2 GB in length. */
	ASFileSysSetEof64Proc				seteof64;

	/** Get a file's current logical size for a file over 2 GB in length. */
	ASFileSysGetEof64Proc				geteof64;

	/** Gets the Windows Explorer or Mac Finder representation of the specified <code>ASPathName</code> as an <code>ASText</code> object. */
	ASFileSysGetNameForDisplayProc		getNameForDisplay;

	/* Added for Acrobat 8.0 */

	/** Get the amount of free space on the volume. */
	ASFileSysGetStorageFreeSpace64Proc	getStorageFreeSpace64;


	/* Added for Acrobat 9.0 */

	/** Determine whether the file is in use by another process. */
	ASFileSysIsInUseProc				isInUse;

} ASFileSysRec;

/*------------------------------------------------------------------------
	Generic monitor typedefs
-------------------------------------------------------------------------*/


/**
	A callback used in ASProgressMonitor that initializes the progress
	monitor and displays it with a current value of zero. This
	method must be called first when the progress monitor is
	used.
	@param clientData IN/OUT User-supplied data that was passed in
	the call to whatever API method required the progress monitor.

	@see PMEndOperationProc
*/
typedef	ACCBPROTO1 void (ACCBPROTO2 *PMBeginOperationProc)(void *clientData);

/**
	A callback used in ASProgressMonitor that draws the progress monitor
	with its current value set to the progress monitor's duration
	(a full progress monitor), then removes the progress monitor
	from the display.
	@param clientData IN/OUT User-supplied data that was passed in
	the call to whatever API method required the progress monitor.

	@see PMBeginOperationProc
*/
typedef	ACCBPROTO1 void (ACCBPROTO2 *PMEndOperationProc)(void *clientData);

/**
	A callback used in ASProgressMonitor that sets the value that corresponds
	to a full progress monitor display. The progress monitor
	is subsequently filled in by setting its current value.
	This method must be called before you can set the progress
	monitor's current value.
	@param duration IN/OUT The maximum value the progress monitor
	will be allowed to have.
	@param clientData IN/OUT User-supplied data that was passed in
	the call to whatever API method required the progress monitor.

	@see PMGetDurationProc
*/
typedef	ACCBPROTO1 void (ACCBPROTO2 *PMSetDurationProc)(ASDuration duration, void *clientData);

/**
	A callback used in ASProgressMonitor that sets the current value
	of the progress monitor and updates the display. The allowed
	value ranges from <code>0</code> (empty) to the value passed to <code>setDuration</code>.
	For example, if the progress monitor's duration is <code>10</code>, the
	current value must be between <code>0</code> and <code>10</code>, inclusive.
	@param currValue IN/OUT The progress monitor's current value.

	@param clientData IN/OUT User-supplied data that was passed in
	the call to whatever API method required the progress monitor.

	@see PMGetCurrValueProc
*/
typedef	ACCBPROTO1 void (ACCBPROTO2 *PMSetCurrValueProc)(ASDuration currValue, void *clientData);

/**
	A callback used in ASProgressMonitor that gets the progress monitor's
	duration, set by the most recent call to the progress monitor's
	PMSetDurationProc().
	@param clientData IN/OUT User-supplied data that was passed in
	the call to whatever API method required the progress monitor.

	@return The progress monitor's maximum value.
	@see PMSetDurationProc
*/
typedef	ACCBPROTO1 ASDuration (ACCBPROTO2 *PMGetDurationProc)(void *clientData);

/**
	A callback used in ASProgressMonitor that gets the progress monitor's
	duration, set by the most recent call to the progress monitor's
	PMSetCurrValueProc().
	@param clientData IN/OUT User-supplied data that was passed in
	the call to whatever API method required the progress monitor.

	@see PMSetCurrValueProc
*/
typedef	ACCBPROTO1 ASDuration (ACCBPROTO2 *PMGetCurrValueProc)(void *clientData);

/**
	A callback within ASProgressMonitorRec that sets the text
	string that is displayed by the progress monitor.

	<p>The built-in document progress monitor (see AVAppGetDocProgressMonitor())
	makes a copy of the text. As such, it is the client's responsibility
	to destroy it. </p>

	@param text IN/OUT The string to display.
	@param clientData IN/OUT A pointer to the data associated with
	the progress monitor (which should be passed in with
	the progress monitor).
*/
typedef ACCBPROTO1 void (ACCBPROTO2 *PMSetTextProc)(ASText text, void *clientData);

/**
	<p>Replaced by ASProgressMonitor in Acrobat 5.0.</p>

	<p>A data structure containing callbacks that implement a progress
	monitor. The callbacks implement the progress monitor functions.
	A progress monitor is used to display progress during potentially
	time-consuming operations. Progress monitors are included
	as parameters in many API calls. Acrobat's built-in progress
	monitor can be obtained by calling AVAppGetDocProgressMonitor().</p>

	@see AVAppGetDocProgressMonitor
	@see PDDocCreateThumbs
	@see PDDocSave
*/
typedef struct _t_ProgressMonitor
{

	/** The size of the data structure. It must be set to <code>sizeof(ProgressMonitorRec)</code>.
	*/
	ASSize_t size;

	/** Initialize the progress monitor and display it with a current value of zero. */
	PMBeginOperationProc	beginOperation;

	/** Draw the progress monitor with its current value set to the progress monitor's duration (a full progress monitor). */
	PMEndOperationProc	endOperation;

	/** Set the progress monitor's duration. */
	PMSetDurationProc	setDuration;

	/** Set the current value of the progress monitor and update the display. */
	PMSetCurrValueProc	setCurrValue;

	/** Get the progress monitor's duration. */
	PMGetDurationProc	getDuration;

	/** Get the progress monitor's duration. */
	PMGetCurrValueProc	getCurrValue;

	/** Set the text string that is displayed by the progress monitor. */
	PMSetTextProc		setText;
}
ASProgressMonitorRec, *ASProgressMonitor;

/* Allow clients to use the older names, without the "AS" prefix.
These are present only for backward compatibility - they must
not be used for new code. */
#define ProgressMonitor ASProgressMonitor
#define ProgressMonitorRec ASProgressMonitorRec

/**
	<p>This callback replaces CancelProc().</p>

	<p>A callback to check for cancelling operations. An ASCancelProc()
	is typically passed to some method that takes a long time
	to complete. At frequent intervals, the method calls the
	ASCancelProc(). If it returns <code>true</code>, the method cancels
	its operation; if it returns <code>false</code>, it continues. </p>
	@param clientData IN/OUT User-supplied data that was passed to
	the method that uses the ASCancelProc().
	@return <code>true</code> if the processing is to be canceled, <code>false</code> otherwise.

	@see PDFLPrintCancelProc (Only available with the PDF Library SDK)

	@see AVAppGetCancelProc
	@see PDDocCreateThumbs
	@see PDDocInsertPages
*/
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *ASCancelProc)(void *clientData);

/* Define older name for backward compatibilty. Do not use this older name
in new code */
#define CancelProc ASCancelProc

#if DEBUG
	#define CHECKTYPE(type, data) ((void *)((data) == ((type)data) ? data : data))
#else	/* !DEBUG */
	#define CHECKTYPE(type, data) ((void *)data)
#endif	/* !DEBUG */

#ifdef __GNUC__
	/* No char* type checking for GCC compiler
	   char* string comparision using == results in GCC warning ("comparison with string literal results in unspecified behavior") */
	#define CHECK_CHARSTR(data) ((void *)data)
#else
	#define CHECK_CHARSTR(data) CHECKTYPE(char *, data)
#endif	/* __GNUC__ */

/* Helper defines for the mystical ASFileSysCreatePathName. */
/**
	A helper macro for the ASFileSysCreatePathName() method.
	@param asfs (May be <code>NULL</code>) The file system through which
	the ASPathName is obtained.
	@param cDIPath A C string containing the device-independent
	path for which the ASPathName is obtained.
	@param aspRelativeTo (May be <code>NULL</code>) An ASPathName against which
	<code>cDIPath</code> will be evaluated if it contains a relative
	path.
	@see ASFileSysCreatePathFromCString
	@see ASFileSysCreatePathFromFSSpec
	@see ASFileSysCreatePathWithFolderName
*/
#define ASFileSysCreatePathFromDIPath(asfs, cDIPath, aspRelativeTo)		\
		ASFileSysCreatePathName(asfs, ASAtomFromString("DIPath"),		\
		(void *)CHECK_CHARSTR(cDIPath), (void *)CHECKTYPE(ASPathName, aspRelativeTo))
/** */
#define ASFileSysCreatePathFromDIPathText(asfs, tDIPath, aspRelativeTo)		\
		ASFileSysCreatePathName(asfs, ASAtomFromString("DIPathWithASText"),		\
		(void *)CHECKTYPE(ASText, tDIPath), (void *)CHECKTYPE(ASPathName, aspRelativeTo))
/**
	Helper macro for the ASFileSysCreatePathName() method.
	@param asfs (May be <code>NULL</code>) The file system through which
	the ASPathName is obtained.
	@param aspFolder An ASPathName containing the path of the
	folder.
	@param cFileName A C string containing the name of the
	file. The returned ASPathName contains the result of appending
	<code>cFileName</code> to <code>aspFolder</code>.
	@see ASFileSysCreatePathFromCString
	@see ASFileSysCreatePathFromDIPath
	@see ASFileSysCreatePathFromFSSpec
*/
#define ASFileSysCreatePathWithFolderName(asfs, aspFolder, cFileName)	\
	ASFileSysCreatePathName(asfs, ASAtomFromString("FolderPathName"),	\
		(void *)CHECKTYPE(ASPathName, aspFolder), (void *)CHECK_CHARSTR(cFileName))
/** */
#define ASFileSysCreatePathWithFolderNameWithASText(asfs, aspFolder, tFileName)	\
	ASFileSysCreatePathName(asfs, ASAtomFromString("FolderPathNameWithASText"),	\
		(void *)CHECKTYPE(ASPathName, aspFolder), (void *)CHECKTYPE(ASText, tFileName))
/**
	Helper macro for the ASFileSysCreatePathName() method.

	@param asfs (May be <code>NULL</code>) The file system through which
	the ASPathName is obtained.
	@param cPath A C string containing the path for which
	the ASPathName is obtained.
	@see ASFileSysCreatePathFromDIPath
	@see ASFileSysCreatePathFromFSSpec
	@see ASFileSysCreatePathWithFolderName
	@note This macro uses a local variable named <code>scratchFourBytes</code>:
	(<code>void* scratchFourBytes</code>). PDF Library users need to provide
	this variable in order to utilize the macro; the variable
	must be local to the client application, not to the library.
	Any client can use this macro provided that it has code
	similar to the following, in the same source file that uses
	the macro: <code>static void* scratchFourBytes;</code>
*/
#define ASFileSysCreatePathFromCString(asfs, cPath)						\
	ASFileSysCreatePathName(asfs, ASAtomFromString("Cstring"),			\
			(void *)CHECK_CHARSTR(cPath), NULL);
#if MAC_PLATFORM
/**
	Helper macro for the ASFileSysCreatePathName() method.
	@param asfs (May be <code>NULL</code>) The file system through which
	the ASPathName is obtained.
	@param cPath The FSSpec for which the ASPathName is obtained.
	@see ASFileSysCreatePathFromCString
	@see ASFileSysCreatePathFromDIPath
	@see ASFileSysCreatePathWithFolderName
*/
#if !AS_ARCH_64BIT
#define ASFileSysCreatePathFromFSSpec(asfs, cPath)						\
	ASFileSysCreatePathName(asfs, ASAtomFromString("FSSpec"),			\
			(void *)CHECKTYPE(FSSpec *, cPath), NULL);
#endif // !AS_ARCH_64BIT
/** */
#define ASFileSysCreatePathFromFSRef(asfs, fsRef)						\
	ASFileSysCreatePathName(asfs, ASAtomFromString("FSRef"),			\
			(void *)CHECKTYPE(FSRef, fsRef), NULL);
/** */
#define ASFileSysCreatePathFromFSRefWithCFStringRef(asfs, fsRefWithCFStringRef)						\
	ASFileSysCreatePathName(asfs, ASAtomFromString("FSRefWithCFStringRef"),			\
			(void *)CHECKTYPE(FSRefWithCFStringRefRec *, fsRefWithCFStringRef), NULL);
/** */
#define ASFileSysCreatePathFromCFURLRef(asfs, cfURLRef)						\
	ASFileSysCreatePathName(asfs, ASAtomFromString("CFURLRef"),			\
			(void *)CHECKTYPE(CFURLRef, cfURLRef), NULL);
/** */
#define ASFileSysCreatePathFromPOSIXPath(asfs, posixPath)						\
	ASFileSysCreatePathName(asfs, ASAtomFromString("POSIXPath"),			\
			(void *)CHECK_CHARSTR(posixPath), NULL);
#endif

/* Definitions related to encodings and encoding conversions */


/**
	Describes the various Unicode formats you can place into
	and read out of an ASText object.
	@see ASTextFromUnicode
	@see ASTextFromSizedUnicode
	@see ASTextSetUnicode
	@see ASTextSetSizedUnicode
	@see ASTextGetUnicodeCopy
*/
enum {
	/** Always returns the bytes in big-endian order.*/
	kUTF16BigEndian,
	/** Returns the bytes in the host's native endian order, whatever is natural for an ASUns16.*/
	kUTF16HostEndian,
	/** Endian neutral.*/
	kUTF8,
	/** Always returns the bytes in big-endian order.*/
	kUTF32BigEndian,
	/** Returns the bytes in the host's native endian order, whatever is natural for an ASUns32.*/
	kUTF32HostEndian
};
typedef ASEnum16 ASUnicodeFormat;

/**
	An integer specifying the host encoding for text. On Mac OS,
	it is a script code. On Windows, it is a <code>CHARSET id</code>.
	In UNIX, Acrobat currently only supports English, so the
	only valid ASHostEncoding is <code>0</code> (Roman). See ASScript.
*/

typedef ASInt32 ASHostEncoding;

/** An ASUnicodeChar is large enough to hold any Unicode character
   (at least 21 bits wide).
*/
typedef ASUns32 ASUnicodeChar;
typedef ASUns32 ASUTF32Val;

/**
	Holds a single 16-bit value from a UTF-16 encoded Unicode
	string. It is typically used to point to the beginning of an UTF-16
	string. For example: <code>	ASUTF16Val *utf16String = ... </code>
	<p>This data type is not large enough to hold any arbitrary
	Unicode character. Use ASUnicodeChar to pass individual
	Unicode characters. </p>
	@see ASTextGetUnicode
	@see ASTextGetUnicodeCopy
*/
typedef ASUns16 ASUTF16Val;

/** An ASUTF8Val holds a single 8-bit value from a UTF-8 encoded
	Unicode string.
*/
typedef ASUns8 ASUTF8Val;

/**
	An enumeration of writing scripts. Not all of these scripts
	are supported on all platforms.
*/
enum {

	/** Roman. */
    kASRomanScript,
	/** Japanese. */
    kASJapaneseScript,
	/** Traditional Chinese. */
    kASTraditionalChineseScript,
	/** Korean. */
    kASKoreanScript,
	/** Arabic. */
    kASArabicScript,
	/** Hebrew. */
    kASHebrewScript,
	/** Greek. */
    kASGreekScript,
	/** Cyrillic. */
    kASCyrillicScript,
	/** RightLeft. */
    kASRightLeftScript,
	/** Devanagari. */
    kASDevanagariScript,
	/** Gurmukhi. */
    kASGurmukhiScript,
	/** Gujarati. */
    kASGujaratiScript,
	/** Oriya. */
    kASOriyaScript,
	/** Bengali. */
    kASBengaliScript,
	/** Tamil. */
    kASTamilScript,
	/** Telugu. */
    kASTeluguScript,
	/** Kannada. */
    kASKannadaScript,
	/** Malayalam. */
    kASMalayalamScript,
	/** Sinhalese. */
    kASSinhaleseScript,
	/** Burmese. */
    kASBurmeseScript,
	/** Khmer */
    kASKhmerScript,
	/** Thai */
    kASThaiScript,
	/** Laotian. */
    kASLaotianScript,
	/** Georgian. */
    kASGeorgianScript,
	/** Armenian. */
    kASArmenianScript,
	/** Simplified Chinese. */
    kASSimplifiedChineseScript,
	/** Tibetan. */
    kASTibetanScript,
	/** Mongolian. */
    kASMongolianScript,
	/** Ge'ez. */
    kASGeezScript,
	/** East European Roman. */
    kASEastEuropeanRomanScript,
	/** Vietnamese. */
    kASVietnameseScript,
	/** Extended Arabic. */
    kASExtendedArabicScript,
	/** Unicode. */
    kASEUnicodeScript,
	/** Unknown. */
    kASDontKnowScript = -1
};
typedef ASInt32 ASScript;

/* UUID structure */
/**
	A structure representing a universal unique identifier (UUID) for the current user or the
	current session.
*/
typedef struct
{
	/** Timestamp low field. */
	ASUns32	timeLow;
	/** Timestamp middle field. */
	ASUns16	timeMid;
	/** Timestamp middle field multiplexed with the version number.*/
	ASUns16	timeHiAndVersion;
	/** High field of the clock sequence multiplexed with the variant. */
	ASUns8	clockSeqHiAndReserved;
	/** Low field of the clock sequence. */
	ASUns8	clockSeqLow;
	/** The spatially unique node identifier. */
	ASUns8	node[6];
} ASUUID;

/**
	A constant for the maximum string length of a unique identifier
	(UUID).
	@see ASUUIDToCString
*/
#define ASUUIDMaxStringLen 40 /* leave a bit of padding just in case */

/*------------------------------------------------------------------------
	typedefs for ASDate
------------------------------------------------------------------------*/

/* ASDate
*
*/

/* string formats
   kASTimePDF = "D:20000911121643-08'00'"
   kASTimeUniversal = "2000.09.11 13:30:20 -08'00' DST"
   kASTimeUniversalH = "2000-09-11 13:30:20 -08'00' DST"
   kASTimeUTC_ASN1 = "000911203020Z"
   kASTimeGENERALIZED_ASN1 = "20000911203020Z" */

/** A constant indicating a string format for representing a date and time. */
enum {
	/** None. */
	kASTimeNone=0,
	/** PDF date format, as defined in the PDF Reference. */
	kASTimePDF,
	/** Slight variations of the time format expressed in ISO 8601. */
	kASTimeUniversal,
	/** Slight variations of the time format expressed in ISO 8601. */
	kASTimeUniversalH,
	/** UTC ASN1 format. */
	kASTimeUTC_ASN1,
	/** ASN1 format. */
	kASTimeGENERALIZED_ASN1
    };
typedef ASEnum8 ASDateTimeFormat;

/** An opaque object holding information for a particular date and time.
	All ASDate objects	are guaranteed to give accurate representation of UTC time, unadjusted for
	leap seconds. This is due to the fact that the introduction of leap seconds
	to the international calendar does not happen according to a well-defined
	rule.
	@see ASDateGetCurrentLocalTime
	@note ASDate objects are not guaranteed to represent local time accurately.
	To be exact, in Mac OS and UNIX, ASDate cannot always determine the prevailing
	daylight saving rule for the operating system's time zone. See ASDateGetCurrentLocalTime()
	for further explanation.
*/
typedef struct _t_ASDateRec *ASDate;

/**
	Represents a calendar time span used to calculate ambiguous
	time spans such as 1 year and 3 months. A calendar time
	span cannot be negative.
	@see ASCalendarTimeSpanAddWithBase
	@see ASCalendarTimeSpanCompare
	@see ASCalendarTimeSpanDiff
*/
typedef struct _t_ASCalendarTimeSpan{
	/** Year. */
	ASUns32 Year;
	/** Month. */
    ASUns32 Month;
	/** Day. */
    ASUns32 Day;
	/** Hour. */
    ASUns32 Hour;
	/** Minute. */
    ASUns32 Minute;
	/** Second. */
    ASUns32 Second;
} ASCalendarTimeSpanRec, *ASCalendarTimeSpan;

/**
	An ASTimeSpan represents an exact time span, measured in seconds. The internal representation uses
	64-bit signed integers to avoid the year 2037 problem. Negative timespans are allowed.
*/
typedef struct _t_ASTimeSpanRec *ASTimeSpan;

#ifdef __cplusplus
}
#endif

#if defined(IA64HPUX)
#if defined(_open64_)
#define open64 _open64_
#undef _open64_
#endif
#endif

#endif /* _H_ASExpT */
