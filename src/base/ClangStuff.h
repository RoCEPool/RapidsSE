/*============================================================================*/
/* ClangStuff.h -- CLANG stuff                                                */
/*============================================================================*/
#pragma once

struct RowInfo {
  i8   m_iInitialize;
  i8   m_iCell[256];
  char m_cStringArea[16384]; // _C_16KB;
  void* m_aRowCellArray;
  char* m_acStrArea;
  char* m_acStrAreaSav;
};

struct IndexInfo {
  i8    m_iInit;
  void *m_aIndex;
  void *m_aIndexTree;
  void *m_aIndexDef;
  void *m_aEntWrk;
  void *m_aEntKey;
  void *m_aEntBottom;
  void *m_aEntUpper;
  void *m_aEntBuffer;
  struct Ent_int* m_aEntInt;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// CODE SYNTH V2
struct Dynamic_T
{

  i8 m_bTrace;
  i8 m_iErrorCode;
  i8 m_iProcessRow;
  i8 m_iRowsProcessed;
  i8 m_iLimitRows;

  // rowBuffer T
  void* m_aRowBuffer;

  // expression stack
  i8 m_aW[256];

  // inflate T (SR_IT)
  i8 m_nRows;
  i8 m_nCols;
  i8 m_iHeadSize;
  enum eSI           m_eStrInflateRule;
  struct HeadLine_T* m_aHeadLine0;
  struct RowInfo  m_xRowInfoIT;

  // inflate J (SR_IJ)
  struct RowInfo m_xRowInfoIJ;

  // Project payload (SR_PL)
  struct RowInfo m_xRowInfoPL;

  // project T (SR_PT)
  struct RowInfo m_xRowInfoPT;

  // join T & J (SR_TJ)
  struct RowInfo m_xRowInfoTJ;

  // aggregate T (SR_AT)
  i8 m_iAggregInit;
  struct RowInfo m_xRowInfoAT;

  // Tub Output (SR_TO)
  void*   m_pTub;
  struct RowInfo m_xRowInfoTO;

  // Pot Ouput (SR_PO)
  i8    m_iPot;
  void* m_pPot;
  struct RowInfo m_xRowInfoPO;

  // index scan T
  struct IndexInfo m_xIndexInfoT;

  // index join with J
  i8 m_iIndexJoinInit;
  i8 m_iJoinedRows;
  struct IndexInfo m_xIndexInfoJ;

  // payload (gby/srt/top)
  i8* m_aiPayload;
  struct IndexInfo m_xIndexInfoPL;

  // ordered aggregates
//  i8      m_iIndexScan;
//  i8      m_iHasOrdAggRow;
//  i8      m_iOrdAggDone;

};

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

extern void* aRowBuffer_GetCellArray(void* aRowBuffer, i8 iRow);
extern void* aRowPix_new(i8 nColumns, void* pColumnInfo) __attribute__((weak_import));
extern void  vRowSingle_Print(void* pRowSingle);

extern void* aPot_AllotRow(void* pPot, i8 iMixIn, char** aazStrArea, i8* piErrorCode);
extern void  vPot_RowStrSize(void* pPot, i8 iMixin, i8 nRowStrSize);

extern void* aTub_AllotRow(void* pTub, char** aazStrArea, i8* piErrorCode);
extern void  vTub_RowStrSize(void* pTub, i8 nRowStrSize);
extern void  vTub_RowPix(void* pTub, void* pRowPix);

extern i8    StrLib_iLike2(char* azStr, char* azRegExp);
extern i8    StrLib_iLike3(char* azStr, char* azRegExp, char cEsc);
extern i8    StrLib_iWordFlipBack(char* aDst, char* aSrc);

extern void  vNeverReachCode(i8 iLine);
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
