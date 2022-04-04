/*============================================================================*/
/* RluCommon.h -- Commnon variables between C & C++ rlu inner loops           */
/*============================================================================*/

// Note: this file is included by tcc

#define C_LOGIC_STACK 4096

#define C_LOGIC_LOCALS 32
#define C_ROWS_PERBUFFER 10000

struct RluCommon                      // **IF YOU CHANGE THIS DO A CLEAN BUILD**
{
  i8   m_iCmnMagic;
  
  i8   m_bCallAgain; // call same handler again
  i8   m_bCalledAgain; // was called again

  i8   m_bPotCompleted;
  
  i8   m_nEnterCount;
  
  i8   m_iValue;
  i8   m_iCtr0;
  i8   m_iSql;
  i8   m_bTrace;

  i8   m_i_TablePtn;

  i8   m_nPtns;
  i8   m_iHash;
  i8   m_i_Ptn;

  void* m_pPot;
  void* m_pIndexDef;
  void* m_pIndexTree;

  //struct Ent_int* m_aEntArray;

  //struct Ent_int* m_aAltEntArray;
  //struct Ent_int* m_aDstEntArray;
  //struct Ent_int* m_aSrcEntArray;
  //struct Ent_int* m_aTmpEntArray;

  //struct Ent_int* m_aAltEnt;
  //struct Ent_int* m_aDstEnt;
  //struct Ent_int* m_aSrcEnt;
  //struct Ent_int* m_aTmpEnt;

  struct Ent_int* m_aWrkEnt;
  void* m_pEntWrk;
  int   m_iJoinedRow;

  void*  m_pRowPix;

  //struct Ent_int* m_aTmpSkipRoots;
  
  //i8 m_nCrwRows;
  //i8 m_i_CrwRow;

  i8 m_nAltHeadSize;
  i8 m_nSrcHeadSize;

  //void* m_aAltCrw;
  //void* m_aSrcCrw;

  //void* m_aSrcHeadCrw;

  char m_caStrWrk[128];

  i8 m_iaDataStack[C_LOGIC_STACK];
  
  i8 m_iaLocals[C_LOGIC_LOCALS]; // locals regs []

  i8 m_iaColSet[64];
  i8 m_i_ColSet;

  byte m_baJoinMap[C_ROWS_PERBUFFER];
  i8   m_nJoinMapCnt;

  i8 m_iCmnFooter;
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
