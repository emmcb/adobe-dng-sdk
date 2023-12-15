/*****************************************************************************/
// Copyright 2011-2022 Adobe Systems Incorporated
// All Rights Reserved.
//
// NOTICE:	Adobe permits you to use, modify, and distribute this file in
// accordance with the terms of the Adobe license agreement accompanying it.
/*****************************************************************************/

#include "dng_jpeg_image.h"

#include "dng_abort_sniffer.h"
#include "dng_area_task.h"
#include "dng_assertions.h"
#include "dng_host.h"
#include "dng_ifd.h"
#include "dng_jxl.h"
#include "dng_image.h"
#include "dng_image_writer.h"
#include "dng_memory_stream.h"
#include "dng_safe_arithmetic.h"
#include "dng_uncopyable.h"

#include <atomic>

/*****************************************************************************/

class dng_lossy_image_encode_task : public dng_area_task,
									private dng_uncopyable
	{
	
	private:
	
		dng_host &fHost;
		
		dng_image_writer &fWriter;
		
		const dng_image &fImage;
	
		dng_lossy_compressed_image &fLossyImage;
		
		uint32 fTileCount;
		
		const dng_ifd &fIFD;
				
		std::atomic_uint fNextTileIndex;
		
	public:
	
		dng_lossy_image_encode_task (dng_host &host,
									 dng_image_writer &writer,
									 const dng_image &image,
									 dng_lossy_compressed_image &lossyImage,
									 uint32 tileCount,
									 const dng_ifd &ifd)

			:	dng_area_task ("dng_lossy_image_encode_task")
		
			,	fHost			  (host)
			,	fWriter			  (writer)
			,	fImage			  (image)
			,	fLossyImage		  (lossyImage)
			,	fTileCount		  (tileCount)
			,	fIFD			  (ifd)
			,	fNextTileIndex	  (0)
			
			{
			
			fMinTaskArea = 16 * 16;
			fUnitCell	 = dng_point (16, 16);
			fMaxTileSize = dng_point (16, 16);
			
			}
	
		void Process (uint32 /* threadIndex */,
					  const dng_rect & /* tile */,
					  dng_abort_sniffer *sniffer)
			{
			
			AutoPtr<dng_memory_block> compressedBuffer;
			AutoPtr<dng_memory_block> uncompressedBuffer;
			AutoPtr<dng_memory_block> subTileBlockBuffer;
			AutoPtr<dng_memory_block> tempBuffer;
			
			uint32 uncompressedSize = SafeUint32Mult (fIFD.fTileLength, 
													  fIFD.fTileWidth, 
													  fIFD.fSamplesPerPixel,
													  fImage.PixelSize ());
			
			uncompressedBuffer.Reset (fHost.Allocate (uncompressedSize));
			
			uint32 tilesAcross = fIFD.TilesAcross ();
	
			while (true)
				{

				// Note: fNextTileIndex is atomic
				
				uint32 tileIndex = fNextTileIndex++;

				if (tileIndex >= fTileCount)
					{
					return;
					}

				dng_abort_sniffer::SniffForAbort (sniffer);
				
				uint32 rowIndex = tileIndex / tilesAcross;
				uint32 colIndex = tileIndex % tilesAcross;
				
				dng_rect tileArea = fIFD.TileArea (rowIndex, colIndex);
				
				dng_memory_stream stream (fHost.Allocator ());
				
				fWriter.WriteTile (fHost,
								   fIFD,
								   stream,
								   fImage,
								   tileArea,
								   1,
								   compressedBuffer,
								   uncompressedBuffer,
								   subTileBlockBuffer,
								   tempBuffer,
								   true);
								  
				fLossyImage.fData [tileIndex].reset
					(stream.AsMemoryBlock (fHost.Allocator ()));
					
				}
			
			}
		
	};

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

