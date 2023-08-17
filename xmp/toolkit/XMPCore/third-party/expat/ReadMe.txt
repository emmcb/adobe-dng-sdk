The XMP Toolkit needs an external XML parser. The source from Adobe is
written to use Expat, although adapters for other parsers can easily
be written. The most recent version of Expat used with XMP is 2.1.0.
To use Expat:

1. Obtain a copy of the Expat distribution:

   git clone https://github.com/libexpat/libexpat.git

2. Copy all the files in libexpat/expat/lib into this path:

   xmp/toolkit/XMPCore/third-party/expat/public/lib/

