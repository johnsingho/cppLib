#ifndef _SINGLIB2019_JSH_
#define _SINGLIB2019_JSH_
#pragma once
/*****************************************************************************
* Name     : SingLib
* Creator  : H.Z.XIN
* Date     : 2019-09-26
* Function : 工具函数, for windows platform
* Parameter:
* Return(s):
* Notices  :
------------------------------------------------------------------------------
Change Log:
  2019-09-26        H.Z.XIN        Create
  2020-05-20        H.Z.XIN        Update some api
*****************************************************************************/

#ifndef _WINDOWS_
#include <windows.h>
#endif

//-------------------------------------------------
//for library
#ifdef __cplusplus
#  define JOHN_C_START           extern "C" {
#  define JOHN_C_END             }
#else
#  define JOHN_C_START
#  define JOHN_C_END
#endif

#if defined(_MSC_VER)
//编译dll时定义此宏
#if defined(JOHN_EXPORTS)
#	define JOHN_DLL __declspec(dllexport)
#elif defined(JOHN_IMPORTS)
//使用dll时定义此宏
#	define JOHN_DLL __declspec(dllimport)
#else
//啥也不定义可用作静态库
#	define JOHN_DLL
#endif
#	define JOHN_LOCAL
#elif defined (__CYGWIN__)
#   define JOHN_DLL
#else
#if (defined __GNUC__ && __GNUC__ >= 4) || defined __INTEL_COMPILER
#if defined(JOHN_EXPORTS) || defined(JOHN_IMPORTS)
#define JOHN_DLL  __attribute__ ((visibility ("default")))
#define JOHN_LOCAL  __attribute__ ((visibility ("hidden")))
#else
#define JOHN_DLL
#define JOHN_LOCAL
#endif
#else
#define JOHN_DLL
#define JOHN_LOCAL
#endif
#endif


#ifndef JOHN_STDCALL
#if defined(_MSC_VER) && !defined(JOHN_DISABLE_STDCALL)
#  define JOHN_STDCALL     __stdcall
#else
#  define JOHN_STDCALL
#endif
#endif

#ifdef _MSC_VER
#  define FORCE_CDECL  __cdecl
#else
#  define FORCE_CDECL
#endif

#define JOHN_INLINE __inline

#if _MSC_VER && _MSC_VER < 0x0708
#include <stddef.h>

#if !defined(__cplusplus)
typedef char _Bool;
#define bool _Bool
#define true 1
#define false 0
#define __bool_true_false_are_defined 1
#endif

typedef signed char int8_t;
typedef unsigned char   uint8_t;
typedef short  int16_t;
typedef unsigned short  uint16_t;
typedef int  int32_t;
typedef unsigned   uint32_t;
typedef __int64  int64_t;
typedef unsigned __int64 uint64_t;

#ifndef __off_t_defined
#define __off_t_defined
#if _FILE_OFFSET_BITS == 64
typedef long long off_t;
#else
typedef long off_t;
#endif
typedef int64_t off64_t;
#endif /* __off_t_defined */

#define SIZE_T_FORMAT "%u"
#else
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>

#define SIZE_T_FORMAT "%zu"
#endif /* _MSC_VER */


//-------------------------------------------------
JOHN_C_START

/////////////////////////////////////////////////////
//forward declare
typedef struct _SList SList;
typedef void** PtrList;
typedef struct _Johnbuf Johnbuf;

/////////////////////////////////////////////////////

// memory relate
JOHN_DLL void* Mem_malloc(size_t size);
//除了参数区别外，*calloc会初始化内存块为0，而*malloc不会。
JOHN_DLL void* Mem_calloc(size_t num, size_t size);
JOHN_DLL void Mem_free(void * ptr);
JOHN_DLL void* Mem_realloc(void* ptr, size_t size);

#define Mem_mallocTP(tp)                ((tp*)Mem_malloc(sizeof(tp)))
#define Mem_callocTP(tp,n)              ((tp*)Mem_calloc((n),sizeof(tp)))

JOHN_DLL BOOL Mem_IsExecutableAddress(void* pAddress);
JOHN_DLL HMODULE Mem_GetModuleHandleFromAddress(void* address);
//此模块当前的HANDLE
JOHN_DLL HMODULE Mem_GetCurrentModuleHandle();
JOHN_DLL bool Mem_IsAddressInCurrentModule(void* address);

// string relate
JOHN_DLL int FORCE_CDECL Str_asnprintf(char* buf, size_t cchBuf, const char* format, ...);
JOHN_DLL int FORCE_CDECL Str_asnvprintf(char* buf, size_t cchBuf, const char* format, va_list ap);
JOHN_DLL int FORCE_CDECL Str_wsnprintf(wchar_t* buf, size_t cchBuf, const wchar_t* format, ...);
JOHN_DLL int FORCE_CDECL Str_wsnvprintf(wchar_t* buf, size_t cchBuf, const wchar_t* format, va_list ap);

//带ptr结尾的，需要用Mem_free释放
JOHN_DLL char* FORCE_CDECL Str_asnprintfPtr(const char* fmt, ...);
JOHN_DLL char* FORCE_CDECL Str_asnvprintfPtr(const char* fmt, va_list ap);
JOHN_DLL wchar_t* FORCE_CDECL Str_wsnprintfPtr(const wchar_t* fmt, ...);
JOHN_DLL wchar_t* FORCE_CDECL Str_wsnvprintfPtr(const wchar_t* fmt, va_list ap);

//计算缓冲区大小, 不包含最后的NULL
JOHN_DLL int FORCE_CDECL Str_CalcReqSizeA(const char* pstrFmt, ...);
JOHN_DLL int FORCE_CDECL Str_CalcReqSizeW(const wchar_t* pstrFmt, ...);
JOHN_DLL int FORCE_CDECL Str_CalcReqSizeVA(const char* pstrFmt, va_list ap);
JOHN_DLL int FORCE_CDECL Str_CalcReqSizeVW(const wchar_t* pstrFmt, va_list ap);

//如果src==NULL的话，返回NULL
JOHN_DLL char* FORCE_CDECL Str_DupPtrA(const char* src);
JOHN_DLL wchar_t* FORCE_CDECL Str_DupPtrW(const wchar_t* src);

// 从src取n个到新缓冲区
JOHN_DLL char* FORCE_CDECL Str_DupNPtrA(const char* src, size_t cch);
JOHN_DLL wchar_t* FORCE_CDECL Str_DupNPtrW(const wchar_t* src, size_t cch);

//字符串拼接
//要求所有参数都是字符串
// 必须以NULL作为结束符，遇到NULL时，终止解释。
JOHN_DLL char* FORCE_CDECL Str_CatPtrA(const char* sFirst, ...);
JOHN_DLL wchar_t* FORCE_CDECL Str_CatPtrW(const wchar_t* sFirst, ...);

// bTailing 末尾是否要分隔符
// delimiter 分隔字符
//要求所有参数都是字符串
// 必须以NULL作为结束符，遇到NULL时，终止解释。
// Str_JoinPtrA(false, ',', "hello", "Johnsing", "123", NULL) ---> hello,Johnsing,123
JOHN_DLL char* FORCE_CDECL Str_JoinPtrA(bool bTailing/*=false*/, char delimiter, ...);
JOHN_DLL wchar_t* FORCE_CDECL Str_JoinPtrW(bool bTailing/*=false*/, wchar_t delimiter, ...);

// bCaseSence == true 区分大小写
JOHN_DLL bool Str_StartWithA(const char* haystack, const char* needle, bool bCaseSence/*=true*/);
JOHN_DLL bool Str_EndWithA(const char* sLong, const char* sSearch, bool bCaseSence);
JOHN_DLL bool Str_StartWithW(const wchar_t* haystack, const wchar_t* needle, bool bCaseSence/*=true*/);
JOHN_DLL bool Str_EndWithW(const wchar_t* sLong, const wchar_t* sSearch, bool bCaseSence);


//支持*，?匹配
// Str_WildCompareA("hello,Johnsing,123", "hello*123") --> true
// Str_WildCompareA("hello,Johnsing,123", "hELLO*sing*3") --> false
// Str_WildCompareA("hello,Johnsing,123", "*John*.png") --> false
// Str_WildCompareA("hello,Johnsing,123", "hello*1?3") --> true
JOHN_DLL int Str_WildCompareA(const char *pstrLong, const char *wild);
JOHN_DLL int Str_WildCompareW(const wchar_t *pstrLong, const wchar_t *wild);

//只支持*简单匹配
// Str_SimpleMatchA("www.TEST.tom.COM", "www.*.com", false) -> true
// Str_SimpleMatchA("A.B.C.D", "a.b.c.d", false)
// Str_SimpleMatchA("127.0.0.0",  "12*.0.*1", false)
JOHN_DLL bool Str_SimpleMatchA(const char* pstrLong, const char* pattern, bool caseSensitive/*=false*/);
JOHN_DLL bool Str_SimpleMatchW(const wchar_t* pstrLong, const wchar_t* pattern, bool caseSensitive/*=false*/);

JOHN_DLL bool Str_IsWhitespaceA(char ch);
JOHN_DLL bool Str_IsWhitespaceW(wchar_t ch);

// 移除两端空格, \r\n等空白字符
JOHN_DLL bool Str_TrimInplaceA(char* lpString);
JOHN_DLL bool Str_TrimInplaceW(wchar_t* lpString);

// 没有替换的话，返回NULL, *pnReplace==0
JOHN_DLL char* Str_ReplacePtrA(const char* pstrBuf, const char* pstrFind, const char* pstrReplace, int* pnReplace/*=NULL*/);
JOHN_DLL wchar_t* Str_ReplacePtrW(const wchar_t* pstrBuf, const wchar_t* pstrFind, const wchar_t* pstrReplace, int* pnReplace/*=NULL*/);

// 跳过
JOHN_DLL void Str_SkipSpaceA(const char** ppch);
JOHN_DLL void Str_SkipSpaceW(const wchar_t** ppch);
JOHN_DLL void Str_SkipNonspaceA(const char** ppch);
JOHN_DLL void Str_SkipNonspaceW(const wchar_t** ppch);

JOHN_DLL void Str_SkipUntilA(const char** pp, const char* pdelim);
JOHN_DLL void Str_SkipOverA(const char** pp, const char* pdelim);
JOHN_DLL void Str_SkipUntilW(const wchar_t** pp, const wchar_t* pdelim);
JOHN_DLL void Str_SkipOverW(const wchar_t** pp, const wchar_t* pdelim);

JOHN_DLL bool Str_SkipPrefixA(const char* prefix, const char** pstr);
JOHN_DLL bool Str_SkipPrefixW(const wchar_t* prefix, const wchar_t** pstr);

//移除右侧特定字符
JOHN_DLL void Str_TrimRightA(char* pszBuf, const char* pfilt);
JOHN_DLL void Str_TrimRightW(wchar_t* pszBuf, const wchar_t* pfilt);


