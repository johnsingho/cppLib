#ifndef COMMRTL_H_6865_2020_07_15_
#define COMMRTL_H_6865_2020_07_15_
#pragma once
/*****************************************************************************
* Name     : common header for CommRtl
* Creator  : H.Z.XIN
* Date     : 2020-07-15
* Function : 工具宏，函数
* Parameter:
* Return(s):
* Notices  :
------------------------------------------------------------------------------
Change Log:
  2020-07-15        H.Z.XIN        Create
  2020-09-10        H.Z.XIN        update nettool
  2020-09-18        H.Z.XIN        update CryptTool
*****************************************************************************/

#if defined(_MSC_VER)
//编译dll时定义此宏
#if defined(COMMRTL_EXPORTS)
#	define COMMRTL_DLL __declspec(dllexport)
#elif defined(COMMRTL_IMPORTS)
//使用dll时定义此宏
#	define COMMRTL_DLL __declspec(dllimport)
#else
//啥也不定义可用作静态库
#	define COMMRTL_DLL
#endif
#	define COMMRTL_LOCAL
#elif defined (__CYGWIN__)
#   define COMMRTL_DLL
#else
#if (defined __GNUC__ && __GNUC__ >= 4) || defined __INTEL_COMPILER
#if defined(COMMRTL_EXPORTS) || defined(COMMRTL_IMPORTS)
#define COMMRTL_DLL  __attribute__ ((visibility ("default")))
#define COMMRTL_LOCAL  __attribute__ ((visibility ("hidden")))
#else
#define COMMRTL_DLL
#define COMMRTL_LOCAL
#endif
#else
#define COMMRTL_DLL
#define COMMRTL_LOCAL
#endif
#endif

#if defined(_MSC_VER) && !defined(FORCE_CDECL)
#  define FORCE_CDECL  __cdecl
#else
#  define FORCE_CDECL
#endif


#if defined(_MSC_VER) && _MSC_VER < 0x0708
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

//////////////////////////////////////////////////////

typedef enum _ETypeID
{
	EType_None             = 0,
	EType_Debug            = 0x10,
	EType_DebugNull        = 0x11,
	EType_ObjectVector     = 0x20, //多线程安全
	EType_DoubleLinkedList = 0x21, 
	EType_StringVectorA    = 0x22, //多线程安全
	EType_StringVectorW    = 0x23, //多线程安全	
	EType_StringMapA       = 0x24, //多线程安全
	EType_StringMapW       = 0x25, //多线程安全
	EType_DynBuf           = 0x26,
	EType_EnumWindow       = 0x30,
	EType_EnumFont         = 0x31,
	EType_EnumProcess      = 0x32,
	EType_EnumFile         = 0x33,
	EType_SharedMemory     = 0x40,
	EType_IPCMessage       = 0x41,
	EType_StringTool       = 0x50,
	EType_NumberTool       = 0x51,
	EType_ProcessTool      = 0x52,
	EType_RegisterTool     = 0x53,
	EType_ServiceTool      = 0x54,
	EType_DiskTool         = 0x55,
	EType_FilePathTool     = 0x56,
	EType_DriverTool       = 0x57,
	EType_IniOper          = 0x58,
	EType_IniOper2         = 0x59,
	EType_HashTool         = 0x5A,
	EType_CryptTool        = 0x5B,
	EType_ImageTool        = 0x5C,
	EType_DateTimeTool     = 0x5D,
	EType_PETool           = 0x5E,
	EType_ClipboardTool    = 0x5F,
	EType_NetTool          = 0x60,
	EType_NetNetAdapterSet = 0x61,
	EType_NetUDPServer     = 0x62,
	EType_NetUDPClient     = 0x63,
	EType_NetTCPServer     = 0x64,
	EType_NetTCPClient     = 0x65,
	EType_OSWinTool        = 0x80,
	EType_PerformTimer     = 0x81,
}ETypeID;

struct COMMRTL_DLL IRtlBase
{
	IRtlBase(ETypeID tid)
		:m_nRef(0)
		, m_typeid(tid)
	{
	}
	virtual ~IRtlBase(){}

	unsigned int AddRef();
	unsigned int Release();
private:
	volatile unsigned int m_nRef;
	ETypeID      m_typeid;
};


extern "C"
{
COMMRTL_DLL IRtlBase* QueryInterface(ETypeID tid);

// memory relate
COMMRTL_DLL void* Mem_malloc(size_t size);
//除了参数区别外，*calloc会初始化内存块为0，而*malloc不会。
COMMRTL_DLL void* Mem_calloc(size_t num, size_t size);
COMMRTL_DLL void Mem_free(void* ptr);
COMMRTL_DLL void* Mem_realloc(void* ptr, size_t size);
#ifndef Mem_mallocTP
#define Mem_mallocTP(tp)                ((tp*)Mem_malloc(sizeof(tp)))
#define Mem_callocTP(tp,n)              ((tp*)Mem_calloc((n),sizeof(tp)))
#define Mem_Amalloc(cch)				((char*)Mem_malloc(cch))
#define Mem_Wmalloc(cch)				((wchar_t*)Mem_malloc(sizeof(wchar_t)*cch))
#endif//Mem_mallocTP

// mbcs OEM char* <----> wchar_t*
COMMRTL_DLL bool Str_MbcsToUnicode(const char* pszStr, int cchStr/*=-1*/, UINT cp/*=CP_ACP*/, wchar_t* poWstr, int cchWstr);
COMMRTL_DLL bool Str_UnicodeToMbcs(const wchar_t* pszSrc, int cchSrc/*=-1*/, UINT cp/*=CP_ACP*/, char* poStr, int cchStr);

COMMRTL_DLL wchar_t* Str_MbcsToUnicodePtr(const char* szStr, UINT cp/*=CP_ACP*/);
COMMRTL_DLL char* Str_UnicodeToMbcsPtr(const wchar_t* szStr, UINT cp/*=CP_ACP*/);

COMMRTL_DLL wchar_t* Str_Utf8ToUnicodePtr(const char* utf8Str);
COMMRTL_DLL char* Str_UnicodeToUtf8Ptr(const wchar_t* UnicodeStr);

COMMRTL_DLL char* Str_MbcsToUtf8Ptr(const char* strSource, UINT cp/* = CP_ACP*/);
COMMRTL_DLL char* Str_Utf8ToMbcsPtr(const char* strSource, UINT cp/* = CP_ACP*/);

// nSrc是字节数
COMMRTL_DLL wchar_t* Str_MbcsToUnicodeNPtr(const BYTE* bySrc, size_t nSrc, UINT cp/*=CP_ACP*/);
COMMRTL_DLL wchar_t* Str_Utf8ToUnicodeNPtr(const BYTE* bySrc, size_t nSrc);

//如果pstrDumpFile为空的话，输出到%temp%
//如果是 (wchar_t*)20170701 的话，输出到exe所在目录
COMMRTL_DLL void Process_SetMiniDump(const wchar_t* pstrDumpPath/*=NULL*/, DWORD dumpType/*=(DWORD)MiniDumpNormal*/);
};


