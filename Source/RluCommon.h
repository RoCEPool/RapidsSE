/*============================================================================*/
/* RluCommon.h -- Commnon variables between C & C++ rlu inner loops           */
/*============================================================================*/

// Note: this file is included by tcc

#define C_LOGIC_STACK 4096

typedef struct Ent_int _Ent_int;

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

  i8   m_i_Slot;

  i8   m_nPtns;
  i8   m_iHash;
  i8   m_i_Ptn;
  
  void* m_pDevDstRsb;
  void* m_pDevSrcRsb;
  void* m_pDevSrcIxt;
  void* m_pDevAltRsb;
  void* m_pDevAltIxt;
  void* m_pDevTmpIxt;
  
  struct Ent_int* m_aEntArray;

  struct Ent_int* m_aAltEntArray;
  struct Ent_int* m_aDstEntArray;
  struct Ent_int* m_aSrcEntArray;
  struct Ent_int* m_aTmpEntArray;

  struct Ent_int* m_aAltEnt;
  struct Ent_int* m_aDstEnt;
  struct Ent_int* m_aSrcEnt;
  struct Ent_int* m_aTmpEnt;
  struct Ent_int* m_aWrkEnt;
  
  i8 m_nAltHeadSize;
  i8 m_nSrcHeadSize;

  void* m_aAltCrw;
  void* m_aSrcCrw;

  char m_caStrWrk[128];

  i8 m_iaDataStack[C_LOGIC_STACK];
  
  i8 m_iaLocals[C_LOCALS]; // locals regs []
  


  i8 m_iCmnFooter;
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
