/*
 *	Default settings for configuration flags
 */

/* Has Asian font support (CJK meant "Chinese/Japanese/Korean") */
#ifndef CJK
#define CJK 1				/* -- always set true */
#endif

#ifndef ENCRYPTION_SUPPORTED
#define ENCRYPTION_SUPPORTED 1		/* -- only false for Shell.h */
#endif

#ifndef MUST_VERIFY_PI_PRIVILEGE
#define MUST_VERIFY_PI_PRIVILEGE 0	/* -- only true for Reader.h */
#endif

#ifndef MUST_VERIFY_PI_TRUSTED
#define MUST_VERIFY_PI_TRUSTED 0	/* -- always set false */
#endif

#ifndef MUST_VERIFY_PI_TRUSTED_405
#define MUST_VERIFY_PI_TRUSTED_405 1	/* No references in PDFLib */
#endif

#ifndef NEEDS_EXTENSION_ENABLER
#define NEEDS_EXTENSION_ENABLER 0	/* in ASExtend.c -- always set false */
#endif

#ifndef PDFEDIT_IN_CORE
#define PDFEDIT_IN_CORE 1		
#endif

#ifndef PDMLIB_CAN_WRITE
#define PDMLIB_CAN_WRITE 1		/* No references in PDFLib -- only in PDMLib.h */
#endif

#ifndef PDSEDIT_IN_CORE
#define PDSEDIT_IN_CORE 1		 
#endif

#ifndef TEXTEXTR
#define TEXTEXTR 1			/* No references in PDFLib */
#endif

#ifndef TOOLKIT
#define TOOLKIT 1			
#endif

#ifndef USE_ADM_DIALOGS
#define USE_ADM_DIALOGS 0		/* No references in PDFLib */
#endif

#ifndef VIEW_LIBRARY
#define VIEW_LIBRARY 1			/* No references in PDFLib */
#endif

/*
	These variables default TRUE
 */

#ifndef CAN_COPY
#define CAN_COPY 1			
#endif

#ifndef CAN_CREATE_NOTEFILE
#define CAN_CREATE_NOTEFILE 0		
#endif

#ifndef CAN_CREATE_THUMBS
#define CAN_CREATE_THUMBS 1		
#endif

#ifndef CAN_CROP_PAGES
#define CAN_CROP_PAGES 1		
#endif

#ifndef CAN_DELETE_PAGES
#define CAN_DELETE_PAGES 1		
#endif

#ifndef CAN_EDIT
#define CAN_EDIT 1			 
#endif

#ifndef CAN_EDIT_ANNOTS
#define CAN_EDIT_ANNOTS 1		 
#endif

#ifndef CAN_EDIT_ASSETS
#define CAN_EDIT_ASSETS 1		/* No references in PDFLib (new to PDFLib6) */
#endif

#ifndef CAN_EDIT_BOOKMARKS
#define CAN_EDIT_BOOKMARKS 1		 
#endif

#ifndef CAN_EDIT_NAMES
#define CAN_EDIT_NAMES 1		/* No references in PDFLib */
#endif

#ifndef CAN_ENUM_OBJECTS
#define CAN_ENUM_OBJECTS 1		 
#endif

#ifndef CAN_EXPORT_HFTS
#define CAN_EXPORT_HFTS 1		
#endif

#ifndef CAN_FIND_NOTES
#define CAN_FIND_NOTES 1		/* No references in PDFLib */
#endif

#ifndef CAN_INSERT_PAGES
#define CAN_INSERT_PAGES 1		
#endif

#ifndef CAN_LOAD_PLUGINS
#define CAN_LOAD_PLUGINS 1		 
#endif

#ifndef CAN_MOVE_PAGES
#define CAN_MOVE_PAGES 1		 
#endif

#ifndef CAN_PRINT
#define CAN_PRINT 1			 
#endif

#ifndef CAN_PRINTPS
#define CAN_PRINTPS 1			
#endif

#ifndef CAN_PRINT_FORM_FIELD_ONLY
#define CAN_PRINT_FORM_FIELD_ONLY 1	/* (new to PDFLib6) */
#endif

#ifndef CAN_PRINT_TILED
#define CAN_PRINT_TILED 1		/* No references in PDFLib (new to PDFLib6) */
#endif