struct IStringVectorA : public IRtlBase
{
public:
	IStringVectorA():IRtlBase(EType_StringVectorA){}
	virtual int Add(const char* szString) =0;
	//return added count
	virtual int AddList(const char* pszList, char chDivider) = 0;
	//return n deleted
	virtual void Remove(const char* lpSearch, bool bCaseSent) = 0;
	virtual void Remove(int i) = 0;
	//return position
	virtual int FindString(const char* lpSearch, bool bCaseSent) = 0;
	virtual uint32_t GetCount() = 0;
	//return cch output, including NULL
	virtual int GetString(int i, char* psOut, int cchOut = -1) = 0;
	// "123", "456", "78" ----> "123;456;78\0"
	//return n chars out, including the last NULL
	virtual uint32_t ExtractToBuffer(char* wszBuffer, const char chDivider = ';') = 0;
	// including the last NULL
	virtual uint32_t GetExtractBufferSize() = 0;
};

struct IStringVectorW : public IRtlBase
{
public:
	IStringVectorW() :IRtlBase(EType_StringVectorW) {}
	virtual int Add(const wchar_t* szString) = 0;
	//return added count
	virtual int AddList(const wchar_t* pszList, wchar_t chDivider) = 0;
	//return n deleted
	virtual void Remove(const wchar_t* lpSearch, bool bCaseSent) = 0;
	virtual void Remove(int i) = 0;
	//return position
	virtual int FindString(const wchar_t* lpSearch, bool bCaseSent) = 0;
	virtual uint32_t GetCount() = 0;
	//return cch output, including NULL
	virtual int GetString(int i, wchar_t* psOut, int cchOut = -1) = 0;
	// "123", "456", "78" ----> "123;456;78\0"
	//return n chars out, including the last NULL
	virtual uint32_t ExtractToBuffer(wchar_t* wszBuffer, const wchar_t chDivider = L';') = 0;
	// including the last NULL
	virtual uint32_t GetExtractBufferSize() = 0;
};


class IObjectVector : public IRtlBase
{
public:
	//用于尝试释放元素。不设定的话不释放元素。
	typedef void(*PfnFreeItem)(void*);

	IObjectVector()
		: IRtlBase(EType_ObjectVector)
		, m_pfnFree(NULL)
	{}

	inline void SetPfnFree(PfnFreeItem pfn=NULL) { m_pfnFree = pfn; }
	virtual void Add(void* pObject) = 0;
	virtual uint32_t GetCount() = 0;
	virtual bool Remove(int i, void** ppObject = NULL) = 0;
	virtual bool GetAt(int i, void** ppObject) = 0;
	virtual void Clear() = 0;
protected:
	PfnFreeItem m_pfnFree;
};

class IDoubleLinkedList : public IRtlBase
{
public:
	typedef struct _DList_node_t DList_node_t;

	//以下三个函数会施加在每个元素上，都有默认值的，按需提供吧。
	// Destruct an item
	typedef void (Pfn_DList_destructor)(void **item);
	// Duplicate an item
	typedef void * (Pfn_DList_duplicator)(const void *item);
	// Compare two items, for sorting
	typedef int (Pfn_DList_comparator)(const void *item1, const void *item2);


	IDoubleLinkedList()
		: IRtlBase(EType_DoubleLinkedList)
	{}

	virtual DList_node_t* AddStart(void* item)=0;
	virtual DList_node_t* AddEnd(void* item) = 0;

	//  Return the number of items in the list
	virtual size_t Size() const = 0;

	virtual void* PickHead() = 0;
	virtual void* PickTail() = 0;

	virtual void* GetFirst() = 0;
	virtual void* GetNext() = 0;
	virtual void* GetPrev() = 0;
	virtual void* GetLast() = 0;

	virtual void* CurData() = 0;
	virtual DList_node_t* Current() = 0;

	virtual void* GetNodeData(DList_node_t* handle) = 0;

	virtual DList_node_t* Find(void* item) = 0;

	virtual void* Detach(DList_node_t* handle) = 0;
	virtual void* DetachCur() = 0;

	// -1失败，0成功
	virtual int Delete(DList_node_t* handle) = 0;

	virtual void MoveStart(DList_node_t* handle) = 0;
	virtual void MoveEnd(DList_node_t* handle) = 0;

	virtual void DeleteAll() = 0;

	virtual void Sort() = 0;

	//插入，如果原来是有序的话，lowValue==true时，从小到大搜索找位置。
	virtual DList_node_t* Insert(void* item, bool lowValue) = 0;

	virtual IDoubleLinkedList* Clone() = 0;

	// 设置子元素析构器。默认情况下是不释放子元素的。
	virtual void SetDestructor(Pfn_DList_destructor destructor) = 0;

	// 设置子元素复制器。默认情况下是不深度复制子元素的。
	virtual void SetDuplicator(Pfn_DList_duplicator duplicator) = 0;

	// 设置子元素比较器。用于find和sort.
	virtual void SetComparator(Pfn_DList_comparator comparator) = 0;
};


class IDebugPrint : public IRtlBase
{
public:
	typedef enum
	{
		LOG_TO_DEBUGGER = 0x01,
		LOG_TO_FILE = 0x02
	}ELogType;

	IDebugPrint(ETypeID tid) : IRtlBase(tid) {}
	virtual void Create(uint32_t logType, const wchar_t* wszFileName) = 0;
	virtual void Create(uint32_t logType, const char* wszFileName) = 0;
	virtual void FORCE_CDECL PrintA(const  char*    format, ...) = 0;
	virtual void FORCE_CDECL PrintW(const  wchar_t* format, ...) = 0;
	virtual void DumpBin(const void* pData, int Length) = 0;
};


class IWndEnum : public IRtlBase
{
public:
	IWndEnum() : IRtlBase(EType_EnumWindow) {}

	virtual BOOL Init(HWND hWndParent) = 0;
	virtual void Reset()=0;
	virtual uint32_t GetCount() = 0;
	virtual BOOL GetWnd(int index, HWND* hWnd) = 0;
	// pstrClass or pstrTitle can NULL
	virtual BOOL FindByClassTitleA(const char* pstrClass, const char* pstrTitle, HWND* pHwnd) = 0;
	virtual BOOL FindByClassTitleW(const wchar_t* pstrClass, const wchar_t* pstrTitle, HWND* pHwnd) = 0;
};

class IFontEnum : public IRtlBase
{
public:
	IFontEnum() : IRtlBase(EType_EnumFont) {}

	virtual BOOL Init(HWND hWnd)=0;
	virtual uint32_t GetCount()=0;
	virtual const LOGFONTW* GetFont(int index)=0;
};


class IFileEnum;
class IFileEnumReceiver
{
public:
	virtual ~IFileEnumReceiver() { };

	friend class IFileEnum;
	virtual DWORD  OnFileFound(IFileEnum* FileEnum, const wchar_t* wszFileName) = 0;
	virtual DWORD  OnCompleted(IFileEnum* FileEnum) = 0;
};

class IFileEnum : public IRtlBase
{
public:
	IFileEnum() : IRtlBase(EType_EnumFile) {}

	virtual void Init(const wchar_t* pstrInitDir, 
					  const wchar_t* pstrFilter, BOOL bScanSubDir, IFileEnumReceiver* Receiver) = 0;
	virtual BOOL StartScan(BOOL bWaitFinish) = 0;
	virtual void AbortScan() = 0;
	virtual BOOL PauseScan() = 0;
	virtual BOOL ResumeScan() = 0;
};

