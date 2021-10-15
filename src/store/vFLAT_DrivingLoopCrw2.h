/*============================================================================*/
/* vFLAT_drivingLoopCrw2.h -- Inflate Driving Loop for crwBuffer               */
/*============================================================================*/

  i8 nBytesTaken;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Inflate column 0
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  i_Col = 0;
  
  uMetaCode = *auMetaCode++;
  uHigh = uMetaCode >> 4;
  uLow  = uMetaCode & 0x0F;

  aSubReturnLabel = && L_RetsubInflateColumn1;
  goto L_InflateAColumn;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Inflate column 1
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  L_RetsubInflateColumn1:

  i_Col = 1;

  uMetaCode = *auMetaCode++;
  uHigh = uMetaCode >> 4;
  uLow  = uMetaCode & 0x0F;

  aSubReturnLabel = && L_RetsubInflateColumn2;
  goto L_InflateAColumn;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Inflate column 2
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  L_RetsubInflateColumn2:

  i_Col = 2;

  uMetaCode = *auMetaCode++;
  uHigh = uMetaCode >> 4;
  uLow  = uMetaCode & 0x0F;

  aSubReturnLabel = && L_RetsubInflateColumn3;
  goto L_InflateAColumn;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Inflate column 3
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  L_RetsubInflateColumn3:

  i_Col = 3;

  uMetaCode = *auMetaCode++;
  uHigh = uMetaCode >> 4;
  uLow  = uMetaCode & 0x0F;

  aSubReturnLabel = && L_RetsubInflateColumn4;
  goto L_InflateAColumn;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Inflate column 4
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  L_RetsubInflateColumn4:

  i_Col = 4;

  uMetaCode = *auMetaCode++;
  uHigh = uMetaCode >> 4;
  uLow  = uMetaCode & 0x0F;

  aSubReturnLabel = && L_RetsubInflateColumn5;
  goto L_InflateAColumn;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Inflate column 5
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  L_RetsubInflateColumn5:

  i_Col = 5;

  uMetaCode = *auMetaCode++;
  uHigh = uMetaCode >> 4;
  uLow  = uMetaCode & 0x0F;

  aSubReturnLabel = && L_RetsubInflateColumn6;
  goto L_InflateAColumn;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Inflate column 6
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  L_RetsubInflateColumn6:

  i_Col = 6;

  uMetaCode = *auMetaCode++;
  uHigh = uMetaCode >> 4;
  uLow  = uMetaCode & 0x0F;

  aSubReturnLabel = && L_RetsubInflateColumn7;
  goto L_InflateAColumn;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Inflate column 7
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  L_RetsubInflateColumn7:

  i_Col = 7;

  uMetaCode = *auMetaCode++;
  uHigh = uMetaCode >> 4;
  uLow  = uMetaCode & 0x0F;

  aSubReturnLabel = && L_RetsubInflateColumn8;
  goto L_InflateAColumn;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Inflate column 8
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  L_RetsubInflateColumn8:

  i_Col = 8;

  uMetaCode = *auMetaCode++;
  uHigh = uMetaCode >> 4;
  uLow  = uMetaCode & 0x0F;

  aSubReturnLabel = && L_RetsubInflateColumn9;
  goto L_InflateAColumn;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Inflate column 9
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  L_RetsubInflateColumn9:

  i_Col = 9;

  uMetaCode = *auMetaCode++;
  uHigh = uMetaCode >> 4;
  uLow  = uMetaCode & 0x0F;

  aSubReturnLabel = && L_RetsubInflateColumn10;
  goto L_InflateAColumn;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Inflate column 10
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  L_RetsubInflateColumn10:

  i_Col = 10;

  uMetaCode = *auMetaCode++;
  uHigh = uMetaCode >> 4;
  uLow  = uMetaCode & 0x0F;

  aSubReturnLabel = && L_RetsubInflateColumn11;
  goto L_InflateAColumn;


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Inflate column 11
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  L_RetsubInflateColumn11:

  i_Col = 11;

  uMetaCode = *auMetaCode++;
  uHigh = uMetaCode >> 4;
  uLow  = uMetaCode & 0x0F;

  aSubReturnLabel = && L_RetsubInflateColumn12;
  goto L_InflateAColumn;


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Inflate column 12
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  L_RetsubInflateColumn12:

  i_Col = 12;

  uMetaCode = *auMetaCode++;
  uHigh = uMetaCode >> 4;
  uLow  = uMetaCode & 0x0F;

  aSubReturnLabel = && L_RetsubInflateCleanupColumn12;
  goto L_InflateAColumn;

L_RetsubInflateCleanupColumn12:

  nBytesTaken = nDifference(racStrArea,pRowBuffer->azStrArea());
  pRowBuffer->vTakeHead(nBytesTaken);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // end of columns

  goto L_FilterRow;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
