/*============================================================================*/
/* vFLAT_InflateAllRowsCols.h -- Iterate and inf;ate all rows and cols        */
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
  
  i8 i_Row = 0;

L_ProcessRow:

  ayDataByte = ADDR_ADD(byte*,aHeadLine,i_BaseOffsetData);
  auMetaCode = META_AREA(aHeadLine);
  i8 i_Col = -1;

  if (bTrace) {
    printf("  i_Row %lld aHeadLine %p\n",i_Row,aHeadLine);
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

  aSubReturnLabel = && L_RetsubSkipAColumn;
  goto L_SkipAColumn;
  L_RetsubSkipAColumn:

  goto L_ProcessCol;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

L_AdvanceNextRow:

  i_Row++;

  if (i_Row >= nRows)
    goto L_Return;

  aHeadLine = ADDR_ADD(struct HeadLine_T*,aHeadLine,iHeadSize);

  goto L_ProcessRow;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
