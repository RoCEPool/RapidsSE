/*============================================================================*/
/* vFLAT_Parameters.h -- Preliminary Fragement for Inflate                    */
/*============================================================================*/

  i8 bTrace    = aCrwTubInflate->m_bTrace;
  void* pTub   = aCrwTubInflate->m_pTub;
  i8 iHeadSize = aCrwTubInflate->m_iHeadSize;
  i8 nRows     = aCrwTubInflate->m_nRows;
  i8 nCols     = aCrwTubInflate->m_nCols;
  struct HeadLine_T* aHeadLine = aCrwTubInflate->m_aHeadLine0;
  enum eSI eStrInflateRule = aCrwTubInflate->m_eStrInflateRule ;

//  i8**       aaCellArray      ;
//  char*      racStrArea       ;
//  enum eSI   eStrInflateRule  ;
//  i8*        iaColSet         ;

#define nullptr 0

  if (bTrace) {
    printf("vFLAT_Parameters:\n",pTub);
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
