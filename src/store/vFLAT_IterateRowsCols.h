/*============================================================================*/
/* vFLAT_IterateRowsCols.h -- Iterate all rows and cols                       */
/*============================================================================*/

  i8 i_Row = 0;

L_ProcessRow:

  if (bTrace) {
    printf("  i_Row %lld aHeadLine %p\n",i_Row,aHeadLine);
  }

L_AdvanceNextRow:

  i_Row++;

  if (i_Row >= nRows)
    goto L_Return;

  aHeadLine = ADDR_ADD(struct HeadLine_T*,aHeadLine,iHeadSize);

  goto L_ProcessRow;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
