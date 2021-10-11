/*============================================================================*/
/* vFLAT_drivingLoopCrw.h -- Inflate Driving Loop for crwBuffer               */
/*============================================================================*/
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 0) each column
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

L_AdvanceNextColumn:
  
  i_Col++;
  
  uMetaCode = *auMetaCode++;
  uHigh = uMetaCode >> 4;
  uLow  = uMetaCode & 0x0F;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // ignore column
  
  if (i_Col != i_ColToRetrieve) {
        
    aSubReturnLabel = && L_RetsubSkipAColumn;
    goto L_SkipAColumn;
    L_RetsubSkipAColumn:
    
    goto L_AdvanceNextColumn;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // inflate column
 
  aSubReturnLabel = && L_RetsubInflateAColumn;
  goto L_InflateAColumn;
  L_RetsubInflateAColumn:

  i_ColToRetrieve = iaColSet[++i_WhichCol];
  if (-1 != i_ColToRetrieve)
    goto L_AdvanceNextColumn;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // end of columns
    
  // update callers aiDstk
  //aiDstk_lcl = (i8*)ayDstByte;
  //*aaCellArray = --aiDstk_lcl;

  goto L_AdvanceNextCrw;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
