/*============================================================================*/
/* vFLAT_drivingLoop.h -- Inflate Driving Loop                                */
/*============================================================================*/
  
  i8 i_WhichCol = 0;
  i8 i_ColToRetrieve = iaColSet[i_WhichCol];
  i8 i_ColOfRow = -1;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 0) each column
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

L_AdvanceNextColOfRow:
  
  i_ColOfRow++;
  
  uMetaCode = *auMetaCode++;
  uHigh = uMetaCode >> 4;
  uLow  = uMetaCode & 0x0F;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // ignore column
  
  if (i_ColOfRow != i_ColToRetrieve) {
        
    aSubReturnLabel = && L_RetsubSkipAColumn;
    goto L_SkipAColumn;
    L_RetsubSkipAColumn:
    
    goto L_AdvanceNextColOfRow;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // inflate column
 
  aSubReturnLabel = && L_RetsubInflateAColumn;
  goto L_InflateAColumn;
  L_RetsubInflateAColumn:

  i_ColToRetrieve = iaColSet[++i_WhichCol];
  if (-1 != i_ColToRetrieve)
    goto L_AdvanceNextColOfRow;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // end of columns
    
  // update callers aiDstk
  aiDstk_lcl = (i8*)ayDstByte;
  *aaCellArray = --aiDstk_lcl;

  return;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