class IStringMapA : public IRtlBase
{
public:
	IStringMapA() : IRtlBase(EType_StringMapA) {}

	virtual bool Add(const char* pKeyBuffer, void* p) = 0;
	virtual uint32_t GetCount() = 0;
	virtual void* GetAt(int nIndex) = 0;
	//return the value
	virtual void* Get(const char* pstrKey) = 0;
	virtual void Remove(const char* pstrKey) = 0;
};
class IStringMapW : public IRtlBase
{
public:
	IStringMapW() : IRtlBase(EType_StringMapW) {}

	virtual bool Add(const wchar_t* pKeyBuffer, void* p) = 0;
	virtual uint32_t GetCount() = 0;
	//return the value
	virtual void* GetAt(int nIndex) = 0;
	virtual void* Get(const wchar_t* pstrKey) = 0;
	virtual void Remove(const wchar_t* pstrKey) = 0;
};

class ISharedMem : public IRtlBase
{
public:
	ISharedMem() : IRtlBase(EType_SharedMemory) {}

	//for server
	virtual BOOL  Create(const wchar_t* pstrMappingName, uint32_t Size)=0;
	//for client
	virtual BOOL Open(const wchar_t* pstrMappingName, uint32_t Size)=0;
	virtual BOOL ReadBuffer(void* poBuf, int nBufLen)=0;
	virtual BOOL WriteBuffer(const void* pData, int nDataLen)=0;
	virtual void ZeroBuffer()=0;
	virtual uint32_t GetBufferSize()=0;
};


// 基于共享内存的IPC
class IIpcMessage : public IRtlBase
{
public:
	typedef void (*PfnProcess)(DWORD, DWORD, PBYTE);
	enum {
		DEFAULT_SHARED_MEMORY_SIZE=1024*1024,
		DEFAULT_SHARED_MEMORY_BUFFERS=2
	};

	IIpcMessage() : IRtlBase(EType_IPCMessage) {}
	//name是共享内存名，只需简单命名就够了，内部会进行拼接
	virtual bool Open(const wchar_t* name) =0;
	virtual bool Create(const wchar_t* name, UINT size = DEFAULT_SHARED_MEMORY_SIZE, UINT buffers = DEFAULT_SHARED_MEMORY_BUFFERS) =0;
	virtual void Close() =0;
	virtual bool StartProcessing(PfnProcess pcbFunc, bool needsCOM = false) =0;
	virtual void StopProcessing() =0;
	virtual bool Send(DWORD type, DWORD size, PBYTE pData) =0;
	//放回接收缓冲区
	virtual bool SendBack(DWORD type, DWORD size, PBYTE pData) =0;
};


typedef struct
{
	DWORD   dwPID;
	DWORD   dwParentPID;
	WCHAR   wszImage[MAX_PATH];
}MyPROCESS_INFO, *PMyPROCESS_INFO;

class IProcessEnum : public IRtlBase
{
public:
	IProcessEnum() : IRtlBase(EType_EnumProcess) {}

	virtual BOOL Init()=0;
	virtual uint32_t GetCount()=0;
	virtual BOOL GetAt(int nIndex, MyPROCESS_INFO* pProcInfo)=0;
};

class IProcessTool : public IRtlBase
{
public:
	IProcessTool() : IRtlBase(EType_ProcessTool) {}
	
	//need to CloseHandle
	virtual HANDLE ProcessID2Handle(DWORD dwPid)=0;
	virtual DWORD GetParentProcessId(DWORD pid) = 0;
	virtual BOOL GetProcNameByPID(DWORD dwPID, wchar_t* pszExeNamme, int cchExeName) = 0;
	//MyPROCESS_INFO
	virtual IObjectVector* GetSubProcessIDs(DWORD dwParentProcID) = 0;
	virtual BOOL IsWow64Process(HANDLE hProcess/*= GetCurrentProcess()*/)= 0;
	virtual BOOL Is64BitPorcess(DWORD dwProcessID, BOOL& bRes)=0;
	virtual BOOL IsProcessIdExist(DWORD dwProcID) = 0;
	virtual DWORD FindProcessID(const wchar_t* wszExeName) = 0;
	//DWORD
	virtual IObjectVector* FindProcessIDs(const wchar_t* wszExeName) = 0;

	virtual BOOL KillProcess(DWORD dwPid) = 0;
	virtual BOOL KillProcess(const wchar_t* wszExeName) = 0;

	virtual BOOL RunProcess(const wchar_t* wszExePathName) = 0;
	virtual BOOL RunProcess(const wchar_t* wszExePathName, const wchar_t* wszCmdLine) = 0;
	virtual BOOL RunDOSCommandW(const wchar_t* pszCommand, BOOL bWaitForExit = true, DWORD* pwExitCode = NULL)=0;
	virtual BOOL RunProcessAsUser(const wchar_t* wszExePathName, const wchar_t* wszCmdLine) = 0;
	virtual BOOL RunProcessAsWinlogin(const wchar_t* wszExePathName, const wchar_t* wszCmdLine) = 0;

	//如果pstrDumpFile为空的话，输出到%temp%
	//dumpType is MINIDUMP_TYPE
	virtual void SetMiniDump(const wchar_t* pstrDumpFile=NULL, DWORD dumpType=0)=0;
	virtual void SetDllHijackingProtection() = 0;
};

class IRegTool : public IRtlBase
{
public:
	IRegTool() : IRtlBase(EType_RegisterTool) {}

	virtual BOOL CreateKey(HKEY hKey, const wchar_t* wszKeyPath, const wchar_t* lpKeyName)=0;
	//including all sub keys
	virtual BOOL DeleteKey(HKEY hKey, const wchar_t* wszKeyPath, const wchar_t* lpKeyName) = 0;

	virtual BOOL GetStringValue(HKEY hKEY, const wchar_t* wszPath, const wchar_t* wszValueName, wchar_t* pstrOut, int cchOut) = 0;
	virtual BOOL SetStringValue(HKEY hKEY, const wchar_t* wszPath, const wchar_t* wszValueName, const wchar_t* wszValue) = 0;

	virtual BOOL GetDWORDValue(HKEY hKEY, const wchar_t* wszPath, const wchar_t* wszValueName, DWORD* dwValue) = 0;
	virtual BOOL SetDWORDValue(HKEY hKEY, const wchar_t* wszPath, const wchar_t* wszValueName, DWORD dwValue) = 0;
};

class IPerformTimer : public IRtlBase
{
public:
	IPerformTimer() : IRtlBase(EType_PerformTimer) {}

	virtual int64_t GetTimerFreq() const = 0;
	virtual int64_t GetTime() = 0;
	
	//与上次调用GetTime()的时间差
	// s
	virtual double GetDiffSec() const = 0;
	// ms
	virtual double GetDiffMS() const = 0;
	// us, 微秒
	virtual double GetDiffMicroSeconds() const = 0;
};

class IServiceTool : public IRtlBase
{
public:
	IServiceTool() : IRtlBase(EType_ServiceTool) {}

