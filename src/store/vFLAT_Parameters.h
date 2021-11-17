/*============================================================================*/
/* vFLAT_Parameters.h -- Preliminary Fragement for Inflate                    */
/*============================================================================*/

  i8 bTrace    = aCombinedT->m_bTrace;
  void* pTub   = aCombinedT->m_pTub;
  i8 iHeadSize = aCombinedT->m_iHeadSize;
  i8 nRows     = aCombinedT->m_nRows;
  i8 nCols     = aCombinedT->m_nCols;

  static i8 iProjectInit = 0;
  static i8 iAllocRow = 1;
  static i8 iIndexJoinInit = 0;
  
  void* pRowSingle = aCombinedT->m_pRowSingleT;
  struct HeadLine_T* aHeadLine = aCombinedT->m_aHeadLine0;
  enum eSI eStrInflateRule = aCombinedT->m_eStrInflateRule ;

#define nullptr 0

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
