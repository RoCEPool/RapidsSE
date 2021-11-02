/*============================================================================*/
/* vFLAT_InflateAllRowsCols.h -- Iterate and inflate all rows and cols        */
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
  u2     u_Tail_rel;

  struct TailLine_T* aTailLine;

  union Edt_T* aEdt;
  union Dte_T* aDte;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  i8 iInflatedInt;
  char acInflatedVch[256];

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 i_Row = 0;

L_ProcessRow:

  ayDataByte = ADDR_ADD(byte*,aHeadLine,i_BaseOffsetData);
  auMetaCode = META_AREA(aHeadLine);
  i8 i_Col = -1;

  aTub_AllotRow(pTub,&racStrArea);

  
  if (bTrace) {
    printf("  i_Row %lld aHeadLine %p racStrArea %p\n",
           i_Row,aHeadLine,racStrArea);
  }


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

L_ProcessCol:

  i_Col++;

  if (i_Col >= nCols)
    goto L_AdvanceNextRow;

  if (bTrace) {
    printf("    i_Col %lld auMetaCode %p ayDataByte %p ",
           i_Col,auMetaCode,ayDataByte);
  }

  uMetaCode = *auMetaCode++;
  uHigh = uMetaCode >> 4;
  uLow  = uMetaCode & 0x0F;

  if (bTrace) {
    printf("uHigh %d uLow %d\n",uHigh,uLow);
  }

  
  iInflatedInt = 0;
  acInflatedVch[0] = '\0';

  aiCellValue = &iInflatedInt;
  ayDstByte   = (byte*)&iInflatedInt;
  eStrInflateRule = SI_STRAREA_ADR;
  acStrArea = acInflatedVch;


  aSubReturnLabel = && L_RetsubInflateAColumn;
  goto L_InflateAColumn;
  L_RetsubInflateAColumn:

  if (bTrace) {
    printf("      %lld `%s`\n",iInflatedInt,acInflatedVch);
  }


  goto L_ProcessCol;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

L_AdvanceNextRow:

  vTub_DiscardRow(pTub);

  i_Row++;

  if (i_Row >= nRows)
    goto L_Return;

  aHeadLine = ADDR_ADD(struct HeadLine_T*,aHeadLine,iHeadSize);

  goto L_ProcessRow;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