//跳到下一行。（会破坏原有数据）
//char* pCurLine = szBuf;
//char* p = szBuf;
//nRet = Str_StripNextLine(&p);  //0还有下一行，1已经是最后一行了
JOHN_DLL int Str_StripNextLine(char** pp);

// 分行符的个数
JOHN_DLL size_t Str_CountLines(const char* pstr);

//移除文本中的CRLF
//返回移除后的总字符数
JOHN_DLL int Str_StripCRLFA(char *s);
JOHN_DLL int Str_StripCRLFW(wchar_t *s);


//在字符串前面加0,一般是数字字符串
//如果nExpLen <= strlen(pstrIn), 直接输出pstrIn
JOHN_DLL void Str_ZeroPrefix(char* pstrOut, int cchOut, const char* pstrIn, int nExpLen);
//pstrOut的大小至少应该是nTarSize+1
JOHN_DLL void Str_EndPadding(char* pstrOut, size_t nTarSize, char ch);

// search for an occurrence of string pstrSearch in string pstrLong
// 返回的是所在位置
//pPos = Str_FindOneOfA("#12356#789#000,ABC,DEF", "#,");
////  pPos == strchr(pstrLong, '#');
JOHN_DLL const char* Str_FindOneOfA(const char* pstrLong,	const char* pstrSearch);
JOHN_DLL const wchar_t* Str_FindOneOfW(const wchar_t* pstrLong, const wchar_t* pstrSearch);

JOHN_DLL const char* Str_FindOneNotOfA(const char* pstrLong, const char* pstrSearch);
JOHN_DLL const wchar_t* Str_FindOneNotOfW(const wchar_t* pstrLong, const wchar_t* pstrSearch);

JOHN_DLL const char* Str_FindLastOneOfA(const char* p1, const char* p2);
JOHN_DLL const wchar_t* Str_FindLastOneOfW(const wchar_t* p1, const wchar_t* p2);

JOHN_DLL const char* Str_FindLastOneNotOfA(const char* p1, const char* p2);
JOHN_DLL const wchar_t* Str_FindLastOneNotOfW(const wchar_t* p1, const wchar_t* p2);

// use strtok
//use SListDeepFree to free list
JOHN_DLL SList* Str_TokenA(const char* input, const char* delimitor, bool bTrim, bool bRemoveEmpty);
JOHN_DLL SList* Str_TokenW(const wchar_t* input, const wchar_t* delimitor, bool bTrim, bool bRemoveEmpty);

//  argv = Str_ToArgvPtr("-q --refresh", &argc);
//  argc==3
//  argv[0]永远都是空
//  Mem_free(argv);
JOHN_DLL char** Str_ToArgvPtr(const char* arg, int* pSize);

// is ch in str?
JOHN_DLL bool Str_IsInSetA(char ch, const char* str);
JOHN_DLL bool Str_IsInSetW(wchar_t ch, const wchar_t* str);

// MultiSZ
/*-------------------------------+
/*| 134\0 | 456\0 | CAT\0 | \0\0 |
*+--------+-------+-------+------+
*/
//make 134\0456\0CAT\0\0\0
JOHN_DLL char* Str_MultiSZ_FromListPtrA(const SList* pList);
JOHN_DLL wchar_t* Str_MultiSZ_FromListPtrW(const SList* pList);

//use SListDeepFree to free list
JOHN_DLL SList* Str_MultiSZ_ToListA(const char* ppstrmz);
JOHN_DLL SList* Str_MultiSZ_ToListW(const wchar_t* ppstrmz);

//不分大小写查找
// 0 -- not found
// >0 -- pos+1
JOHN_DLL DWORD Str_MultiSz_FindA(const char* ppstrmz, const char* pszToFind);
JOHN_DLL DWORD Str_MultiSz_FindW(const wchar_t* ppstrmz, const wchar_t* pszToFind);

//这四个函数其实都不更改内容的
JOHN_DLL char* Str_ForwardScanA(char* lpSource, char* lpSearch, bool fCaseSensitive);
JOHN_DLL wchar_t* Str_ForwardScanW(wchar_t* lpSource, wchar_t* lpSearch, bool fCaseSensitive);

//const char* pos = Str_ReverseScanA((char*)pstrLong, NULL, (char*)"hello", false);
//pos = Str_ReverseScanA((char*)pstrLong, (char*)pos - 1, (char*)"hello", false);
JOHN_DLL char* Str_ReverseScanA(
	char* lpSource,
	char* lpLast/*=NULL*/,
	char* lpSearch,
	bool fCaseSensitive);
JOHN_DLL wchar_t* Str_ReverseScanW(
	wchar_t* lpSource,
	wchar_t* lpLast/*=NULL*/,
	wchar_t* lpSearch,
	bool fCaseSensitive);

//找不到返回-1
//注意一个wchar_t = 2bytes
JOHN_DLL int Str_KMPMatcher(
	const BYTE* pData, const uint32_t cchData,
	const BYTE* pSearch, const uint32_t cchSearch,
	const uint32_t iBegin/* = 0*/
);


//pointer array
/*--------+-------+-------+------+
/*| void* | void* | void* | NULL |
*+--------+-------+-------+------+
*/
JOHN_DLL PtrList PtrList_Add(PtrList other/*=NULL*/, void* pObj);
//只释放容器，子元素的释放不是我的责任
JOHN_DLL void PtrList_Free(PtrList ppList);

JOHN_DLL int PtrList_Count(PtrList ppList);
JOHN_DLL void* PtrList_GetField(PtrList ppList, uint32_t iField);
//浅复制
JOHN_DLL PtrList PtrList_Duplicate(PtrList ppList);
//两边为空的话，返回NULL
JOHN_DLL PtrList PtrList_Merge(PtrList papszOrig, PtrList papszOverride);

typedef void PfnPtrListWalk(void* pitem, void* pExtra);
JOHN_DLL void PtrList_Walk(PtrList ppList, PfnPtrListWalk pfn, void* pExtra/*=NULL*/);


// 简单的动态缓冲区
JOHN_DLL Johnbuf* Johnbuf_new();
JOHN_DLL void Johnbuf_free(Johnbuf* hBuf);
//重置内部指针，重用内存
JOHN_DLL void Johnbuf_reset(Johnbuf* pBuf);
//访问内部指针
JOHN_DLL BYTE* Johnbuf_value(const Johnbuf* pBuf);
//使用了多少字节
JOHN_DLL size_t Johnbuf_length(const Johnbuf* pBuf);

/* Steal the value and close the strbuffer */
/* 直接窃取字符串缓冲区，原对象变得无效 
*  指针需要使用Mem_free()释放
*/
JOHN_DLL BYTE* Johnbuf_steal_value(Johnbuf* pBuf);

//放入文本，不包含NULL
//cchStr==0的话，自动统计
//返回负数表示失败
JOHN_DLL int Johnbuf_appendA(Johnbuf* pBuf, const char* pstr, int cchStr/*=0*/);
JOHN_DLL int Johnbuf_appendW(Johnbuf* pBuf, const wchar_t* pstr, int cchStr/*=0*/);

JOHN_DLL int Johnbuf_append_byte(Johnbuf* pBuf, char by);
JOHN_DLL int Johnbuf_append_bytes(Johnbuf* pBuf, const BYTE* data, size_t size);

/**
 * Set len bytes of the buffer to charvalue, starting at offset offset.
 * Similar to calling memset(x, charvalue, len);
 *
 * The memory allocated for the buffer is extended as necessary.
 *
 * If offset is -1, this starts at the end of the current data in the buffer.
 */
JOHN_DLL int Johnbuf_memset(Johnbuf* pBuf, int offset, int chVal, int len);
JOHN_DLL int FORCE_CDECL Johnbuf_sprintf(Johnbuf* pBuf, const char* msg, ...);

//弹出最后一个字符
JOHN_DLL char Johnbuf_pop(Johnbuf* pBuf);

// 字节操作
JOHN_DLL bool Byte_Equal(const void* s1, const void* s2, size_t n);
//这里没有对src进行多重循环以填充dest
JOHN_DLL void Byte_Xor(void* dest, const void* src, size_t nSrc);

JOHN_DLL void Byte_RevorderInplace(BYTE* data, int size);
JOHN_DLL void Byte_RevorderTo(const BYTE *src, BYTE *dst, size_t size);

//凑合用
// 0x11223344 ---> 0x44332211
JOHN_DLL uint32_t Byte_Swap32(uint32_t x);
// 0x11223344 ---> 0x33441122
JOHN_DLL uint32_t Byte_Swap32Word(uint32_t x);
// 0x1122 ---> 0x2211
JOHN_DLL uint16_t Byte_Swap16(uint16_t x);
// 0x1122334455667788 ---> 0x8877665544332211
JOHN_DLL uint64_t Byte_Swap64(const uint64_t* pval);

//返回1bit的数量
// 7 ---> 3
JOHN_DLL size_t Byte_BitCount32(uint32_t bits);
JOHN_DLL size_t Byte_BitCount64(uint64_t bits);
// 0x11       --> 0x88
// 0b00010001 --> 0b10001000
JOHN_DLL BYTE Byte_BitRev(BYTE n);

//奇偶字节交换
//简单实现，效率很差
// 0x12345678 --> 0x34127856
// 0x1122334455667788 ---> 0x2211443366558877
JOHN_DLL void Byte_Swap(
	BYTE *src,
	BYTE *dest,
	size_t cchDest
);

//0x00000011 ---> 0x88000000
JOHN_DLL uint32_t Byte_Flip32(uint32_t x);

//产生随机数
JOHN_DLL bool Byte_RandomBytes(uint32_t len, BYTE *buf);
JOHN_DLL int32_t Byte_RandomInt32();
JOHN_DLL uint32_t Byte_RandomUint32();
JOHN_DLL uint32_t Byte_RandomUint32NonZero();
JOHN_DLL uint64_t Byte_RandomUint64();
JOHN_DLL double Byte_RandomDouble();
//会自动添加NULL，cchBuf包含了NULL
JOHN_DLL bool Byte_RandomStr(size_t cchBuf, char* buf);
JOHN_DLL char* Byte_RandomStrPtr(size_t n);

//产生随机数[min, max]
JOHN_DLL int32_t Byte_RandomInt32Range(int32_t min, int32_t max);
// [min, max)
JOHN_DLL double Byte_RandomDoubleRange(double min, double max);

//与Str_EndPadding类似, 返回填充的个数，<0失败。
//如果nOutCurSize<nTarSize的话，往pstrOut的末尾填充ch.
//如果nOutCurSize>=nTarSize, 无动作。
JOHN_DLL int Byte_EndPadding(BYTE* pstrOut, size_t nOutCurSize, size_t nTarSize, BYTE ch/*=NULL*/);

//16个字节
JOHN_DLL bool Byte_GenUUID(uint8_t* uuid_data);

//bHasSpilt==false, cchBuf至少要有32+1个字符 "6609F82E963F4C8DBE8FA0F40E2C9656"
//bHasSpilt==true, cchBuf至少要有36+1个字符. "E5723717-133F-42D2-B81A-C64E22E2F111"
JOHN_DLL bool Str_GenUUID(char* buffer, size_t cchBuf, bool bHasSpilt/*=false*/);

