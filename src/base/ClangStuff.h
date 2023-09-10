/*============================================================================*/
/* ClangStuff.h -- CLANG stuff                                                */
/*============================================================================*/
#pragma once

extern void* aExtern_EntBufferNextEntry(void** ppEntBuffer, void* aPrevEntry) __attribute__((weak_import));
extern void  vExtern_EntPayloadCreate(void* pEntWrk, i8 nPayloadBytes) __attribute__((weak_import));

extern void  vExtern_EntWrkAppendElementInt(void* pEntWrk, i8 iInt) __attribute__((weak_import));
extern void  vExtern_EntWrkAppendElementStz(void* pEntWrk, char* sValue) __attribute__((weak_import));
extern void  vExtern_EntWrkEntKind(void* pEntWrk, i8 bPayLoad, i8 iKeyKind) __attribute__((weak_import));
extern void  vExtern_EntWrkPayloadCreate(void* pEntWrk, i8 nPayloadBytes) __attribute__((weak_import));
extern void  vExtern_EntWrkPrint(void* pEntWrk, void* pIndexDef) __attribute__((weak_import));
extern void  vExtern_EntWrkReset(void* pEntWrk) __attribute__((weak_import));

extern void* aExtern_FindTable(void* aTableName) __attribute__((weak_import));
extern void* aExtern_FindIndex(void* aTable, void* aIndexName) __attribute__((weak_import));
extern void* aExtern_GetIndexDef(void* aIndex) __attribute__((weak_import));
extern void* aExtern_GetIndexTree(void* aTable, void* aIndex) __attribute__((weak_import));

extern void* aExtern_IndexTreeAllotPayStr(void* pIndexTree, char* sStr) __attribute__((weak_import));
extern void* aExtern_IndexTreeBegin(void* pIndexTree, void** ppEntBuffer) __attribute__((weak_import));
extern void* aExtern_IndexTreeFindEntry(void* aIndexTree, void* aKey) __attribute__((weak_import));
extern void* aExtern_IndexTreeFindEntry_hint(void* aIndexTree, void* aKey, void* aEnt_recent) __attribute__((weak_import));
extern void* aExtern_IndexTreeInsertEnt(void* aIndexDef, void* aEnt, i8* piErrorCode) __attribute__((weak_import));
extern void* aExtern_IndexTreeInsertEntTop(void* aIndexTree, void* aEnt, void** aaEntBottom, i8 iTopCount, i8* piErrorCode)
  __attribute__((weak_import));
extern void* aExtern_IndexDefNew(i8 nNumCols, void* pColumnInfo) __attribute__((weak_import));
extern void* aExtern_IndexTreeNew(void* aIndexDef, i8* aiKeyKind) __attribute__((weak_import));
extern void* aExtern_IndexTreePositionGE(void* aIndexTree, void** aaEntBuffer, void* aKey) __attribute__((weak_import));
extern void* aExtern_IndexTreePositionGT(void* aIndexTree, void** aaEntBuffer, void* aKey) __attribute__((weak_import));
extern void *aExtern_IndexTreePositionLE(void* aIndexTree, void* aKey) __attribute__((weak_import));
extern void *aExtern_IndexTreePositionLT(void* aIndexTree, void* aKey) __attribute__((weak_import));

extern void* aExtern_RowBufferCellArray(void* aRowBuffer, i8 iRow) __attribute__((weak_import));
extern void* aExtern_RowPixNew(i8 nColumns, void* pColumnInfo) __attribute__((weak_import));

extern void* aExtern_PotAllotRow(void* pPot, i8 iMixIn, char** aazStrArea, i8* piErrorCode) __attribute__((weak_import));
extern void  vExtern_PotRowStrSize(void* pPot, i8 iMixin, i8 nRowStrSize) __attribute__((weak_import));
extern void  vExtern_PotRowPix(void* pPot, void* pRowPix) __attribute__((weak_import));

extern void* aExtern_StreamDefNew(i8 nNumCols, void* pColumnInfo) __attribute__((weak_import));

extern i8    iExtern_StrLibLike2(char* azStr, char* azRegExp) __attribute__((weak_import));
extern i8    iExtern_StrLibLike3(char* azStr, char* azRegExp, char cEsc) __attribute__((weak_import));
extern i8    iExtern_StrLibWordFlipBack(char* aDst, char* aSrc) __attribute__((weak_import));

extern void* aExtern_TubAllotRow(void* pTub, char** aazStrArea, i8* piErrorCode) __attribute__((weak_import));
extern void  vExtern_TubRowStrSize(void* pTub, i8 nRowStrSize) __attribute__((weak_import));
extern void  vExtern_TubRowPix(void* pTub, void* pRowPix) __attribute__((weak_import));

extern void  vNeverReachCode(i8 iLine) __attribute__((weak_import));
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
