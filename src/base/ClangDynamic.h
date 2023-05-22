/*============================================================================*/
/* ClangDynamic.h -- CLANG dynamic                                            */
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