// 1024             ---> "1KB"
// 5242880          ---> 5.0 MB
// 7516192768       ---> 7.0 GB
// 7881299347898368 ---> 7.0 EB
JOHN_DLL void Str_FormatBytes(char* lpOutput, int cchOutput, uint64_t dwBytes);

//1KB ---> 1024
//5.0 MB --> 5242880
// 要求单位大写带B, 支持单位：KB,MB,GB
JOHN_DLL uint64_t Str_ParseBytes(const char *astring);

//不需要带B, 不支持小数，不区分大小写
//支持单位：KB, MB, GB
//sample: 5MB, 5 M, 7g
JOHN_DLL uint64_t Str_ParseBytesCase(const char *bs);

typedef enum _eEscapeType {
	/*把字符串转换成适合用作C字符串的形式。转换掉slash,quote,换行符等, 不允许行中有\0*/
	ET_BackslashQuotable=0, 
	/*把字符串转换成适合放入XML CDATA的形式。转换掉<,>,",&等, 不允许行中有\0*/
	ET_XML=1,
	/*把字符串转换成适合放在URL传送的形式。转换掉:,/,&,?等，RFC1738*/
	ET_URL=2,
	/*把字符串转换成适合放在SQL语句中的形式。一个单引号变两个*/
	ET_SQL=3,
	/*CSV转义，一个双引号，变两个*/
	ET_CSV=4,
	/*XML(preserves quotes, 不转义双引号成&quotes;)*/
	ET_XML_BUT_QUOTES=5,
	/*CSV (forced quoting)*/
	ET_CSV_FORCE_QUOTING=6,
	/*SQL identifiers*/
	ET_SQLI=7
}EEscapeType;

JOHN_DLL char *Str_EscapeStringPtr(const char *pszInput, int nLength, EEscapeType nScheme);
// ET_CSV 和 ET_CSV_FORCE_QUOTING 解释未实现
JOHN_DLL char *Str_UnescapeStringPtr(const char *pszInput, int *pnLength, EEscapeType nScheme);



//Locale
//zh-CN
//localeType 是 LOCALE_*开头的常量
JOHN_DLL wchar_t* Str_LocaleInfoPtr(LCTYPE localeType);
JOHN_DLL const wchar_t* Str_LangDefault();

// LANG_*开头的常量
JOHN_DLL void OS_GetDefaultLangID(LANGID* psysLang, LANGID* puserLang);

//CPxxx
JOHN_DLL const char* Str_GetDefaultCodePage();
JOHN_DLL const char* Str_GetInputCodePage();

//ex: 936, 950, ...
bool Str_IsDBCSCodePage(int codePage);
bool Str_IsDBCSLeadByte(int dbcsCodePage, char ch);

// mbcs OEM char* <----> wchar_t*
JOHN_DLL wchar_t* Str_MbcsToUnicodePtr(const char* szStr, UINT cp/*=CP_ACP*/);
JOHN_DLL char* Str_UnicodeToMbcsPtr(const wchar_t* szStr, UINT cp/*=CP_ACP*/);

JOHN_DLL wchar_t* Str_Utf8ToUnicodePtr(const char* utf8Str);
JOHN_DLL char* Str_UnicodeToUtf8Ptr(const wchar_t* UnicodeStr);

JOHN_DLL char* Str_MbcsToUtf8Ptr(const char* strSource, UINT cp/* = CP_ACP*/);
JOHN_DLL char* Str_Utf8ToMbcsPtr(const char* strSource, UINT cp/* = CP_ACP*/);

// nSrc是字节数
JOHN_DLL wchar_t* Str_MbcsToUnicodeNPtr(const BYTE* bySrc, size_t nSrc, UINT cp/*=CP_ACP*/);
JOHN_DLL wchar_t* Str_Utf8ToUnicodeNPtr(const BYTE* bySrc, size_t nSrc);

//相当于code page转换
//GBK             936
//GB18030         54936
//BIG5            950
//char* pstrUn = Str_RecodePtr(pstr, 950, CP_ACP);
JOHN_DLL char* Str_RecodePtr(const char* src, UINT src_cp, UINT dst_cp);

//UTF8字符个数
//相对而言，strlen返回的是字节数
JOHN_DLL int Str_Utf8Len(const char *pszUTF8Str);

//bool bIsValid = Str_IsLegalUTF8Sequence((BYTE*)pstr, (BYTE*)pstr+strlen(pstr));
JOHN_DLL bool Str_IsLegalUTF8Sequence(const uint8_t* source, const uint8_t* sourceEnd);
// length的取值： 1~4
JOHN_DLL bool Str_IsLegalUTF8(const uint8_t *source, int length);


// 朱镕基只在GBK，不在GB2312
JOHN_DLL bool Str_IsLegalGB2312(const void* stream, unsigned length);
JOHN_DLL bool Str_IsLegalGBK(const void* stream, unsigned length);
JOHN_DLL bool Str_IsLegalBIG5(const void* stream, unsigned length);


typedef enum {
	conversionOK = 0, 	/* conversion successful */
	sourceExhausted,	/* partial character in source, but hit end */
	targetExhausted,	/* insuff. room in target for conversion */
	sourceIllegal		/* source sequence is illegal/malformed */
} ConversionResult;

typedef enum {
	strictConversion = 0,
	lenientConversion
} ConversionFlags;

// UTF8 <---> UTF16
//似乎对UTF16 BE支持得不好
JOHN_DLL
ConversionResult Str_UTF16toUTF8(
	const uint16_t** sourceStart, const uint16_t* sourceEnd,
	uint8_t** targetStart, uint8_t* targetEnd, ConversionFlags flags);

JOHN_DLL
ConversionResult Str_UTF8toUTF16(
	const uint8_t** sourceStart, const uint8_t* sourceEnd,
	uint16_t** targetStart, uint16_t* targetEnd, ConversionFlags flags);


// 数字与字符串互转, 不需要释放字符串内存
JOHN_DLL const char* Num_StrInt16(int16_t val);
JOHN_DLL const char* Num_StrUInt16(uint16_t val);
JOHN_DLL const char* Num_StrInt32(int32_t val);
JOHN_DLL const char* Num_StrUInt32(uint32_t val);
JOHN_DLL const char* Num_StrInt64(int64_t val);
JOHN_DLL const char* Num_StrUInt64(uint64_t val);

//最多保留4位小数
//340282346638528859811704183484516925440.0000
JOHN_DLL const char* Num_StrFloat(float val);

//最多保留6位小数, Mem_free释放内存
//179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.000000
JOHN_DLL char* Num_StrDoublePtr(double val);

//最多保留16位小数, Mem_free释放内存
//2.225073858507201e-308
JOHN_DLL char* Num_StrLongDoublePtr(const long double* pval);

// 0xXXXXXXXX
JOHN_DLL const char* Num_StrAddress(const void* pval);
// "true"
// "false"
JOHN_DLL const char* Num_StrBool(bool val);

JOHN_DLL int32_t Str_Int32(const char* pstr);
JOHN_DLL int64_t Str_Int64(const char* pstr);

//支持十进制字符串，也支持0x12345678十六进制字符串
JOHN_DLL uint32_t Str_UInt32(const char* pstr);
JOHN_DLL uint64_t Str_UInt64(const char* pstr);

JOHN_DLL float Str_Float(const char* pstr);
JOHN_DLL double Str_Double(const char* pstr);

// 简单实现的
// "5A" ---> 90
// "ffff" ---> 65535
JOHN_DLL uint64_t Str_hex2llA(const char *buf);
JOHN_DLL uint64_t Str_hex2llW(const wchar_t *buf);

// "true", "TRUE", "yes" ---> true
// other ---> false
JOHN_DLL bool Str_Bool(const char* pstr);


//////////////////////////////////////////////

//计算CRC32
/*
   Usage example:
	 uLong crc = Byte_Crc32(0L, NULL, 0);
	 while (read_buffer(buffer, length) != EOF) {
	   crc = Byte_Crc32(crc, buffer, length);
	 }
	 if (crc != original_crc) error();
*/
JOHN_DLL unsigned long Byte_Crc32(unsigned long crc, const unsigned char* buf, unsigned int len);


//分步
typedef struct _TCrc64 TCrc64;
JOHN_DLL TCrc64* Byte_Crc64Init();
JOHN_DLL void Byte_Crc64Update(TCrc64* pcrc64, const void *data, size_t size);
//pdigest与返回值功能一样
JOHN_DLL uint64_t Byte_Crc64Final(TCrc64* pcrc64, BYTE* pdigest/*=NULL*/);

//一步到位
JOHN_DLL uint64_t Byte_Crc64(const void *data, size_t size);


// base64编码，返回输出 strlen(dest)+1
// 失败的话返回-1
// 需要自己加上NULL
JOHN_DLL size_t Byte_Base64Encode(char* dest, const char* src, size_t srcLen);
// base64解码，返回输出 strlen(dest)
// 失败的话返回-1
// 需要自己加上NULL
JOHN_DLL size_t Byte_Base64Decode(char* dest, const char* src, size_t srcLen);
//预估大小
inline size_t Byte_Base64_EncodeLen(size_t srcLen)
{
	return ((srcLen + 2) / 3 * 4 + 1);
}
inline size_t Byte_Base64_DecodeLen(size_t srcLen)
{
	return (srcLen / 4 * 3 + 2);
}

typedef enum {
	INCLUDE_PADDING, /*允许有最后的=填充(如有必要的话)*/
	OMIT_PADDING     /*移除最后的=填充*/
}EBase64UrlEncodeKind;

JOHN_DLL char* Byte_Base64UrlEncodePtr(
	const char* input,
	EBase64UrlEncodeKind encodeKind
);

typedef enum {
	REQUIRE_PADDING, /*如果未对齐的话，要求末尾必须要有=填充*/
	IGNORE_PADDING,  /*不介意有没有正确地末尾填充*/
	DISALLOW_PADDING /*拒绝接受有=填充的*/
}EBase64UrlDecodeKind;

JOHN_DLL char* Byte_Base64UrlDecodePtr(
	const char* input,
	EBase64UrlDecodeKind decodeKind
);

//字节 ---> Z85编码
// datasz_必须可以被4整除；
//
JOHN_DLL char* Byte_z85_encode(char* dest_, const BYTE* data_, size_t datasz_);
inline size_t Byte_z85_EncodeLen(size_t srcLen)
{
	return srcLen*5/4+1;
}

// strlen(string_)必须可以被5整除；
JOHN_DLL BYTE* Byte_z85_decode(BYTE* dest_, const char* string_);
inline size_t Byte_z85_DecodeLen(size_t srcLen)
{
	return srcLen * 4 / 5+1; //need +1?
}