static void CommonConfigureIFD (dng_ifd &ifd,
								const dng_negative &negative,
								const dng_image &image,
								const uint32 compressionCode,
								const dng_point &tileSize,
								dng_point &outTileSize,
								dng_point &outImageSize)
	{
	
	DNG_REQUIRE (image.PixelType () == ttByte  ||
				 image.PixelType () == ttShort ||
				 image.PixelType () == ttFloat,
				 "Unsupported pixel type");

	uint32 bitDepth = 8;

	if (image.PixelType () == ttShort)
		bitDepth = 16;

	else if (image.PixelType () == ttFloat)
		bitDepth = (negative.RawFloatBitDepth () == 16) ? 16 : 32;

	dng_point imageSize = image.Bounds ().Size ();
	
	ifd.fImageWidth				   = imageSize.h;
	ifd.fImageLength			   = imageSize.v;
	
	ifd.fSamplesPerPixel		   = image.Planes ();
	
	ifd.fBitsPerSample [0]		   = bitDepth;
	ifd.fBitsPerSample [1]		   = bitDepth;
	ifd.fBitsPerSample [2]		   = bitDepth;
	ifd.fBitsPerSample [3]		   = bitDepth;
	
	ifd.fPhotometricInterpretation = piLinearRaw;
	
	ifd.fCompression			   = compressionCode;

	ifd.FindTileSize (SafeUint32Mult (uint32 (tileSize.h),
									  uint32 (tileSize.v),
									  ifd.fSamplesPerPixel));

	outTileSize.h = ifd.fTileWidth;
	outTileSize.v = ifd.fTileLength;

	outImageSize  = imageSize;
	
	}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

dng_jpeg_image::dng_jpeg_image ()
	{
	
	fCompressionCode = ccLossyJPEG;
	
	}

/*****************************************************************************/

void dng_jpeg_image::Encode (dng_host &host,
							 const dng_negative &negative,
							 dng_image_writer &writer,
							 const dng_image &image)
	{
	
	#if qDNGValidate
	dng_timer timer ("Encode JPEG Proxy time");
	#endif
	
	DNG_REQUIRE (image.PixelType () == ttByte,
				 "Cannot JPEG encode non-byte image");
	
	dng_ifd ifd;

	CommonConfigureIFD (ifd,
						negative,
						image,
						ccLossyJPEG,
						dng_point (512, 512),
						fTileSize,
						fImageSize);

	// Choose JPEG encode quality. Need a higher quality for raw proxies than
	// non-raw proxies, since users often perform much greater color changes.
	// Also, if we are targeting a "large" size proxy (larger than 5 MP), or
	// this is a full size proxy, then use a higher quality.
	
	bool useHigherQuality = (uint64) ifd.fImageWidth *
							(uint64) ifd.fImageLength > 5000000 ||
							image.Bounds ().Size () == negative.OriginalDefaultFinalSize ();
	
	if (negative.IsSceneReferred ())
		{
		ifd.fCompressionQuality = useHigherQuality ? 11 : 10;
		}
	
	else
		{
		ifd.fCompressionQuality = useHigherQuality ? 10 : 8;
		}

	EncodeTiles (host,
				 writer,
				 image,
				 ifd);
	
	}
			
/*****************************************************************************/

void dng_jpeg_image::DoFindDigest (dng_host & /* host */,
								   std::vector<dng_fingerprint> &digests) const
	{
	
	// Compute digest of JPEG tables, if any, and add to the given list.
		
	if (fJPEGTables.Get ())
		{
		
		dng_md5_printer printer;
		
		printer.Process (fJPEGTables->Buffer	  (),
						 fJPEGTables->LogicalSize ());
						 
		digests.push_back (printer.Result ());
		
		}
	
	}
			
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

dng_lossy_compressed_image::~dng_lossy_compressed_image ()
	{
	
	}

/*****************************************************************************/

uint64 dng_lossy_compressed_image::NonHeaderSize () const
	{
	
	uint64 size = 0;
	
	for (size_t index = 0; index < fData.size (); index++)
		{
		
		size += RoundUp2 (fData [index]->LogicalSize ());
		
		}
		
	return size;
	
	}

/*****************************************************************************/