	virtual BOOL CreateSrv(
		const wchar_t* wszServiceName,
		const wchar_t* wszBinPath,
		const wchar_t* wszDescription,
		DWORD  dwServiceType,
		DWORD  dwStartType,
		DWORD  dwErrorControl
	)=0;

	virtual BOOL OpenSrv(const wchar_t* wszServiceName)=0;

	virtual BOOL DeleteSrv()=0;
	virtual BOOL IsServiceRunning()=0;
	virtual BOOL Start()=0;
	virtual BOOL Stop(DWORD msWait)=0;
};

class IDynBuf : public IRtlBase
{
public:
	IDynBuf() : IRtlBase(EType_DynBuf) {}
	virtual void Reset()=0;
	virtual BYTE* Data()=0;
	virtual size_t DataLength()=0;

	//-1失败
	//可能会发生大幅度扩张(sz*1.4)
	virtual int AppendBytes(const BYTE* data, size_t size)=0;
};

class IStringTool : public IRtlBase
{
public:
	IStringTool() : IRtlBase(EType_StringTool) {}

	//need Mem_free
	virtual char* FORCE_CDECL SprintfPtr(const char* fmt, ...)=0;
	virtual wchar_t* FORCE_CDECL SprintfPtr(const wchar_t* fmt, ...)=0;

	virtual char* FORCE_CDECL CatPtr(const char* pstrFirst, ...) = 0;
	virtual wchar_t* FORCE_CDECL CatPtr(const wchar_t* pstrFirst, ...) = 0;

	virtual IStringVectorA* StrToken(const char* input, const char* delimitor, bool bTrim, bool bRemoveEmpty=true) = 0;
	virtual IStringVectorW* StrToken(const wchar_t* input, const wchar_t* delimitor, bool bTrim, bool bRemoveEmpty=true) = 0;

	virtual bool StartWith(const char* haystack, const char* needle, bool bCaseSence = true) = 0;
	virtual bool StartWith(const wchar_t* haystack, const wchar_t* needle, bool bCaseSence = true) = 0;
	virtual bool EndWith(const char* haystack, const char* needle, bool bCaseSence = true) = 0;
	virtual bool EndWith(const wchar_t* haystack, const wchar_t* needle, bool bCaseSence = true) = 0;

	virtual const char* Stristr(const char* str, const char* substr) = 0;
	virtual const wchar_t* Stristr(const wchar_t* str, const wchar_t* substr) = 0;

	//不区分大小写比较
	virtual int CaseCompare(const char* pstr1, const char* pstr2) = 0;
	virtual int CaseCompare(const wchar_t* pstr1, const wchar_t* pstr2) = 0;

	//支持*，?匹配
	// WildCompare("hello,Johnsing,123", "hello*123") --> true
	// WildCompare("hello,Johnsing,123", "hELLO*sing*3") --> false
	// WildCompare("hello,Johnsing,123", "*John*.png") --> false
	// WildCompare("hello,Johnsing,123", "hello*1?3") --> true
	virtual bool WildCompare(const char *pstrLong, const char *wild) = 0;
	virtual bool WildCompare(const wchar_t *pstrLong, const wchar_t *wild) = 0;

	virtual bool IsWhitespaceA(char ch) = 0;
	virtual bool IsWhitespaceW(wchar_t ch) = 0;
	virtual bool TrimInplace(char* lpString) = 0;
	virtual bool TrimInplace(wchar_t* lpString) = 0;

	//分隔符相关
	virtual void SkipUntil(const char** pp, const char* pdelim) = 0;
	virtual void SkipUntil(const wchar_t** pp, const wchar_t* pdelim) = 0;
	virtual void SkipOver(const char** pp, const char* pdelim) = 0;
	virtual void SkipOver(const wchar_t** pp, const wchar_t* pdelim) = 0;

	// MultiSZ
	/*-------------------------------+
	/*| 134\0 | 456\0 | CAT\0 | \0\0 |
	*+--------+-------+-------+------+
	*/
	virtual char* ToMultiSZPtr(IStringVectorA* pList) = 0;
	virtual wchar_t* ToMultiSZPtr(IStringVectorW* pList) = 0;
	virtual IStringVectorA* FromMultiSZ(const char* ppstrmz) = 0;
	virtual IStringVectorW* FromMultiSZ(const wchar_t* ppstrmz) = 0;

	virtual bool MbcsToUnicode(const char* pszStr, int cchStr/*=-1*/, UINT cp/*=CP_ACP*/, wchar_t* poWstr, int cchWstr) = 0;
	virtual bool UnicodeToMbcs(const wchar_t* pszSrc, int cchSrc/*=-1*/, UINT cp/*=CP_ACP*/, char* poStr, int cchStr) = 0;

	virtual wchar_t* MbcsToUnicodePtr(const char* szStr, UINT cp=CP_ACP) = 0;
	virtual char* UnicodeToMbcsPtr(const wchar_t* szStr, UINT cp=CP_ACP) = 0;

	virtual wchar_t* Utf8ToUnicodePtr(const char* utf8Str) = 0;
	virtual char* UnicodeToUtf8Ptr(const wchar_t* UnicodeStr) = 0;

	virtual char* MbcsToUtf8Ptr(const char* strSource, UINT cp=CP_ACP) = 0;
	virtual char* Utf8ToMbcsPtr(const char* strSource, UINT cp=CP_ACP) = 0;

	// 朱镕基只在GBK，不在GB2312
	virtual bool IsLegalGB2312(const void* stream, unsigned length) = 0;
	virtual bool IsLegalGBK(const void* stream, unsigned length) = 0;
	virtual bool IsLegalBIG5(const void* stream, unsigned length) = 0;

	// base64编码，返回输出 strlen(dest)+1
	// 失败的话返回-1
	// 需要自己加上NULL
	virtual size_t Base64Encode(char* dest, const char* src, size_t srcLen) = 0;
	// base64解码，返回输出 strlen(dest)
	// 失败的话返回-1
	// 需要自己加上NULL
	virtual size_t Base64Decode(char* dest, const char* src, size_t srcLen) = 0;
	//预估大小
	virtual size_t Base64EncodeLen(size_t srcLen) = 0;
	virtual size_t Base64DecodeLen(size_t srcLen) = 0;

	virtual char* ToHexStrPtr(int nBytes, const BYTE* pabyData) = 0;
	virtual BYTE* FromHexStrPtr(const char* pszHex, int* pnBytes/*=NULL*/) = 0;

	//简单加密
	virtual void EncryptMy(BYTE* lpbBuf, size_t iLen)=0;
	virtual void DecryptMy(BYTE* lpbBuf, size_t iLen)=0;
};

class INumberTool : public IRtlBase
{
public:
	INumberTool() : IRtlBase(EType_NumberTool) {}

	virtual bool RandomBytes(uint32_t len, BYTE *buf)=0;
	virtual bool RandomStr(size_t cchBuf, char* buf) = 0;
	//产生随机数[min, max]
	virtual int32_t RandomInt32Range(int32_t min, int32_t max) = 0;
	// [min, max)
	virtual double RandomDoubleRange(double min, double max) = 0;
	//bHasSpiltter==false, cchBuf至少要有32+1个字符 "6609F82E963F4C8DBE8FA0F40E2C9656"
	//bHasSpiltter==true, cchBuf至少要有36+1个字符. "E5723717-133F-42D2-B81A-C64E22E2F111"
	virtual bool GenUUID(char* buffer, size_t cchBuf, bool bHasSpiltter=false) = 0;

