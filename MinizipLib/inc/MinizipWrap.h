#pragma once
/*****************************************************************************
* Name     : MinizipWrap.h
* Creator  : H.Z.XIN
* Date     : 2018-04-02
* Function : minizip wrap
* Parameter:  π”√wchar_t
* Return(s):
* Notices  :
------------------------------------------------------------------------------
Change Log:
2018-04-02        H.Z.XIN        Create
*****************************************************************************/

#include <tchar.h>
#include "MultiSZ.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char BYTE;
typedef unsigned long ULONG;


//------------------------------------------------------------------------------
// Description: Compress files to a ZIP file.
// Parameter: lpszSourceFiles Source files, supporting wildcards.
// Parameter: lpszDestFile The ZIP file path.
// Parameter: compressLevel  1~9, -1 Default
// Parameter: pszPassword 
// Parameter: bUtf8>0 If using UTF-8 to encode the file name.
// Return Value: 0 -- success
//               -1 -- fail
//------------------------------------------------------------------------------
int ZipCompress(const TCHAR* lpszSourceFiles, 
	const TCHAR* lpszDestFile, 
	int compressLevel/*=-1*/,
	const char* pszPassword/*=NULL*/,
	char bUtf8 /*= 0*/);


int ZipCompressMulti(TMultiSZ srcFileOrDirs,
	const TCHAR* lpszDestFile,
	int compressLevel/*=-1*/,
	const char* pszPassword/*=NULL*/,
	char bUtf8/* = 0*/);


//------------------------------------------------------------------------------
// Description: Extract files from a ZIP file.
// Parameter: lpszSourceFile Source ZIP file.
// Parameter: lpszDestFolder The folder to output files. The parent of the
//                           specified folder MUST exist.
// Parameter: pszPassword 
// Return Value: 0 -- success
//               -1 -- fail
//------------------------------------------------------------------------------
int ZipExtract(const TCHAR* lpszSourceFile, 
			   const TCHAR* lpszDestFolder, 
			   const char* pszPassword/*=NULL*/);

//from zlib.h
//#define Z_OK            0
//#define Z_STREAM_END    1
//#define Z_NEED_DICT     2
//#define Z_ERRNO        (-1)
//#define Z_STREAM_ERROR (-2)
//#define Z_DATA_ERROR   (-3)
//#define Z_MEM_ERROR    (-4)
//#define Z_BUF_ERROR    (-5)
//#define Z_VERSION_ERROR (-6)


/*
     Compresses the source buffer into the destination buffer.  The level
   parameter has the same meaning as in deflateInit.  sourceLen is the byte
   length of the source buffer.  Upon entry, destLen is the total size of the
   destination buffer, which must be at least the value returned by
   compressBound(sourceLen).  Upon exit, destLen is the actual size of the
   compressed data.

     compress2 returns Z_OK if success, Z_MEM_ERROR if there was not enough
   memory, Z_BUF_ERROR if there was not enough room in the output buffer,
   Z_STREAM_ERROR if the level parameter is invalid.
*/
int ZipMemCompress(BYTE *dest, ULONG *destLen,
					const BYTE *source, ULONG sourceLen,
					int level/*=-1*/);
								  
/*
     compressBound() returns an upper bound on the compressed size after
   compress() or compress2() on sourceLen bytes.  It would be used before a
   compress() or compress2() call to allocate the destination buffer.
*/
ULONG ZipMemCompressBound(ULONG sourceLen);

/*
     Decompresses the source buffer into the destination buffer.  sourceLen is
   the byte length of the source buffer.  Upon entry, destLen is the total size
   of the destination buffer, which must be large enough to hold the entire
   uncompressed data.  (The size of the uncompressed data must have been saved
   previously by the compressor and transmitted to the decompressor by some
   mechanism outside the scope of this compression library.) Upon exit, destLen
   is the actual size of the uncompressed data.

     uncompress returns Z_OK if success, Z_MEM_ERROR if there was not
   enough memory, Z_BUF_ERROR if there was not enough room in the output
   buffer, or Z_DATA_ERROR if the input data was corrupted or incomplete.  In
   the case where there is not enough room, uncompress() will fill the output
   buffer with the uncompressed data up to that point.
*/
int ZipMemUncompress(BYTE *dest, ULONG *destLen,
					 const BYTE *source, ULONG *sourceLen);
									
									


#ifdef __cplusplus
}
#endif