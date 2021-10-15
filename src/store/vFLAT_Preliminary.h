/*============================================================================*/
/* vFLAT_preliminary.h -- Preliminary Fragement for Inflate                   */
/*============================================================================*/
  
//RASSERT(SI_CELLARRAY_ADR == eStrInflateRule || SI_CELLARRAY_PST == eStrInflateRule);


#ifdef __TINYC__

  i8**       aaCellArray      ;
  char*&     racStrArea       ;
  eSI        eStrInflateRule  ;
  HeadLine_T*  aHeadLine        ;
  i8*        iaColSet         ;
  i8         nCols            ;

#else

  i8**       aaCellArray      = _aaCellArray;
  char*&     racStrArea       = _racStrArea;
  eSI        eStrInflateRule  = _eStrInflateRule;
  HeadLine_T*  aHeadLine        = _aHeadLine;
  i8*        iaColSet         = _iaColSet;
  i8         nCols            = _nCols;

#endif


  i8 nRoundedStrBytes;

  //vTrace("vInflate_ColumnSet_into_CellArray\n"_Stz);
  
  u1* auMetaCode;
  byte* ayDataByte;
  
  //auMetaCode = aHeadLine->auMetaArea();
  auMetaCode = META_AREA(aHeadLine);

  i8 i_BaseOffsetData = C_1B   // 1 byte for the headline size (in words)
                      + C_8B   // (ControlDate) 8 bytes for the perspective
                      + nCols; // metaSize
  
  //ayDataByte = aAdd<byte*>(aHeadLine,i_BaseOffsetData);

  ayDataByte = ADDR_ADD(byte*,aHeadLine,i_BaseOffsetData);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //

  char* acStrArea = nullptr;
  
  i8* aiDstk_lcl = *aaCellArray;
  
  byte* ayDstByte = (byte*)++aiDstk_lcl;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // for each column

  i8 i_WhichCol = 0;
  i8 i_ColToRetrieve = iaColSet[i_WhichCol];
  
  i8 nSizeUnits;
  i8 nHeadBytes;
  i8 i_Over_rel;
  i8 nStrSize;
  
  i8* aiCellValue;
  u1 uMetaCode;
  u1 uHigh;
  u1 uLow;
  
  u2 u_Tail_rel;
  
  char* azDst;
  char** aacCellValue;
  
  Edt* aEdt;
  Dte* aDte;
  
  OverLine_T* aOverLine;
  TailLine_T* aTailLine;
  
  i8 i_Col = -1;

  void* aSubReturnLabel;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