	//四舍五入
	// -3.81 --> -4
	// 3.81  --> 4
	virtual int RoundInt(double value) = 0;
	// nPrecision=多少位小数
	// Num_RoundDouble(3.115, 2) --> 3.12
	// Num_RoundDouble(-3.115, 2) --> -3.12
	virtual double RoundDouble(double doValue, int nPrecision) = 0;

	virtual bool FloatsEqual(const double& num1, const double& num2) = 0;

	// 数字与字符串互转, 不需要释放字符串内存
	virtual const char*    ToStrInt16A(int16_t val) = 0;
	virtual const wchar_t* ToStrInt16W(int16_t val) = 0;
	virtual const char*    ToStrUInt16A(uint16_t val) = 0;
	virtual const wchar_t* ToStrUInt16W(uint16_t val) = 0;
	virtual const char*    ToStrInt32A(int32_t val) = 0;
	virtual const wchar_t* ToStrInt32W(int32_t val) = 0;
	virtual const char*    ToStrUInt32A(uint32_t val) = 0;
	virtual const wchar_t* ToStrUInt32W(uint32_t val) = 0;
	virtual const char*    ToStrInt64A(const int64_t& val) = 0;
	virtual const wchar_t* ToStrInt64W(const int64_t& val) = 0;
	virtual const char*    ToStrUInt64A(const uint64_t& val) = 0;
	virtual const wchar_t* ToStrUInt64W(const uint64_t& val) = 0;

	//最多保留4位小数
	//340282346638528859811704183484516925440.0000
	virtual const char*    ToStrFloatA(float val) = 0;
	virtual const wchar_t* ToStrFloatW(float val) = 0;

	//最多保留6位小数, Mem_free释放内存
	virtual char*    ToStrDoublePtrA(double val) = 0;
	virtual wchar_t* ToStrDoublePtrW(double val) = 0;

	// "true"
	// "false"
	virtual const char*    ToStrBoolA(bool val) = 0;
	virtual const wchar_t* ToStrBoolW(bool val) = 0;

	virtual int32_t FromStrInt32A(const char* pstr) = 0;
	virtual int32_t FromStrInt32W(const wchar_t* pstr) = 0;
	virtual int64_t FromStrInt64A(const char* pstr) = 0;
	virtual int64_t FromStrInt64W(const wchar_t* pstr) = 0;

	//支持十进制字符串，也支持0x12345678十六进制字符串
	virtual uint32_t FromStrUInt32A(const char* pstr) = 0;
	virtual uint32_t FromStrUInt32W(const wchar_t* pstr) = 0;
	virtual uint64_t FromStrUInt64A(const char* pstr) = 0;
	virtual uint64_t FromStrUInt64W(const wchar_t* pstr) = 0;

	virtual float  FromStrFloatA(const char* pstr) = 0;
	virtual float  FromStrFloatW(const wchar_t* pstr) = 0;
	virtual double FromStrDoubleA(const char* pstr) = 0;
	virtual double FromStrDoubleW(const wchar_t* pstr) = 0;
};

class IFilePathTool : public IRtlBase
{
public:
	IFilePathTool() : IRtlBase(EType_FilePathTool) {}	

	//字节数
	virtual bool GetFileSize(const wchar_t* path, uint64_t* pusize) = 0;

	//生成exe目录下文件路径
	// pstrFile为空则，返回exe所有目录。例如： d:\\johnsingdebug\\ 
	virtual char* MakeExePathFilePtrA(HMODULE module_handle/*=NULL*/, const char* pstrFile) = 0;
	virtual wchar_t* MakeExePathFilePtrW(HMODULE module_handle/*=NULL*/, const wchar_t* pstrFile) = 0;
	
	virtual BOOL GetLinkPath(const wchar_t* pszLnkFile, wchar_t* pszResPath, int cchResPath) = 0;

	//是否以 \ 结尾
	virtual bool IsSlashEndA(const char* pstrPath) = 0;
	virtual bool IsSlashEndW(const wchar_t* pstrPath) = 0;

	// bKeepExist=true 保证pstrPath后面有一个 \ 结尾
	// 用户自己保证pstrPath应当有多一个字符的内存，以备加上 \ 
	virtual bool KeepSlashEndA(char* pstrPath, bool bKeepExist=true) = 0;
	virtual bool KeepSlashEndW(wchar_t* pstrPath, bool bKeepExist=true) = 0;

	// c:\123\, file.txt ---> c:\123\file.txt
	// c:\123, file.txt  ---> c:\123\file.txt
	// 其中一个为空的话，返回另一个的拷贝
	virtual char* MakeFilePathPtrA(const char* pstrDir, const char* pstrFile) = 0;
	virtual wchar_t* MakeFilePathPtrW(const wchar_t* pstrDir, const wchar_t* pstrFile) = 0;

	// >0成功，<=0失败
	// cchPath应当包含NULL
	virtual int CurrentDirA(char* path, int cchPath) = 0;
	virtual int CurrentDirW(wchar_t *path, int cchPath) = 0;

	// "d:\\temp\\..\\mydir\\program\\.\\..\\..\\22.txt" ---> "d:\\22.txt"
	//碰上无效路径的话，尽量让它规整。"d:\\\\..\\..\\..\\..\\22.txt" --> "d:\\22.txt"
	// 返回简化后的实际字符数
	virtual int CanonicalizePath(wchar_t* path) = 0;

	//弹出选择路径框，选择路径
	virtual wchar_t* SelectFolderPtr(HWND hWnd, const wchar_t* pstrDefaultDir, const wchar_t* pstrTitle) = 0;

	//弹出通用对话框，选择文件名
	//缓冲区至少应当为 FILENAME_MAX(260)	
	virtual bool GetFileName(HWND hwnd,
			const wchar_t* dlgtitle/*=NULL*/,
			const wchar_t* initDir/*=NULL*/,
			wchar_t* pstrFileName,
			size_t cchFileName,
			bool save) = 0;


	// 把文件(夹)静默地移到回收站，出错时也不报错。
	// bPhyDelete=TRUE时，实施物理删除，不允许撤销
	virtual BOOL RecyclePath(const wchar_t* pszPath, BOOL bPhyDelete=FALSE) = 0;

	// cchFileName = MAX_PATH, API限制，再大也没什么用。
	virtual BOOL GetTempFilenameA(char* pstrFilename, int cchFileName, const char* pszPrefix=NULL) = 0;
	virtual BOOL GetTempFilenameW(wchar_t* pstrFilename, int cchFileName, const wchar_t* pszPrefix=NULL) = 0;

	// ".\\"   --> "d:\\johntest\\"
	// "."     --> "d:\\johntest"
	// ".\\dir\\myfile.txt"     --> "d:\\johntest\\dir\\myfile.txt"
	virtual wchar_t* GetAbsolutePath(const wchar_t *pstrRelatePath, wchar_t *pstrAbsPath, size_t cchAbs) = 0;

	// 检查文件或文件夹是否存在
	// bCheckDir == true时检查文件夹
	virtual BOOL IsPathExist(const wchar_t* filepath_in, bool bCheckDir) = 0;