// "0123A" ---> "3031323341"
// buflen = srclen*2+1
JOHN_DLL size_t Byte_ToHexStr(char* buffer, size_t buflen, const BYTE* pstrc, size_t srclen);
//可带分隔符，一般是空格
// buflen = srclen*3+1
JOHN_DLL size_t Byte_ToHexStrDelimiter(char* buffer, size_t buflen, const BYTE* psrc, size_t srclen, char delimiter/*=0*/);

// 不带分隔符时，应当 buflen >= srclen/2
// 带分隔符时，应当   buflen > (srclen+1)/3+1
// If the buffer is too short, we return 0
JOHN_DLL size_t Byte_FromHexStr(BYTE* pBuf, size_t buflen, const char* psrc, size_t srclen);
JOHN_DLL size_t Byte_FromHexStrDelimiter(BYTE* pBuf, size_t buflen, const char* psrc, size_t srclen, char delimiter/*=0*/);

JOHN_DLL char* Byte_ToHexStrPtr(int nBytes, const BYTE* pabyData);
JOHN_DLL BYTE* Byte_FromHexStrPtr(const char* pszHex, int* pnBytes/*=NULL*/);


//////////////////////////////////////////////

// 0 --> false
// 16 -->true
// 17 -->false
inline bool Num_IsPowerOfTwo(uint32_t n)
{
	return n > 0 && ((n & (n - 1)) == 0);
}

//四舍五入
// -3.81 --> -4
// 3.81  --> 4
JOHN_DLL int Num_RoundInt(double value);

// nPrecision=多少位小数
// Num_RoundDouble(3.115, 2) --> 3.12
// Num_RoundDouble(-3.115, 2) --> -3.12
JOHN_DLL double Num_RoundDouble(double doValue, int nPrecision);

JOHN_DLL bool Num_FloatsEqual(double dblA, double dblB);

//向上取整到divisor的倍数
//适用于divisor是2的N次方
//如果divisor不是2的N次方,那结果就很难预料了。
// (4,5) --> 8
// (4,3) --> 4
// (4,4) --> 4
JOHN_DLL size_t Num_RoundUpToMultipleOf(size_t divisor, size_t x);
// (8, 5) --> 0
// (8, 17) --> 16
// (8, 8) --> 8
JOHN_DLL size_t Num_RoundDownToMultipleOf(size_t divisor, size_t x);

//带溢出检查的乘法
JOHN_DLL size_t Num_CheckMul(size_t mul1, size_t mul2, bool *pbOverflowFlag);
//7 --> 1
//100 --> 3
//1011 --> 4
JOHN_DLL int Num_CountDigits(uint64_t x);

///////////////////////////////////////////
// Simplest single list
/** List element structure. */

// SListAppend(NULL, pData); //创建新列表
JOHN_DLL SList* SListAppend(SList *psList, void *pData);

//只释放容器，不负责释放元素。
//释放元素是用户自己的责任
JOHN_DLL void SListFree(SList* plist);

//如果元素是使用Mem_malloc分配的话，可以使用此方法
JOHN_DLL void SListDeepFree(SList* plist);

//nPosition为负，不干活
//nPosition>已有元素个数，引起列表扩张，从而使nPosition有效
//nPosition<已有元素个数, 插入到指定位置
//nPosition==已有元素个数, 插入到末尾
JOHN_DLL SList* SListInsert(SList *psList, void *pData, int nPosition);

// nPosition = 0 ~ n-1
JOHN_DLL SList* SListGet(SList *psList, int nPosition);

JOHN_DLL SList* SListGetLast(SList *psList);

//迭代容器
#if 0
SList* pCur = pList;
while (pCur)
{
	printf("item %02d = %d\n", i++, (int)SListGetData(pCur));
	pCur = SListGetNext(pCur);
}
#endif
JOHN_DLL SList* SListGetNext(SList *psElement);

JOHN_DLL void* SListGetData(const SList *psElement);
//先SListGet, 再SListGetData
JOHN_DLL void* SListGetItemData(const SList *psList, int nPosition);

//O(n)
JOHN_DLL int SListCount(const SList *psList);

// nPosition = 0 ~ n-1
JOHN_DLL SList* SListRemove(SList *psList, int nPosition);
JOHN_DLL SList* SListRemoveElement(SList *psList, SList *psElement);


typedef bool (*PfnSListFind)(void* pe, void* pFind);
JOHN_DLL SList* SListFind(SList *psList, PfnSListFind pfn, void *pFind);

typedef void* (*PfnSListWalk)(SList *psElement, void* pExtra);
JOHN_DLL void SListWalk(SList *psList, PfnSListWalk pfn, void* pExtra/*=NULL*/);

//返回true的话，中止后续扫描
typedef bool (*PfnSListWalkUntil)(SList *psElement, void* pExtra);
JOHN_DLL void SListWalkUntil(SList *psList, PfnSListWalkUntil pfn, void* pExtra/*=NULL*/);

//简单排序
#if 0
int MyCmp(const SList* pItem1, const SList* pItem2)
{
	void* pd1 = SListGetData(*(SList**)pItem1); //!
	void* pd2 = SListGetData(*(SList**)pItem2);
	return (int)pd1 - (int)pd2;
}

SList* pList = NULL;
pList = SListAppend(pList, (void*)(123));
pList = SListAppend(pList, (void*)456);
pList = SListSort(pList, MyCmp);

#endif
typedef int (*PfnSListCompare)(const SList* pItem1, const SList* pItem2);
JOHN_DLL SList* SListSort(SList *psList, PfnSListCompare pfnCompare);


// windows relate
JOHN_DLL void WaitCursor_Begin();
JOHN_DLL void WaitCursor_End();

//简单创建线程
JOHN_DLL
HANDLE Thread_Create(uint32_t stack_size/*=0*/, 
					 uint32_t initflag/*=0*/,
					 uint32_t(FORCE_CDECL *pfn)(void* parg),
					 void* parg);

JOHN_DLL bool Thread_Join(HANDLE hnd, DWORD dwMs/*=INFINITE*/);

//此函数只适用于调试状态,由thread自己调用
//sample: 
//#ifdef _DEBUG
//SetThreadName(GetCurrentThreadId(), name_.c_str());
//#endif
JOHN_DLL void Thread_SetName(DWORD thread_id, const char* name);

JOHN_DLL HANDLE Thread_GetCurrentRealHandle(void);
JOHN_DLL BOOL OS_DuplicatedHandle(DWORD sourcePID, HANDLE sourceHandle, HANDLE* pHandle);

//在xp的话，它是CRITICAL_SECTION
typedef struct _John_rwlock John_rwlock;
JOHN_DLL John_rwlock* John_rwlock_new();
JOHN_DLL void John_rwlock_free(John_rwlock *lock);
//0--ok
//-1--失败
JOHN_DLL int John_rwlock_rdlock(John_rwlock *lock);
JOHN_DLL int John_rwlock_rdunlock(John_rwlock *lock);
JOHN_DLL int John_rwlock_wrlock(John_rwlock *lock);
JOHN_DLL int John_rwlock_wrunlock(John_rwlock *lock);


// 只有是32bit app在win64运行时，Wow64Process==TRUE
// 其它时候为FALSE。
// 返回值没什么用，仅代表函数是否调用成功
JOHN_DLL BOOL Process_IsWow64(HANDLE hProcess, PBOOL Wow64Process);

JOHN_DLL BOOL OS_Is64Bit();
JOHN_DLL BOOL Process_ThisIs64Bit(void);
JOHN_DLL BOOL Process_Is64Bit(HANDLE hProcess);

//杀进程，及其子进程
JOHN_DLL BOOL Process_TerminateAll(HANDLE process, int code);

//创建子进程，并使用管道通讯
typedef struct Process_Pipe Process_Pipe_t;

enum EProcessPipeType{
	PP_Read=1,
	PP_Write=2
};

JOHN_DLL Process_Pipe_t* Process_Pipe_Create(const wchar_t *cmd_line, enum EProcessPipeType eType);
JOHN_DLL int Process_Pipe_Destory(Process_Pipe_t *pp);

JOHN_DLL size_t Process_Pipe_Read(Process_Pipe_t *pp, uint8_t *data, size_t len);
JOHN_DLL size_t Process_Pipe_Write(Process_Pipe_t *pp, const uint8_t *data, size_t len);


/**
 * Launch a child process with the specified arguments.
 * exePath 通常是全路径
 * @note argv[0] is ignored
 */
JOHN_DLL
BOOL Process_LaunchChild(const wchar_t *exePath, 
						int argc, wchar_t **argv, 
						HANDLE userToken/* = NULL*/,
						HANDLE *hProcess /*= NULL*/);

JOHN_DLL char* Process_MakeCommandLinePtrA(int argc, char **argv);
JOHN_DLL wchar_t* Process_MakeCommandLinePtrW(int argc, wchar_t **argv);

//执行进程
//pbHijacked是否被拦截执行
JOHN_DLL BOOL Process_ExecuteFile(const wchar_t* lpszFilename, 
						const wchar_t* lpszParam/*=L""*/,
						DWORD* pdwExitCode/*=NULL*/, 
						BOOL* pbHijacked/* = NULL*/);

//返回值只代表命令有执行，但不关心执行结果
JOHN_DLL BOOL Process_RunDOSCommandA(const char* pszCommand, BOOL bWaitForExit/*=true*/);
//const wchar_t* pstrRemove = L"del C:\\Users\\john\\desktop\\Test2019.exe*.lnk";
//bok = Process_RunDOSCommandW(pstrRemove, TRUE);
JOHN_DLL BOOL Process_RunDOSCommandW(const wchar_t* pszCommand, BOOL bWaitForExit/*=true*/);

JOHN_DLL BOOL Process_Run(const wchar_t *application, HANDLE *process/*=NULL*/);
JOHN_DLL HANDLE Process_RunAS(const wchar_t* application, 
							  const wchar_t* command, 
							  BOOL bHide/*=FALSE*/, 
							  BOOL bWaitExit/*=FALSE*/);
JOHN_DLL BOOL Process_RunWithCommand(const wchar_t *application, 
									 const wchar_t *command, 
									 HANDLE *process/*=NULL*/);
JOHN_DLL BOOL Process_RunWithRedirection(const wchar_t *application,
								const wchar_t *command,
								HANDLE input,
								HANDLE output,
								HANDLE error,
								HANDLE *process/*=NULL*/);

JOHN_DLL BOOL Process_RunLowIntegrity(const wchar_t* pszCommandLine, HANDLE* phProcess/*=NULL*/);

// pszProcessName通常是 xxx.exe
// 同时返回多个
// DWORD dwID = (DWORD)SListGetItemData(plist, 0);
JOHN_DLL SList* Process_GetIdFromName(const wchar_t* pszProcessName);

//get computer name
//pstrOut至少应当为16个字符(含NULL)
JOHN_DLL bool OS_GetHostNamePtr(wchar_t* pstrOut, unsigned int cchOut);

// 添加到PATH环境变量
// 只影响本进程
JOHN_DLL BOOL OS_EnvPrependPath(const wchar_t* directoryToPrepend);

