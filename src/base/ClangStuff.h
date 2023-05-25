/*============================================================================*/
/* ClangStuff.h -- CLANG stuff                                                */
/*============================================================================*/
#pragma once

extern void* aExtern_EntBufferNextEntry(void** ppEntBuffer, void* aPrevEntry);
extern void  vExtern_EntPayloadCreate(void* pEntWrk, i8 nPayloadBytes);

extern void  vExtern_EntWrkAppendElementInt(void* pEntWrk, i8 iInt);
extern void  vExtern_EntWrkAppendElementStz(void* pEntWrk, char* sValue);
extern void  vExtern_EntWrkEntKind(void* pEntWrk, i8 bPayLoad, i8 eKeyKind);
extern void  vExtern_EntWrkPayloadCreate(void* pEntWrk, i8 nPayloadBytes);
extern void  vExtern_EntWrkPrint(void* pEntWrk, void* pIndexDef);
extern void  vExtern_EntWrkReset(void* pEntWrk);

extern void* aExtern_IndexTreeAllotPayStr(void* pIndexTree, char* sStr);
extern void* aExtern_IndexTreeBegin(void* pIndexTree, void** ppEntBuffer);
extern void* aExtern_IndexTreeFindEntry(void* aIndexTree, void* aKey);
extern void* aExtern_IndexTreeFindEntry_hint(void* aIndexTree, void* aKey, void* aEnt_recent);
extern void* aExtern_IndexTreeInsertEnt(void* aIndexDef, void* aEnt, i8* piErrorCode);
extern void* aExtern_IndexTreeInsertEntTop(void* aIndexTree, void* aEnt, void** aaEntBottom, i8 iTopCount, i8* piErrorCode);
extern void* aExtern_IndexTreePositionGE(void* aIndexTree, void** aaEntBuffer, void* aKey);
extern void* aExtern_IndexTreePositionGT(void* aIndexTree, void** aaEntBuffer, void* aKey);
extern void *aExtern_IndexTreePositionLE(void* aIndexTree, void* aKey);
extern void *aExtern_IndexTreePositionLT(void* aIndexTree, void* aKey);

extern void* aExtern_RowBufferCellArray(void* aRowBuffer, i8 iRow) __attribute__((weak_import));
extern void* aExtern_RowPixNew(i8 nColumns, void* pColumnInfo) __attribute__((weak_import));

extern void* aExtern_PotAllotRow(void* pPot, i8 iMixIn, char** aazStrArea, i8* piErrorCode);
extern void  vExtern_PotRowStrSize(void* pPot, i8 iMixin, i8 nRowStrSize);

extern void* aExtern_StreamDefNew(i8 nNumCols, void* pColumnInfo) __attribute__((weak_import));

extern i8    iExtern_StrLibLike2(char* azStr, char* azRegExp);
extern i8    iExtern_StrLibLike3(char* azStr, char* azRegExp, char cEsc);
extern i8    iExtern_StrLibWordFlipBack(char* aDst, char* aSrc);

extern void* aExtern_TubAllotRow(void* pTub, char** aazStrArea, i8* piErrorCode);
extern void  vExtern_TubRowStrSize(void* pTub, i8 nRowStrSize);
extern void  vExtern_TubRowPix(void* pTub, void* pRowPix);

extern void  vNeverReachCode(i8 iLine);
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