	// 一次创建多层目录
	virtual BOOL CreateDirRecur(const wchar_t* full_path) = 0;
	// 删除多层目录，与Path_Recycle功能一样
	// pszPath以\\结尾，容量最少应当为wcslen(pszPath)+2
	// 返回值是GetLastError(), 成功ERROR_SUCCESS
	virtual BOOL DeleteDirRecur(const wchar_t* pszPath) = 0;


	// 扫描，返回所有匹配的文件的全路径
	// extTypeFilter应当类似于： "*.dll", "123?.txt"
	// 使用SListDeepFree释放内存
	virtual IStringVectorW* GetFilesInFolderPtrW(const wchar_t* inFolder, const wchar_t* extTypeFilter) = 0;


	// 负数表示失败
	virtual int ReadFileByHandle(HANDLE fd, void *buf, DWORD cbBuf) = 0;
	virtual int WriteFileByHandle(HANDLE fd, void *buf, DWORD cbBuf) = 0;
	virtual int ReadFileByPath(const wchar_t *filepath, void *data_out, size_t size) = 0;
	virtual int WriteFileByPath(const wchar_t *filepath, bool bCreateAlways/*=false*/, const void *data, size_t size) = 0;

	virtual BOOL IsFileLocked(const wchar_t* lpszFile) = 0;
	virtual BOOL SetFileWritable(const wchar_t *path, BOOL bWritable) = 0;
	virtual BOOL SetFileHidden(const wchar_t *path, BOOL hidden) = 0;
	//修改最后访问时间
	virtual BOOL TouchFile(const wchar_t* path)=0;

	//auto delete after close
	virtual HANDLE GetTempFile() = 0;
};

class IHashTool : public IRtlBase
{
public:
	IHashTool() : IRtlBase(EType_HashTool) {}

	virtual uint32_t Crc32(uint32_t crc, const unsigned char* buf, size_t len)=0;
	virtual uint64_t Crc64(const void *data, size_t size) = 0;
	
	virtual char* MD5_StrPtr(const char *pszText) = 0;
	virtual char* SHA1_StrPtr(const char *pszText) = 0;

};

/* i5-5200 2.2GHZ
*Encrypt [AES128],		 us=1194.700000, ms=1.195600, s=0.001196
*Decrypt [AES128],		 us=387.400000, ms=0.388100, s=0.000389
*Encrypt [AES256],		 us=155.800000, ms=0.156500, s=0.000157
*Decrypt [AES256],		 us=265.900000, ms=0.266400, s=0.000267
*Encrypt [AES256_SDCTR], us=146.100000, ms=0.146700, s=0.000147
*Decrypt [AES256_SDCTR], us=254.100000, ms=0.254600, s=0.000255
*Encrypt [DES_CBC],		 us=393.100000, ms=0.393800, s=0.000394
*Decrypt [DES_CBC],		 us=469.300000, ms=0.470200, s=0.000471
*Encrypt [ThreeDES_CBC], us=1168.900000, ms=1.169800, s=0.001170
*Decrypt [ThreeDES_CBC], us=788.500000, ms=0.789600, s=0.000790
*Encrypt [BlowFish128_CBC], us=1650.000000, ms=1.651000, s=0.001651
*Decrypt [BlowFish128_CBC], us=486.500000, ms=0.487200, s=0.000488
*Encrypt [BlowFish256_CBC], us=317.800000, ms=0.318500, s=0.000319
*Decrypt [BlowFish256_CBC], us=455.400000, ms=0.456100, s=0.000456
*/
class ICryptTool : public IRtlBase
{
public:
	ICryptTool() : IRtlBase(EType_CryptTool) {}

	typedef enum {
		DES_CBC,
		ThreeDES_CBC,
		BlowFish128_CBC,
		BlowFish256_CBC,
		AES128_CBC,
		AES256_CBC,
		AES256_SDCTR
	}EcryptMethod;

	virtual bool Init(EcryptMethod eMethod, const char* pstrKey, const char* pstrIV = NULL)=0;
	//加密后数据>=原始数
	virtual size_t CalcEncryptedDataLen(size_t cbData)=0;

	//ccbOut不要小于CalcEncryptedDataLen()的返回值
	//pbout允许与pRawData一样
	virtual int Encrypt(void* pbout, int ccbOut, const void* pRawData, int cbRawData)=0;
	//pbout允许与pEncryptData一样
	virtual int Decrypt(void* pbout, int ccbOut, const void* pEncryptData, int cbEncryptData)=0;
};

class IImageTool : public IRtlBase
{
public:
	IImageTool() : IRtlBase(EType_ImageTool) {}

	virtual HBITMAP Bitmap_FromRect(HWND hWnd, RECT* pRect)=0;
	virtual HBITMAP Bitmap_FromScreenRect(RECT* lpRect, BYTE *pData=NULL, BITMAPINFO *pHeader=NULL)=0;
	//bmp only
	virtual BOOL Bitmap_ToFile(HBITMAP hBitmap, const wchar_t* lpFileName)=0;
	//support bmp,jpg,png,gif,tif
	//nQuality: 0~100
	virtual BOOL Bitmap_ToFileEx(HBITMAP hBitmap, const wchar_t* lpFileName, BYTE nQuality=90)=0;
	virtual HBITMAP Bitmap_Load(const wchar_t *pszFileName, COLORREF clrBack=0)=0;
	virtual HBITMAP Bitmap_LoadFromBuffer(const BYTE* pData, DWORD cbData, COLORREF clrBack=0)=0;
};

class IDateTimeTool : public IRtlBase
{
public:
	IDateTimeTool() : IRtlBase(EType_DateTimeTool) {}

	// 2019-12-20 12:30:50
	// at china, localTime = UTC+8
	virtual const char* LocalTimeStr(const int64_t& t)=0;
	// 2019年12月20日 12:30:50
	virtual const wchar_t* LocalTimeStr_Chn(const int64_t& t) = 0;

	// 2019_12_15_20_04_54_246
	virtual const char* NowForFileNameA() = 0;
	virtual const wchar_t* NowForFileNameW() = 0;

	//timestamp from local time zone
	virtual int64_t LocalTimeStamp() = 0;

	//time_t比较
	virtual double DiffSecond(const int64_t& tEnd, const int64_t& tStart) = 0;
	virtual double DiffMinutes(const int64_t& tEnd, const int64_t& tStart) = 0;
	virtual double DiffHours(const int64_t& tEnd, const int64_t& tStart) = 0;
	virtual double DiffDays(const int64_t& tEnd, const int64_t& tStart) = 0;

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
	//Parse(pstrDateTim, &tim);
	//char* pstrTim = GMTTimeStr(tim);
	virtual int Parse(const char *date, int64_t *ptimOut)=0;
};

class IPETool : public IRtlBase
{
public:
	IPETool() : IRtlBase(EType_PETool) {}

	typedef struct {
		int major;
		int minor;
		int build;
		int revis;
	}win_version_info;

	//path可以是全路径，也可以是dll名
	//如： File_GetVersion(L"kernel32", &ver);
	virtual bool GetPEVersion(const wchar_t* path, win_version_info* info)=0;
	virtual char* GetModuleVerPtr(HMODULE hMod/*=NULL*/)=0;
};

