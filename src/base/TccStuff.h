/*============================================================================*/
/* TccStuff.h -- Tcc Stuff                                                    */
/*============================================================================*/
#pragma once

struct Combined_T
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // CODE SYNTH V1

  void*   m_pTub;
  void*   m_pPot;
  i8      m_bTrace;
  i8      m_iErrorCode;

  i8      m_iHashRows;

  // table T scan inflate (crwBuffer)
  struct  HeadLine_T* m_aHeadLine0;
  i8        m_iHeadSize;
  enum eSI  m_eStrInflateRule;

  // expression stack
  i8      m_aW[256];

  // rowBuffer T
  void*   m_aRowBuffer;

  // common inflate
  void*   m_pRowSingleT;
  void*   m_aRowCellArrayT;
  char*   m_racStrAreaT;
  char*   m_racStrAreaTSav;
  i8      m_nRows;
  i8      m_nCols;

  // index scan T
  i8      m_iIndexScan;
  void*   m_aIndexT;
  void*   m_aIndexTreeT;
  void*   m_aIndexDefT;
  void*   m_aEntKeyT;
  void*   m_aEntWrkT;
  void*   m_aEntUpperT;
  void*   m_aEntBufferT;
  struct Ent_int* m_aEntIntT;

  // index join with J
  i8      m_iIndexJoinInit;
  i8      m_iJoinedRows;
  void*   m_aRowSingleJ;
  void*   m_aRowCellArrayJ;
  char*   m_racStrAreaJ;
  char*   m_racStrAreaJSav;
  void*   m_aIndexJ;
  void*   m_aIndexTreeJ;
  void*   m_aIndexDefJ;
  void*   m_aEntKeyJ;
  void*   m_aEntWrkJ;
  void*   m_aEntUpperJ;
  void*   m_aEntBufferJ;
  struct Ent_int* m_aEntIntJ;

  // join rows T and J
  void*   m_aRowSingleTJ;
  void*   m_aRowCellArrayTJ;
  char*   m_racStrAreaTJ;
  char*   m_racStrAreaTJSav;

  // project (col/exp)
  i8      m_iProjectInit;
  void*   m_aRowSingleP;
  void*   m_aRowCellArrayP;
  char*   m_racStrAreaP;
  char*   m_racStrAreaPSav;

  // project (agg)
  i8      m_iRowsProcessed;
  void*   m_aRowSingleAgg;
  void*   m_aRowCellArrayAgg;
  char*   m_racStrAreaAgg;
  char*   m_racStrAreaAggSav;

  // ordered aggregates
  i8      m_iHasOrdAggRow;
  i8      m_iOrdAggDone;

  // project (gby)
  i8      m_nAggRows;
  i8      m_iProcessRow;

  // payload (gby/srt/top)
  i8*     m_aiPayload;
  void*   m_aPayloadRow;
  void*   m_aPayloadCellArray;
  char*   m_racPayloadArea;
  char*   m_racPayloadAreaSav;
  void*   m_aIndexTreePL;
  void*   m_aIndexDefPL;
  void*   m_aEntBufferPL;
  void*   m_aEntWrkPL;
  void*   m_aEntKeyPL;
  void*   m_aEntBottomPL;
  struct Ent_int* m_aEntIntPL;

  // top
  i8      m_iTopInit;
  i8      m_iInsertCntTOP;

  // limit
  i8     m_iLimitRows;

  // final tub
  void*   m_aRowTub;
  void*   m_aRowTubCellArray;
  char*   m_racTubStrArea;

  // final pot
  void*   m_aRowPot;
  void*   m_aRowPotCellArray;
  char*   m_racPotStrArea;
};

void vEnt_payloadCreate(void* pEntWrk, i8 nPayloadBytes);

void* aEntBuffer_nextEntry(void** ppEntBuffer, void* aPrevEntry);

void vEntWrk_appendElementEntInt(void* pEntWrk, i8 iInt);
void vEntWrk_appendElementEntStz(void* pEntWrk, char* sValue);
void vEntWrk_entKind(void* pEntWrk, i8 bPayLoad, i8 eKeyKind);
void vEntWrk_payloadCreate(void* pEntWrk, i8 nPayloadBytes);
void vEntWrk_print(void* pEntWrk, void* pIndexDef);
void vEntWrk_reset(void* pEntWrk);

void* aIndexTree_allotPayStr(void* pIndexTree, char* sStr);
void* aIndexTree_begin(void* pIndexTree, void** ppEntBuffer);
void* aIndexTree_findEntry(void* aIndexTree, void* aKey);
void* aIndexTree_findEntry_hint(void* aIndexTree, void* aKey, void* aEnt_recent);
void* aIndexTree_insertEnt(void* aIndexDef, void* aEnt, i8* piErrorCode);
void* aIndexTree_insertEntTop(void* aIndexTree, void* aEnt, void** aaEntBottom, i8 iTopCount, i8* piErrorCode);
void* aIndexTree_positionGE(void* aIndexTree, void** aaEntBuffer, void* aKey);
void* aIndexTree_positionGT(void* aIndexTree, void** aaEntBuffer, void* aKey);
void *aIndexTree_positionLE(void* aIndexTree, void* aKey);
void *aIndexTree_positionLT(void* aIndexTree, void* aKey);

void* aRowBuffer_GetCellArray(void* aRowBuffer, i8 iRow);
void  vRowSingle_Print(void* pRowSingle);

void* aPot_AllotRow(void* pPot, i8 iMixIn, char** aazStrArea, i8* piErrorCode);
void  vPot_RowStrSize(void* pPot, i8 iMixin, i8 nRowStrSize);

void* aTub_AllotRow(void* pTub, char** aazStrArea, i8* piErrorCode);
void  vTub_RowStrSize(void* pTub, i8 nRowStrSize);
void  vTub_RowPix(void* pTub, void* pRowPix);

i8    StrLib_iLike2(char* azStr, char* azRegExp);
i8    StrLib_iLike3(char* azStr, char* azRegExp, char cEsc);
i8    StrLib_iWordFlipBack(char* aDst, char* aSrc);

void* aRowPix_new(i8 nColumns, void* pColumnInfo);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