//bExpand -- 如果值里面有变量，是否尝试展开
JOHN_DLL char* OS_EnvGetPtrA(const char* pstrEnv, bool bExpand/*=false*/);
JOHN_DLL wchar_t* OS_EnvGetPtrW(const wchar_t* pstrEnv, bool bExpand/*=false*/);

//简单get/set
// cchVal包含NULL
// 只影响本进程
JOHN_DLL BOOL OS_EnvGetA(const char* pstrEnv, char* pstrVal, size_t cchVal);
JOHN_DLL BOOL OS_EnvGetW(const wchar_t* pstrEnv, wchar_t* pstrVal, size_t cchVal);

JOHN_DLL BOOL OS_EnvSetA(const char* pstrEnv, const char* pstrVal);
JOHN_DLL BOOL OS_EnvSetW(const wchar_t* pstrEnv, const wchar_t* pstrVal);

JOHN_DLL BOOL OS_EnvDeleteA(const char* pstrEnv);
JOHN_DLL BOOL OS_EnvDeleteW(const wchar_t* pstrEnv);

//对所有%var%变量进行一次展开
//pstr = OS_EnvExpandPtrW(L"%JOHN2012%_%SystemRoot%");
JOHN_DLL char* OS_EnvExpandPtrA(const char* s);
JOHN_DLL wchar_t* OS_EnvExpandPtrW(const wchar_t* s);


JOHN_DLL void FORCE_CDECL OS_MessageBoxA(HWND hwnd/* = null*/, const char* fmt, ...);
JOHN_DLL void FORCE_CDECL OS_MessageBoxW(HWND hwnd/* = null*/, const wchar_t* fmt, ...);

// 操作系统版本粗略判定
JOHN_DLL bool OS_IsWindowsVistaOrGreater();
JOHN_DLL bool OS_IsWindowsServer();
JOHN_DLL bool OS_IsWindowsVersionOrGreater(WORD wMajorVersion, WORD wMinorVersion, WORD wServicePackMajor);

inline bool OS_IsWindowsXPSP3OrGreater()
{
	return OS_IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WINXP), LOBYTE(_WIN32_WINNT_WINXP), 3);
}
inline bool OS_IsWindows7OrGreater()
{
	return OS_IsWindowsVersionOrGreater(HIBYTE(0x0601), LOBYTE(0x0601), 0);
}
inline bool OS_IsWindows1OorGreater()
{
	return OS_IsWindowsVersionOrGreater(HIBYTE(0x0A00), LOBYTE(0x0A00), 0);
}

JOHN_DLL int OS_NumOfProcessCore();
//全部物理内存，可用物理内存，多少MB
JOHN_DLL bool OS_PhysicalRAMSize(uint64_t* pnTotalPhys, uint64_t* pnAvailPhys);

//返回可用bytes
//pszDirname是一个已经存在的目录(不允许是不存在的)
//可以直接用 c:\ 
JOHN_DLL int64_t OS_GetDiskFreeSpace(const wchar_t* pszDirname);

// pszPath like: c:\ 
JOHN_DLL BOOL OS_IsSupportSparseFiles(const wchar_t* pszPath);

// lpszPrivilege是 SE_*_NAME常量
// 如：SE_SECURITY_NAME
JOHN_DLL BOOL OS_EnableSpecifiedPrivilege(const wchar_t* lpszPrivilege, BOOL bEnable);
JOHN_DLL char* OS_GetMachineGuidPtr();

// type is SE_OBJECT_TYPE
JOHN_DLL BOOL OS_SetObjectToLowIntegrity(HANDLE hObject, DWORD type /*= SE_KERNEL_OBJECT*/);

// Determine if the user is part of the adminstators group. This will return
// true in case of XP and 2K if the user belongs to admin group. In case of
// Vista, it only returns true if the admin is running elevated.
JOHN_DLL BOOL OS_IsUserAdmin();

// Returns true if the user is running as a non-elevated admin in case of
// Vista. In case of XP always returns false.
JOHN_DLL BOOL OS_IsUserNonElevatedAdmin();

// Determine the mandatory level of a SID
JOHN_DLL HRESULT OS_GetSidIntegrityLevel(PSID sid, MANDATORY_LEVEL* level);

// Determine the mandatory level of a process
//   processID, the process to query, or (0) to use the current process
//   On Vista, level should alwys be filled in with either
//     MandatoryLevelLow (IE)
//     MandatoryLevelMedium(user), or
//     MandatoryLevelHigh( Elevated Admin)
//   On error, level remains unchanged
JOHN_DLL HRESULT OS_GetProcessIntegrityLevel(DWORD processID, MANDATORY_LEVEL* level, DWORD* pRID/*=NULL*/);

// *pRID 是RID，如：SECURITY_MANDATORY_MEDIUM_RID
JOHN_DLL BOOL OS_GetProcessIntegrityLevelByHandle(HANDLE hProcess, MANDATORY_LEVEL* level, DWORD* pRID/*=NULL*/);

//检测是否 level < SECURITY_MANDATORY_MEDIUM_RID
// 一般的进程是SECURITY_MANDATORY_MEDIUM_RID
JOHN_DLL BOOL OS_IsCurrentProcessLowIntegrity();


typedef struct IPC_pipe_server IPC_pipe_server_t;
typedef struct IPC_pipe_client IPC_pipe_client_t;

typedef void(*IPC_pipe_read_t)(void *param, uint8_t *data, size_t size);

JOHN_DLL IPC_pipe_server_t* IPC_pipe_server_start(const wchar_t *name, IPC_pipe_read_t read_callback, void *param);
JOHN_DLL void IPC_pipe_server_free(IPC_pipe_server_t *pipe);

JOHN_DLL IPC_pipe_client_t* IPC_pipe_client_open(const wchar_t *name);
JOHN_DLL void IPC_pipe_client_free(IPC_pipe_client_t *pipe);
JOHN_DLL bool IPC_pipe_client_write(IPC_pipe_client_t *pipe, const void *data, size_t size);
JOHN_DLL bool IPC_pipe_client_valid(IPC_pipe_client_t *pipe);

typedef struct _shMemory SHMemory;

JOHN_DLL SHMemory* OS_SHMemory_new();
//free SHMemory
JOHN_DLL void OS_SHMemory_free(SHMemory* pshm);

//Create or Open选其一
//size必须大于0
JOHN_DLL BOOL OS_SHMemory_Create(SHMemory* pshm, const wchar_t* name, DWORD size, BOOL bLowIntegrity/*=FALSE*/);
JOHN_DLL BOOL OS_SHMemory_Open(SHMemory* pshm, const wchar_t* name);

JOHN_DLL BOOL OS_SHMemory_OpenView(SHMemory* pshm, bool bWrite);
JOHN_DLL BYTE* OS_SHMemory_View(SHMemory* pshm);
JOHN_DLL void OS_SHMemory_CloseView(SHMemory* pshm);

typedef struct _win_version_info {
	int major;
	int minor;
	int build;
	int revis;
}win_version_info;

//path可以是全路径，也可以是dll名
//如： File_GetVersion(L"kernel32", &ver);
JOHN_DLL bool File_GetVersion(const wchar_t* path, win_version_info* info);
//字节数
JOHN_DLL bool File_GetSize(const wchar_t* path, uint64_t* pusize);


//本进程使用的内存bytes
JOHN_DLL size_t Process_GetMemoryUsage();

//0.0.0.0
JOHN_DLL char* File_GetModuleVerPtr(HMODULE hMod/*=NULL*/);
//功能更多
//cchBufLen是每个缓冲区的大小
JOHN_DLL bool File_GetModuleVerMore(
	HMODULE hMod/*=NULL*/,
	uint32_t cchBufLen,
	wchar_t *szComments,
	wchar_t *szCompanyName, wchar_t *szFileDescription,
	wchar_t *szFileVersion, wchar_t *szLegalCopyright,
	wchar_t *szProductName, wchar_t *szProductVersion);

//只从system32和用户目录加载dll
JOHN_DLL void OS_DllHijackingProtection();

JOHN_DLL BOOL Win_IsGUIExecutable(HMODULE hMod/*=NULL*/);

// windows UI
JOHN_DLL BOOL Win_IsAppThemed();

JOHN_DLL void Win_CalculateForText(
	HWND wnd,
	const wchar_t* text,
	size_t* width,
	size_t* height);

JOHN_DLL HFONT Win_GetDefaultFont();
JOHN_DLL BOOL Win_ResizeClient(HWND hWnd, int nWidth, int nHeight, BOOL bRedraw/* = TRUE*/);
JOHN_DLL HWND Win_GetTopLevelParent(HWND hWnd);
JOHN_DLL HWND Win_GetTopLevelWindow(HWND hWnd);

JOHN_DLL HICON Win_GetIconFromFile(const wchar_t* pstrFile);

//需要自己DeleteObject(hImageBMP);
JOHN_DLL HBITMAP Bitmap_FromIcon(HICON hIcon, HBRUSH hBackground, int width, int height);
JOHN_DLL HBITMAP Bitmap_FromRect(HWND hWnd, RECT* pRect);
JOHN_DLL HBITMAP Bitmap_FromScreenRect(RECT* lpRect, BYTE *pData/*=NULL*/, BITMAPINFO *pHeader/*=NULL*/);

JOHN_DLL HICON Bitmap_ToIcon(HBITMAP BitmapHandle, int Width, int Height);

JOHN_DLL BOOL Bitmap_TransparentBlt(
	HDC hdcDest,
	int nXOriginDest,
	int nYOriginDest,
	int nWidthDest,
	int nHeightDest,
	HDC hdcSrc,
	int nXOriginSrc,
	int nYOriginSrc,
	int nWidthSrc,
	int nHeightSrc,
	UINT crTransparent
);

//bmp only
JOHN_DLL BOOL Bitmap_ToFile(HBITMAP hBitmap, const wchar_t* lpFileName);
//support bmp,jpg,png,gif,tif
//nQuality: 0~100
JOHN_DLL BOOL Bitmap_ToFileEx(HBITMAP hBitmap, const wchar_t* lpFileName, BYTE nQuality/*=90*/);

JOHN_DLL void Bitmap_Flip(HBITMAP bitmap, int width, int height);

// 加载(.BMP .DIB .EMF .GIF .ICO .JPG .WMF)
JOHN_DLL HBITMAP Bitmap_Load(const wchar_t *pszFileName, COLORREF clrBack/*=-1*/);
JOHN_DLL HBITMAP Bitmap_LoadFromBuffer(const BYTE* pData, DWORD cbData, COLORREF clrBack/*=-1*/);



// date time relate

//闰年?
inline bool Time_IsLeapYear(int y)
{
	return ((y % 4) == 0 && (y % 100) != 0) || (y % 400) == 0;
}

//一个time_t具体意味着什么时间，是由用户赋予的时区决定的。
//它本身没有存储时区信息。

//JOHN_ASSERT(sizeof(int64_t) == sizeof(time_t));