class IClipboardTool : public IRtlBase
{
public:
	IClipboardTool() : IRtlBase(EType_ClipboardTool) {}

	//检测是否有特定格式数据在clipboard
	// pformats以0作为终结
	virtual bool ContainsFormat(const UINT* pformats)=0;
	virtual bool HasImage() = 0;
	virtual bool HasString() = 0;
	virtual bool IsFormatAvailableHDROP() = 0;
	virtual char* GetStringPtrA() = 0;
	virtual wchar_t* GetStringPtrW() = 0;
	virtual bool SetStringA(const char* str) = 0;
	virtual bool SetStringW(const wchar_t* str) = 0;

	virtual bool SetData(UINT uFormat, const void *data, size_t size) = 0;
	virtual bool SetBitmap(HBITMAP hBitmap) = 0;

};


class IOSWinTool : public IRtlBase
{
public:
	IOSWinTool() : IRtlBase(EType_OSWinTool) {}

	virtual BOOL Is64BitOS()=0;
	virtual BOOL DisableUACByReg()=0;
	virtual bool IsWindowsVistaOrGreater() = 0;

	virtual BOOL IsUserAdmin() = 0;
	//return 0x0603 --> win8.1
	virtual uint16_t GetOSVersion(char* postrVer, int cchVer)=0;	
	virtual int GetNumOfProcessCore() = 0;
	virtual BOOL GetPhysicalRAMSize(uint64_t* pnTotalPhys, uint64_t* pnAvailPhys) = 0;
	virtual char* GetMachineGuidPtr() = 0;

	virtual bool GetHostNamePtr(wchar_t* pstrOut, unsigned int cchOut)=0;

	//修改系统环境变量,影响系统
	virtual BOOL SetSystemEnvW(bool bUserEnv, const wchar_t* pstrKeyName, const wchar_t* pstrVal)=0;
	virtual BOOL DeleteSystemEnvW(bool bUserEnv, const wchar_t* pstrKeyName, const wchar_t* pstrVal)=0;

	virtual BOOL GetActiveSID(DWORD* posid)=0;

	virtual BOOL WinGetExeName(HWND hwnd, wchar_t* szExeName, int cchExeName, DWORD* pdwProcessId=NULL) = 0;
	virtual BOOL WinCenter(HWND hWnd, HWND hWndContainer=NULL) = 0;
	//窗口半透明,iAlpha = 0~100
	virtual void WinSetTransparentMode(HWND hwnd, BYTE iAlpha) = 0;


	// 返回hr错误号的解释
	// 需要Mem_free()释放
	// hr 可以是GetLastError()的返回值，也可以是COM函数返回值。
	// dwLangId = MAKELANGID LANG_NEUTRAL, SUBLANG_DEFAULT);
	// 常用的是LANG_SYSTEM_DEFAULT, LANG_USER_DEFAULT
	// english: MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US)
	virtual char* GetHRMessagePtrA(HRESULT hr, DWORD dwLangId = 0) = 0;
	virtual wchar_t* GetHRMessagePtrW(HRESULT hr, DWORD dwLangId=0)=0;
};


typedef struct _MyNET_ADAPTER_INFO
{
	wchar_t  wszAdapterName[128];
	wchar_t  wszDescription[128];
	char     wszMacAddr[64];
}MyNET_ADAPTER_INFO;


typedef struct
{
	BYTE    busType; //STORAGE_BUS_TYPE
	char    szSerialID[64];
	wchar_t wszSerialID[64];
	int64_t DiskSize;
}DISK_INFO;

class IDiskTool : public IRtlBase
{
public:
	IDiskTool() : IRtlBase(EType_DiskTool) {}

	virtual void MakePhysicalDriveName(DWORD dwDiskNum, wchar_t* pstrDeviceName, size_t cchDevName)=0;
	virtual BOOL  GetDiskInfo(DWORD dwDiskNum, DISK_INFO* pDiskInfo)=0;
	virtual BOOL  GetSerialID_NoneUsb(DWORD dwDiskNum, char* szSerialID, int cchSerialID) = 0;
	virtual DWORD GetDiskNumber(const wchar_t* szPath) = 0;
	virtual BOOL  GetInstancePath(DWORD dwDrvNum, wchar_t*  wszInstPath, int cchInstPath) = 0;
	virtual int64_t GetDiskSize(DWORD dwDiskNum) = 0;
	virtual BOOL GetVolumeLabel(const wchar_t * pstrPath, wchar_t* wszVolume, int cchVol)=0;
};

//注意：pipe的名字通常是 \\.\pipe\\pipeName
//
class IPipeClient : public IRtlBase
{
public:
	IPipeClient() : IRtlBase(EType_DiskTool) {}

	virtual BOOL Connect(const wchar_t* wszPipeName)=0;
	virtual DWORD SendData(const void* data, int dataLen) = 0;
	virtual DWORD RecvData(void* data, int dataLen) = 0;
	virtual void  DisConnect() = 0;
};

class IPipeServerDataReceiver
{
public:
	virtual ~IPipeServerDataReceiver() {};
	virtual DWORD OnDataReceived(HANDLE hClient, void* data, int dataLen) = 0;
};


class IPipeServer : public IRtlBase
{
public:
	IPipeServer() : IRtlBase(EType_DiskTool) {}

	virtual void  Create(const wchar_t* pstrPipe, IPipeServerDataReceiver* receiver) = 0;
	virtual BOOL  Startup() = 0;
	virtual void  Shutdown() = 0;
	virtual DWORD SendData(HANDLE hClient, void* data, int dataLen) = 0;
};


class IDriverTool : public IRtlBase
{
public:
	IDriverTool() : IRtlBase(EType_DriverTool) {}

	virtual BOOL Install(LPCTSTR DriverName, LPCTSTR driverExec, DWORD startType)=0;
	virtual BOOL QueryInstall(LPCTSTR DriverName) = 0;
	virtual BOOL Remove(LPCTSTR DriverName) = 0;
	virtual BOOL Start(LPCTSTR DriverName) = 0;
	virtual BOOL Stop(LPCTSTR DriverName) = 0;
	virtual BOOL Exchange(LPCTSTR DriverName,
						unsigned long ioctlCode,
						PWCHAR pInStr,
						DWORD inStr_size,
						PWCHAR pOutStr,
						DWORD outStr_size,
						PDWORD pdwReturned) = 0;
};

//use the old windows API
class IIniOper : public IRtlBase
{
public:
	IIniOper() : IRtlBase(EType_IniOper) {}

	virtual void SetFileName(const wchar_t* szFileName) = 0;
	virtual void SetFileName(const char* szFileName) = 0;

	virtual int ReadInteger(const wchar_t* szSection, const wchar_t* szKey, int iDefaultValue) = 0;
	virtual int ReadInteger(const char* szSection, const char* szKey, int iDefaultValue) = 0;
	virtual double ReadDouble(const wchar_t* szSection, const wchar_t* szKey, double fltDefaultValue) = 0;
	virtual double ReadDouble(const char* szSection, const char* szKey, double fltDefaultValue) = 0;
	virtual bool ReadBoolean(const wchar_t* szSection, const wchar_t* szKey, bool bDefault) = 0;
	virtual bool ReadBoolean(const char* szSection, const char* szKey, bool bDefault) = 0;
	virtual bool ReadString(const wchar_t* szSection, const wchar_t* szKey, const wchar_t* szDefaultValue, wchar_t* poStr, int cchStr) = 0;
	virtual bool ReadString(const char* szSection, const char* szKey, const char* szDefaultValue, char* poStr, int cchStr) = 0;