dng_fingerprint dng_lossy_compressed_image::FindDigest (dng_host &host) const
	{
	
	const uint32 tileCount = TileCount ();

	std::vector<dng_fingerprint> digests (tileCount);
	
	// Compute digest of each compressed tile.

	dng_range_parallel_task::Do
		(host,
		 dng_range_parallel_task::info (int32 (0),
										int32 (tileCount)),
		 "dng_lossy_compressed_image::FindDigest",
		 [this, &digests] (const dng_range_parallel_task::range &ra)
		 {
			 
		 for (int32 i = ra.fBegin; i < ra.fEnd; i++)
			 {
				 
			 dng_md5_printer printer;
				
			 printer.Process (fData [i]->Buffer	     (),
							  fData [i]->LogicalSize ());
								 
			 digests [i] = printer.Result ();
				 
			 }
			 
		 });

	// Subclasses can add extra data here.

	DoFindDigest (host, digests);

	// Combine digests into a single digest.
	
		{
		
		dng_md5_printer printer;
		
		for (const auto &digest : digests)
			printer.Process (digest.data,
							 uint32 (sizeof (digest.data)));
			
		return printer.Result ();
		
		}
	
	}

/*****************************************************************************/

void dng_lossy_compressed_image::EncodeTiles (dng_host &host,
											  dng_image_writer &writer,
											  const dng_image &image,
											  const dng_ifd &ifd)
	{
	
	uint32 tilesAcross = ifd.TilesAcross ();
	uint32 tilesDown   = ifd.TilesDown	 ();
	
	uint32 tileCount = tilesAcross * tilesDown;
	
	fData.resize (tileCount);
	
	uint32 threadCount = Min_uint32 (tileCount,
									 host.PerformAreaTaskThreads ());
										 
	dng_lossy_image_encode_task task (host,
									  writer,
									  image,
									  *this,
									  tileCount,
									  ifd);
									  
	host.PerformAreaTask (task,
						  dng_rect (0, 0, 16, 16 * threadCount));
		
	}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

#if qDNGSupportJXL

/*****************************************************************************/

dng_jxl_image::dng_jxl_image ()
	{
	
	fCompressionCode = ccJXL;
	
	}

/*****************************************************************************/

void dng_jxl_image::Encode (dng_host &host,
							const dng_negative &negative,
							dng_image_writer &writer,
							const dng_image &image,
							uint32 newSubFileType,
							bool isProxy)
	{
	
	#if qDNGValidate
	dng_timer timer ("Encode JXL time");
	#endif

	dng_ifd ifd;

	CommonConfigureIFD (ifd,
						negative,
						image,
						ccJXL,
						dng_point (1024, 1024),
						fTileSize,
						fImageSize);

	// Choose JXL encode settings.

	AutoPtr<dng_jxl_encode_settings> settings
		(host.MakeJXLEncodeSettings (negative,
									 image,
									 newSubFileType,
									 isProxy));
	
	#if qDNGValidate
	printf ("JXL using distance: %.2f and effort %d\n",
			float (settings->Distance ()),
			int (settings->Effort ()));
	#endif

	ifd.fJXLEncodeSettings.reset (settings.Release ());
	
	EncodeTiles (host,
				 writer,
				 image,
				 ifd);
	
	}

/*****************************************************************************/

#endif	// qDNGSupportJXL

/*****************************************************************************/

dng_lossy_compressed_image * KeepLossyCompressedImage (dng_host &host,
													   const dng_ifd &ifd)
	{
	
	AutoPtr<dng_lossy_compressed_image> lossyImage;
	
	if (host.SaveDNGVersion () &&
	   !host.PreferredSize  () &&
	   !host.ForPreview     ())
		{
		
		// See if we should grab the compressed JPEG data.
		
		if (host.SaveDNGVersion () >= MinBackwardVersionForCompression (ccLossyJPEG) &&
			ifd.IsBaselineJPEG ())
			{
			
			lossyImage.Reset (new dng_jpeg_image);
			
			}
		
		#if qDNGSupportJXL

		if (host.SaveDNGVersion () >= MinBackwardVersionForCompression (ccJXL) &&
			ifd.fCompression == ccJXL)
			{
			
			lossyImage.Reset (new dng_lossy_compressed_image);
			
			}
			
		#endif

		}
		
	if (lossyImage.Get ())
		{
		
		lossyImage->fCompressionCode = ifd.fCompression;
		
		lossyImage->fRowInterleaveFactor    = ifd.fRowInterleaveFactor;
		lossyImage->fColumnInterleaveFactor = ifd.fColumnInterleaveFactor;
		
		}
		
	return lossyImage.Release ();
		
	}

/*****************************************************************************/
