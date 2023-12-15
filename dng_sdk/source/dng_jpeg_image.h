/*****************************************************************************/
// Copyright 2011-2022 Adobe Systems Incorporated
// All Rights Reserved.
//
// NOTICE:	Adobe permits you to use, modify, and distribute this file in
// accordance with the terms of the Adobe license agreement accompanying it.
/*****************************************************************************/

#ifndef __dng_jpeg_image__
#define __dng_jpeg_image__

/*****************************************************************************/

#include "dng_auto_ptr.h"
#include "dng_fingerprint.h"
#include "dng_memory.h"
#include "dng_point.h"
#include "dng_tag_values.h"

#include <memory>
#include <vector>

/*****************************************************************************/

class dng_lossy_compressed_image
	{
	
	public:
	
		dng_point fImageSize;
		
		dng_point fTileSize;
		
		bool fUsesStrips = false;

		uint32 fCompressionCode = 0;

		std::vector<std::shared_ptr<dng_memory_block> > fData;
		
		uint32 fRowInterleaveFactor    = 1;
		uint32 fColumnInterleaveFactor = 1;
		
	public:

		virtual ~dng_lossy_compressed_image ();
	
		uint32 TilesAcross () const
			{
			if (fTileSize.h)
				{
				return (fImageSize.h + fTileSize.h - 1) / fTileSize.h;
				}
			else
				{
				return 0;
				}
			}
		
		uint32 TilesDown () const
			{
			if (fTileSize.v)
				{
				return (fImageSize.v + fTileSize.v - 1) / fTileSize.v;
				}
			else
				{
				return 0;
				}
			}
			
		uint32 TileCount () const
			{
			return TilesAcross () * TilesDown ();
			}
		
		uint64 NonHeaderSize () const;

		dng_fingerprint FindDigest (dng_host &host) const;

	protected:

		virtual void DoFindDigest (dng_host & /* host */,
								   std::vector<dng_fingerprint> & /* digests */) const
			{

			}

		virtual void EncodeTiles (dng_host &host,
								  dng_image_writer &writer,
								  const dng_image &image,
								  const dng_ifd &ifd);
			
	};

/*****************************************************************************/

class dng_jpeg_image : public dng_lossy_compressed_image
	{
	
	public:
	
		AutoPtr<dng_memory_block> fJPEGTables;
		
	public:

		dng_jpeg_image ();
	
		void Encode (dng_host &host,
					 const dng_negative &negative,
					 dng_image_writer &writer,
					 const dng_image &image);

	protected:
			
		void DoFindDigest (dng_host &host,
						   std::vector<dng_fingerprint> &digests) const override;
			
	};

/*****************************************************************************/

#if qDNGSupportJXL

class dng_jxl_image : public dng_lossy_compressed_image
	{
	
	public:

		dng_jxl_image ();
	
		void Encode (dng_host &host,
					 const dng_negative &negative,
					 dng_image_writer &writer,
					 const dng_image &image,
					 uint32 newSubFileType = sfMainImage,
					 bool isProxy = true);

	};

#endif	// qDNGSupportJXL

/*****************************************************************************/

dng_lossy_compressed_image * KeepLossyCompressedImage (dng_host &host,
													   const dng_ifd &ifd);

/*****************************************************************************/

#endif	// __dng_jpeg_image__
	
/*****************************************************************************/
