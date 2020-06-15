#pragma once
/*****************************************************************************
* Name     : MultiSZ.h
* Creator  : H.Z.XIN
* Date     : 2018-04-02
* Function : ��Multi zero string
* Parameter:
* Return(s):
* Notices  : �ڲ�ʹ��wchar_t�洢
------------------------------------------------------------------------------
Change Log:
2018-04-02        H.Z.XIN        Create
*****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

typedef void* TMultiSZ;


#define RET_OK  (0)
#define RET_BAD (-1)

TMultiSZ MultiSZ_Create();
TMultiSZ MultiSZ_CreateFromRaw(const wchar_t* ppstrmz);

size_t MultiSZ_GetSize(TMultiSZ pszz);
int MultiSZ_PutStrA(TMultiSZ pszz, const char* pstr);
int MultiSZ_PutStrW(TMultiSZ pszz, const wchar_t* pstr);

//���ṩA�汾
const wchar_t* MultiSZ_GetStrW(TMultiSZ pszz, int ind);

//get: abc\0EFG\0\0
wchar_t* MultiSZ_GetRaw(TMultiSZ pszz);
void MultiSZ_FreeRaw(wchar_t** ppstrmz);

void MultiSZ_Destory(TMultiSZ* ppszz);



#ifdef __cplusplus
}
#endif