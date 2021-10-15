/*============================================================================*/
/* vFLAT_preliminary.h -- Preliminary Fragement for Inflate                   */
/*============================================================================*/
  
//RASSERT(SI_CELLARRAY_ADR == eStrInflateRule || SI_CELLARRAY_PST == eStrInflateRule);

#ifdef __TINYC__

  i8**       aaCellArray      ;
  char*      racStrArea       ;
  enum eSI   eStrInflateRule  ;
  struct HeadLine_T*  aHeadLine        ;
  i8*        iaColSet         ;
  i8         nCols            ;

#define nullptr 0

#else

  i8**       aaCellArray      = _aaCellArray;
  char*&     racStrArea       = _racStrArea;
  eSI        eStrInflateRule  = _eStrInflateRule;
  HeadLine_T*  aHeadLine        = _aHeadLine;
  i8*        iaColSet         = _iaColSet;
  i8         nCols            = _nCols;

#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  u1* auMetaCode = META_AREA(aHeadLine);

  i8 i_BaseOffsetData = C_1B   // 1 byte for the headline size (in words)
                      + C_8B   // (ControlDate) 8 bytes for the perspective
                      + nCols; // metaSize

  byte* ayDataByte = ADDR_ADD(byte*,aHeadLine,i_BaseOffsetData);

  i8*   aiDstk_lcl = *aaCellArray;
  byte* ayDstByte  = (byte*)++aiDstk_lcl;
  char* acStrArea  = nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  i8*  aiCellValue;
  Edt* aEdt;
  Dte* aDte;

  OverLine_T* aOverLine;
  TailLine_T* aTailLine;

  i8 nHeadBytes;
  u1 uHigh;
  u1 uLow;
  i8 nRoundedStrBytes;
  u1 uMetaCode;
  i8 nSizeUnits;
  i8 nStrSize;

  i8 i_Over_rel;
  u2 u_Tail_rel;

  char** aacCellValue;
  char*  azDst;
  void*  aSubReturnLabel;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
