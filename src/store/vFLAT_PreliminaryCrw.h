/*============================================================================*/
/* vFLAT_preliminaryCrw.h -- Preliminary Fragment - Inflate crwBuffer to Row  */
/*============================================================================*/

  i8 i_BaseOffsetData = C_1B   // 1 byte for the headline size (in words)
                        + C_8B   // (ControlDate) 8 bytes for the perspective
                        + nCols; // metaSize

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
  i8     nRowStrSize;
  u2     u_Tail_rel;
  void*  aRow;

  struct TailLine_T* aTailLine;

  union Edt_T* aEdt;
  union Dte_T* aDte;

  if ( !aHeadLine )
    goto L_ProcessEof;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // for each row
  i8 i_Row = 0;

L_ProcessRow:
  ayDataByte = ADDR_ADD(byte*,aHeadLine,i_BaseOffsetData);

  auMetaCode = META_AREA(aHeadLine);
  i8 i_Col = -1;

  ayDstByte = (byte*)aRowSingle_GetCellArray(pRowSingle,&racStrArea);
  aRow = ayDstByte;
  acStrArea = racStrArea;
  eStrInflateRule = SI_STRAREA_OFS;

  if (bTrace) {
    printf("  i_Row %lld aHeadLine %p racStrArea %p\n",
    i_Row,aHeadLine,racStrArea);
  }
  goto L_InflateT;

L_AdvanceNextRow:

  //nRowStrSize = racStrArea - acStrArea;
  //vTub_RowStrSize(pTub,nRowStrSize);

  i_Row++;

  if (i_Row == nRows)
    goto L_Return;

  // get the headline
  aHeadLine = ADDR_ADD(struct HeadLine_T*,aHeadLine,iHeadSize);
  goto L_ProcessRow;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
