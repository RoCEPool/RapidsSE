/*============================================================================*/
/* ClangStuff.h -- CLANG stuff                                                */
/*============================================================================*/
#pragma once

extern void vEnt_payloadCreate(void* pEntWrk, i8 nPayloadBytes);

extern void* aEntBuffer_nextEntry(void** ppEntBuffer, void* aPrevEntry);

extern void vEntWrk_appendElementEntInt(void* pEntWrk, i8 iInt);
extern void vEntWrk_appendElementEntStz(void* pEntWrk, char* sValue);
extern void vEntWrk_entKind(void* pEntWrk, i8 bPayLoad, i8 eKeyKind);
extern void vEntWrk_payloadCreate(void* pEntWrk, i8 nPayloadBytes);
extern void vEntWrk_print(void* pEntWrk, void* pIndexDef);
extern void vEntWrk_reset(void* pEntWrk);

extern void* aIndexTree_allotPayStr(void* pIndexTree, char* sStr);
extern void* aIndexTree_begin(void* pIndexTree, void** ppEntBuffer);
extern void* aIndexTree_findEntry(void* aIndexTree, void* aKey);
extern void* aIndexTree_findEntry_hint(void* aIndexTree, void* aKey, void* aEnt_recent);
extern void* aIndexTree_insertEnt(void* aIndexDef, void* aEnt, i8* piErrorCode);
extern void* aIndexTree_insertEntTop(void* aIndexTree, void* aEnt, void** aaEntBottom, i8 iTopCount, i8* piErrorCode);
extern void* aIndexTree_positionGE(void* aIndexTree, void** aaEntBuffer, void* aKey);
extern void* aIndexTree_positionGT(void* aIndexTree, void** aaEntBuffer, void* aKey);
extern void *aIndexTree_positionLE(void* aIndexTree, void* aKey);
extern void *aIndexTree_positionLT(void* aIndexTree, void* aKey);


extern void* aPot_AllotRow(void* pPot, i8 iMixIn, char** aazStrArea, i8* piErrorCode);
extern void  vPot_RowStrSize(void* pPot, i8 iMixin, i8 nRowStrSize);


extern void* aExtern_RowBufferCellArray(void* aRowBuffer, i8 iRow) __attribute__((weak_import));
extern void* aExtern_RowPixNew(i8 nColumns, void* pColumnInfo) __attribute__((weak_import));
extern void* aExtern_TubAllotRow(void* pTub, char** aazStrArea, i8* piErrorCode);
extern void  vExtern_TubRowStrSize(void* pTub, i8 nRowStrSize);
extern void  vExtern_TubRowPix(void* pTub, void* pRowPix);


extern i8 iExtern_StrLibLike2(char* azStr, char* azRegExp);
extern i8 iExtern_StrLibLike3(char* azStr, char* azRegExp, char cEsc);
extern i8 iExtern_StrLibWordFlipBack(char* aDst, char* aSrc);

extern void  vNeverReachCode(i8 iLine);
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