	virtual IStringVectorW* GetSectionAllKeys(const wchar_t* lpSecName) = 0;
	virtual IStringVectorA* GetSectionAllKeys(const char* lpSecName) = 0;

	virtual IStringVectorW* GetAllSectionNamesW() = 0;
	virtual IStringVectorA* GetAllSectionNamesA() = 0;

	virtual bool WriteString(const wchar_t* szSection, const wchar_t* szKey, const wchar_t* szVal)=0;
	virtual bool WriteString(const char* szSection, const char* szKey, const char* szVal) =0;
};

class IIniOper2 : public IRtlBase
{
public:
	IIniOper2() : IRtlBase(EType_IniOper2) {}
	enum ECharType {
		eCharType_MBCS = 0,
		eCharType_UTF8 = 1,
		eCharType_WCHAR = 2
	};

	virtual bool Init(ECharType dataType = eCharType_MBCS, bool bCaseSent = false, bool bMultiLine = false)=0;
	virtual ECharType GetDataType()const = 0;
	virtual void SetDataType(ECharType dataType) = 0;
	virtual bool IsEmpty()const = 0;

	virtual bool Load(const char* pstrFileName) = 0;
	virtual bool Load(const wchar_t* pstrFileName) = 0;
	virtual bool LoadBuf(const BYTE* pstrBuf, size_t nBufLen) = 0;
	virtual bool Save(const char* pstrFileName, bool bAddBOM = false) = 0;
	virtual bool Save(const wchar_t* pstrFileName, bool bAddBOM = false) = 0;
	virtual IDynBuf* SaveBuf(bool bAddBOM = false) = 0;

	virtual IStringVectorW* GetAllSections()const = 0;
	virtual int GetSectionKeyCount(const wchar_t* pSection)const = 0;
	virtual IStringVectorW* GetSectionAllKeys(const wchar_t* pSection)const = 0;

	virtual const wchar_t* GetString(
		const wchar_t* pstrSection,
		const wchar_t* pstrKey,
		const wchar_t* pstrDefault = NULL
	) = 0;
	virtual bool SetString(
		const wchar_t* pstrSection,
		const wchar_t* pstrKey,
		const wchar_t* pstrValue,
		const wchar_t* pstrComment = NULL
	) = 0;
	virtual int32_t GetInt32(
		const wchar_t* pstrSection,
		const wchar_t* pstrKey,
		int32_t        nDefault = 0
	) = 0;
	virtual bool SetInt32(
		const wchar_t* pstrSection,
		const wchar_t* pstrKey,
		int32_t        nValue,
		const wchar_t* pstrComment = NULL,
		bool            bUseHex = false
	) = 0;

	virtual bool DeleteValue(
		const wchar_t* pstrSection,
		const wchar_t* pstrKey
	) = 0;
	virtual bool DeleteSection(
		const wchar_t* pstrSection,
		bool           bRemoveEmpty = true
	) = 0;
};

class INetAdapterSet : public IRtlBase
{
public:
	INetAdapterSet() : IRtlBase(EType_NetNetAdapterSet) {}
	enum EAddressType
	{
		ALL = 0,
		IPV4 = 2,
		IPV6 = 23
	};

	virtual DWORD Enum(EAddressType addrType)=0;
	virtual DWORD GetCount()=0;
	virtual const MyNET_ADAPTER_INFO* GetInfo(DWORD dwIndex)=0;
	//pstrMAC like: 6A-14-01-60-3A-73
	virtual const MyNET_ADAPTER_INFO* Find(const char* pstrMAC)=0;
};


typedef struct {
	int family; //AF_INET, AF_INET6
	char szIPAddr[100];
}MyIPAddress;

class INetTool : public IRtlBase
{
public:
	INetTool() : IRtlBase(EType_NetTool) {}

	//get IP Address by hostname
	//返回 MyIPAddress*
	virtual IObjectVector* ResolveHostname(const char* pstrHostname) = 0;
	virtual bool IsDottedIPAddress(const char* pstrIP, unsigned int* addrp)=0;
};

class IUdpServer;
class IUdpServerDataReceiver
{
public:
	virtual ~IUdpServerDataReceiver() { };
	virtual DWORD  OnDataReceived(const char* szClientIP, int nClientPort, void* data, int dataLen) = 0;		
};


class IUdpServer : public IRtlBase
{
public:
	IUdpServer() : IRtlBase(EType_NetUDPServer) {}
	friend class IUdpServerDataReceiver;

	virtual void Init(bool bIPv4, int nPort, IUdpServerDataReceiver* receiver)=0;
	virtual void Init(const char* pstrIP, int nPort, IUdpServerDataReceiver* receiver) = 0;
	virtual bool Startup() = 0;
	virtual int  SendData(const char* szClientIp, int nClientPort, const void* data, int cbData) = 0;
	virtual void Shutdown() = 0;
};

class IUdpClient : public IRtlBase
{
public:
	IUdpClient() : IRtlBase(EType_NetUDPClient) {}

	virtual void Init(bool bIPv4, const char* szServerAddr, int nPort)=0;
	virtual bool Startup() = 0;
	virtual int  SendData(const void* data, int dataLen) = 0;
	virtual int  RecvData(void* data, int dataLen) = 0;
	virtual void Shutdown() = 0;
};

// ITCPServerDataReceiver通常是多线程实现的
class ITcpServer;
class ITCPServerDataReceiver {
public:
	virtual ~ITCPServerDataReceiver() {}
	virtual void OnCreate(ITcpServer* pServer) = 0;
	virtual void OnAccept(ITcpServer* pServer, int sock, const char* pstrClientIP, int nPort) = 0;
	virtual void OnError(ITcpServer* pServer, int sockErrno) = 0;
};

class ITcpServer : public IRtlBase
{
public:
	ITcpServer() : IRtlBase(EType_NetTCPServer) {}
	friend class ITCPServerDataReceiver;

	virtual void Init(bool bIPv4, int nPort, int _backlog/*=256*/, ITCPServerDataReceiver* receiver)=0;
	virtual void Init(const char* pstrIP, int nPort, int _backlog/*=256*/, ITCPServerDataReceiver* receiver)=0;
	virtual bool Startup()=0;
	virtual void Shutdown()=0;
};

class ITcpClient : public IRtlBase
{
public:
	ITcpClient() : IRtlBase(EType_NetTCPClient) {}

	virtual void Init(bool bIPv4, const char* szServerAddr, int nPort) = 0;
	virtual bool Startup() = 0;
	// msTimeout<0 没有超时限制
	// return -1 for timeout
	virtual int  SendData(const void* data, int dataLen, int msTimeout=-1) = 0;
	virtual int  RecvData(void* data, int dataLen, int msTimeout=-1) = 0;
	virtual void Shutdown() = 0;
};


#endif// COMMRTL_H_6865_2020_07_15_