// 2019-12-20 12:30:50
// at china, localTime = UTC+8
JOHN_DLL const char* Time_LocalTimeStr(int64_t t);
// 2019年12月20日 12:30:50
JOHN_DLL const wchar_t* Time_LocalTimeStr_Chn(int64_t t);

// 把输入的t, 按UTC+0转换成日期字符串
// 2019-12-20 4:30:50
JOHN_DLL const char* Time_GMTTimeStr(int64_t t);

// 2019_12_15_20_04_54_246
JOHN_DLL const char* Time_NowForFileNameA();
JOHN_DLL const wchar_t* Time_NowForFileNameW();

// at china, localTime = UTC+8
// at US, localTime = UTC-5
JOHN_DLL char* Time_LocalTimeNowStr(char* buffer, size_t buffer_size);
JOHN_DLL char* Time_UTCNowStr(char* buffer, size_t buffer_size);

//timestamp from local time zone
JOHN_DLL int64_t Time_LocalTimeStamp();

//make a __time64_t
//int64_t ts = Time_MakeTime(2019, 12, 20, 0, 0, 0, 0); //1576771200
//const char* pstr = Time_LocalTimeStr(ts); //2019-12-20 00:00:00
JOHN_DLL int64_t Time_MakeTime(int nYear, int nMonth, int nDay,
							   int nHour, int nMin, int nSec, int nDST/*=0*/);

// __time64_t ---> SYSTEMTIME
JOHN_DLL bool Time_GetAsSystemTime(const int64_t* ptime, SYSTEMTIME* ptimeDest);
// FILETIME ---> __time64_t
JOHN_DLL uint64_t Time_FileTimeToUint64(const FILETIME* pft);
JOHN_DLL bool Time_SecondsSince1601(unsigned year, unsigned month, unsigned day,
	                                unsigned hour, unsigned min, unsigned sec, uint64_t* presSeconds);

JOHN_DLL double Time_DiffSecond(int64_t tEnd, int64_t tStart);
JOHN_DLL double Time_DiffMinutes(int64_t tEnd, int64_t tStart);
JOHN_DLL double Time_DiffHours(int64_t tEnd, int64_t tStart);
JOHN_DLL double Time_DiffDays(int64_t tEnd, int64_t tStart);

JOHN_DLL BOOL OS_CurrentCpuTime(uint64_t* ptimNow, uint64_t* ptimUser, uint64_t* ptimKernel);

// Return current system clock as milliseconds. 
//ms，13236255568034
// 这个值会因为系统的时间更改而改变，使用时要注意。
JOHN_DLL int64_t Time_ClockMS();

//current monotonic clock in milliseconds
//ms， 208293938
// 这个值不会因为系统的时间更改，可用于特定场合。
JOHN_DLL int64_t Time_ClockMono();

//current monotonic clock in microseconds
// us，208293938136
JOHN_DLL int64_t Time_ClockUSec();

/*
用于常见英文日期字符串解释：
	Sun, 06 Nov 1994 08:49:37 GMT  ; RFC 822, updated by RFC 1123
	Sunday, 06-Nov-94 08:49:37 GMT ; RFC 850, obsoleted by RFC 1036
	Sun Nov  6 08:49:37 1994       ; ANSI C's asctime() format
	06 Nov 1994 08:49:37 GMT
	06-Nov-94 08:49:37 GMT
	Nov  6 08:49:37 1994
	06 Nov 1994 08:49:37
	06-Nov-94 08:49:37
	Sun, 12 Sep 2004 15:05:58 -0700
	Sat, 11 Sep 2004 21:32:11 +0200
	1994 Nov 6

	20040912 15:05:58 -0700
	20040911 +0200
	2004/09/12 15:05:58
	2020-03-31 11:11:11
	2004/09/12
	2020-03-31
*/
//vs2005+默认情况下, sizeof(time_t)==8
//return:
// 0 -- ok
// -1 -- failed
// 1 -- later
// 2 -- sooner
// 不带时区的话，返回的time_t是UTC+0，要转换回字面日期的话，可以：
//Time_Parse(pstrDateTim, &tim);
//char* pstrTim = Time_GMTTimeStr(tim);
JOHN_DLL int Time_Parse(const char *date, int64_t *ptimOut);

//UTC+0 --> UTC+x (在中国是UTC+8)
//时区变大了，所以返回值变小了。
// 1592245682 ---> 1592216882
JOHN_DLL int64_t Time_UTC2Local(int64_t tim);

//get exe name from HWND
JOHN_DLL BOOL Win_GetExeName(HWND hwnd, wchar_t* szExeName, int cchExeName, DWORD* pdwProcessId/*=NULL*/);
JOHN_DLL BOOL Win_Center(HWND hWnd, HWND hWndContainer/* = NULL*/);

//nFlag for SetWindowPos
JOHN_DLL BOOL Win_ModifyStyle(
	HWND hWnd,
	DWORD dwRemove,
	DWORD dwAdd,
	UINT nFlags/* = 0*/);
JOHN_DLL BOOL Win_ModifyStyleEx(
	HWND hWnd,
	DWORD dwRemove,
	DWORD dwAdd,
	UINT nFlags/* = 0*/);

// 返回hr错误号的解释
// 需要Mem_free()释放
// hr 可以是GetLastError()的返回值，也可以是COM函数返回值。
// dwLangId = MAKELANGID LANG_NEUTRAL, SUBLANG_DEFAULT);
// 常用的是LANG_SYSTEM_DEFAULT, LANG_USER_DEFAULT
// english: MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US)
//
JOHN_DLL
char* Win_GetHRMessagePtrA(
	HRESULT hr,
	DWORD dwLangId/* = 0*/);

JOHN_DLL 
wchar_t* Win_GetHRMessagePtrW(
			HRESULT hr,
			DWORD dwLangId/* = 0*/);

//窗口半透明
// iAlpha = 0~100
JOHN_DLL void Win_SetTransparentMode(HWND hwnd, BYTE iAlpha);

//简单输出调试信息，默认缓冲区为320个字符
JOHN_DLL void FORCE_CDECL Debug_TraceA(const char *format, ...);
JOHN_DLL void FORCE_CDECL Debug_TraceW(const wchar_t *format, ...);


//Path,Direcotry, File
//use COM, please remeber to call CoInitializeEx
// pszLnkDir是存放*.lnk的目录
JOHN_DLL BOOL Path_CreateLnk(const wchar_t* pszLnkDir, const wchar_t* pszPath);
//是否 *.lnk
JOHN_DLL BOOL Path_IsLnkFile(const wchar_t* pszPath);
JOHN_DLL BOOL Path_GetLnkPath(const wchar_t* pszLnkFile, wchar_t* pszResPath, int cchResPath);
//如果pszPath是指向目录的lnk，那么输出其指向到pszResPath
JOHN_DLL BOOL Path_IsLnkToDirectory(const wchar_t* pszPath, wchar_t* pszResPath, int cchResPath);

//是否以 \ 结尾
JOHN_DLL bool Path_IsSlashEndA(const char* pstrPath);
JOHN_DLL bool Path_IsSlashEndW(const wchar_t* pstrPath);

// bKeepExist=true 保证pstrPath后面有一个 \ 结尾
// 用户自己保证pstrPath应当有多一个字符的内存，以备加上 \ 
JOHN_DLL bool Path_KeepSlashEndA(char* pstrPath, bool bKeepExist/*=true*/);
JOHN_DLL bool Path_KeepSlashEndW(wchar_t* pstrPath, bool bKeepExist/*=true*/);

// c:\123\, file.txt ---> c:\123\file.txt
// c:\123, file.txt  ---> c:\123\file.txt
// 其中一个为空的话，返回另一个的拷贝
JOHN_DLL char* Path_MakeFilePathPtrA(const char* pstrDir, const char* pstrFile);
JOHN_DLL wchar_t* Path_MakeFilePathPtrW(const wchar_t* pstrDir, const wchar_t* pstrFile);

// >0成功，<=0失败
// cchPath应当包含NULL
JOHN_DLL int Path_CurrentDirA(char* path, int cchPath);
JOHN_DLL int Path_CurrentDirW(wchar_t *path, int cchPath);

// "d:\\temp\\..\\mydir\\program\\.\\..\\..\\22.txt" ---> "d:\\22.txt"
//碰上无效路径的话，尽量让它规整。"d:\\\\..\\..\\..\\..\\22.txt" --> "d:\\22.txt"
// 返回简化后的实际字符数
JOHN_DLL int Path_Canonicalize(wchar_t* path);

/*
* 与Path_Canonicalize功能一样，不过修正无效路径的方式不一样。
* "d:\\\\..\\..\\..\\..\\22.txt" --> "\22.txt"
* "d:\\\\..\\..\\..\\..\\."      --> "\."
*
*  Simplify a filename into its canonical form
*  by making the following changes :
*
*   * removing any trailing and duplicate /
*   * convert / . / into just /
*   * convert / A / .. / where A is any simple name into just /
*/
JOHN_DLL int Path_SimplifyNameA(char *pszBuf, int cchBuf);
JOHN_DLL int Path_SimplifyNameW(wchar_t *pszBuf, int cchBuf);

// 用于长路径，缩略显示，不可逆
// C:\\Program Files (x86)\\HTML Help Workshop\include\\htmlhelp.h --> C:\Program Files ...\htmlhelp.h
JOHN_DLL bool Path_Compact(const wchar_t* lpstrIn, wchar_t* lpstrOut, int cchOut);


/**
 * Extract directory path portion of filename.
 *
 * Returns a string containing the directory path portion of the passed
 * filename.  If there is no path in the passed filename an empty string
 * will be returned (not NULL).
 *
 * bKeepLastSlash 如果路径最后有/或\的话，是否保留?
 *
 * Path_GetPathPtrA("C:\\", false) == "C:"
 * Path_GetPathPtrA("C:\\", true) == "C:\\"
 * Path_GetPathPtrA("C:\\Users\\", false) == "C:\\Users"
 * Path_GetPathPtrA("C:\\Users\\", true) == "C:\\Users\\"
 * Path_GetPathPtrA( "abc/def.xyz", false) == "abc"
 * Path_GetPathPtrA( "/abc/def/", false) == "/abc/def"
 * Path_GetPathPtrA( "/", false) == "/"
 * Path_GetPathPtrA( "/abc/def", false) == "/abc"
 * Path_GetPathPtrA( "abc", false) == ""
 */
JOHN_DLL char *Path_GetPathPtrA(const char *pszFilename, bool bKeepLastSlash/*=false*/);
JOHN_DLL wchar_t *Path_GetPathPtrW(const wchar_t *pszFilename, bool bKeepLastSlash/*=false*/);

/*
*  Path_GetBasenameA("C:\\Users\\johnsing\\desktop\\123.txt") == "123.txt"
*  Path_GetBasenameA("C:\\Users\\johnsing\\desktop\\") == ""
*  Path_GetBasenameA( "abc/def.xyz" ) == "def.xyz"
*  Path_GetBasenameA( "/abc/def/" ) == ""
*  Path_GetBasenameA( "abc/def" ) == "def"
*/
JOHN_DLL const char* Path_GetBasenameA(const char *pszFullFilename);
JOHN_DLL const wchar_t* Path_GetBasenameW(const wchar_t *pszFullFilename);

