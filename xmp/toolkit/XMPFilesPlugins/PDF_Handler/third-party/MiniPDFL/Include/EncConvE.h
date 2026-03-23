/* EncConvE.h -- error codes for the PDFCMap system.
** Raised using Raise(PDFCMapError(pdErrSomeError)).
** These are only valid if ErrGetSystem(err) == ErrSysPDDoc (see ASError.h)
** Use the ErrGetCode macro to get these values from an Int32.
** May be passed to the PDErrorMonitor.
** For all errors which contain '%s' in the string, a value will be passed in the
** PDErrorInfo->string field.
**
** NOTE: Do not remove or reorder any error codes. They are part of the API. To
** obsolete an error, rename it by appending OBSOLETE.
** Add all new errors at the end.
** Changing error names will break plug-ins when they are recompiled.
** You can change the text of errors.
*/

DefineErr(pdErrBadCMap,					"The encoding (CMap) specified by a font is corrupted.")
DefineErr(pdErrCMapNotFound,			"The encoding (CMap) specified by a font is missing.")


