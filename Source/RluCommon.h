/*============================================================================*/
/* RluCommon.h -- Commnon variables between C & C++ rlu inner loops           */
/*============================================================================*/

// Note: this file is included by tcc

#define C_LOGIC_STACK 4096

struct RluCommon
{
  i8 m_iCmnMagic;
  
  i8   m_iValue;
  i8   m_iCtr0;
  i8   m_iSql;
  i8   m_bTrace;

  i8   m_i_Slot;

  i8   m_nPtns;
  i8   m_iHash;
  i8   m_i_Ptn;

  i8 m_iaDataStack[C_LOGIC_STACK];

  i8 m_iCmnFooter;
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
