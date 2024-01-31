/*============================================================================*/
/* ClangDynamic.h -- CLANG dynamic                                            */
/*============================================================================*/
#pragma once

struct ColumnInfo {
  i8    m_iColumnNo;      // this columns subscript
  char  m_cColumnNm[256]; // name of the column
  i8    m_iColumnTypeId;  // column typeId (CT_INTEGER etc) // ALKA COME BACK TO THIS
  i8    m_iColumnLength;  // logical length of the column
  i8    m_iColumnScale;   // logical scale of the column
  bool  m_bIsNullable;    // is column nullable?
  bool  m_bPtnKey;        // is partition key
  bool  m_bIsAscending;   // is ascending
};

struct RowInfo {
  bool m_bInitialize;
  i8   m_iCell[256];
  char m_cStringArea[16384]; // _C_16KB;
	i8   m_nStringAreaSize;
  void* m_aRowCellArray;
  char* m_acStrArea;
  char* m_acStrAreaSav;
};

struct IndexInfo {
  bool  m_bInitialize;
  char  m_cTableName[256];
  char  m_cIndexName[256];
  void* m_aTable;
  void* m_aIndex;
  void* m_aIndexTree;
  void* m_aIndexDef;
  void* m_aEntWrk;
  void* m_aEntKey;
  void* m_aEntBottom;
  void* m_aEntUpper;
  void* m_aEntBuffer;
  struct Ent_int* m_aEntInt;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// CODE SYNTH V2
struct Dynamic_T
{

  i8 m_bTrace;
  i8 m_iErrorCode;
  i8 m_iRowsProcessed;
  i8 m_iLimitRows;

	bool m_bProcessRow;
	bool m_bHasAggRow;
	bool m_bHasRowToAgg;
  bool m_bIndexScan;

	// pdsRowBuffer T
	void* m_aPdsRowBuffer;

  // expression stack
  i8 m_aW[256];

  // inflate T (SR_IT)
  i8 m_nRows;
  i8 m_nCols;
  i8 m_iHeadSize;
  enum eSI           m_eStrInflateRule;
  struct HeadLine_T* m_aHeadLine0;
  struct RowInfo     m_xRowInfoIT;

  // inflate J (SR_IJ)
  struct RowInfo m_xRowInfoIJ;

  // Project payload (SR_PL)
  struct RowInfo m_xRowInfoPL;

  // project T (SR_PT)
  struct RowInfo m_xRowInfoPT;

  // join T & J (SR_TJ)
  struct RowInfo m_xRowInfoTJ;

  // aggregate T (SR_AT)
  struct RowInfo m_xRowInfoAT;

  // Tub Output (SR_TO)
  void*  m_pTub;
  struct RowInfo m_xRowInfoTO;

  // Pot Ouput (SR_PO)
  bool  m_bGeneratePot;
  void* m_pPot;
  struct RowInfo m_xRowInfoPO;

  // index scan T
  struct IndexInfo m_xIndexInfoT;

  // index join with J
  i8 m_iJoinedRows;
  struct IndexInfo m_xIndexInfoJ;

  // payload (gby/srt/top)
  i8* m_aiPayload;
  struct IndexInfo m_xIndexInfoPL;

  void* m_aStreamDefOut;

  // ordered aggregates
//  i8      m_iIndexScan;
//  i8      m_iHasOrdAggRow;
//  i8      m_iOrdAggDone;

  i8    m_iaPow10[32];  // This needs to be at least as big as the max BCD scale being supported
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
