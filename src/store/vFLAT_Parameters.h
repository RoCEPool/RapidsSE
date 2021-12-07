/*============================================================================*/
/* vFLAT_Parameters.h -- Parameters Fragement for Inflate of row              */
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
  char*  acStrArea;
  char*  racStrArea;
  i8     nStrSize;
  u2     u_Tail_rel;
  void*  aRow;

  struct TailLine_T* aTailLine;

  union Edt_T* aEdt;
  union Dte_T* aDte;

  i8 iHeadSize = aCombinedT->m_iHeadSize;

  static i8 iIndexJoinInit = 0;

  struct HeadLine_T* aHeadLine = aCombinedT->m_aHeadLine0;
  struct HeadLine_T* aHeadLineJ;
  enum eSI eStrInflateRule = aCombinedT->m_eStrInflateRule ;

  i8 i_BaseOffsetData = C_1B   // 1 byte for the headline size (in words)
                      + C_8B   // (ControlDate) 8 bytes for the perspective
                      + aCombinedT->m_nCols; // metaSize

  i8 i_BaseOffsetDataJ = C_1B   // 1 byte for the headline size (in words)
                       + C_8B   // (ControlDate) 8 bytes for the perspective
                       + aCombinedT->m_nCols; // metaSize

  i8 i_Col;

#define nullptr 0

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