//生成exe目录下文件路径
// pstrFile为空则，返回exe所有目录。例如： d:\\johnsingdebug\\ 
JOHN_DLL char* Path_MakeExePathFilePtrA(HMODULE module_handle/*=NULL*/, const char* pstrFile);
JOHN_DLL wchar_t* Path_MakeExePathFilePtrW(HMODULE module_handle/*=NULL*/, const wchar_t* pstrFile);

//Is path like: c:\xxx.txt
JOHN_DLL bool Path_IsFullPathA(const char* szPath);
JOHN_DLL bool Path_IsFullPathW(const wchar_t* szPath);

//弹出选择路径框，选择路径
JOHN_DLL wchar_t* Path_SelectFolderPtr(HWND hWnd, const wchar_t* pstrDefaultDir, const wchar_t* pstrTitle);

//弹出通用对话框，选择文件名
//缓冲区至少应当为 FILENAME_MAX(260)
JOHN_DLL
bool OS_GetFileName(HWND hwnd,
	const wchar_t* dlgtitle/*=NULL*/,
	const wchar_t* initDir/*=NULL*/,
	wchar_t* pstrFileName,
	size_t cchFileName,
	bool save);

// 把文件(夹)静默地移到回收站，出错时也不报错。
// bPhyDelete=TRUE时，实施物理删除，不允许撤销
JOHN_DLL BOOL Path_Recycle(const wchar_t* pszPath, BOOL bPhyDelete/*=FALSE*/);

// cchFileName = MAX_PATH, API限制，再大也没什么用。
JOHN_DLL HRESULT Path_GetTempFilenameA(char* pstrFilename, int cchFileName, const char* pszPrefix/*=NULL*/);
JOHN_DLL HRESULT Path_GetTempFilenameW(wchar_t* pstrFilename, int cchFileName, const wchar_t* pszPrefix/*=NULL*/);

// ".\\"   --> "d:\\johntest\\"
// "."     --> "d:\\johntest"
// ".\\dir\\myfile.txt"     --> "d:\\johntest\\dir\\myfile.txt"
JOHN_DLL wchar_t* Path_GetAbsolutePath(const wchar_t *pstrRelatePath, wchar_t *pstrAbsPath, size_t cchAbs);
JOHN_DLL wchar_t* Path_GetAbsolutePathPtr(const wchar_t* path);

typedef enum _eSpecialPath {
	SP_Desktop,    /*桌面虚拟目录*/
	SP_DesktopDir, /*C:\Documents and Settings\ username\Desktop*/
	SP_AppData,    /*C:\Documents and Settings\ username\Application Data*/
	SP_LocalAppData, /*C:\Documents and Settings\ username\Local Settings\Application Data*/
	SP_MyDocument,   /*C:\Documents and Settings\ username\My Documents*/
	SP_Cookies,      /*IE Cookies, C:\Documents and Settings\ username\Cookies*/	
	SP_ProgramFiles, /*C:\Program Files*/
	SP_ProgramFilesX86, /*C:\Program Files(x86)*/
	SP_StartMenu, /*C:\Documents and Settings\ username\Start Menu*/
	SP_StartUp,   /*C:\Documents and Settings\ username\Start Menu\Programs\Startup*/
	SP_System     /*C:\Windows\System32*/
}ESpecialPath;
//win10 v1903
/*
SP_Desktop    --> C:\Users\Johnsing\Desktop
SP_DesktopDir --> C:\Users\Johnsing\Desktop
SP_AppData    --> C:\Users\Johnsing\AppData\Roaming
SP_LocalAppData  --> C:\Users\Johnsing\AppData\Local
SP_MyDocument    --> C:\Users\Johnsing\Documents
SP_Cookies       --> C:\Users\Johnsing\AppData\Local\Microsoft\Windows\INetCookies
SP_ProgramFiles    --> x86 application, C:\Program Files (x86)
SP_ProgramFilesX86 --> C:\Program Files (x86)
SP_StartMenu --> C:\Users\Johnsing\AppData\Roaming\Microsoft\Windows\Start Menu
SP_StartUp   --> C:\Users\Johnsing\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Startup
SP_System    --> C:\WINDOWS\system32
*/

// 返回的路径末尾是没有slash的
// 如果pstrDirOrFilename有东西的话，把pstrDirOrFilename也加上
JOHN_DLL wchar_t* Path_GetSpecialPathPtr(const wchar_t* pstrDirOrFilename/*=NULL*/, ESpecialPath kind);


// 根据地址来返回模块所有的路径
// 通常用于ocx, dll之类的
// sample: Path_ModulePathFromAddrPtr(&Path_ModulePathFromAddrPtr, NULL); ==> "d:\johnsingtest"
JOHN_DLL wchar_t* Path_ModulePathFromAddrPtr(void* ptrAddr, HMODULE* poModule/*=NULL*/);

// 检查文件或文件夹是否存在
// bCheckDir == true时检查文件夹
JOHN_DLL bool Path_IsExist(const wchar_t* filepath_in, bool bCheckDir);

// 一次创建多层目录
JOHN_DLL BOOL Path_CreateDirRecur(const wchar_t* full_path);
// 删除多层目录，与Path_Recycle功能一样
// pszPath以\\结尾，容量最少应当为wcslen(pszPath)+2
// 返回值是GetLastError(), 成功ERROR_SUCCESS
JOHN_DLL DWORD Path_DeleteDirRecur(wchar_t* pszPath);


// 扫描，返回所有匹配的文件的全路径
// extTypeFilter应当类似于： "*.dll", "123?.txt"
// 使用SListDeepFree释放内存
SList* Path_GetFilesInFolderPtrW(const wchar_t* inFolder, const wchar_t* extTypeFilter);

//取得已加载dll模块的位置
// bShortName==true,路径过长的话，转换成DOS短路径
JOHN_DLL
BOOL Path_GetModuleDirByHandle(
	HMODULE hMod,
	BOOL bShortName,
	wchar_t*  pszBuf,
	DWORD   cchBuf);

// moduleName不分大小写，不带后缀的话，默认是dll
JOHN_DLL
BOOL Path_GetLoadedModuleDir(
	const wchar_t* moduleName,
	BOOL bShortName,
	wchar_t*  pszBuf,
	DWORD   cchBuf);

/* File_MiniCopy
*	fIn:		Source file pathname.
*	fOut:		Destination file pathname.
*   return:     -1 文件操作失败
*               -2 文件过大
*               -3 mapping file error
*                0 ok
* for x86, 只支持小文件拷贝
 */
JOHN_DLL int File_MiniCopy(const wchar_t *src, const wchar_t *dst);

// 负数表示失败
JOHN_DLL int File_ReadByHandle(HANDLE fd, void *buf, DWORD cbBuf);
JOHN_DLL int File_WriteByHandle(HANDLE fd, void *buf, DWORD cbBuf);
JOHN_DLL int File_ReadByPath(const wchar_t *filepath, void *data_out, size_t size);
JOHN_DLL int File_WriteByPath(const wchar_t *filepath, bool bCreateAlways/*=false*/, const void *data, size_t size);

JOHN_DLL BOOL File_IsLocked(const wchar_t* lpszFile);
JOHN_DLL BOOL File_SetWritable(const wchar_t *path, BOOL bWritable);
JOHN_DLL BOOL File_SetHidden(const wchar_t *path, BOOL hidden);
//修改最后访问时间
JOHN_DLL BOOL File_Touch(const wchar_t* path);

//Config file
typedef struct TConf_tag TConf;

typedef enum
{
	// note: 测试是否成功，应该用 (retval >= 0)
	TCE_OK = 0,        //No error
	TCE_UPDATED = 1,   //An existing value was updated
	TCE_INSERTED = 2,  //A new value was inserted

	// note: 测试是否失败，应该用 (retval < 0)
	TCE_FAIL = -1,    //Generic failure
	TCE_NOMEM = -2,   //Out of memory error
	TCE_FILE = -3     //File error (see errno for detail error)
} TCONF_ERR;

//文本的编码
typedef enum
{
	TConf_DT_MBCS,
	TConf_DT_UTF8,
	TConf_DT_WCHAR
} TConf_DataType;

JOHN_DLL TConf* Conf_new(TConf_DataType charType/*=TConf_DT_MBCS*/, bool bCaseSent/*=false*/, bool bMultiLine/*=false*/);
JOHN_DLL void Conf_free(TConf* pconf);
JOHN_DLL void Conf_reset(TConf* pconf);

JOHN_DLL void Conf_SetDataType(TConf* pconf, TConf_DataType dt);
JOHN_DLL TConf_DataType Conf_GetDataType(const TConf* pconf);
JOHN_DLL void Conf_SetMultiLine(TConf* pconf, bool bMultiLine/*=false*/);
JOHN_DLL bool Conf_IsMultiLine(const TConf* pconf);
JOHN_DLL void Conf_SetUseSpaces(TConf* pconf, bool bSpaces/* = true*/);
JOHN_DLL bool Conf_IsUsingSpaces(const TConf* pconf);
JOHN_DLL bool Conf_IsEmpty(const TConf* pconf);

//加载数据
JOHN_DLL TCONF_ERR Conf_LoadFileA(TConf* pconf, const char* pszFile);
JOHN_DLL TCONF_ERR Conf_LoadFileW(TConf* pconf, const wchar_t* pwszFile);
JOHN_DLL TCONF_ERR Conf_LoadData(TConf* pconf, const BYTE* byData, size_t byLen);

JOHN_DLL TCONF_ERR Conf_SaveFileA(TConf* pconf, const char* pszFile, bool bAddSign);
JOHN_DLL TCONF_ERR Conf_SaveFileW(TConf* pconf, const wchar_t* pwszFile, bool bAddSign);
JOHN_DLL TCONF_ERR Conf_SaveDataBuf(TConf* pconf, Johnbuf* buf, bool bAddSign);

//Section下key的个数
JOHN_DLL int Conf_GetSectionKeyCount(TConf* pconf, const wchar_t* pSection);

//SList内存放的wchar_t*由内部管理，不需要释放。仅需要释放SList*
JOHN_DLL SList* Conf_GetAllSections(TConf* pconf);
JOHN_DLL SList* Conf_GetSectionAllKeys(TConf* pconf, const wchar_t* pSection);

JOHN_DLL
const wchar_t* Conf_GetValueW(
	TConf* pconf,
	const wchar_t* pstrSection,
	const wchar_t* pstrKey,
	const wchar_t* pstrDefault/*=NULL*/
);

//mbcs版本，要自己Mem_free
JOHN_DLL
char* Conf_GetValueAPtr(
	TConf* pconf,
	const char* pstrSection,
	const char* pstrKey,
	const char* pstrDefault/*=NULL*/
);

