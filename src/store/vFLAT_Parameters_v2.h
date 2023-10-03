/*============================================================================*/
/* vFLAT_Parameters_v2.h -- Parameters Fragement for Inflate of row           */
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

  i8*    aiCellValue      __attribute__ ((unused));
  char** aacCellValue     __attribute__ ((unused));
  char*  azDst            __attribute__ ((unused));
  i8     nRoundedStrBytes __attribute__ ((unused));
  byte*  ayDstByte        __attribute__ ((unused));
  char*  acStrArea        __attribute__ ((unused));
  char*  racStrArea       __attribute__ ((unused));
  i8     nStrSize         __attribute__ ((unused));
  u2     u_Tail_rel       __attribute__ ((unused));
  void*  aRow             __attribute__ ((unused));

  struct TailLine_T* aTailLine __attribute__ ((unused));

  union Edt_T* aEdt __attribute__ ((unused));
  union Dte_T* aDte __attribute__ ((unused));

  i8 iHeadSize = aDynamicT->m_iHeadSize;

  struct HeadLine_T* aHeadLine = aDynamicT->m_aHeadLine0;
  struct HeadLine_T* aHeadLineSav = aHeadLine;
  enum eSI eStrInflateRule = aDynamicT->m_eStrInflateRule ;

  i8 i_BaseOffsetData = C_1B   // 1 byte for the headline size (in words)
                      + C_8B   // (ControlDate) 8 bytes for the perspective
                      + aDynamicT->m_nCols; // metaSize


  i8 i_Col;

#define nullptr 0

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
