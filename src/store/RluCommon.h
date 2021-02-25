/*============================================================================*/
/* RluCommon.h -- Commnon variables between C & C++ rlu inner loops           */
/*============================================================================*/

// Note: this file is included by tcc

#define C_LOGIC_STACK 4096

#define C_LOGIC_LOCALS 32

struct Rlu_Ixt
{
  char  m_cIndexNm[256];  // this will go (with the demo queries)
  void* m_pIdxTree;
  void* m_aEnt;
  void* m_aUpperEnt;
  void* m_pEntBuffer;
  void* m_aCrw;
  void* m_pCrwBuffer;
  i8    m_i_Row;
  i8    m_nRows;
};

struct Rlu_Row
{
  char  m_cTableNm[256];  // this will go (with the demo queries)
  i8    m_i_Row;
  i8    m_nRows;
  void* m_aRow;
  void* m_pRowBuffer;
  void* m_pStreamDef;
};

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
  
  void* m_pDevDstRow;
  void* m_pDevSrcRow;
  void* m_pDevAltRow;

  void* m_pDevSrcIxt;
  void* m_pDevAltIxt;
  void* m_pDevTmpIxt;

  //struct  Rlu_Row m_xDstRow;
  //struct  Rlu_Row m_xSrcRow;
  //struct  Rlu_Row m_xDstRow;

  struct  Rlu_Ixt m_xSrcIxt;
  //struct  Rlu_Ixt m_xAltIxt;
  //struct  Rlu_Ixt m_xTmpIxt;

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

  struct Ent_int* m_aTmpSkipRoots;
  
  i8 m_nCrwRows;
  i8 m_i_CrwRow;

  i8 m_nAltHeadSize;
  i8 m_nSrcHeadSize;

  void* m_aAltCrw;
  void* m_aSrcCrw;

  void* m_aSrcHeadCrw;

  char m_caStrWrk[128];

  i8 m_iaDataStack[C_LOGIC_STACK];
  
  i8 m_iaLocals[C_LOGIC_LOCALS]; // locals regs []

  i8 m_iCmnFooter;

/*
  struct StructA
  {
    struct StructB0
    {
      struct StructC00
      {
        i8    m_int;
        void* m_ptr;
      };

      struct StructC01
      {
        i8    m_int;
        void* m_ptr;
      };

      struct StructC02
      {
        i8    m_int;
        void* m_ptr;
      };
    };

    struct StructB1
    {
      struct StructC10
      {
        i8    m_int;
        void* m_ptr;
      };

      struct StructC11
      {
        i8    m_int;
        void* m_ptr;
      };

      struct StructC12
      {
        i8    m_int;
        void* m_ptr;
      };
    };

    struct StructB2
    {
      struct StructC20
      {
        i8    m_int;
        void* m_ptr;
      };

      struct StructC21
      {
        i8    m_int;
        void* m_ptr;
      };

      struct StructC22
      {
        i8    m_int;
        void* m_ptr;
      };
    };
  };
*/
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
