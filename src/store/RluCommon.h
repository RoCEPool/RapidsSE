/*============================================================================*/
/* RluCommon.h -- Commnon variables between C & C++ rlu inner loops           */
/*============================================================================*/

// Note: this file is included by tcc

struct RluCommon                      // **IF YOU CHANGE THIS DO A CLEAN BUILD**
{
  i8   m_iCmnMagic;
  
  i8   m_bCallAgain; // call same handler again
  i8   m_bCalledAgain; // was called again

  void* m_pPot;

  i8 m_iCmnFooter;
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
