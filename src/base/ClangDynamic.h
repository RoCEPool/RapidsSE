/*============================================================================*/
/* ClangDynamic.h -- CLANG dynamic                                            */
/*============================================================================*/
#pragma once

#define C_MAX_DUP_OPS 4

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
	void* m_aPdsRowBuffer;
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
  // Note: 128 bit integers, which can represent up to
  // 39 digits (though not all 39 digit numbers).
  // We put this first so it is 128 bit aligned
  i16 m_iaPow10[39];

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

  struct RowInfo     m_xRowInfoIT[C_MAX_DUP_OPS];

  // inflate J (SR_IJ)
  struct RowInfo m_xRowInfoIJ[C_MAX_DUP_OPS];

  // Project payload (SR_PL)
  struct RowInfo m_xRowInfoPL[C_MAX_DUP_OPS];

  // project T (SR_PT)
  struct RowInfo m_xRowInfoPT[C_MAX_DUP_OPS];

  // join T & J (SR_TJ)
  struct RowInfo m_xRowInfoTJ[C_MAX_DUP_OPS];

  // aggregate T (SR_AT)
  struct RowInfo m_xRowInfoAT[C_MAX_DUP_OPS];

  // Tub Output (SR_TO)
  void*  m_pTub;
  struct RowInfo m_xRowInfoTO[C_MAX_DUP_OPS];

  // Pot Ouput (SR_PO)
  bool  m_bGeneratePot;
  void* m_pPot;
  struct RowInfo m_xRowInfoPO[C_MAX_DUP_OPS];

  // index scan T
  struct IndexInfo m_xIndexInfoT[C_MAX_DUP_OPS];

  // index join with J
  i8 m_iJoinedRows;
  struct IndexInfo m_xIndexInfoJ[C_MAX_DUP_OPS];

  // payload (gby/srt/top)
  i8* m_aiPayload;
  struct IndexInfo m_xIndexInfoPL[C_MAX_DUP_OPS];

  void* m_aStreamDefOut;

	//
	// Extern Function Pointers
	//

	// EntryBuffer
	void  (*m_fp_vExtern_EntWrkAppendElementStz)(void*,char*);
	void* (*m_fp_aExtern_EntBufferNextEntry)(void**,void*);
	void  (*m_fp_vExtern_EntPayloadCreate)(void*,i8);
	void  (*m_fp_vExtern_EntWrkReset)(void*);

	// Index
	void* (*m_fp_vExtern_IndexFind)(void*,char*,char*);
	void* (*m_fp_vExtern_IndexNew)(void*,i8,void*,bool);

	// IndexTree
	void* (*m_fp_aExtern_IndexTreeAllotPayStr)(void*,char*);
	void* (*m_fp_aExtern_IndexTreeBegin)(void*,void**);
	void* (*m_fp_aExtern_IndexTreeFindEntryAsc)(void*,void*);
	void* (*m_fp_aExtern_IndexTreeFindEntryAscDsc)(void*,void*);
	void* (*m_fp_aExtern_IndexTreeFindInsertGenAsc)(void*,i8,bool*);
	void* (*m_fp_aExtern_IndexTreeInsertEnt)(void*,void*,i8*);
	void* (*m_fp_aExtern_IndexTreeInsertEntTop)(void*,void*,void**,i8,i8*);

	void  (*m_fp_vExtern_IndexTreePositionGE_gen)(void*);
	void  (*m_fp_vExtern_IndexTreePositionGT_gen)(void*);
	void  (*m_fp_vExtern_IndexTreePositionLE_gen)(void*);
	void  (*m_fp_vExtern_IndexTreePositionLT_gen)(void*);

	void  (*m_fp_vExtern_IndexTreePositionGE_int)(void*);
	void  (*m_fp_vExtern_IndexTreePositionGT_int)(void*);
	void  (*m_fp_vExtern_IndexTreePositionLE_int)(void*);
	void  (*m_fp_vExtern_IndexTreePositionLT_int)(void*);

  // Pds
	void* (*m_fp_aExtern_PdsFirstRowCellArray)(void*,void**);
	void* (*m_fp_aExtern_PdsNextRowCellArray)(void*,void**);
	void  (*m_fp_vExtern_PotAllotRowPds)(void*,byte*,i8,char**,i8*);
	void  (*m_fp_vExtern_PotRowStrSize)(void*,i8);
	void  (*m_fp_vExtern_PotRowPix)(void*,i8,void*);

	// StreamDef
	void* (*m_fp_aExtern_StreamDefNew)(i8,void*);

	// Tub
	void  (*m_fp_vExtern_TubAllotRowPds)(void*,char**,i8*);
	void  (*m_fp_vExtern_TubRowPix)(void*,i8,void*);
	void  (*m_fp_vExtern_TubRowStrSize)(void*,i8);

  w8 m_wDummy; // I can't explain why, but without this padding
               // the compiler seems to expand the size of the struct
               // by 1 word. This word is not initialised however,
               // which then causes an assertion in debug mode.
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
