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

  ayDstByte = (byte*)aDynamicT->m_xRowInfoIT.m_aRowCellArray;
  //aDynamicT->m_xRowInfoT.m_aRowCellArray = ayDstByte;
  acStrArea = (char*)aDynamicT->m_xRowInfoIT.m_acStrAreaSav;
  racStrArea = (char*)aDynamicT->m_xRowInfoIT.m_acStrAreaSav;
  eStrInflateRule = SI_STRAREA_ADR;

  if (aDynamicT->m_bTrace) {
    printf("  i_Row %lld aHeadLine %p racStrArea %p\n",
    i_Row,aHeadLine,racStrArea);
  }
  goto L_InflateCrwT;

L_AdvanceNextCrwT:
  aDynamicT->m_iJoinedRows = 0;
  i_Row++;

  if (i_Row == aDynamicT->m_nRows)
    goto L_Return;

  // get the headline
  aHeadLine = ADDR_ADD(struct HeadLine_T*,aHeadLine,iHeadSize);
  goto L_ProcessRow;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
