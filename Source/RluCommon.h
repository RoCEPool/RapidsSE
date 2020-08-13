/*============================================================================*/
/* RluCommon.h -- Commnon variables between C & C++ rlu inner loops           */
/*============================================================================*/

// Note: this file is included by tcc

#define C_LOGIC_STACK 4096

struct RluCommon
{
  i8   m_iCmnMagic;
  
  i8   m_bCallAgain; // call same handler again
  i8   m_bCalledAgain; // was called again

  i8   m_iValue;
  i8   m_iCtr0;
  i8   m_iSql;
  i8   m_bTrace;

  i8   m_i_Slot;

  i8   m_nPtns;
  i8   m_iHash;
  i8   m_i_Ptn;
  
  void*  m_pDevDstRsb;
  void*  m_pDevSrcRsb;
  void*  m_pDevSrcIxt;
  void*  m_pDevAltRsb;
  void*  m_pDevAltIxt;
  void*  m_pDevTmpIxt;

  char m_caStrWrk[128];

  i8 m_iaDataStack[C_LOGIC_STACK];

  i8 m_iCmnFooter;
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