#ifndef CAN_REPLACE_PAGES
#define CAN_REPLACE_PAGES 1		 
#endif

#ifndef CAN_ROTATE_PAGES
#define CAN_ROTATE_PAGES 1		 
#endif

#ifndef CAN_SAVE
#define CAN_SAVE 1		 
#endif

#ifndef CAN_SELECT_BOOKMARKS
#define CAN_SELECT_BOOKMARKS 1		/* No references in PDFLib */
#endif

#ifndef CAN_SELECT_GRAPHICS
#define CAN_SELECT_GRAPHICS 1		/* No references in PDFLib */
#endif

#ifndef CAN_SELECT_IMAGES
#define CAN_SELECT_IMAGES 1		/* No references in PDFLib (new to PDFLib6) */
#endif

#ifndef CAN_SELECT_LINKS
#define CAN_SELECT_LINKS 1		/* No references in PDFLib */
#endif

#ifndef CAN_SELECT_NAMES
#define CAN_SELECT_NAMES 1		/* No references in PDFLib */
#endif

#ifndef CAN_SELECT_NOTES
#define CAN_SELECT_NOTES 1		/* No references in PDFLib */
#endif

#ifndef CAN_SELECT_TEXT
#define CAN_SELECT_TEXT 1		 
#endif

#ifndef CAN_SELECT_THUMBS
#define CAN_SELECT_THUMBS 1		/* No references in PDFLib */
#endif

#ifndef CAN_USE_LONG_MENUS
#define CAN_USE_LONG_MENUS 1		/* Only reference is in Environment.h */
#endif

#ifndef HAS_32BIT_ATOMS
#define HAS_32BIT_ATOMS 0		 
#endif

#ifndef HAS_ACTION_PROPS
#define HAS_ACTION_PROPS 1		/* No references in PDFLib */
#endif

#ifndef HAS_ADM
#define HAS_ADM 1			 
#endif

#ifndef HAS_ANNOTS
#define HAS_ANNOTS 1			/* No references in PDFLib */
#endif

#ifndef HAS_APPLE_EVENTS
#define HAS_APPLE_EVENTS 1		/* No references in PDFLib */
#endif

#ifndef HAS_ASSETS
#define HAS_ASSETS 1			/* No references in PDFLib (new to PDFLib6) */
#endif

#ifndef HAS_BOOKMARKS
#define HAS_BOOKMARKS 1			
#endif

#ifndef HAS_CALCOLOR
#define HAS_CALCOLOR 1			 
#endif

#ifndef HAS_COLOR_PROFILES
#define HAS_COLOR_PROFILES 1		/* No references in PDFLib (new to PDFLib6) */
#endif

#ifndef HAS_COOLTYPE
#define HAS_COOLTYPE 1			/* -- only false for Shell.h */
#endif

#ifndef HAS_CROP_TOOL
#define HAS_CROP_TOOL 1			/* No references in PDFLib */
#endif

#ifndef HAS_DELIVERCFF
#define HAS_DELIVERCFF 1		/* Setting overridden in PDFont.cpp (only use) */
#endif

#ifndef HAS_DISPLAYLIST
#define HAS_DISPLAYLIST 1		 
#endif

#ifndef HAS_DOCINFO
#define HAS_DOCINFO 1			/* No references in PDFLib -- always set true */
#endif

#ifndef HAS_DOC_PROPS_PANEL
#define HAS_DOC_PROPS_PANEL 1		/* No references in PDFLib (new to PDFLib6) */
#endif

#ifndef HAS_ENCODE_FILTERS
#define HAS_ENCODE_FILTERS 1		 
#endif

#ifndef HAS_ERROR_STRINGS
#define HAS_ERROR_STRINGS 1		/* No references in PDFLib */
#endif

#ifndef HAS_EXPANDCFF
#define HAS_EXPANDCFF 1			/* Setting overridden in PDFont.cpp (only use) */
#endif

#ifndef HAS_FIND
#define HAS_FIND 1			 
#endif

#ifndef HAS_FONTS
#define HAS_FONTS 1			/* Defaults to true in only use */
#endif

#ifndef HAS_FONTSERVER
#define HAS_FONTSERVER 1		/* Only reference in PDGlobal.cpp */
#endif

