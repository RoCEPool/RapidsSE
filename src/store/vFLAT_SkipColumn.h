/*============================================================================*/
/* vFLAT_SkipColumn.h -- Skip Column Inflation                                */
/*============================================================================*/
  
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// 2) skip a column
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

L_SkipAColumn:

L_Overflowed:
  
  if (uHigh <=  MC_STRING_x1) // numeric & date
    ayDataByte += uLow;
  else
    if (uHigh >=  MC_NEGATIVE)  // small value
    {;}
  else
  if (uHigh == MC_STRING_x1)  // string x 1
    ayDataByte += STRx1(uLow);
  else
  if (uHigh == MC_STRING_x2)  // string x 2
    ayDataByte += STRx2(uLow);
  else
  if (uHigh == MC_STRING_x4)  // string x 4
    ayDataByte += STRx4(uLow);
  else
  if (uHigh == MC_STRING_x8)  // string x 8
    ayDataByte += STRx8(uLow);
  else
  if (uHigh == MC_STRING_TAIL)  // string tail
    ayDataByte += 2;
  else
  if (uHigh == MC_OVERFLOW) { // overflow
    
    //OverLine* aOverLine = aHeadLine->aGetOver();
    
    // calculation to get overflow
    nSizeUnits = aHeadLine->w8_1B(C_HEADLINE_OFS_WORDS);
    if (nSizeUnits >= 128) nSizeUnits -= 128;
    nHeadBytes = nSizeUnits * C_BYTES_PER_WORD;
    i_Over_rel = aHeadLine->w8_2B(nHeadBytes - C_2B);
    
    aOverLine = RowNub::aGetOver_rel(aHeadLine,i_Over_rel);
    //OverLine* aOverLine = (OverLine*)aGetTail_rel(aHeadLine,i_Over_rel);
    
    uMetaCode = *((u1*)aOverLine + C_U_1B);
    METACODE_SPLIT(uMetaCode,uHigh,uLow);
    ayDataByte = (byte*)aOverLine + C_U_1B + C_M_1B;
    goto L_Overflowed;
  }
  else if (uHigh == MC_NULL) {
    // nothing
  }
  else {
    RASSERT(0);
  }

  goto *aSubReturnLabel;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
