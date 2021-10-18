/*============================================================================*/
/* vFLAT_InflateColumn.h -- Column Inflator                                   */
/*============================================================================*/
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 1) apply the filter to the row
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

L_FilterRow:
  pRowStruct = (struct SR_IT*)&aRow->m_wxaColumnCell[0];
  if (pRowStruct->L_ORDERKEY > 2) {
    goto L_AdvanceNextCrw;
  }
  else {
    pRowBuffer->vDiscardInvalidRow();
    goto L_AdvanceNextCrw;
  }

      
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
