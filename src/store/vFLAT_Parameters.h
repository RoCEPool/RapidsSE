/*============================================================================*/
/* vFLAT_Parameters.h -- Parameters Fragement for Inflate                     */
/*============================================================================*/

  byte* ayDataByte;
  u1* auMetaCode;
  u1 uMetaCode;
  u1 uHigh;
  u1 uLow;

  void*  aSubReturnLabel;

  i8 nSizeUnits;
  i8 nHeadBytes;
  i8 i_Over_rel;
  struct OverLine_T* aOverLine;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // added for inflate

  i8*    aiCellValue;
  char** aacCellValue;
  char*  azDst;
  i8     nRoundedStrBytes;
  byte*  ayDstByte;
  char*  acStrArea;   //why??
  char*  racStrArea;  //?????
  i8     nStrSize;
  //i8     nRowStrSize;
  u2     u_Tail_rel;
  void*  aRow;

  struct TailLine_T* aTailLine;

  union Edt_T* aEdt;
  union Dte_T* aDte;

  i8 bTrace    = aCombinedT->m_bTrace;
  void* pTub   = aCombinedT->m_pTub;
  i8 iHeadSize = aCombinedT->m_iHeadSize;
  i8 nRows     = aCombinedT->m_nRows;
  i8 nCols     = aCombinedT->m_nCols;

  //static i8 iProjectInit = 0;
  static i8 iAllocRow = 1;
  static i8 iIndexJoinInit = 0;
  
  void* pRowSingle = aCombinedT->m_pRowSingleT;
  struct HeadLine_T* aHeadLine = aCombinedT->m_aHeadLine0;
  enum eSI eStrInflateRule = aCombinedT->m_eStrInflateRule ;

  i8 i_BaseOffsetData = C_1B   // 1 byte for the headline size (in words)
                      + C_8B   // (ControlDate) 8 bytes for the perspective
                      + nCols; // metaSize

  i8 i_Col;

#define nullptr 0

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
