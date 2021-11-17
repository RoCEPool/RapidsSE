/*============================================================================*/
/* vFLAT_preliminaryCrw.h -- Preliminary Fragment - Inflate crwBuffer to Row  */
/*============================================================================*/

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // for each row
  i8 i_Row = 0;

L_ProcessRow:
  ayDataByte = ADDR_ADD(byte*,aHeadLine,i_BaseOffsetData);

  auMetaCode = META_AREA(aHeadLine);
  i_Col = -1;

  ayDstByte = (byte*)aRowSingle_GetCellArray(pRowSingle,&aCombinedT->m_racStrAreaT);
  aCombinedT->m_aRowCellArrayT = ayDstByte;
  acStrArea = (char*)aCombinedT->m_racStrAreaT;
  racStrArea = (char*)aCombinedT->m_racStrAreaT;
  eStrInflateRule = SI_STRAREA_OFS;

  if (bTrace) {
    printf("  i_Row %lld aHeadLine %p racStrArea %p\n",
    i_Row,aHeadLine,racStrArea);
  }
  goto L_InflateT;

L_AdvanceNextRow:

  i_Row++;

  if (i_Row == nRows)
    goto L_Return;

  // get the headline
  aHeadLine = ADDR_ADD(struct HeadLine_T*,aHeadLine,iHeadSize);
  goto L_ProcessRow;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