//创建Section或key
JOHN_DLL
TCONF_ERR Conf_SetValueW(
	TConf* pconf,
	const wchar_t* pstrSection,
	const wchar_t* pstrKey,
	const wchar_t* pstrValue,
	const wchar_t* pstrComment/* = NULL*/
);

JOHN_DLL
TCONF_ERR Conf_SetValueA(
	TConf* pconf,
	const char* pstrSection,
	const char* pstrKey,
	const char* pstrValue,
	const char* pstrComment/* = NULL*/
);

JOHN_DLL
bool Conf_GetBoolValue(
	TConf* pconf,
	const wchar_t* pstrSection,
	const wchar_t* pstrKey,
	bool           bDefault/*=false*/
);

JOHN_DLL
TCONF_ERR Conf_SetBoolValue(
	TConf* pconf,
	const wchar_t* pstrSection,
	const wchar_t* pstrKey,
	bool           bValue,
	const wchar_t* pstrComment /*= NULL*/
);

//支持10进制数字，以及0x12345678十六进制字符串
JOHN_DLL
long Conf_GetLongValue(
	TConf* pconf,
	const wchar_t* pstrSection,
	const wchar_t* pstrKey,
	long           nDefault/*=0*/
);

JOHN_DLL
TCONF_ERR Conf_SetLongValue(
	TConf* pconf,
	const wchar_t*  pstrSection,
	const wchar_t*  pstrKey,
	long            nValue,
	const wchar_t*  pstrComment/* = NULL*/,
	bool            bUseHex/* = false*/
);

JOHN_DLL
double Conf_GetDoubleValue(
	TConf* pconf,
	const wchar_t* pstrSection,
	const wchar_t* pstrKey,
	double         nDefault/*=0.0*/
);

JOHN_DLL
TCONF_ERR Conf_SetDoubleValue(
	TConf* pconf,
	const wchar_t*  pstrSection,
	const wchar_t*  pstrKey,
	double          nValue,
	const wchar_t*  pstrComment/* = NULL*/
);

JOHN_DLL
int64_t Conf_GetInt64Value(
	TConf* pconf,
	const wchar_t* pstrSection,
	const wchar_t* pstrKey,
	int64_t        nDefault/*=0LL*/
);

JOHN_DLL
TCONF_ERR Conf_SetInt64Value(
	TConf* pconf,
	const wchar_t*  pstrSection,
	const wchar_t*  pstrKey,
	int64_t         nValue,
	const wchar_t*  pstrComment/* = NULL*/,
	bool            bUseHex/* = false*/
);


JOHN_DLL 
bool Conf_DeleteValue(
	TConf*         pconf,
	const wchar_t* pstrSection,
	const wchar_t* pstrKey
);

JOHN_DLL
bool Conf_DeleteSection(
	TConf* pconf,
	const wchar_t* pstrSection,
	bool           bRemoveEmpty/*=true*/
);


//Resource
JOHN_DLL bool Resource_Load(HMODULE hMod, LPCWSTR Type, LPCWSTR ID, HRSRC* hoResource, HGLOBAL* hoGlobal);
JOHN_DLL bool Resource_LoadWithLang(HMODULE hMod, LPCWSTR Type, LPCWSTR ID, WORD wLanguage, HRSRC* hoResource, HGLOBAL* hoGlobal);
JOHN_DLL DWORD Resource_GetSize(HMODULE hMod, HRSRC hResource);
JOHN_DLL LPVOID Resource_Lock(HGLOBAL hGlobal);
JOHN_DLL void Resource_Release(HGLOBAL hGlobal);

//直接load RT_RCDATA
JOHN_DLL LPVOID Resource_LoadData(HMODULE hMod, const wchar_t* resId, DWORD* pdwSize/*=NULL*/);

/*
计算时间差：
	timStart = PerfTimer_GetTime();
	Sleep(100);
	timEnd = PerfTimer_GetTime();
	double dblMS = PerfTimer_ToMilliseconds(timEnd - timStart); //100.56ms
*/

JOHN_DLL int64_t PerfTimer_GetTime();
JOHN_DLL int64_t PerfTimer_GetPerfTimerFreq();

// us, 微秒, 1us = 1ms/1000;
JOHN_DLL double PerfTimer_ToMicroseconds(const double perfTime);
// ms, 毫秒
JOHN_DLL double PerfTimer_ToMilliseconds(const double perfTime);
// s
JOHN_DLL double PerfTimer_ToSeconds(const double perfTime);

JOHN_DLL uint64_t PerfTimer_FromMicroseconds(const double microseconds);
JOHN_DLL uint64_t PerfTimer_FromMilliseconds(const double milliseconds);
JOHN_DLL uint64_t PerfTimer_FromSeconds(const double seconds);


//检测是否有特定格式数据在clipboard
// pformats以0作为终结
JOHN_DLL bool Clipboard_ContainsFormat(const UINT* pformats);
JOHN_DLL bool Clipboard_HasImage();
JOHN_DLL bool Clipboard_HasString();
JOHN_DLL bool Clipboard_IsFormatAvailableHDROP();
JOHN_DLL char* Clipboard_GetStringPtrA();
JOHN_DLL wchar_t* Clipboard_GetStringPtrW();
JOHN_DLL bool Clipboard_SetStringA(const char* str);
JOHN_DLL bool Clipboard_SetStringW(const wchar_t* str);

JOHN_DLL bool Clipboard_SetData(UINT uFormat, const void *data, size_t size);
JOHN_DLL bool Clipboard_SetBitmap(HBITMAP hBitmap);

// ciper, hash

// MD5 hash
typedef struct _MD5Context Hash_MD5_CTX;
JOHN_DLL Hash_MD5_CTX* Hash_MD5_new();
JOHN_DLL void Hash_MD5_free(Hash_MD5_CTX* context);
JOHN_DLL void Hash_MD5_Update(Hash_MD5_CTX *context, unsigned char const *buf, unsigned len);
//pdigest 至少要16个字节
JOHN_DLL void Hash_MD5_Final(Hash_MD5_CTX *context, unsigned char* pdigest);
// 一步到位
JOHN_DLL char* Hash_MD5_StrPtr(const char *pszText);

// SHA1 hash
typedef struct _Hash_SHA1_CTX Hash_SHA1_CTX;

JOHN_DLL Hash_SHA1_CTX* Hash_SHA1_new();
JOHN_DLL void Hash_SHA1_free(Hash_SHA1_CTX *ctx);
JOHN_DLL int Hash_SHA1_Update(Hash_SHA1_CTX *ctx, const void *data, size_t len);
//pdigest 至少要20个字节
JOHN_DLL int Hash_SHA1_Final(Hash_SHA1_CTX *ctx, BYTE* pdigest);
// 一步到位
JOHN_DLL char* Hash_SHA1_StrPtr(const char *pszText);


typedef void* TCiper;
typedef void* THash;

typedef enum _ECliperKind {
	ESSH_BLOWFISH256_CBC,
	ESSH_BLOWFISH256_SDCTR,
	ESSH_BLOWFISH128_CBC,

	ESSH_AES256_SDCTR,
	ESSH_AES256_SDCTR_HW,
	ESSH_AES256_SDCTR_SW,
	ESSH_AES256_CBC,
	ESSH_AES256_CBC_HW,
	ESSH_AES256_CBC_SW,
	ESSH_AES192_SDCTR,
	ESSH_AES192_SDCTR_HW,
	ESSH_AES192_SDCTR_SW,
	ESSH_AES192_CBC,
	ESSH_AES192_CBC_HW,
	ESSH_AES192_CBC_SW,
	ESSH_AES128_SDCTR,
	ESSH_AES128_SDCTR_HW,
	ESSH_AES128_SDCTR_SW,
	ESSH_AES128_CBC,
	ESSH_AES128_CBC_HW,
	ESSH_AES128_CBC_SW,

	ESSH_3DES_InnerCBC,
	ESSH_3DES_SDCTR,
	ESSH_3DES_CBC,
	ESSH_DES_CBC //single-DES CBC
}ECliperKind;

JOHN_DLL TCiper Ciper_new(ECliperKind kind);
JOHN_DLL void Ciper_free(TCiper c);
JOHN_DLL void Ciper_require_datalen(TCiper c, int* pnIVorBlkLen, int* pnKey);
JOHN_DLL void Ciper_setiv(TCiper c, const void *iv);
JOHN_DLL void Ciper_setkey(TCiper c, const void *key);
//方便使用的字符串版本
JOHN_DLL bool Ciper_setivStr(TCiper c, const char *iv);
JOHN_DLL bool Ciper_setkeyStr(TCiper c, const char *key);

//len应当是pnIVorBlkLen的整数倍
JOHN_DLL void Ciper_encrypt(TCiper c, void *blk, int len);
JOHN_DLL void Ciper_decrypt(TCiper c, void *blk, int len);

//封装以上两个方法
//从pRawData复制数据到blk，不足blkLen的话，填充NULL。再实施加密，解密。
//// Sample:
/*
int nRemaing = nRawData;
const BYTE* pSrc = (BYTE*)pstrDecode;
for (BYTE* ptr = (BYTE*)pstrEncoded;
	nRemaing > 0;
	nRemaing -= nIVorBlkLen)
{
	int nData = min(nIVorBlkLen, nRemaing);
	Ciper_CryptEx(pciperDec, false, ptr, nIVorBlkLen, pSrc, nData);
	ptr += nData;
	pSrc += nData;
}
*/
JOHN_DLL bool Ciper_CryptEx(TCiper c, bool bEncrypt, void *blk, int blkLen, const void* pRawData, int rawDataLen);

typedef enum _EHashKind
{
	ESSH_SHA256,    //32bytes结果
	ESSH_SHA256_HW,
	ESSH_SHA256_SW,
	ESSH_SHA384,   //48bytes结果
	ESSH_SHA512    //64bytes结果
}EHashKind;

JOHN_DLL THash Hash_new(EHashKind kind);
JOHN_DLL void Hash_free(THash h);
JOHN_DLL void Hash_require_datalen(THash h, int* pnBlockLen/*=NULL*/, int* pnResult);
JOHN_DLL bool Hash_put_data(THash h, void* data, unsigned int data_len);
//outval的大小，应该至少是pnResult的大小
JOHN_DLL bool Hash_final(THash h, unsigned char* outval);


////////////////////////////////////////////////////////
JOHN_C_END


#ifdef __cplusplus
//for clear object
template <typename T>
class CAutoMemFree
{
public:
	CAutoMemFree(T* ptr):m_ptr(ptr) {}
	~CAutoMemFree() { Mem_free(m_ptr); }
	inline operator T*()const { return m_ptr; }
	inline T* getPtr() { return m_ptr; }
	inline const T* getPtr()const { return m_ptr; }
	inline T* operator->() { return m_ptr; }
	inline const T* operator->()const { return m_ptr; }

	CAutoMemFree& operator=(T* ptr)
	{
		Mem_free(m_ptr);
		m_ptr = ptr;
		return *this;
	}
private:
	CAutoMemFree();
	T* m_ptr;
};

#endif  /* __cplusplus */


#endif // _SINGLIB2019_JSH_