/*============================================================================*/
/* vFLAT_Parameters.h -- Preliminary Fragement for Inflate                    */
/*============================================================================*/

  i8 bTrace    = aCombinedT->m_bTrace;
  void* pTub   = aCombinedT->m_pTub;
  i8 iHeadSize = aCombinedT->m_iHeadSize;
  i8 nRows     = aCombinedT->m_nRows;
  i8 nCols     = aCombinedT->m_nCols;

  void* pRowSingle = aCombinedT->m_pRowSingle;
  struct HeadLine_T* aHeadLine = aCombinedT->m_aHeadLine0;
  enum eSI eStrInflateRule = aCombinedT->m_eStrInflateRule ;

//  i8**       aaCellArray      ;
//  char*      racStrArea       ;
//  enum eSI   eStrInflateRule  ;
//  i8*        iaColSet         ;

#define nullptr 0

  if (bTrace) {
    printf("vFLAT_Parameters:\n",pTub);
    printf("  pRowSingle %p\n",pRowSingle);
    printf("  pTub %p\n",pTub);
    printf("  aHeadLine %p\n",aHeadLine);
    printf("  iHeadSize %lld\n",iHeadSize);
    printf("  nRows %lld\n",nRows);
    printf("  nCols %lld\n",nCols);
    printf("  eStrInflateRule %lld\n",(i8)eStrInflateRule);
  }

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