#ifndef HAS_FORMS
#define HAS_FORMS 1			/* No references in PDFLib */
#endif

#ifndef HAS_FULL_SCREEN
#define HAS_FULL_SCREEN 1		/* No references in PDFLib */
#endif

#ifndef HAS_JBIG2OPTIMIZER
#define HAS_JBIG2OPTIMIZER 1		/* (new to PDFLib6) */
#endif

#ifndef HAS_LINEARIZER
#define HAS_LINEARIZER 1		
#endif

#ifndef HAS_LINKS
#define HAS_LINKS 1			/* No references in PDFLib */
#endif

#ifndef HAS_MENUBAR
#define HAS_MENUBAR 1			/* No references in PDFLib */
#endif

#ifndef HAS_MENUS
#define HAS_MENUS 1			/* No references in PDFLib */
#endif

#ifndef HAS_NAMES
#define HAS_NAMES 1			/* No references in PDFLib */
#endif

#ifndef HAS_NOTES
#define HAS_NOTES 1			/* No references in PDFLib */
#endif

#ifndef HAS_NOTIFICATION
#define HAS_NOTIFICATION 1		 
#endif

#ifndef HAS_PAGECACHE
#define HAS_PAGECACHE 1			
#endif

#ifndef HAS_PAGEPDECONTENT_HFT
#define HAS_PAGEPDECONTENT_HFT 1	/* No references in PDFLib */
#endif

#ifndef HAS_PAGE_PARSER
#define HAS_PAGE_PARSER 1		/* No references in PDFLib */
#endif

#ifndef HAS_PDDOCINPDCORE
#define HAS_PDDOCINPDCORE 1		 
#endif

#ifndef HAS_PDFEDIT_READ_PROCS
#define HAS_PDFEDIT_READ_PROCS 1	 
#endif

#ifndef HAS_PDFEDIT_WRITE_PROCS
#define HAS_PDFEDIT_WRITE_PROCS 1	 
#endif

#ifndef HAS_PDFONTINPDCORE
#define HAS_PDFONTINPDCORE 1		 
#endif

#ifndef HAS_PDPAGEINPDCORE
#define HAS_PDPAGEINPDCORE 1		 
#endif

#ifndef HAS_PDSEDIT_READ_PROCS
#define HAS_PDSEDIT_READ_PROCS 1	 
#endif

#ifndef HAS_PDSEDIT_WRITE_PROCS
#define HAS_PDSEDIT_WRITE_PROCS 1	
#endif

#ifndef HAS_PREFS_PANEL
#define HAS_PREFS_PANEL 1		/* No references in PDFLib*/
#endif

#ifndef HAS_RASTERIZER
#define HAS_RASTERIZER 1		
#endif

#ifndef HAS_SELECTIONS
#define HAS_SELECTIONS 1		/* No references in PDFLib */
#endif

#ifndef HAS_SMARTGUY
#define HAS_SMARTGUY 1			/* -- always set true */
#endif

#ifndef HAS_SYSTEM_CALCOLOR
#define HAS_SYSTEM_CALCOLOR 1		/* No references in PDFLib */
#endif

#ifndef HAS_THREADS
#define HAS_THREADS 1			/* -- only false for Shell.h*/
#endif 

#ifndef HAS_THUMBS
#define HAS_THUMBS 1			 
#endif

#ifndef HAS_TOOLBAR
#define HAS_TOOLBAR 1			/* No references in PDFLib */
#endif

#ifndef HAS_TOOLS
#define HAS_TOOLS 1			/* No references in PDFLib */
#endif

#ifndef HAS_UCS_DECOMPOSE
#define HAS_UCS_DECOMPOSE 1		/* -- only false for PDFLibLite.h */
#endif

#ifndef HAS_XAP_METADATA    /* True when the configuration uses the Adobe XMP Toolkit library */
#define HAS_XAP_METADATA 1		
#endif

#ifndef HAS_FILEINFO  /* True when the configuration uses the FileInfo dialog library.
					   * Since this is an interactive feature, it should only be turned
					   * on in application configurations, never in PDFLib.
					   * If this is true, HAS_XAP_METADATA must also be true. */
#define HAS_FILEINFO 0 
#endif 
