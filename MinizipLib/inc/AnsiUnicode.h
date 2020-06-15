#pragma once
/*****************************************************************************
* Name     : AnsiUnicode.h
* Creator  : H.Z.XIN
* Date     : 2016-04-02
* Function : mbcs, unicode, utf-8
* Parameter:
* Return(s):
* Notices  : 
------------------------------------------------------------------------------
Change Log:
2016-04-02        H.Z.XIN        Create
*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif


void StrPtrFree_char(char* p);
void StrPtrFree_wchar(wchar_t* p);

//char* --> wchar_t*
//����֮���Լ�StrPtrFree_wchar() �ڴ�
wchar_t* AnsiToUnicode(const char* szStr);

//wchar_t* --> char*
//����֮���Լ�StrPtrFree_char() �ڴ�
char* UnicodeToAnsi(const wchar_t* szStr);


//utf-8 char* --> wchar_t*
//����֮���Լ�StrPtrFree_wchar() �ڴ�
wchar_t* Utf8ToUnicode(const char* utf8Str);

//wchar_t* --> utf-8 char*
//����֮���Լ�StrPtrFree_char() �ڴ�
char* UnicodeToUtf8(const wchar_t* UnicodeStr);

//char* --> utf-8 char*
//����֮���Լ�StrPtrFree_char() �ڴ�
char* AnsiToUtf8(const char* strSource);
//utf-8 char* --> char*
//����֮���Լ�StrPtrFree_char() �ڴ�
char* Utf8ToAnsi(const char* strSource);


#ifdef __cplusplus
}
#endif