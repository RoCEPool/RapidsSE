/*============================================================================*/
/* vFLAT_preliminaryCrw.h -- Preliminary Fragement for Inflate of crwBuffer   */
/*============================================================================*/
  
//RASSERT(SI_CELLARRAY_ADR == eStrInflateRule || SI_CELLARRAY_PST == eStrInflateRule);

  StreamDef* pStreamDef       = m_pStreamDef;

  //i8 iaColSet[pStreamDef->nColumns()+1];

  // inflate all the columns
  //for (i8 i_Col = 0; i_Col < pStreamDef->nColumns(); i_Col++)
  //  iaColSet[i_Col] = i_Col;
  //iaColSet[pStreamDef->nColumns()] = C_UNKNOWN;

  RowBuffer* pRowBuffer = nullptr;
  i8 nCrws = m_oRowTray.pNub()->nHeads();
  RowPix* pRowPix = pStreamDef->pRowPix();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // for each crw
  i8 i_Crw = -1;

L_AdvanceNextCrw:
  i_Crw++;

  if (i_Crw == nCrws)
    return;

  // get the headline
  HeadLine_T *aHeadLine = (HeadLine_T *)aGetHead(i_Crw);

  // ignore deleted headlines
  //if (aHeadLine->bIsDeleted())
  //  goto L_AdvanceNextCrw;

  pRowBuffer = _pRowBuffer(pTub->pWorkingBuffer());
  if (! pRowBuffer) {
    pRowBuffer = new RowBuffer;
    pRowBuffer->vCopyRowPix(pRowPix);
    pTub->vAddBuffer(pRowBuffer);
  }

  S_Row* aRow = (S_Row*)pRowBuffer->pAllotRow();

  if (! aRow) {
    pRowBuffer = new RowBuffer;
    pTub->vAddBuffer(pRowBuffer);
    pRowBuffer->vCopyRowPix(pRowPix);
    aRow = (S_Row*)pRowBuffer->pAllotRow();
    ASSERT(aRow);
  }

  i8*    aCellArray      = (i8*)&aRow->m_wxaColumnCell[0];
  i8**   aaCellArray     = &aCellArray;
  char*  racStrArea      = (char*)pRowBuffer->azStrArea();
  eSI    eStrInflateRule = SI_STRAREA_OFS;

  i8 nRoundedStrBytes;

  //vTrace("vInflate_ColumnSet_into_CellArray\n"_Stz);
  
  u1* auMetaCode;
  byte* ayDataByte;
  
  i8 nCols = pStreamDef->nColumns();

  //auMetaCode = aHeadLine->auMetaArea();
  auMetaCode = META_AREA(aHeadLine);

  i8 i_BaseOffsetData = C_W_1B   // (ControlDate) 1 byte for the headline size (in words)
  + C_T_8B // (ControlDate) 8 bytes for the perspective
  + nCols; // metaSize
  
  ayDataByte = aAdd<byte*>(aHeadLine,i_BaseOffsetData);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //

  char* acStrArea = nullptr;
  
  //i8* aiDstk_lcl = *aaCellArray;
  
  byte* ayDstByte = (byte*)*aaCellArray;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // for each column

  i8 i_WhichCol = 0;
  //i8 i_ColToRetrieve = iaColSet[i_WhichCol];
  
  i8 nSizeUnits;
  i8 nHeadBytes;
  i8 i_Over_rel;
  i8 nStrSize;
  
  i8* aiCellValue;
  u1 uMetaCode;
  u1 uHigh;
  u1 uLow;
  
  u2 u_Tail_rel;
  
  char* azDst;
  char** aacCellValue;
  
  Edt* aEdt;
  Dte* aDte;
  
  OverLine_T* aOverLine;
  TailLine_T* aTailLine;
  
  i8 i_Col = -1;

  void* aSubReturnLabel;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
