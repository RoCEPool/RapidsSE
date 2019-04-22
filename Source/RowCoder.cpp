/*============================================================================*/
/* RowCoder.cpp -- Row Coder                                                  */
/*============================================================================*/

#include "Data.h"

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* analyse nub                                                                */
/*----------------------------------------------------------------------------*/
void RowCoder::vAnalyse(RowNub* pNub)
{
  i8 nTails = 0;
  i8 iSumTail = 0;
  i8 iMinTail = C_I8_MAX;
  i8 iMaxTail = C_I8_MIN;

  i8 nHeads = 0;
  i8 iSumHead = 0;
  i8 iMinHead = C_I8_MAX;
  i8 iMaxHead = C_I8_MIN;
  
  i8 nWouldReduce = 0;
  
  for (i8 i_Tail = pNub->iLastTail();
       i_Tail != C_UNKNOWN;
       i_Tail = pNub->iPrevTail(i_Tail)) {
    
    TailLine* aTail = pNub->aGetTail(i_Tail);
    
    i8 iLenTail = aTail->w8_1B(1);
    RASSERT(iLenTail <= 56);
    iSumTail += iLenTail;
    nTails++;
    
    if (iLenTail < iMinTail)
      iMinTail = iLenTail;
    if (iLenTail > iMaxTail)
      iMaxTail = iLenTail;
  }
  
  for (i8 i_Head = 0; i_Head < pNub->nHeads();  i_Head++) {
    
    HeadLine* aHead = pNub->aGetHead(i_Head);
    
    i8 nHeadFree = nFreeSpaceHead(pNub,aHead);
    iSumHead += nHeadFree;
    nHeads++;
    
    if (nHeadFree < iMinHead)
      iMinHead = nHeadFree;
    if (nHeadFree > iMaxHead)
      iMaxHead = nHeadFree;
    
    i8 nTailUsed = nShocoTailSize(pNub,aHead);
    
    if (nTailUsed != C_UNKNOWN) {
      
      i8 nRemainder = nTailUsed - nHeadFree;
      
      i8 nUnitsTail = pNub->nUnits(nTailUsed);
      i8 nUnitsRemainder = pNub->nUnits(nRemainder);
      
      if (nUnitsRemainder < nUnitsTail)
        nWouldReduce++;
      
    }
  }

  vTrace("Heads %lld Tails %lld; "
         "Avg head free %lldB Min %lldB Max %lldB; "
         "Avg tail use %lldB Min %lldB Max %lldB; "
         "WouldReduce %lld\n"_Stz,
         
         pNub->nHeads(),
         pNub->nTails(),

         nHeads ? iSumHead/nHeads : 0,
         iMinHead,
         iMaxHead,

         nTails ? iSumTail/nTails : 0,
         iMinTail,
         iMaxTail,
         
         nWouldReduce
         );
}

/*----------------------------------------------------------------------------*/
/* copy an encoded row from one tray to another                               */
/*----------------------------------------------------------------------------*/
bool RowCoder::bCopy(RowNub* pIntoNub, RowNub* pFromNub)
{
  static Peruse xPeruse("Split"_Stz);
  
  static i8 nCalls = 0;
  nCalls++;
  
  static i8 nFolds = 0;
  RASSERT(pIntoNub->pDesc() == pFromNub->pDesc());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // check for space in target nub
  i8 nSpaceNeeded = pFromNub->nUsedBytes();
  if (! pIntoNub->bHasSpace(nSpaceNeeded))
    return false;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  ColumnVec* pColVec = pIntoNub->pDesc()->pColumnVec();

  i8 i_IntoHead = pIntoNub->iAllotHead();
  HeadLine* aIntoHead = pIntoNub->aGetHead(i_IntoHead);
  HeadLine* aFromHead = pFromNub->aGetHead(0);
 
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  MemLib::vCopy(aIntoHead,aFromHead,sizeof(HeadLine),CN_ROWBUFFER);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef WITH_TRACE_ENCODING
  RowCoder::vHeadDumpNat(aIntoHead, pIntoNub->pDesc()->pColumnVec());
#endif
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  i8 nColumns = pColVec->nItems();
  i8 i_LastColumn = 0;
  i8 i_FromOffset = 8;
  i8 i_IntoOffset = 8;
  i8 i_OldFromOffset  = 0;
  
  Coda* pIntoCoda = pIntoNub->pCoda();

  for (i8 i_ForCol = 0; i_ForCol < nColumns; i_ForCol++) {
  
    Column* pCol = pColVec->pGet(i_ForCol);
    eDM eDatum = pCol->eDatumType();
    
#ifdef WITH_TRACE_ENCODING
    vTrace("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - \n"_Stz);
#endif
    
    // init coda
    if (0 == i_IntoHead) {
      pIntoCoda->m_aeDatum[i_ForCol] = eDatum;
      pIntoCoda->m_aiAux[i_ForCol].m_iInt = 0;
    }

    // save old fromOffset
    i_OldFromOffset = i_FromOffset;
    eMN eEncoding = eGetEncodingNat(i_ForCol,aFromHead,pColVec,
                                    i_LastColumn,i_FromOffset);
    
    // calculate new IntoOffset
    i_IntoOffset += i_FromOffset - i_OldFromOffset;
 
    //  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    
    if (0 == i_IntoHead && eDatum == DM_INT) {
      vSetAuxInt(pIntoCoda,aIntoHead,pCol,eEncoding,i_IntoOffset);
    }
    
    //  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    //
#ifdef WITH_EXTENDED_DATE
    if (DM_DTE == eDatum) {
      vRecodeDteDays(pIntoNub,aIntoHead,pCol,i_IntoOffset,i_FromOffset);
#ifdef WITH_TRACE_ENCODING
      RowCoder::vHeadDumpColumn(aIntoHead, pIntoNub->pDesc()->pColumnVec(), i_ForCol,"vRecodeDteDays"_Stz);
#endif
      continue;
    }
#endif
 
#ifdef WITH_EXTENDED_INT
    if (DM_INT == eDatum) {
      vRecodeInt(pIntoNub,aIntoHead,pCol,i_IntoOffset,i_FromOffset);
    }
#endif
    
//    if (0 == i_ForCol) {
//      Datum oCol0;
//      i8 i_LastColumn = 0;
//      i8 i_LastOffset = 8;
//      vDecode(&oCol0,aIntoHead,pColVec,pCol,pIntoCoda,i_LastColumn,i_LastOffset);
//    }
    
#ifdef WITH_EXTENDED_BCD
    if (DM_BCD == eDatum) {
      vRecodeBcdTenth(pIntoNub,aIntoHead,pCol,i_IntoOffset,i_FromOffset);
//#ifdef WITH_TRACE_ENCODING
//      RowCoder::vHeadDumpColumn(aIntoHead, pIntoNub->pDesc()->pColumnVec(), i_ForCol, "vRecodeBcdTenth"_Stz);
//#endif
      continue;
    }
#endif
    
    // copy a shoco tail
    if (DM_VCH == eDatum && MN_SHOCO_2B == eEncoding) {
      vCopyShoco(pIntoNub,aIntoHead,i_IntoHead,pFromNub,pCol,
                 i_IntoOffset,i_FromOffset);
#ifdef WITH_TRACE_ENCODING
      RowCoder::vHeadDumpColumn(aIntoHead, pIntoNub->pDesc()->pColumnVec(), i_ForCol, "vCopyShoco"_Stz);
#endif

//      if (15 == i_ForCol &&
//          (MN_SHOCO_2B == aIntoHead->w8_1N(15) || MN_SHOCO_SPLIT == aIntoHead->w8_1N(15)))
//        RowCoder::vHeadDump(aIntoHead, pIntoNub->pDesc()->pColumnVec());

      continue;
    }

#ifdef WITH_EXTENDED_VCH
    // recode a Vch1B into an Alpha
    //!? a recode of Vch1B van only be done on char(1) columns
    if (DM_VCH == eDatum && MN_1B == eEncoding) {
       vRecodeVch1B(pIntoNub, aIntoHead, pCol, i_IntoOffset, i_FromOffset);
#ifdef WITH_TRACE_ENCODING
      RowCoder::vHeadDumpColumn(aIntoHead, pIntoNub->pDesc()->pColumnVec(), i_ForCol, "vRecodeVch1B"_Stz);
#endif
      continue;
    }
#endif
    
  }
  
#ifdef WITH_TRACE_ENCODING
  RowCoder::vHeadDump(aIntoHead, pIntoNub->pDesc()->pColumnVec());
  HV_Rec hvRec;
  RowCoder::vRecord(hvRec,aIntoHead,pColVec);
  hvRec->vPrintRow();
  vTrace("\n"_Stz);
#endif
 
  return true;
}

/*----------------------------------------------------------------------------*/
/* get encoded datum from cache line                                          */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecode(Datum* pDatum, RowNub* pNub, i8 i_Row, Column* pCol,
                       i8& i_LastColumn, i8& i_LastOffset)
{
  HeadLine* aHeadLine = pNub->aGetHead(i_Row);
  
  ColumnVec* pColVec = pNub->pDesc()->pColumnVec();

  Coda* pCoda = pNub->pCoda();
  
  vDecode(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
}

/*----------------------------------------------------------------------------*/
/* get encoded datum from cache line                                          */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecode(Datum* pDatum, HeadLine* aHeadLine,
                       ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                       i8& i_LastColumn, i8& i_LastOffset)
{
  eDM eDatum = pCol->eDatumType();
  eDM eDatumCoda = pCoda->m_aeDatum[pCol->iColumnNo()];
  
  switch (eDatumCoda) {
      
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // the regular datums
      
    case DM_BCD:
      vDecodeBcd(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
      break;
    case DM_BOL:
      vDecodeBol(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
      break;
    case DM_DTE:
      vDecodeDte(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
      break;
    case DM_FLT:
      vDecodeFlt(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
      break;
    case DM_INT:
      vDecodeInt(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
      break;
    case DM_IVL:
      vDecodeIvl(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
      break;
    case DM_TME:
      vDecodeTme(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
      break;
    case DM_TSP:
      vDecodeTsp(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
      break;
    case DM_VCH:
      vDecodeVch(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
      break;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // the extended datums
      
    case DM_ALPHA:
      vDecodeAlpha(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
      break;
    
    case DM_DAYS:
      vDecodeDays(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
      break;
      
    case DM_DELTA_PLUS:
      vDecodeDeltaPlus(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
      break;
      
    case DM_DIGIT:
      vDecodeDigit(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
      break;
      
    case DM_TENTH:
      vDecodeTenth(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
      break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* encode given datum into cache line                                         */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncode(RowNub* pNub, Column* pCol, Datum* pDatum)
{
  i8 i_Column = pCol->iColumnNo();
  
  //! don't encode columns 16+                                             (fix)
  if (i_Column >= 16)
    return;
  
  eDM eDatum = pCol->eDatumType();
  
  Coda* pCoda = pNub->pCoda();
  pCoda->m_aeDatum[i_Column] = eDatum;
  
  switch (eDatum) {
    case DM_BCD: vEncodeBcd(pNub,pCol,pDatum); return;
    case DM_BOL: vEncodeBol(pNub,pCol,pDatum); return;
    case DM_DTE: vEncodeDte(pNub,pCol,pDatum); return;
    case DM_FLT: vEncodeFlt(pNub,pCol,pDatum); return;
    case DM_INT: vEncodeInt(pNub,pCol,pDatum); return;
    case DM_IVL: vEncodeIvl(pNub,pCol,pDatum); return;
    case DM_TME: vEncodeTme(pNub,pCol,pDatum); return;
    case DM_TSP: vEncodeTsp(pNub,pCol,pDatum); return;
    case DM_VCH: vEncodeVch(pNub,pCol,pDatum); return;
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* sprint decode row from cache line into string                              */
/*----------------------------------------------------------------------------*/
void RowCoder::vHeadDumpNat(HeadLine* aHeadLine, ColumnVec* pColVec)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  RowBuffer* pRowBuffer = RowBuffer::pToRowBuffer(aHeadLine);
  Coda* pCoda = pRowBuffer->pRowTray()->pCoda();
  
   MemLib::vWordDump(aHeadLine, sizeof(HeadLine));
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // get no of columns
  i8 nColumns = pColVec->nItems();
  
  Datum oDatum;
  i8 i_LastOffset = 8;
  
  vTrace("Column# ColumnName      Datum      ExtDatum        Encoding \n"_Stz);
  
  // for each column
  for (i8 i_Column = 0; i_Column < nColumns; i_Column++) {
    
    // get the column at index
    Column* pCol = pColVec->pGet(i_Column);
    
    // first get the encoded value into datum
    //vDecode(&oDatum,aHeadLine,pColVec,pCol,pCoda,i_Column,i_LastOffset);
    eDM eDatum = pCol->eDatumType();
    eDM eDatumCoda = pCoda->m_aeDatum[pCol->iColumnNo()];
    eMN eEncoding = eGetEncodingExt(i_Column,aHeadLine,pColVec,pCoda,i_Column,i_LastOffset);
    Stz* aEncoding = sEncoding(eEncoding, eDatum, DM_NONE);
    
    vTrace("%7lld %-15s %-10s %-15s %-16s\n"_Stz, i_Column, pCol->sColumnName().aChar(), aDM[eDatum],
           aDM[eDatumCoda], aEncoding);
    
  }
}

/*----------------------------------------------------------------------------*/
/* sprint decode row from cache line into string                              */
/*----------------------------------------------------------------------------*/
void RowCoder::vHeadDump(HeadLine* aHeadLine, ColumnVec* pColVec)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  RowBuffer* pRowBuffer = RowBuffer::pToRowBuffer(aHeadLine);
  Coda* pCoda = pRowBuffer->pRowTray()->pCoda();
  
  MemLib::vWordDump(aHeadLine, sizeof(HeadLine));
  
  i8 nFreeBytes = nFreeSpaceHead(aHeadLine,pColVec,pCoda);
  vTrace("FreeBytes %lld\n"_Stz,nFreeBytes);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // get no of columns
  i8 nColumns = pColVec->nItems();
  
  HV_Rec hvRec;
  
  i8 i_LastOffset = 8;
  
  vTrace("Col ColumnName      Datum  ExtDatum    Encoding"
         "       Offset  Bytes              Value\n"_Stz);
  
  // for each column
  for (i8 i_Column = 0; i_Column < nColumns; i_Column++) {
    
    // get the column at index
    Column* pCol = pColVec->pGet(i_Column);
    
    // first get the encoded value into datum
    //vDecode(&oDatum,aHeadLine,pColVec,pCol,pCoda,i_Column,i_LastOffset);
    eDM eDatum = pCol->eDatumType();
    eDM eDatumCoda = pCoda->m_aeDatum[pCol->iColumnNo()];
    eMN eEncoding = eGetEncodingExt(i_Column,aHeadLine,pColVec,pCoda,i_Column,i_LastOffset);
    Stz* aEncoding = sEncoding(eEncoding, eDatum, eDatumCoda);

    i8 nBytes = nDataBytes(eDatumCoda,eEncoding);
    
    i8 iNibble = aHeadLine->w8_1N(i_Column);

    vTrace("%3lld %-15s %-6s %-11s %x %-16s %2lld %2lld "_Stz,
           i_Column,
           pCol->sColumnName().aChar(),
           aDM[eDatum],
           aDM[eDatumCoda],
           iNibble,
           aEncoding,
           i_LastOffset,
           nBytes);
    
    if (nBytes)
     for (i8 i_Byte = 0; i_Byte < nBytes; i_Byte++)
        vTrace("%02x"_Stz,aHeadLine->w8_1B(i_LastOffset+i_Byte));
    for (i8 i_Byte = nBytes; i_Byte < 8; i_Byte++)
      vTrace("__"_Stz);
    vTrace(" "_Stz);
    
    hvRec->vReset();
    vAppend(hvRec,aHeadLine,pColVec,pCol,pCoda,i_Column,i_LastOffset);
    vTrace("%s"_Stz,hvRec->szRecStr());
    
    vTrace("\n"_Stz);

    i_LastOffset += nBytes;
    
    if (eDatumCoda == DM_VCH && eEncoding == MN_SHOCO_SPLIT)
      int iDebug = 1;

  }
  
  int iDebug = 1;
}

/*----------------------------------------------------------------------------*/
/* sprint decode row column from cache line into string                       */
/*----------------------------------------------------------------------------*/
void RowCoder::vHeadDumpNatColumn(HeadLine* aHeadLine, ColumnVec* pColVec, i8 iCol)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  RowBuffer* pRowBuffer = RowBuffer::pToRowBuffer(aHeadLine);
  Coda* pCoda = pRowBuffer->pRowTray()->pCoda();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Datum oDatum;
  i8 i_LastOffset = 8;
  
  // get the column at index
  Column* pCol = pColVec->pGet(iCol);
  
  // first get the encoded value into datum
  //vDecode(&oDatum,aHeadLine,pColVec,pCol,pCoda,i_Column,i_LastOffset);
  eDM eDatum = pCol->eDatumType();
  eDM eDatumCoda = pCoda->m_aeDatum[iCol];
  eMN eEncoding = eGetEncodingExt(iCol,aHeadLine,pColVec,pCoda,iCol,i_LastOffset);
  Stz* aEncoding = sEncoding(eEncoding, eDatum, DM_NONE);
  
  vTrace("%7lld %-15s %-10s %-15s %-16s\n"_Stz, iCol, pCol->sColumnName().aChar(), aDM[eDatum],
         aDM[eDatumCoda], aEncoding);
  
}

/*----------------------------------------------------------------------------*/
/* sprint decode row from cache line into string                              */
/*----------------------------------------------------------------------------*/
void RowCoder::vHeadDumpColumn(HeadLine* aHeadLine, ColumnVec* pColVec, i8 iCol, Stz* aRecodeFunction)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  RowBuffer* pRowBuffer = RowBuffer::pToRowBuffer(aHeadLine);
  Coda* pCoda = pRowBuffer->pRowTray()->pCoda();
  
  //MemLib::vWordDump(aHeadLine, sizeof(HeadLine));
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // get no of columns
  i8 nColumns = pColVec->nItems();
  
  Datum oDatum;
  i8 i_LastOffset = 8;
  
  // get the column at index
  Column* pCol = pColVec->pGet(iCol);
  
  // first get the encoded value into datum
  //vDecode(&oDatum,aHeadLine,pColVec,pCol,pCoda,i_Column,i_LastOffset);
  eDM eDatum = pCol->eDatumType();
  eDM eDatumCoda = pCoda->m_aeDatum[iCol];
  eMN eEncoding = eGetEncodingExt(pCol->iColumnNo(),aHeadLine,pColVec,pCoda,iCol,i_LastOffset);
  Stz* aEncoding = sEncoding(eEncoding, eDatum, eDatumCoda);
  vTrace("%7lld %-15s %-10s %-15s %-16s %s\n"_Stz, iCol, pCol->sColumnName().aChar(), aDM[eDatum],
         aDM[eDatumCoda], aEncoding, aRecodeFunction);
}
/*----------------------------------------------------------------------------*/
/* print decoded datum from cache line                                        */
/*----------------------------------------------------------------------------*/
void RowCoder::vPrint(RowNub* pNub, i8 i_Row, Column* pCol, i8& i_LastColumn, i8& i_LastOffset)
{
  Datum oDatum;
  HV_Rec hvRec;
  
  vDecode(&oDatum,pNub,i_Row,pCol,i_LastColumn,i_LastOffset);
  
  eDM eDatum = oDatum.m_eDatum;
  
  switch (eDatum) {
    case DM_BCD: hvRec->vAppendBcd(pCol,&oDatum); break;
    case DM_BOL: hvRec->vAppendBol(pCol,&oDatum); break;
    case DM_DTE: hvRec->vAppendDte(pCol,&oDatum); break;
    case DM_FLT: hvRec->vAppendFlt(pCol,&oDatum); break;
    case DM_INT: hvRec->vAppendInt(pCol,&oDatum); break;
    case DM_IVL: hvRec->vAppendIvl(pCol,&oDatum); break;
    case DM_TME: hvRec->vAppendTme(pCol,&oDatum); break;
    case DM_TSP: hvRec->vAppendTsp(pCol,&oDatum); break;
    case DM_VCH: hvRec->vAppendVch(pCol,&oDatum); break;
    default:
      NEVER_REACH;
  }
  
//  vTrace("Column[%lld %s %s] (e%lld,@%lld) : %s\n"_Stz,
//         pCol->iColumnNo(),
//         pCol->sColumnName().aStz(),
//         aDM[pCol->eDatumType()],
//
//         eEncoding,
//         i_Offset,
//
//         hvRec->szRecStr());
  
}

/*----------------------------------------------------------------------------*/
/* sprint decode row from cache line into string                              */
/*----------------------------------------------------------------------------*/
void RowCoder::vRecord(Record* pRec, RowNub* pNub, i8 i_Row)
{
  HeadLine* aHeadLine = pNub->aGetHead(i_Row);

  ColumnVec* pColVec = pNub->pDesc()->pColumnVec();
  
  vRecord(pRec,aHeadLine,pColVec);
}

/*----------------------------------------------------------------------------*/
/* sprint decode row from cache line into string                              */
/*----------------------------------------------------------------------------*/
void RowCoder::vRecord(Record* pRec, HeadLine* aHeadLine, ColumnVec* pColVec)
{
  pRec->vReset();
  
  //vHeadDump(aHeadLine, pColVec);
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  RowBuffer* pRowBuffer = RowBuffer::pToRowBuffer(aHeadLine);
  Coda* pCoda = pRowBuffer->pRowTray()->pCoda();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // get no of columns
  i8 nColumns = pColVec->nItems();
  
  i8 i_LastColumn = 0;
  i8 i_LastOffset = 8;
  
  // for each column
  for (i8 i_Column = 0; i_Column < nColumns; i_Column++) {
    
    // get the column at index
    Column* pCol = pColVec->pGet(i_Column);
    
    // comma separate column values
    if (i_Column)
      pRec->szRecStr()->vConcat(","_Stz);
    
    // append column value
    vAppend(pRec,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
    //vTrace("%s\n"_Stz,pRec->szRecStr());
  }
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* decode and append column datum from cache line                             */
/*----------------------------------------------------------------------------*/
void RowCoder::vAppend(Record* pRec, HeadLine* aHeadLine,
                       ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                       i8& i_LastColumn, i8& i_Offset)
{
  Datum oDatum;
  
  // first get the encoded value into datum
  vDecode(&oDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_Offset);
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // we have the decoded value in oDatum
  
  eDM eDatum = pCol->eDatumType();
  switch (eDatum) {
    case DM_BCD: pRec->vAppendBcd(pCol,&oDatum); break;
    case DM_BOL: pRec->vAppendBol(pCol,&oDatum); break;
    case DM_DTE: pRec->vAppendDte(pCol,&oDatum); break;
    case DM_FLT: pRec->vAppendFlt(pCol,&oDatum); break;
    case DM_INT: pRec->vAppendInt(pCol,&oDatum); break;
    case DM_IVL: pRec->vAppendIvl(pCol,&oDatum); break;
    case DM_TME: pRec->vAppendTme(pCol,&oDatum); break;
    case DM_TSP: pRec->vAppendTsp(pCol,&oDatum); break;
    case DM_VCH: pRec->vAppendVch(pCol,&oDatum); break;
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* copy an encoded row from one tray to another                               */
/*----------------------------------------------------------------------------*/
void RowCoder::vCopyShoco(RowNub* pIntoNub, HeadLine* aIntoHead, i8 i_IntoHead,
                          RowNub* pFromNub, Column* pCol,
                          i8 i_IntoOffset, i8 i_FromOffset)
{
  static i8 nFolds = 0;
  static Peruse xSplits("Splits"_Stz);
 
  ColumnVec* pColVec = pIntoNub->pDesc()->pColumnVec();

  HeadLine* aFromHead = pFromNub->aGetHead(0);
  TailLine* aIntoTail = nullptr;
  i8 i_Column = pCol->iColumnNo();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy shoco tail line from nub
  i8 i_Distance = aFromHead->w8_2B(i_FromOffset);
  TailLine* aShocoTail = pFromNub->aGetTail(i_Distance);
  
  /*
   +=====================================+
   |            Shoco Tail Line          +
   +=====================================+
   | U | C | <compressed bytes> <unused> +
   +-------------------------------------+
   | 0 | 1 | 2                        63 |
   +-------------------------------------+
   
   where: U is one byte being the unit length of the tail line
   C is one byte being the number of compressed bytes
   */
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef WITH_SHOCO_FOLDLAST
  if (pCol->bIsLastColumn()) {
    
    i8 nCompressed = aShocoTail->w8_1B(C_SHOCO_COMPRESSED_OFFSET);
    //RASSERT(nCompressed <= 56);
    
    i8 nAvailHeadBytes = sizeof(HeadLine) - i_IntoOffset;

    //MemLib::vWordDump(aIntoHead,sizeof(HeadLine));
    
    // FIT case?
    if (C_SHOCO_COMPRESSED_BYTES + nCompressed <= nAvailHeadBytes) {
      
      aIntoHead->v1N(i_Column,MN_SHOCO_FIT);
      
      aIntoHead->v1B(i_IntoOffset,nCompressed);
      
      byte* aFrom = (byte*)aShocoTail + C_SHOCO_COMPRESSED_OFFSET
                                      + C_SHOCO_COMPRESSED_BYTES;
      
      byte* aInto = (byte*)aIntoHead + i_IntoOffset
                                     + C_SHOCO_COMPRESSED_BYTES;
      
      MemLib::vCopy(aInto,aFrom,nCompressed,CN_ROWBUFFER);
      
      nFolds++;
      //if (IntLib::bIsDecaCycle(nFolds))
      //  vTrace("%lld folds\n"_Stz,nFolds);
      
      return;
    }
    
    // SPLIT case?

    // calc the
    i8 nTailOverHang = (nCompressed + 2) % sizeof(DoubleWord);

    if ( (nTailOverHang > 0  && nTailOverHang <= nAvailHeadBytes) ||
         (nTailOverHang == 0 && nAvailHeadBytes > 3) ) {

      //MemLib::vWordDump(aIntoHead,sizeof(HeadLine));
//      HV_Rec hvRec;
//      RowCoder::vRecord(hvRec,aFromHead,pColVec);
//      hvRec->vPrintRow();
//      vTrace("\n"_Stz);

      i8 nShocoTailUnits = aShocoTail->w8_1B(C_TAIL_UNITS_OFFSET);
      RASSERT(nShocoTailUnits > 1);
      //MemLib::vWordDump(aShocoTail,nTailUnits * sizeof(DoubleWord));

      aIntoHead->v1N(i_Column,MN_SHOCO_SPLIT);

      i8 nPartA = nAvailHeadBytes - 2;
      i8 nPartB = nCompressed - nPartA;

      byte* ayInto = (byte*)aIntoHead + i_IntoOffset + 2;
      byte* ayFrom = (byte*)aShocoTail + C_TAIL_UNITS_BYTES;

      MemLib::vCopy(ayInto,ayFrom,nPartA,CN_ROWBUFFER);
      //MemLib::vWordDump(aIntoHead,sizeof(HeadLine));

      RowNub::vCut(aShocoTail,nShocoTailUnits*sizeof(DoubleWord),
                   C_TAIL_UNITS_BYTES,nPartA);
      //MemLib::vWordDump(aShocoTail,nTailUnits * sizeof(DoubleWord));

      i8 i_IntoTail = pIntoNub->iAllotTail(nPartB);
      aIntoTail = pIntoNub->aGetTail(i_IntoTail);

      //MemLib::vWordDump(aIntoTail,(1+nTailUnits) * sizeof(DoubleWord));
      MemLib::vCopy((byte*)aIntoTail  + C_TAIL_UNITS_BYTES,
                    (byte*)aShocoTail + C_TAIL_UNITS_BYTES,
                    nPartB,
                    CN_ROWBUFFER);
      //MemLib::vWordDump(aIntoTail,(1+nTailUnits) * sizeof(DoubleWord));

      // what gets encoded is the unit difference between head and tail
      i8 i_TailDistance = pIntoNub->iUnitDiff(i_IntoHead,i_IntoTail);
      aIntoHead->vPut(i_IntoOffset,i_TailDistance,MN_2B);

      //RowCoder::vRecord(hvRec,aIntoHead,pColVec);
      //hvRec->vPrintRow();
      //vTrace("\n"_Stz);

      //MemLib::vWordDump(aIntoTail,nTailUnits * sizeof(TailLine));
//      if (0 == (nCompressed - (nFreeHeadBytes+1))) {
//        vHeadDump(aIntoHead,pColVec);
//        MemLib::vWordDump(aIntoTail,nNewTailUnits * sizeof(DoubleWord));
//        int iDebug = 0;
//      }
      //vHeadDump(aIntoHead,pColVec);
      //xSplits.vAdd(nPartB,"\n"_Stz);

      return;
    }
    
    // fall thru
    MemLib::vWordDump(aIntoHead,sizeof(HeadLine));
    MemLib::vWordDump(aShocoTail,sizeof(HeadLine));
    
  }
#endif
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef WITH_SHOCO_DUPLICATES
  i8 i_FoundTail = iFindTail(pIntoNub,aShocoTail);
  
  if (C_UNKNOWN != i_FoundTail) {
    
    aIntoTail = pIntoNub->aGetTail(i_FoundTail);
    
    // what gets encoded is the unit difference between head and tail
    i8 i_TailDistance = pIntoNub->iUnitDiff(i_IntoHead,i_FoundTail);
    aIntoHead->vPut(i_IntoOffset,i_TailDistance,MN_2B);
    
    return;
  }
  else {
    //MemLib::vWordDump(aShocoTail,sizeof(HeadLine));
    int iDebug = 0;
  }
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 nTailUnits = aShocoTail->w8_1B(C_TAIL_UNITS_OFFSET);
  //RASSERT(nTailUnits <= 4);
  i8 nShocoBytes = aShocoTail->w8_1B(C_SHOCO_COMPRESSED_OFFSET);
  //RASSERT(nShocoBytes <= 56);
  
  i8 i_IntoTail = pIntoNub->iAllotTail(2 + nShocoBytes);
  aIntoTail = pIntoNub->aGetTail(i_IntoTail);
  
  MemLib::vCopy(aIntoTail,aShocoTail,
                nTailUnits*sizeof(DoubleWord),CN_ROWBUFFER);
  //MemLib::vWordDump(aIntoTail,nTailUnits*sizeof(DoubleWord));

  // what gets encoded is the unit difference between head and tail
  i8 i_TailDistance = pIntoNub->iUnitDiff(i_IntoHead,i_IntoTail);
  aIntoHead->vPut(i_IntoOffset,i_TailDistance,MN_2B);
  //MemLib::vWordDump(aIntoHead,sizeof(HeadLine));
  
  return;
}

/*----------------------------------------------------------------------------*/
/* attempt to re-encode an Int as DeltaPlus                                   */
/*----------------------------------------------------------------------------*/
void RowCoder::vRecodeDteDays(RowNub* pIntoNub, HeadLine* aIntoHead, Column* pCol,
                              i8& i_IntoOffset, i8& i_FromOffset)
{
  RASSERT(DM_DTE == pCol->eDatumType());
  
  i8 i_FirstDate = pCol->iFirstDate();
  if (C_UNKNOWN == i_FirstDate)
    return;
  
  i8 i_Column = pCol->iColumnNo();
  if (i_Column == i_FirstDate)
    return;

  Coda* pCoda = pIntoNub->pCoda();

  eMN eCurrentEncoding = (eMN)aIntoHead->w8_1N(i_Column);
  
  i8 iThisDte = 0;
  switch (eCurrentEncoding) {
      
    case MN_NULL: return;
      
    case MN_SILVER_1B:    iThisDte = aIntoHead->w8_1B(i_IntoOffset); break;
    case MN_SILVER_2B:    iThisDte = aIntoHead->w8_2B(i_IntoOffset); break;
      
    default:
      RASSERT(0);
  }

  Datum oFirstDte;
  ColumnVec* pColVec = pIntoNub->pDesc()->pColumnVec();
  Column* pColFirst = pColVec->pGet(i_FirstDate);
  i8 i_LastColumn = 0;
  i8 i_LastOffset = 8;
  vDecodeDte(&oFirstDte,aIntoHead,pColVec,pColFirst,pCoda,i_LastColumn,i_LastOffset);
  
  i8 iFirstDte = 0;
  
  eMN eFirstEncoding = (eMN)aIntoHead->w8_1N(i_FirstDate);

  switch (eFirstEncoding) {
      
    case MN_NULL: return;
      
    case MN_SILVER_1B:    iFirstDte = aIntoHead->w8_1B(i_LastOffset); break;
    case MN_SILVER_2B:    iFirstDte = aIntoHead->w8_2B(i_LastOffset); break;
      
    default:
      RASSERT(0);
  }

  i8 iDiff = iThisDte - iFirstDte;
  
  if (iDiff >= 0 && iDiff <= (i8)C_1_BYTE) {
    
    // change the encoding to extended days
    pCoda->m_aeDatum[i_Column] = DM_DAYS;
    
    // revise the encoding
    eMN eExtendedEncoding = MN_DAYS_PLUS_1B;
    aIntoHead->v1N(i_Column,eExtendedEncoding);

    //
    i8 i1B = iDiff;
    aIntoHead->v1B(i_IntoOffset,i1B);
    
    // cut 1B char out of the into head
    if (MN_SILVER_2B == eCurrentEncoding) {
      pIntoNub->vCut(aIntoHead,sizeof(HeadLine),i_IntoOffset+1,1);
      i_IntoOffset--;
    }
  }
  else
  if (iDiff >= -255 && iDiff < 0) {
    
    // change the encoding to extended days
    pCoda->m_aeDatum[i_Column] = DM_DAYS;
    
    // revise the encoding
    eMN eExtendedEncoding = MN_DAYS_MINUS_1B;
    aIntoHead->v1N(i_Column,eExtendedEncoding);

    //
    i8 i1B = - iDiff;
    aIntoHead->v1B(i_IntoOffset,i1B);
    
    // cut 1B char out of the into head
    if (MN_SILVER_2B == eCurrentEncoding) {
      pIntoNub->vCut(aIntoHead,sizeof(HeadLine),i_IntoOffset+1,1);
      i_IntoOffset--;
    }
  }
  else {
    vTrace("Huh??\n"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* re-encode an Int as Digit                                                  */
/*----------------------------------------------------------------------------*/
void RowCoder::vRecodeIntDigit(RowNub* pIntoNub, HeadLine* aIntoHead, Column* pCol,
                               i8& i_IntoOffset, i8& i_FromOffset)
{
  eMN eExtendedEncoding;
  
  Coda* pCoda = pIntoNub->pCoda();
  i8 i_Column = pCol->iColumnNo();
  
  eMN eCurrentEncoding = (eMN)aIntoHead->w8_1N(i_Column);
  //vTrace("CurrentEncoding %lld\n"_Stz, eCurrentEncoding);
  
  RASSERT(DM_INT == pCol->eDatumType());
  //RASSERT(MN_1B == eCurrentEncoding);
  
  // change the encoding to extended digit1
  pCoda->m_aeDatum[i_Column] = DM_DIGIT;
  
  // check for NULL
  if (eCurrentEncoding == MN_NULL)
    return;
  
  // is it a 1B encoding ?
  if (eCurrentEncoding == MN_1B) {
    
    // get the 1B int from the data portion of the intoHead
    i8 c1BInt = aIntoHead->w8_1B(i_IntoOffset);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // revise the encoding

    if (c1BInt >= 0 && c1BInt <= 7) {
      
      // revise the encoding
      eExtendedEncoding = (eMN)(MN_DIGIT_0 + c1BInt);
      //vTrace("Rewising column %lld to E%llx\n"_Stz,i_Column, eExtendedEncoding);
      aIntoHead->v1N(i_Column,eExtendedEncoding);
      
      // cut the 1B char out of the into head
      pIntoNub->vCut(aIntoHead,sizeof(HeadLine),i_IntoOffset,1);
      
      // go back one char in the intoOffset
      i_IntoOffset--;
      return;
    }
  }
    
  switch (eCurrentEncoding) {
    case MN_1B: eExtendedEncoding = MN_DIGIT_1B; break;
    case MN_2B: eExtendedEncoding = MN_DIGIT_2B; break;
    case MN_3B: eExtendedEncoding = MN_DIGIT_3B; break;
    case MN_4B: eExtendedEncoding = MN_DIGIT_4B; break;
    case MN_5B:
      // extend by 1B
      pIntoNub->vEntend(aIntoHead,sizeof(HeadLine),i_IntoOffset,1);
      eExtendedEncoding = MN_6B;
      break;
    case MN_6B: eExtendedEncoding = MN_DIGIT_6B; break;
    case MN_7B:
      // extend by 1B
      pIntoNub->vEntend(aIntoHead,sizeof(HeadLine),i_IntoOffset,1);
      eExtendedEncoding = MN_6B;
      break;
    case MN_8B: eExtendedEncoding = MN_DIGIT_8B; break;
    default:
      RASSERT(0);
  }
  
  //vTrace("Rewising column %lld to E%llx\n"_Stz,i_Column, eExtendedEncoding);

  aIntoHead->v1N(i_Column,eExtendedEncoding);

  //vTrace("CurrentEncoding %lld ~ %lld\n"_Stz, eCurrentEncoding, eExtendedEncoding);
}

/*----------------------------------------------------------------------------*/
/* attempt to re-encode an Int depending on extended datums so far            */
/*----------------------------------------------------------------------------*/
void RowCoder::vRecodeInt(RowNub* pIntoNub, HeadLine* aIntoHead, Column* pCol,
                               i8& i_IntoOffset, i8& i_FromOffset)
{
  Coda* pIntoCoda = pIntoNub->pCoda();

  switch (pIntoCoda->m_aeDatum[pCol->iColumnNo()]) {
      
    case DM_DELTA_PLUS:
      bRecodeIntDelta(pIntoNub,aIntoHead,pCol,i_IntoOffset,i_FromOffset);
      break;
      
    case DM_DIGIT:
      vRecodeIntDigit(pIntoNub,aIntoHead,pCol,i_IntoOffset,i_FromOffset);
      break;
      
    case DM_INT:
      if (bRecodeIntDelta(pIntoNub,aIntoHead,pCol,i_IntoOffset,i_FromOffset))
        break;
      
      vRecodeIntDigit(pIntoNub,aIntoHead,pCol,i_IntoOffset,i_FromOffset);
      break;
      
    default:
      break;
  }
}

/*----------------------------------------------------------------------------*/
/* attempt to re-encode an Int as DeltaPlus                                   */
/*----------------------------------------------------------------------------*/
bool RowCoder::bRecodeIntDelta(RowNub* pIntoNub, HeadLine* aIntoHead, Column* pCol,
                               i8& i_IntoOffset, i8& i_FromOffset)
{
//  static DecaCnt dcSame("Same"_Stz);
//  static DecaCnt dcLess("Less"_Stz);
//  static DecaCnt dcLeft("Left"_Stz);
//  static DecaCnt dcPlus1("Plus1"_Stz);
//  static DecaCnt dcPlus2("Plus2"_Stz);

  Coda* pCoda = pIntoNub->pCoda();
  i8 i_Column = pCol->iColumnNo();
  
  eMN eCurrentEncoding = (eMN)aIntoHead->w8_1N(i_Column);
  //vTrace("CurrentEncoding %lld\n"_Stz, eCurrentEncoding);
  
  // if encoding as digit then dont proceed
  if (DM_DIGIT == pCoda->m_aeDatum[i_Column]) {
    return false;
  }

  RASSERT(DM_INT == pCol->eDatumType());
  
  i8 iCurrentInt = 0;
  
  switch (eCurrentEncoding) {
      
    case MN_NULL: return true;
      
    case MN_1B:   iCurrentInt = aIntoHead->w8_1B(i_IntoOffset); break;
    case MN_2B:   iCurrentInt = aIntoHead->w8_2B(i_IntoOffset); break;
    case MN_3B:   iCurrentInt = aIntoHead->w8_3B(i_IntoOffset); break;
    case MN_4B:   iCurrentInt = aIntoHead->w8_4B(i_IntoOffset); break;
    case MN_5B:   iCurrentInt = aIntoHead->w8_5B(i_IntoOffset); break;
    case MN_6B:   iCurrentInt = aIntoHead->w8_6B(i_IntoOffset); break;
    case MN_7B:   iCurrentInt = aIntoHead->w8_7B(i_IntoOffset); break;
    case MN_8B:   iCurrentInt = aIntoHead->w8_8B(i_IntoOffset); break;
      
    case MN_MINUS_ONE: return true;
    case MN_ZERO:      return true;
    case MN_PLUS_ONE:  return true;
      
    default:
      NEVER_REACH;
  }
  
  // defer to vRecodeInt
  if ( (iCurrentInt >= 0 && iCurrentInt <= 7) &&
       DM_INT == pCoda->m_aeDatum[i_Column]  )
    return false;

  i8 iFirstInt = pCoda->m_aiAux[i_Column].m_iInt;
  
  eMN eExtendedEncoding;
 
  if (iCurrentInt < iFirstInt) {
    //dcLess.vInc("\n"_Stz);
    return true;
  }
  
  if (iCurrentInt == iFirstInt) {
    
    // change the encoding to extended deltaPlus
    pCoda->m_aeDatum[i_Column] = DM_DELTA_PLUS;
    
   // revise the encoding in the metaword of into head
    eExtendedEncoding = MN_SAME;
    aIntoHead->v1N(i_Column,eExtendedEncoding);
    
    pIntoNub->vCut(aIntoHead,sizeof(HeadLine),i_IntoOffset,eCurrentEncoding);
    //MemLib::vWordDump(aIntoHead,sizeof(HeadLine));
    
    // go back one char in the intoOffset
    i_IntoOffset -= eCurrentEncoding;
    
    //dcSame.vInc("\n"_Stz);
    return true;
  }
  
  if (iCurrentInt > iFirstInt && iCurrentInt - iFirstInt <= 255) {
    
    //MemLib::vWordDump(aIntoHead,sizeof(HeadLine));
    
    // change the encoding to extended deltaPlus
    pCoda->m_aeDatum[i_Column] = DM_DELTA_PLUS;
    
    // revise the encoding in the metaword of into head
    eExtendedEncoding = MN_PLUS_1B;
    aIntoHead->v1N(i_Column,eExtendedEncoding);
    
    i8 i1B = iCurrentInt - iFirstInt;
    aIntoHead->v1B(i_IntoOffset,i1B);
    
    i8 nToCut = IntLib::iByteMagnitude(iCurrentInt);
    if (nToCut > 1) {
      pIntoNub->vCut(aIntoHead,sizeof(HeadLine),i_IntoOffset+1,nToCut-1);
      i_IntoOffset -= (nToCut-1);
    }
    
    //MemLib::vWordDump(aIntoHead,sizeof(HeadLine));
    //dcPlus1.vInc("\n"_Stz);
    return true;
  }
  
  if (iCurrentInt > iFirstInt && (u8)(iCurrentInt - iFirstInt) <= C_2_BYTES) {
    
    //MemLib::vWordDump(aIntoHead,sizeof(HeadLine));
    
    // change the encoding to extended deltaPlus
    pCoda->m_aeDatum[i_Column] = DM_DELTA_PLUS;
    
    // revise the encoding in the metaword of into head
    eExtendedEncoding = MN_PLUS_2B;
    aIntoHead->v1N(i_Column,eExtendedEncoding);
    
    i8 i2B = iCurrentInt - iFirstInt;
    aIntoHead->v2B(i_IntoOffset,i2B);
    
    i8 nToCut = IntLib::iByteMagnitude(iCurrentInt);
    if (nToCut > 2) {
      pIntoNub->vCut(aIntoHead,sizeof(HeadLine),i_IntoOffset+1,nToCut-2);
      i_IntoOffset -= (nToCut-2);
    }
    
    //MemLib::vWordDump(aIntoHead,sizeof(HeadLine));
    //dcPlus2.vInc("\n"_Stz);
    return true;
  }

  
  //dcLeft.vInc(" (e.g. %lld (first %lld))\n"_Stz,iCurrentInt,iFirstInt);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* re-encode an Bcd as Tenth                                                  */
/*----------------------------------------------------------------------------*/
void RowCoder::vRecodeBcdTenth(RowNub* pIntoNub, HeadLine* aIntoHead, Column* pCol,
                               i8& i_IntoOffset, i8& i_FromOffset)
{
  eMN eExtendedEncoding;
  
  Coda* pCoda = pIntoNub->pCoda();
  i8 i_Column = pCol->iColumnNo();
  
  eMN eCurrentEncoding = (eMN)aIntoHead->w8_1N(i_Column);
  //vTrace("CurrentEncoding %lld\n"_Stz, eCurrentEncoding);
  
  RASSERT(DM_BCD == pCol->eDatumType());
  //RASSERT(MN_1B == eCurrentEncoding);
  
  
  // check for NULL
  if (eCurrentEncoding == MN_NULL)
    return;
  
  if (eCurrentEncoding != MN_1B) {
    RASSERT(DM_BCD == pCoda->m_aeDatum[i_Column]);
    return;
  }

  // is it a 1B encoding ?
  if (eCurrentEncoding == MN_1B) {
    
    // get the 1B int from the data portion of the intoHead
    i8 i1B = aIntoHead->w8_1B(i_IntoOffset);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // revise the encoding
    
    if (i1B >= 0 && i1B <= 10) {
      
      // change the encoding to extended tenth
      pCoda->m_aeDatum[i_Column] = DM_TENTH;
      
     // revise the encoding
      eExtendedEncoding = (eMN)(MN_TENTH_0 + i1B);
      //vTrace("Rewising column %lld to E%llx\n"_Stz,i_Column, eExtendedEncoding);
      aIntoHead->v1N(i_Column,eExtendedEncoding);
      
      // cut the 1B char out of the into head
      pIntoNub->vCut(aIntoHead,sizeof(HeadLine),i_IntoOffset,1);
      
      // go back one char in the intoOffset
      i_IntoOffset--;
      return;
    }
  }
  
  switch (eCurrentEncoding) {
    case MN_1B: return;
    case MN_2B: eExtendedEncoding = MN_TENTH_2B; break;
    case MN_3B: RASSERT(0); break;
    case MN_4B: eExtendedEncoding = MN_TENTH_4B; break;
    case MN_5B: RASSERT(0); break;
    case MN_6B: RASSERT(0); break;
    case MN_7B: RASSERT(0); break;
    case MN_8B: eExtendedEncoding = MN_TENTH_8B; break;
    default:
      RASSERT(0);
  }
  
  //vTrace("Rewising column %lld to E%llx\n"_Stz,i_Column, eExtendedEncoding);
  
  aIntoHead->v1N(i_Column,eExtendedEncoding);
  
  //vTrace("CurrentEncoding %lld ~ %lld\n"_Stz, eCurrentEncoding, eExtendedEncoding);
}

/*----------------------------------------------------------------------------*/
/* re-encode a 1B Vch as Alpha                                                */
/*----------------------------------------------------------------------------*/
void RowCoder::vRecodeVch1B(RowNub* pIntoNub, HeadLine* aIntoHead, Column* pCol,
                            i8& i_IntoOffset, i8& i_FromOffset)
{
  eMN eExtendedEncoding;
  
  Coda* pCoda = pIntoNub->pCoda();
  i8 i_Column = pCol->iColumnNo();
  
  eMN eCurrentEncoding = (eMN)aIntoHead->w8_1N(i_Column);

  RASSERT(DM_VCH == pCol->eDatumType());
  RASSERT(MN_1B == eCurrentEncoding);

  // get the 1B char from the data portion of the intoHead
  char c1BChar = aIntoHead->w8_1B(i_IntoOffset);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // search for the 1B in coda aux for this column
  
  i8 i_Char;
  for (i_Char = 0; i_Char < C_EXTENDED_AUX; i_Char++) {
    
    // did we find a match?
    if (c1BChar == pCoda->m_aiAux[i_Column].m_caChr[i_Char])
      break;
    
    // run out of existing aux chars?
    if (! pCoda->m_aiAux[i_Column].m_caChr[i_Char])
      break;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // determine if we can revise the encoding

  
  // no space, leave as is
  if (i_Char >= C_EXTENDED_AUX)
    return;
  
  
  // found existing alpha?
  if (pCoda->m_aiAux[i_Column].m_caChr[i_Char]) {
    
    // change the encoding to extended alpha
    RASSERT(DM_ALPHA == pCoda->m_aeDatum[i_Column] ||
            DM_VCH == pCoda->m_aeDatum[i_Column]);
    pCoda->m_aeDatum[i_Column] = DM_ALPHA;
 
    // revise the encoding
    eExtendedEncoding = (eMN)(MN_ALPHA_0 + i_Char);
    aIntoHead->v1N(i_Column,eExtendedEncoding);
    
    // cut the 1B char out of the into head
    //MemLib::vWordDump(aIntoHead,sizeof(HeadLine));
    pIntoNub->vCut(aIntoHead,sizeof(HeadLine),i_IntoOffset,1);
    //MemLib::vWordDump(aIntoHead,sizeof(HeadLine));
    
    // go back one char in the intoOffset
    i_IntoOffset--;
    return;
  }
  
  
  // didn't find existing alpha, add it
  pCoda->m_aiAux[i_Column].m_caChr[i_Char] = c1BChar;
  
  // change the extended encoding to alpha
  pCoda->m_aeDatum[i_Column] = DM_ALPHA;
  
  // revise the encoding in the metaword of into head
  eExtendedEncoding = (eMN)(MN_ALPHA_0 + i_Char);
  aIntoHead->v1N(i_Column,eExtendedEncoding);
 
  // cut the 1B char out of the into head
  //MemLib::vWordDump(aIntoHead,sizeof(HeadLine));
  pIntoNub->vCut(aIntoHead,sizeof(HeadLine),i_IntoOffset,1);
  //MemLib::vWordDump(aIntoHead,sizeof(HeadLine));

  // go back one char in the intoOffset
  i_IntoOffset--;
}

/*----------------------------------------------------------------------------*/
/* return number of data bytes of encoding                                    */
/*----------------------------------------------------------------------------*/

//! this may be better done as a 2d array
i8 RowCoder::nDataBytes(eDM eDatum, eMN eEncoding)
{
  if (eEncoding == MN_NULL)
    return 0;
  
  //if (eEncoding <= MN_8B)
  //  return eEncoding;
  
  if ((eDatum <= DM_VCH || eDatum == DM_ALPHA) && eEncoding <= MN_8B)
    return eEncoding;
  
  if (MN_OVERFLOW == eEncoding)
    RASSERT(0); //! not implemented yet
  
  switch (eDatum) {
      
    case DM_BCD:
      switch (eEncoding) {
        case MN_MINUS_ONE: return 0;
        case MN_ZERO:      return 0;
        case MN_PLUS_ONE:  return 0;
        default:
          NEVER_REACH;
      }
      break;
      
    case DM_BOL:
      switch (eEncoding) {
        case MN_FALSE: return 0;
        case MN_TRUE:  return 0;
        default:
          NEVER_REACH;
      }
      break;
      
    case DM_DTE:
      switch (eEncoding) {
        case MN_CLASSICAL_1B: return 1;
        case MN_CLASSICAL_2B: return 2;
        case MN_SILVER_1B:    return 1;
        case MN_SILVER_2B:    return 2;
        case MN_MODERN_1B:    return 1;
        case MN_MODERN_2B:    return 2;
        default:
          NEVER_REACH;
      }
      break;
      
    case DM_FLT:
      switch (eEncoding) {
        case MN_MINUS_ONE: return 0;
        case MN_ZERO:      return 0;
        case MN_PLUS_ONE:  return 0;
        default:
          NEVER_REACH;
      }
      break;
      
    case DM_INT:
      switch (eEncoding) {
        case MN_MINUS_ONE:  return 0;
        case MN_ZERO:       return 0;
        case MN_PLUS_ONE:   return 0;
        default:
          NEVER_REACH;
      }
      break;
      
    case DM_IVL:
      switch (eEncoding) {
        default:
          NEVER_REACH;
      }
      break;
      
    case DM_TME:
      switch (eEncoding) {
        default:
          NEVER_REACH;
      }
      break;
      
    case DM_TSP:
      switch (eEncoding) {
         default:
          NEVER_REACH;
      }
      break;
      
    case DM_VCH:
      switch (eEncoding) {
        case MN_EMPTY: {
          return 0;
        }
        case MN_SHOCO_FIT: {
          //aHeadLine->w8_1B(i_Offset);
          return 1;
        }
        case MN_SHOCO_SPLIT:
          return 3;
        case MN_SHOCO_2B:
          return 2;
        default:
          NEVER_REACH;
      }
      break;
      
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case DM_ALPHA:
      return 0;
      
    case DM_DAYS:
      return 1;
      
    case DM_DELTA_PLUS:
      if (eEncoding <= MN_8B)
        return eEncoding;
      
     switch (eEncoding) {
        case MN_MINUS_ONE:  return 0;
        case MN_ZERO:       return 0;
        case MN_PLUS_ONE:   return 0;
        case MN_SAME:       return 0;
        case MN_PLUS_1B:    return 1;
        case MN_PLUS_2B:    return 2;
        default:
          NEVER_REACH;
      }
      break;
      
    case DM_DIGIT:
      if (eEncoding >= MN_DIGIT_0 && eEncoding <= MN_DIGIT_7)
        return 0;
      else {
        i8 nBytes = eEncoding - MN_DIGIT_7;
        return nBytes;
      }
      
    case DM_TENTH:
      if (eEncoding >= MN_TENTH_0 && eEncoding <= MN_TENTH_10)
        return 0;
      switch (eEncoding) {
        case MN_TENTH_2B:  return 2;
        case MN_TENTH_4B:  return 4;
        case MN_TENTH_8B:  return 8;
        default:
          NEVER_REACH;
      }

    default:
      NEVER_REACH;
  }
  
}

/*----------------------------------------------------------------------------*/
/* decode decimal from given cache line                                       */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeBcd(Datum* pDatum, HeadLine* aHeadLine,
                          ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                          i8& i_LastColumn, i8& i_Offset)
{
  i8 i_Column = pCol->iColumnNo();
  
  pDatum->m_eDatum = DM_BCD;
  
  eMN eEncoding = eGetEncodingExt(i_Column,aHeadLine,pColVec,pCoda,i_LastColumn,i_Offset);
  switch (eEncoding) {
      
    case MN_NULL: pDatum->m_iInt = C_DATUM_NULL;               break;
    case MN_1B:   pDatum->m_iInt = aHeadLine->w8_1B(i_Offset); break;
    case MN_2B:   pDatum->m_iInt = aHeadLine->w8_2B(i_Offset); break;
    case MN_3B:   pDatum->m_iInt = aHeadLine->w8_3B(i_Offset); break;
    case MN_4B:   pDatum->m_iInt = aHeadLine->w8_4B(i_Offset); break;
    case MN_5B:   pDatum->m_iInt = aHeadLine->w8_5B(i_Offset); break;
    case MN_6B:   pDatum->m_iInt = aHeadLine->w8_6B(i_Offset); break;
    case MN_7B:   pDatum->m_iInt = aHeadLine->w8_7B(i_Offset); break;
    case MN_8B:   pDatum->m_iInt = aHeadLine->w8_8B(i_Offset); break;
      
    case MN_MINUS_ONE: pDatum->m_iInt = (i8)-1; break;
    case MN_ZERO:      pDatum->m_iInt = (i8)0;  break;
    case MN_PLUS_ONE:  pDatum->m_iInt = (i8)1;  break;
      
    default:
      NEVER_REACH;
  }
 }

/*----------------------------------------------------------------------------*/
/* decode boolean from given cache line                                       */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeBol(Datum* pDatum, HeadLine* aHeadLine,
                          ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                          i8& i_LastColumn, i8& i_Offset)
{
  i8 i_Column  = pCol->iColumnNo();
  
  pDatum->m_eDatum = DM_BOL;
  
  eMN eEncoding = eGetEncodingExt(i_Column,aHeadLine,pColVec,pCoda,i_LastColumn,i_Offset);

  switch (eEncoding) {
      
    case MN_NULL:  pDatum->m_iInt = C_DATUM_NULL; break;
    case MN_FALSE: pDatum->m_iInt = (i8)0;        break;
    case MN_TRUE:  pDatum->m_iInt = (i8)1;        break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* decode date from given cache line                                          */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeDte(Datum* pDatum, HeadLine* aHeadLine,
                          ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                          i8& i_LastColumn, i8& i_Offset)
{
  i8 i_Column  = pCol->iColumnNo();
  
  pDatum->m_eDatum = DM_DTE;
  
  eMN eEncoding = eGetEncodingExt(i_Column,aHeadLine,pColVec,pCoda,i_LastColumn,i_Offset);
  switch (eEncoding) {
      
    case MN_NULL: pDatum->m_iInt = C_DATUM_NULL; break;
      
//    case MN_1B: pDatum->m_iInt = aHeadLine->w8_1B(i_Offset); break;
//    case MN_2B: pDatum->m_iInt = aHeadLine->w8_2B(i_Offset); break;
//    case MN_3B: pDatum->m_iInt = aHeadLine->w8_3B(i_Offset); break;
//    case MN_4B: pDatum->m_iInt = aHeadLine->w8_4B(i_Offset); break;
//    case MN_5B: pDatum->m_iInt = aHeadLine->w8_5B(i_Offset); break;
//    case MN_6B: pDatum->m_iInt = aHeadLine->w8_6B(i_Offset); break;
//    case MN_7B: pDatum->m_iInt = aHeadLine->w8_7B(i_Offset); break;
    case MN_8B: pDatum->m_iInt = aHeadLine->w8_8B(i_Offset); break;

    case MN_CLASSICAL_1B: pDatum->m_iInt = aHeadLine->w8_1B(i_Offset); break;
    case MN_CLASSICAL_2B: pDatum->m_iInt = aHeadLine->w8_2B(i_Offset); break;
    case MN_SILVER_1B:    pDatum->m_iInt = aHeadLine->w8_1B(i_Offset); break;
    case MN_SILVER_2B:    pDatum->m_iInt = aHeadLine->w8_2B(i_Offset); break;
    case MN_MODERN_1B:    pDatum->m_iInt = aHeadLine->w8_1B(i_Offset); break;
    case MN_MODERN_2B:    pDatum->m_iInt = aHeadLine->w8_2B(i_Offset); break;

    default:
      NEVER_REACH;
  }
  
  vRestoreDte(pDatum,eEncoding);
}

/*----------------------------------------------------------------------------*/
/* decode float from given cache line                                         */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeFlt(Datum* pDatum, HeadLine* aHeadLine,
                          ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                          i8& i_LastColumn, i8& i_Offset)
{
  i8 i_Column  = pCol->iColumnNo();
  
  pDatum->m_eDatum = DM_FLT;
  
  eMN eEncoding = eGetEncodingExt(i_Column,aHeadLine,pColVec,pCoda,i_LastColumn,i_Offset);
  switch (eEncoding) {
      
    case MN_NULL: pDatum->m_iInt = C_DATUM_NULL;                break;
    case MN_1B:   pDatum->m_iInt = aHeadLine->w8_1B(i_Offset); break;
    case MN_2B:   pDatum->m_iInt = aHeadLine->w8_2B(i_Offset); break;
    case MN_3B:   pDatum->m_iInt = aHeadLine->w8_3B(i_Offset); break;
    case MN_4B:   pDatum->m_iInt = aHeadLine->w8_4B(i_Offset); break;
    case MN_5B:   pDatum->m_iInt = aHeadLine->w8_5B(i_Offset); break;
    case MN_6B:   pDatum->m_iInt = aHeadLine->w8_6B(i_Offset); break;
    case MN_7B:   pDatum->m_iInt = aHeadLine->w8_7B(i_Offset); break;
    case MN_8B:   pDatum->m_iInt = aHeadLine->w8_8B(i_Offset); break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* decode integer from given cache line                                       */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeInt(Datum* pDatum, HeadLine* aHeadLine,
                          ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                          i8& i_LastColumn, i8& i_Offset)
{
  i8 i_Column  = pCol->iColumnNo();
  
  pDatum->m_eDatum = DM_INT;
  
  eMN eEncoding = eGetEncodingExt(i_Column,aHeadLine,pColVec,pCoda,i_LastColumn,i_Offset);
  //vTrace("Decoding %lld\n"_Stz, eEncoding);
  
  switch (eEncoding) {
      
    case MN_NULL: pDatum->m_iInt = C_DATUM_NULL;                break;
    case MN_1B:   pDatum->m_iInt = aHeadLine->w8_1B(i_Offset); break;
    case MN_2B:   pDatum->m_iInt = aHeadLine->w8_2B(i_Offset); break;
    case MN_3B:   pDatum->m_iInt = aHeadLine->w8_3B(i_Offset); break;
    case MN_4B:   pDatum->m_iInt = aHeadLine->w8_4B(i_Offset); break;
    case MN_5B:   pDatum->m_iInt = aHeadLine->w8_5B(i_Offset); break;
    case MN_6B:   pDatum->m_iInt = aHeadLine->w8_6B(i_Offset); break;
    case MN_7B:   pDatum->m_iInt = aHeadLine->w8_7B(i_Offset); break;
    case MN_8B:   pDatum->m_iInt = aHeadLine->w8_8B(i_Offset); break;
      
    case MN_MINUS_ONE: pDatum->m_iInt = (i8)-1; break;
    case MN_ZERO:      pDatum->m_iInt = (i8)0;  break;
    case MN_PLUS_ONE:  pDatum->m_iInt = (i8)1;  break;
      
    case MN_SAME:
      pDatum->m_iInt = pCoda->m_aiAux[i_Column].m_iInt;
      break;
      
//    case MN_PLUS_1B:
//      pDatum->m_iInt = pCoda->m_aiAux[i_Column].m_iInt
//                     + aHeadLine->w8_1B(i_Offset);
//      break;
//
//    case MN_MINUS_1B:
//      pDatum->m_iInt = pCoda->m_aiAux[i_Column].m_iInt
//                     - aHeadLine->w8_1B(i_Offset);
      break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* decode interval from given cache line                                      */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeIvl(Datum* pDatum, HeadLine* aHeadLine,
                          ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                          i8& i_LastColumn, i8& i_Offset)
{
  i8 i_Column  = pCol->iColumnNo();
  
  pDatum->m_eDatum = DM_IVL;
  
  eMN eEncoding = eGetEncodingExt(i_Column,aHeadLine,pColVec,pCoda,i_LastColumn,i_Offset);
  switch (eEncoding) {
      
    case MN_NULL: pDatum->m_iInt = C_DATUM_NULL;                break;
    case MN_1B:   pDatum->m_iInt = aHeadLine->w8_1B(i_Offset); break;
    case MN_2B:   pDatum->m_iInt = aHeadLine->w8_2B(i_Offset); break;
    case MN_3B:   pDatum->m_iInt = aHeadLine->w8_3B(i_Offset); break;
    case MN_4B:   pDatum->m_iInt = aHeadLine->w8_4B(i_Offset); break;
    case MN_5B:   pDatum->m_iInt = aHeadLine->w8_5B(i_Offset); break;
    case MN_6B:   pDatum->m_iInt = aHeadLine->w8_6B(i_Offset); break;
    case MN_7B:   pDatum->m_iInt = aHeadLine->w8_7B(i_Offset); break;
    case MN_8B:   pDatum->m_iInt = aHeadLine->w8_8B(i_Offset); break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* decode time from given cache line                                          */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeTme(Datum* pDatum, HeadLine* aHeadLine,
                          ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                          i8& i_LastColumn, i8& i_Offset)
{
  i8 i_Column  = pCol->iColumnNo();
  
  pDatum->m_eDatum = DM_TME;
  
  eMN eEncoding = eGetEncodingExt(i_Column,aHeadLine,pColVec,pCoda,i_LastColumn,i_Offset);
  switch (eEncoding) {
      
    case MN_NULL: pDatum->m_iInt = C_DATUM_NULL;                break;
    case MN_1B:   pDatum->m_iInt = aHeadLine->w8_1B(i_Offset); break;
    case MN_2B:   pDatum->m_iInt = aHeadLine->w8_2B(i_Offset); break;
    case MN_3B:   pDatum->m_iInt = aHeadLine->w8_3B(i_Offset); break;
    case MN_4B:   pDatum->m_iInt = aHeadLine->w8_4B(i_Offset); break;
    case MN_5B:   pDatum->m_iInt = aHeadLine->w8_5B(i_Offset); break;
    case MN_6B:   pDatum->m_iInt = aHeadLine->w8_6B(i_Offset); break;
    case MN_7B:   pDatum->m_iInt = aHeadLine->w8_7B(i_Offset); break;
    case MN_8B:   pDatum->m_iInt = aHeadLine->w8_8B(i_Offset); break;
      
    default:
      NEVER_REACH;
  }
  
  vRestoreTme(pDatum,eEncoding);
}

/*----------------------------------------------------------------------------*/
/* decode timestamp from given cache line                                     */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeTsp(Datum* pDatum, HeadLine* aHeadLine,
                          ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                          i8& i_LastColumn, i8& i_Offset)
{
  i8 i_Column  = pCol->iColumnNo();
  
  pDatum->m_eDatum = DM_TSP;
  
  eMN eEncoding = eGetEncodingExt(i_Column,aHeadLine,pColVec,pCoda,i_LastColumn,i_Offset);
  switch (eEncoding) {
      
    case MN_NULL: pDatum->m_iInt = C_DATUM_NULL;                break;
    case MN_1B:   pDatum->m_iInt = aHeadLine->w8_1B(i_Offset); break;
    case MN_2B:   pDatum->m_iInt = aHeadLine->w8_2B(i_Offset); break;
    case MN_3B:   pDatum->m_iInt = aHeadLine->w8_3B(i_Offset); break;
    case MN_4B:   pDatum->m_iInt = aHeadLine->w8_4B(i_Offset); break;
    case MN_5B:   pDatum->m_iInt = aHeadLine->w8_5B(i_Offset); break;
    case MN_6B:   pDatum->m_iInt = aHeadLine->w8_6B(i_Offset); break;
    case MN_7B:   pDatum->m_iInt = aHeadLine->w8_7B(i_Offset); break;
    case MN_8B:   pDatum->m_iInt = aHeadLine->w8_8B(i_Offset); break;
      
    default:
      NEVER_REACH;
  }
  
  if (eEncoding == MN_5B)
    vRestoreTsp(pDatum,eEncoding);
}

/*----------------------------------------------------------------------------*/
/* decode varchar from given cache line                                       */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeVch(Datum* pDatum, HeadLine* aHeadLine,
                          ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                          i8& i_LastColumn, i8& i_Offset)
{
  i8 i_Column  = pCol->iColumnNo();
  
  pDatum->m_eDatum = DM_VCH;
  
  //! needs more work
  eMN eEncoding = eGetEncodingExt(i_Column,aHeadLine,pColVec,pCoda,i_LastColumn,i_Offset);
  
  if (MN_NULL == eEncoding)
    pDatum->m_iInt = C_DATUM_NULL;

  else if (eEncoding <= MN_8B)
    vDecodeVchSmall(pDatum,aHeadLine,pCol,eEncoding,i_Offset);
  
  else if (MN_EMPTY == eEncoding) {
    pDatum->m_nChars = 0;
    *pDatum->m_xWsp.aArea() = '\0';
    pDatum->m_iInt = 0;
  }
  
  else if (MN_SHOCO_2B == eEncoding)
    vDecodeVchShoco(pDatum,aHeadLine,pCol,eEncoding,i_Offset);
  
  else if (MN_SHOCO_FIT == eEncoding)
    vDecodeVchShocoFit(pDatum,aHeadLine,pCol,eEncoding,i_Offset);

  else if (MN_SHOCO_SPLIT == eEncoding)
    vDecodeVchShocoSplit(pDatum,aHeadLine,pCol,eEncoding,i_Offset);
  
  else
    NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* decode varchar from given cache line                                       */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeAlpha(Datum* pDatum, HeadLine* aHeadLine,
                            ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                            i8& i_LastColumn, i8& i_Offset)
{
  i8 i_Column  = pCol->iColumnNo();
  
  pDatum->m_eDatum = DM_VCH;
  
  //!! needs more work
  eMN eEncoding = eGetEncodingExt(i_Column,aHeadLine,pColVec,pCoda,i_LastColumn,i_Offset);
  
  //RASSERT(eEncoding >= MN_ALPHA_0 && eEncoding <= MN_ALPHA_5);

  if (eEncoding < MN_ALPHA_0 || eEncoding > MN_ALPHA_5) {
    vDecodeVch(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_Offset);
    return;
  }
  
  //vDecodeVchSmall(pDatum,aHeadLine,pCol,eEncoding,i_Offset);
  
  pDatum->m_xWsp.aArea()[0] = pCoda->m_aiAux[i_Column].m_caChr[eEncoding - MN_ALPHA_0];
  pDatum->m_xWsp.aArea()[1] = '\0';
  
  //char cChr = aHeadLine->w8_1B(i_Offset);
  //RASSERT(cChr == pDatum->m_xWsp.aArea()[0]);

  //RASSERT(pDatum->m_xWsp.aArea()[0] == (char)pCoda->m_aiAux[i_Column]);
}

/*----------------------------------------------------------------------------*/
/* decode days from given cache line                                          */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeDays(Datum* pDatum, HeadLine* aHeadLine,
                            ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                            i8& i_LastColumn, i8& i_Offset)
{
  i8 i_Column  = pCol->iColumnNo();
  i8 i_FirstColumn  = pCol->iFirstDate();
  Column* pFirstColumn  = pColVec->pGet(i_FirstColumn);

  pDatum->m_eDatum = DM_DTE;
  
  //!! needs more work
  eMN eThisEncoding = eGetEncodingExt(i_Column,aHeadLine,pColVec,pCoda,i_LastColumn,i_Offset);
  
  i8 iDays = 0;
  switch (eThisEncoding) {
      
    case MN_NULL: pDatum->m_iInt = C_DATUM_NULL; return;
      
    case MN_DAYS_PLUS_1B:  iDays = aHeadLine->w8_1B(i_Offset);  break;
    case MN_DAYS_MINUS_1B: iDays = - aHeadLine->w8_1B(i_Offset); break;
    default:
      NEVER_REACH;
  }
  
  
  i8 i_SearchColumn = 0;
  i8 i_SearchOffset = 8;
  eMN eFirstEncoding = eGetEncodingExt(pCol->iFirstDate(),aHeadLine,pColVec,
                                       pCoda,i_SearchColumn,i_SearchOffset);

  switch (eFirstEncoding) {
    case MN_SILVER_1B:    pDatum->m_iInt = aHeadLine->w8_1B(i_SearchOffset); break;
    case MN_SILVER_2B:    pDatum->m_iInt = aHeadLine->w8_2B(i_SearchOffset); break;
    default:
      NEVER_REACH;
  }

  pDatum->m_iInt += iDays;
  
  i8 iEra = C_SILVER_ERA;
  vFromBaseDays(pDatum,iEra);

  
}

/*----------------------------------------------------------------------------*/
/* decode integer from given cache line                                       */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeDeltaPlus(Datum* pDatum, HeadLine* aHeadLine,
                                ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                                i8& i_LastColumn, i8& i_Offset)
{
  i8 i_Column  = pCol->iColumnNo();
  
  pDatum->m_eDatum = DM_INT;
  
  eMN eEncoding = eGetEncodingExt(i_Column,aHeadLine,pColVec,pCoda,i_LastColumn,i_Offset);
  //vTrace("Decoding %lld\n"_Stz, eEncoding);
  
  switch (eEncoding) {
      
    case MN_NULL: pDatum->m_iInt = C_DATUM_NULL;                break;
    case MN_1B:   pDatum->m_iInt = aHeadLine->w8_1B(i_Offset); break;
    case MN_2B:   pDatum->m_iInt = aHeadLine->w8_2B(i_Offset); break;
    case MN_3B:   pDatum->m_iInt = aHeadLine->w8_3B(i_Offset); break;
    case MN_4B:   pDatum->m_iInt = aHeadLine->w8_4B(i_Offset); break;
    case MN_5B:   pDatum->m_iInt = aHeadLine->w8_5B(i_Offset); break;
    case MN_6B:   pDatum->m_iInt = aHeadLine->w8_6B(i_Offset); break;
    case MN_7B:   pDatum->m_iInt = aHeadLine->w8_7B(i_Offset); break;
    case MN_8B:   pDatum->m_iInt = aHeadLine->w8_8B(i_Offset); break;
      
    case MN_MINUS_ONE: pDatum->m_iInt = (i8)-1; break;
    case MN_ZERO:      pDatum->m_iInt = (i8)0;  break;
    case MN_PLUS_ONE:  pDatum->m_iInt = (i8)1;  break;
      
    case MN_SAME:
      pDatum->m_iInt = pCoda->m_aiAux[i_Column].m_iInt;
      break;
      
    case MN_PLUS_1B:
      pDatum->m_iInt = pCoda->m_aiAux[i_Column].m_iInt
                     + aHeadLine->w8_1B(i_Offset);
      break;
      
    case MN_PLUS_2B:
      pDatum->m_iInt = pCoda->m_aiAux[i_Column].m_iInt
                     + aHeadLine->w8_2B(i_Offset);
      break;
      
      //
      //    case MN_MINUS_1B:
      //      pDatum->m_iInt = pCoda->m_aiAux[i_Column].m_iInt
      //                     - aHeadLine->w8_1B(i_Offset);
      break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* decode digit from given cache line                                         */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeDigit(Datum* pDatum, HeadLine* aHeadLine,
                            ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                            i8& i_LastColumn, i8& i_Offset)
{
  i8 i_Column  = pCol->iColumnNo();
  
  pDatum->m_eDatum = DM_INT;
  
  //!! needs more work
  eMN eEncoding = eGetEncodingExt(i_Column,aHeadLine,pColVec,pCoda,i_LastColumn,i_Offset);
  
  if (eEncoding >= MN_DIGIT_0 && eEncoding <= MN_DIGIT_7) {
    pDatum->m_iInt = (i8)(eEncoding - MN_DIGIT_0);
  }
  else {
    switch (eEncoding) {
      case MN_NULL:       pDatum->m_iInt = C_DATUM_NULL;                break;
      case MN_DIGIT_1B:   pDatum->m_iInt = aHeadLine->w8_1B(i_Offset); break;
      case MN_DIGIT_2B:   pDatum->m_iInt = aHeadLine->w8_2B(i_Offset); break;
      case MN_DIGIT_3B:   pDatum->m_iInt = aHeadLine->w8_3B(i_Offset); break;
      case MN_DIGIT_4B:   pDatum->m_iInt = aHeadLine->w8_4B(i_Offset); break;
      case MN_DIGIT_6B:   pDatum->m_iInt = aHeadLine->w8_6B(i_Offset); break;
      case MN_DIGIT_8B:   pDatum->m_iInt = aHeadLine->w8_8B(i_Offset); break;
      default:
        NEVER_REACH;
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* decode tenth from given cache line                                         */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeTenth(Datum* pDatum, HeadLine* aHeadLine,
                            ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                            i8& i_LastColumn, i8& i_Offset)
{
  i8 i_Column  = pCol->iColumnNo();
  
  pDatum->m_eDatum = DM_BCD;
  
  //!! needs more work
  eMN eEncoding = eGetEncodingExt(i_Column,aHeadLine,pColVec,pCoda,i_LastColumn,i_Offset);
  
  if (eEncoding >= MN_TENTH_0 && eEncoding <= MN_TENTH_10) {
    pDatum->m_iBcd = (i8)(eEncoding - MN_TENTH_0);
  }
  else {
    switch (eEncoding) {
      case MN_NULL:       pDatum->m_iInt = C_DATUM_NULL;                break;
      case MN_TENTH_2B:   pDatum->m_iInt = aHeadLine->w8_2B(i_Offset); break;
      case MN_TENTH_4B:   pDatum->m_iInt = aHeadLine->w8_4B(i_Offset); break;
      case MN_TENTH_8B:   pDatum->m_iInt = aHeadLine->w8_8B(i_Offset); break;
      default:
        NEVER_REACH;
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* decode varchar from given cache line                                       */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeVchShoco(Datum* pDatum, HeadLine* aHeadLine,
                               Column* pCol, eMN eEncoding, i8 i_Offset)
{
  i8 i_TailDistance = aHeadLine->w8_2B(i_Offset);
  TailLine* aShocoTail = RowNub::aToTail(aHeadLine,i_TailDistance);
  
  //MemLib::vWordDump(aShocoTail,sizeof(TailLine));
  
  /*
   +=====================================+
   |            Shoco Tail Line          +
   +=====================================+
   | U | C | <compressed bytes> <unused> +
   +-------------------------------------+
   | 0 | 1 | 2                        63 |
   +-------------------------------------+
   
   where: U is one byte being the unit length of the tail line
          C is one byte being the number of compressed bytes
   */

  i8 nCompressed = aShocoTail->w8_1B(C_SHOCO_COMPRESSED_OFFSET);
  //RASSERT(nCompressed <= 4);

  i8 nDecompressed =
    Compress::iFromShoco(pDatum->m_xWsp.aArea(),   // decompressed destination
                         255,                      // max destination
                         ((byte*)aShocoTail)       // from compressed source
                           + C_SHOCO_COMPRESSED_OFFSET
                           + C_SHOCO_COMPRESSED_BYTES,
                         nCompressed);             // bytes compressed
  ASSERT(nDecompressed);
  pDatum->m_nChars = nDecompressed;
}

/*----------------------------------------------------------------------------*/
/* decode varchar from given cache line                                       */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeVchShocoFit(Datum* pDatum, HeadLine* aHeadLine,
                               Column* pCol, eMN eEncoding, i8 i_Offset)
{
  i8 nCompressed = aHeadLine->w8_1B(i_Offset);
  RASSERT(nCompressed <= 56);

  i8 nDecompressed =
    Compress::iFromShoco((char*)pDatum->m_xWsp.aStz(),
                         255,
                         ((byte*)aHeadLine)+i_Offset+1,
                         nCompressed);
  ASSERT(nDecompressed);
  pDatum->m_nChars = nDecompressed;
}

/*----------------------------------------------------------------------------*/
/* decode varchar from given cache line                                       */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeVchShocoSplit(Datum* pDatum, HeadLine* aHeadLine,
                                  Column* pCol, eMN eEncoding, i8 i_Offset)
{
  byte caShoco[C_4KB];
  
  i8 i_TailDistance = aHeadLine->w8_2B(i_Offset);
  TailLine* aShocoTail = RowNub::aToTail(aHeadLine,i_TailDistance);
  
  i8 nCompressed = aHeadLine->w8_1B(i_Offset+2);
  RASSERT(nCompressed <= 56);
  
  i8 nBytesPartA = sizeof(HeadLine) - i_Offset - 3;
  i8 nBytesPartB = nCompressed - nBytesPartA;

  MemLib::vCopy((byte*)caShoco+2,
                (byte*)aHeadLine + i_Offset + 3,
                nBytesPartA,
                CN_ROWTRAY);
  MemLib::vCopy((byte*)caShoco+2+nBytesPartA,
                (byte*)aShocoTail+1,
                nBytesPartB,
                CN_ROWTRAY);
  
  //MemLib::vWordDump(caShoco+2,3 * sizeof(DoubleWord));
  
  i8 nDecompressed =
    Compress::iFromShoco((char*)pDatum->m_xWsp.aStz(),
                         255,
                         (byte*)caShoco+2,
                         nCompressed);
  ASSERT(nDecompressed);
  pDatum->m_nChars = nDecompressed;
}

/*----------------------------------------------------------------------------*/
/* decode varchar from given cache line                                       */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeVchSmall(Datum* pDatum, HeadLine* aHeadLine,
                               Column* pCol, eMN eEncoding, i8 i_Offset)
{
  switch (eEncoding) {
      
    case MN_1B:
      pDatum->m_iInt = aHeadLine->w8_1B(i_Offset);
      *(i8*)pDatum->m_xWsp.aStz() = pDatum->m_iInt;
      ((char*)pDatum->m_xWsp.aStz())[1] = '\0';
      pDatum->m_nChars = 1;
      break;
    case MN_2B:
      pDatum->m_iInt = aHeadLine->w8_2B(i_Offset);
      *(i8*)pDatum->m_xWsp.aStz() = pDatum->m_iInt;
      ((char*)pDatum->m_xWsp.aStz())[2] = '\0';
      pDatum->m_nChars = 2;
      break;
    case MN_3B:
      pDatum->m_iInt = aHeadLine->w8_3B(i_Offset);
      *(i8*)pDatum->m_xWsp.aStz() = pDatum->m_iInt;
      ((char*)pDatum->m_xWsp.aStz())[3] = '\0';
      pDatum->m_nChars = 3;
      break;
    case MN_4B:
      pDatum->m_iInt = aHeadLine->w8_4B(i_Offset);
      *(i8*)pDatum->m_xWsp.aStz() = pDatum->m_iInt;
      ((char*)pDatum->m_xWsp.aStz())[4] = '\0';
      pDatum->m_nChars = 4;
      break;
    case MN_5B:
      pDatum->m_iInt = aHeadLine->w8_5B(i_Offset);
      *(i8*)pDatum->m_xWsp.aStz() = pDatum->m_iInt;
      ((char*)pDatum->m_xWsp.aStz())[5] = '\0';
      pDatum->m_nChars = 5;
      break;
    case MN_6B:
      pDatum->m_iInt = aHeadLine->w8_6B(i_Offset);
      *(i8*)pDatum->m_xWsp.aStz() = pDatum->m_iInt;
      ((char*)pDatum->m_xWsp.aStz())[6] = '\0';
      pDatum->m_nChars = 6;
      break;
    case MN_7B:
      pDatum->m_iInt = aHeadLine->w8_7B(i_Offset);
      *(i8*)pDatum->m_xWsp.aStz() = pDatum->m_iInt;
      ((char*)pDatum->m_xWsp.aStz())[7] = '\0';
      pDatum->m_nChars = 7;
      break;
    case MN_8B:
      pDatum->m_iInt = aHeadLine->w8_8B(i_Offset);
      *(i8*)pDatum->m_xWsp.aStz() = pDatum->m_iInt;
      ((char*)pDatum->m_xWsp.aStz())[8] = '\0';
      pDatum->m_nChars = 8;
      break;
    default:
      NEVER_REACH;
  }

}

/*----------------------------------------------------------------------------*/
/* encode decimal                                                             */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeBcd(RowNub* pNub, Column* pCol, Datum* pDatum)
{
  HeadLine* aHeadLine = pNub->aGetHead(0);
  
  i8 i_Column = pCol->iColumnNo();
  w8 wOriginal = pDatum->m_iBcd;
  
  //! temp
  pDatum->m_iInt = wOriginal;
  
  // hash the column value if part of partition key
  if (pCol->bIsPartitionKey()) {
    i8 iHash = HshLib::iHash(&pDatum->m_iInt,sizeof(w8));
    pNub->vHashInc(iHash);
  }
  
  i8 nBytesEncoded;
  eMN eEncoding = eEncodingBcd(pDatum,nBytesEncoded);
  RASSERT(nBytesEncoded == nDataBytes(DM_BCD,eEncoding));
  
  // set 4bit meta data for column value
  aHeadLine->v1N(i_Column,eEncoding);
  
  // set n-byte data for column value
  if (nBytesEncoded)
    aHeadLine->vPut(pNub->i_Offset(),pDatum->m_iInt,eEncoding);
  
  pNub->vOffsetInc(nBytesEncoded);
  
  Coda* pCoda = pNub->pCoda();
  pCoda->m_aiAux[i_Column].m_iInt = nBytesEncoded;

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
#ifdef WITH_TRACE_ENCODING
  i8 i_LastOffset = 8;
  i8 i_lastColumn = 0;
  vDecode(pDatum,pNub,0,pCol,i_lastColumn, i_LastOffset);
  RASSERT(wOriginal == (w8)pDatum->m_iInt);
#endif
}

/*----------------------------------------------------------------------------*/
/* encode boolean                                                             */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeBol(RowNub* pNub, Column* pCol, Datum* pDatum)
{
  HeadLine* aHeadLine = pNub->aGetHead(0);

  i8 i_Column = pCol->iColumnNo();
  w8 wOriginal = pDatum->m_iBol;
  
  //! temp
  pDatum->m_iInt = wOriginal;
  
  // hash the column value if part of partition key
  if (pCol->bIsPartitionKey()) {
    i8 iHash = HshLib::iHash(&pDatum->m_iInt,sizeof(w8));
    pNub->vHashInc(iHash);
  }
  
  i8 nBytesEncoded;
  eMN eEncoding = eEncodingBol(pDatum,nBytesEncoded);
  RASSERT(nBytesEncoded == nDataBytes(DM_BOL,eEncoding));
  
  // set 4bit meta data for column value
  aHeadLine->v1N(i_Column,eEncoding);
  
  pNub->vOffsetInc(nBytesEncoded);

  Coda* pCoda = pNub->pCoda();
  pCoda->m_aiAux[i_Column].m_iInt = nBytesEncoded;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
#ifdef WITH_TRACE_ENCODING
  i8 i_LastOffset = 8;
  i8 i_lastColumn = 0;
  vDecode(pDatum,pNub,0,pCol,i_lastColumn, i_LastOffset);
  RASSERT(wOriginal == (w8)pDatum->m_iInt);
#endif
}

/*----------------------------------------------------------------------------*/
/* encode date                                                                */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeDte(RowNub* pNub, Column* pCol, Datum* pDatum)
{
  HeadLine* aHeadLine = pNub->aGetHead(0);

  i8 i_Column = pCol->iColumnNo();
  w8 wOriginal = pDatum->m_iDte;
  
  //! temp
  pDatum->m_iInt = wOriginal;
  
  // hash the column value if part of partition key
  if (pCol->bIsPartitionKey()) {
    i8 iHash = HshLib::iHash(&pDatum->m_iInt,sizeof(w8));
    pNub->vHashInc(iHash);
  }
  
  i8 nBytesEncoded;
//  vTrace("++ %lld-%lld-%lld\n"_Stz,
//         pDatum->xDte.m_uYearDelta,pDatum->xDte.m_uMonth,pDatum->xDte.m_uDay);
  
  eMN eEncoding = eEncodingDte(pDatum,nBytesEncoded);
  RASSERT(nBytesEncoded == nDataBytes(DM_DTE,eEncoding));
  
  // set 4bit meta data for column value
  aHeadLine->v1N(i_Column,eEncoding);
  
  // set n-byte data for column value
  if (nBytesEncoded)
    aHeadLine->vPut(pNub->i_Offset(),pDatum->m_iInt,nBytesEncoded);
  
  pNub->vOffsetInc(nBytesEncoded);

  Coda* pCoda = pNub->pCoda();
  pCoda->m_aiAux[i_Column].m_iInt = nBytesEncoded;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
#ifdef WITH_TRACE_ENCODING
  i8 i_LastOffset = 8;
  i8 i_lastColumn = 0;
  vDecode(pDatum,pNub,0,pCol,i_lastColumn, i_LastOffset);
//  vTrace("-- %lld-%lld-%lld\n"_Stz,
//         pDatum->xDte.m_uYearDelta,pDatum->xDte.m_uMonth,pDatum->xDte.m_uDay);
  RASSERT(wOriginal == (w8)pDatum->m_iInt);
#endif
}

/*----------------------------------------------------------------------------*/
/* encode float                                                               */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeFlt(RowNub* pNub, Column* pCol, Datum* pDatum)
{
  HeadLine* aHeadLine = pNub->aGetHead(0);

  i8 i_Column = pCol->iColumnNo();
  w8 wOriginal = (w8)pDatum->m_wWord;
  
  //! temp
  pDatum->m_iInt = wOriginal;
  
  // hash the column value if part of partition key
  if (pCol->bIsPartitionKey()) {
    i8 iHash = HshLib::iHash(&pDatum->m_iInt,sizeof(w8));
    pNub->vHashInc(iHash);
  }
  
  i8 nBytesEncoded;
  eMN eEncoding = eEncodingFlt(pDatum,nBytesEncoded);
  RASSERT(nBytesEncoded == nDataBytes(DM_FLT,eEncoding));
  
  // set 4bit meta data for column value
  aHeadLine->v1N(i_Column,eEncoding);
  
  // set n-byte data for column value
  if (nBytesEncoded)
    aHeadLine->vPut(pNub->i_Offset(),pDatum->m_iInt,eEncoding);
  
  pNub->vOffsetInc(nBytesEncoded);

  Coda* pCoda = pNub->pCoda();
  pCoda->m_aiAux[i_Column].m_iInt = nBytesEncoded;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
#ifdef WITH_TRACE_ENCODING
  i8 i_LastOffset = 8;
  i8 i_lastColumn = 0;
  vDecode(pDatum,pNub,0,pCol,i_lastColumn,i_LastOffset);
  RASSERT(wOriginal == (w8)pDatum->m_iInt);
#endif
}

/*----------------------------------------------------------------------------*/
/* encode integer                                                             */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeInt(RowNub* pNub, Column* pCol, Datum* pDatum)
{
  HeadLine* aHeadLine = pNub->aGetHead(0);

  i8 i_Column = pCol->iColumnNo();
  w8 wOriginal = pDatum->m_iInt;
  
  //! temp
  pDatum->m_iInt = wOriginal;
  
  // hash the column value if part of partition key
  if (pCol->bIsPartitionKey()) {
    i8 iHash = HshLib::iHash(&pDatum->m_iInt,sizeof(w8));
    pNub->vHashInc(iHash);
  }
  
  i8 nBytesEncoded;
  eMN eEncoding = eEncodingInt(pDatum,nBytesEncoded);
  RASSERT(nBytesEncoded == nDataBytes(DM_INT,eEncoding));
  
  // set 4bit meta data for column value
  aHeadLine->v1N(i_Column,eEncoding);
  
  // set n-byte data for column value
  if (nBytesEncoded)
    aHeadLine->vPut(pNub->i_Offset(),pDatum->m_iInt,eEncoding);
  
  pNub->vOffsetInc(nBytesEncoded);

  Coda* pCoda = pNub->pCoda();
  pCoda->m_aiAux[i_Column].m_iInt = nBytesEncoded;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
#ifdef WITH_TRACE_ENCODING
  i8 i_LastOffset = 8;
  i8 i_lastColumn = 0;
  vDecode(pDatum,pNub,0,pCol,i_lastColumn,i_LastOffset);
  RASSERT(wOriginal == (w8)pDatum->m_iInt);
#endif
}

/*----------------------------------------------------------------------------*/
/* encode interval                                                            */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeIvl(RowNub* pNub, Column* pCol, Datum* pDatum)
{
  HeadLine* aHeadLine = pNub->aGetHead(0);

  i8 i_Column = pCol->iColumnNo();
  w8 wOriginal = pDatum->m_iIvl;
  
  //! temp
  pDatum->m_iInt = wOriginal;
  
  // hash the column value if part of partition key
  if (pCol->bIsPartitionKey()) {
    i8 iHash = HshLib::iHash(&pDatum->m_iInt,sizeof(w8));
    pNub->vHashInc(iHash);
  }
  
  i8 nBytesEncoded;
  eMN eEncoding = eEncodingIvl(pDatum,nBytesEncoded);
  RASSERT(nBytesEncoded == nDataBytes(DM_IVL,eEncoding));
  
  // set 4bit meta data for column value
  aHeadLine->v1N(i_Column,eEncoding);
  
  // set n-byte data for column value
  if (nBytesEncoded)
    aHeadLine->vPut(pNub->i_Offset(),pDatum->m_iInt,eEncoding);
  
  pNub->vOffsetInc(nBytesEncoded);

  Coda* pCoda = pNub->pCoda();
  pCoda->m_aiAux[i_Column].m_iInt = nBytesEncoded;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
#ifdef WITH_TRACE_ENCODING
  i8 i_LastOffset = 8;
  i8 i_lastColumn = 0;
  vDecode(pDatum,pNub,0,pCol,i_lastColumn,i_LastOffset);
  RASSERT(wOriginal == (w8)pDatum->m_iInt);
#endif
}

/*----------------------------------------------------------------------------*/
/* encode time                                                                */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeTme(RowNub* pNub, Column* pCol, Datum* pDatum)
{
  HeadLine* aHeadLine = pNub->aGetHead(0);

  i8 i_Column = pCol->iColumnNo();
  w8 wOriginal = pDatum->m_iTme;
  
  //! temp
  pDatum->m_iInt = wOriginal;
  
  // hash the column value if part of partition key
  if (pCol->bIsPartitionKey()) {
    i8 iHash = HshLib::iHash(&pDatum->m_iInt,sizeof(w8));
    pNub->vHashInc(iHash);
  }
  
  i8 nBytesEncoded;
  eMN eEncoding = eEncodingTme(pDatum,nBytesEncoded);
  RASSERT(nBytesEncoded == nDataBytes(DM_TME,eEncoding));
  
  // set 4bit meta data for column value
  aHeadLine->v1N(i_Column,eEncoding);
  
  // set n-byte data for column value
  if (nBytesEncoded)
    aHeadLine->vPut(pNub->i_Offset(),pDatum->m_iInt,eEncoding);
  
  pNub->vOffsetInc(nBytesEncoded);

  Coda* pCoda = pNub->pCoda();
  pCoda->m_aiAux[i_Column].m_iInt = nBytesEncoded;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
#ifdef WITH_TRACE_ENCODING
  i8 i_LastOffset = 8;
  i8 i_lastColumn = 0;
  vDecode(pDatum,pNub,0,pCol,i_lastColumn,i_LastOffset);
  RASSERT(wOriginal == (w8)pDatum->m_iInt);
#endif
}

/*----------------------------------------------------------------------------*/
/* encode timestamp                                                           */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeTsp(RowNub* pNub, Column* pCol, Datum* pDatum)
{
  HeadLine* aHeadLine = pNub->aGetHead(0);

  i8 i_Column = pCol->iColumnNo();
  w8 wOriginal = pDatum->m_iTsp;
  
  //! temp
  pDatum->m_iInt = wOriginal;
  
  // hash the column value if part of partition key
  if (pCol->bIsPartitionKey()) {
    i8 iHash = HshLib::iHash(&pDatum->m_iInt,sizeof(w8));
    pNub->vHashInc(iHash);
  }
  
  i8 nBytesEncoded;
  eMN eEncoding = eEncodingTsp(pDatum,nBytesEncoded);
  RASSERT(nBytesEncoded == nDataBytes(DM_TSP,eEncoding));
  
  // set 4bit meta data for column value
  aHeadLine->v1N(i_Column,eEncoding);
  
  // set n-byte data for column value
  if (nBytesEncoded)
    aHeadLine->vPut(pNub->i_Offset(),pDatum->m_iInt,eEncoding);
  
  pNub->vOffsetInc(nBytesEncoded);

  Coda* pCoda = pNub->pCoda();
  pCoda->m_aiAux[i_Column].m_iInt = nBytesEncoded;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
#ifdef WITH_TRACE_ENCODING
  i8 i_LastOffset = 8;
  i8 i_lastColumn = 0;
  vDecode(pDatum,pNub,0,pCol,i_lastColumn,i_LastOffset);
  RASSERT(wOriginal == (w8)pDatum->m_iInt);
#endif
}

/*----------------------------------------------------------------------------*/
/* encode varchar                                                             */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeVch(RowNub* pNub, Column* pCol, Datum* pDatum)
{
  HeadLine* aHeadLine = pNub->aGetHead(0);

  i8 i_Column = pCol->iColumnNo();
  //w8 wOriginal = (w8)pDatum->m_xWsp.aArea();
    
  // hash the column value if part of partition key
  if (pCol->bIsPartitionKey()) {
    i8 iHash = HshLib::iHash(pDatum->m_xWsp.aArea(),sizeof(w8));
    pNub->vHashInc(iHash);
  }
  
  eMN eEncoding = eEncodingVch(pDatum,pDatum->m_nChars);

  // set 4bit meta data for column value
  aHeadLine->v1N(i_Column,eEncoding);
  
  // set n-byte data for column value for non-null's
  if (eEncoding != MN_NULL) {
    if (pDatum->m_nChars) {
      if (eEncoding == MN_SHOCO_2B)
        vEncodeVchShoco(pNub,pCol,pDatum);
      else
        vEncodeVchSmall(pNub,pCol,pDatum);
    }
    else {
      // empty string
      aHeadLine->v1N(i_Column,MN_EMPTY);

//      aHeadLine->v1N(i_Column,MN_1B);
//      i8 i_Offset = pNub->i_Offset();
//      aHeadLine->v1B(i_Offset,0);
//      pNub->vOffsetInc(1);
//      Coda* pCoda = pNub->pCoda();
//      pCoda->m_aiAux[i_Column].m_iInt = 1;
      
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
}

/*----------------------------------------------------------------------------*/
/* encode varchar as shoco                                                    */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeVchShoco(RowNub* pNub, Column* pCol, Datum* pDatum)
{
  static i8 nShortOnes = 0;
  static i8 nLongOnes = 0;
  
  i8 i_Column = pCol->iColumnNo();
  
  char caShoco[C_4KB];
  
  HeadLine* aHeadLine = pNub->aGetHead(0);
  
  /*
     +=====================================+
     |            Shoco Tail Line          +
     +=====================================+
     | U | C | <compressed bytes> <unused> +
     +-------------------------------------+
     | 0 | 1 | 2                        63 |
     +-------------------------------------+
   
     where: U is one byte being the unit length of the tail line
            C is one byte being the number of compressed bytes
   */

  // compress varchar into tail shoco cache line
  i8 nCompressed =
    Compress::iIntoShoco((byte*)caShoco,         // into compressed destination
                         sizeof(caShoco) - 1,    // destination size
                         pDatum->m_xWsp.aArea(), // from uncompressed source
                         pDatum->m_nChars);      // source bytes

  //! temporary limitation
  //RASSERT(nCompressed < 56);
  
  // allot a tail cache line
  i8 i_Shoco = pNub->iAllotTail(2 + nCompressed);
  TailLine* aShocoTail = pNub->aGetTail(i_Shoco);
  
  // copy shoco into tail
  char *aWithinTail = (char*)aShocoTail
                        + C_SHOCO_COMPRESSED_BYTES + C_SHOCO_COMPRESSED_BYTES;
  MemLib::vCopy(aWithinTail,caShoco,nCompressed,CN_ROWBUFFER);
  
  // set data for column value as the index of the shoco cache line
  i8 i_Offset = pNub->i_Offset();
  aHeadLine->vPut(i_Offset,i_Shoco,MN_2B);
  pNub->vOffsetInc(2);
  
  Coda* pCoda = pNub->pCoda();
  pCoda->m_aiAux[i_Column].m_iInt = 2;
  
  // set length of the compressed string encoded into tail shoco cache line
  aShocoTail->v1B(C_SHOCO_COMPRESSED_OFFSET,nCompressed);
  
  if (nCompressed > 20) {
    nLongOnes++;
//    if (IntLib::bIsDecaCycle(nLongOnes))
//      vTrace("%lld long ones (eg. %lld)\n"_Stz,nLongOnes,nCompressed);
  }
  else {
    nShortOnes++;
//    if (IntLib::bIsDecaCycle(nShortOnes))
//      vTrace("%lld short ones (eg. %lld)\n"_Stz,nShortOnes,nCompressed);
  }
    
  
  
  //MemLib::vWordDump(aShocoLine,sizeof(HeadLine));
  
  //    s256 sDecomp;
  //    i8 iLenDecompressed =
  //      Compress::iFromShoco(sDecomp.aChar(), 255,
  //                           ((byte*)aShocoLine)+8, C_BYTES_PER_CACHE_LINE-8);
  //    sDecomp.aChar()[iLenDecompressed] = '\0';
  //    vTrace("Decomp: %s\n"_Stz,sDecomp.aChar());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
}

/*----------------------------------------------------------------------------*/
/* encode varchar as small                                                    */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeVchSmall(RowNub* pNub, Column* pCol, Datum* pDatum)
{
  HeadLine* aHeadLine = pNub->aGetHead(0);
  
  i8 i_Column = pCol->iColumnNo();
  
  aHeadLine->vPut(pNub->i_Offset(),
                   *(w8*)pDatum->m_xWsp.aArea(),
                   pDatum->m_nChars);
  
  pNub->vOffsetInc(pDatum->m_nChars);
  
  Coda* pCoda = pNub->pCoda();
  pCoda->m_aiAux[i_Column].m_iInt = pDatum->m_nChars;
}

/*----------------------------------------------------------------------------*/
/* encoding of bcd                                                            */
/*----------------------------------------------------------------------------*/
eMN RowCoder::eEncodingBcd(Datum* pDatum, i8& nBytesEncoded)
{
  eMN eEncoding;
  
  if (pDatum->m_iInt == C_DATUM_NULL) {
    eEncoding = MN_NULL;
    nBytesEncoded = 0;
  }
  else {
    eEncoding = (eMN)IntLib::iByteMagnitude(pDatum->m_iInt);
    nBytesEncoded = eEncoding;
  }
  
  return eEncoding;
}

/*----------------------------------------------------------------------------*/
/* encoding of bol                                                            */
/*----------------------------------------------------------------------------*/
eMN RowCoder::eEncodingBol(Datum* pDatum, i8& nBytesEncoded)
{
  eMN eEncoding;
  
  if (pDatum->m_iInt == C_DATUM_NULL)
    eEncoding = MN_NULL;
  else if (pDatum->m_iInt)
    eEncoding = MN_TRUE;
  else
    eEncoding = MN_FALSE;
  
  nBytesEncoded = 0;
  
  return eEncoding;
}

/*----------------------------------------------------------------------------*/
/* encoding of dte                                                            */
/*----------------------------------------------------------------------------*/
eMN RowCoder::eEncodingDte(Datum* pDatum, i8& nBytesEncoded)
{
  eMN eEncoding;
  
  if (pDatum->m_iInt == C_DATUM_NULL) {
    nBytesEncoded = 0;
    return MN_NULL;
  }
  
//  bool bSawNonZero = true;
//
//  while (! pDatum->m_yaByte[0]) {
//
//    bSawNonZero = false;
//
//    for (i8 i_Byte = 0; i_Byte < 7; i_Byte++)
//      if ( (pDatum->m_yaByte[i_Byte] = pDatum->m_yaByte[i_Byte+1]) )
//        bSawNonZero = true;
//    pDatum->m_yaByte[7] = 0;
//
//    if (! bSawNonZero)
//      break;
//  }
  
  i8 iEra = 0;

  vIntoBaseDays(pDatum,iEra);
  //vFromBaseDays(pDatum);
 
  //pDatum->m_iInt >>= 47;
  
  i8 iBytes = (eMN)IntLib::iByteMagnitude(pDatum->m_iInt);
  
  if (C_SILVER_ERA == iEra) {
    if (1 == iBytes) {
      eEncoding = MN_SILVER_1B;
      nBytesEncoded = 1;
    }
    else {
      eEncoding = MN_SILVER_2B;
      nBytesEncoded = 2;
    }
  }
  else if (C_MODERN_ERA == iEra) {
    if (1 == iBytes) {
      eEncoding = MN_MODERN_1B;
      nBytesEncoded = 1;
    }
    else {
      eEncoding = MN_MODERN_2B;
      nBytesEncoded = 2;
    }
  }
  else {
    ASSERT(C_CLASSICAL_ERA == iEra);
    if (1 == iBytes) {
      eEncoding = MN_CLASSICAL_1B;
      nBytesEncoded = 1;
    }
    else {
      eEncoding = MN_CLASSICAL_2B;
      nBytesEncoded = 2;
    }
  }
  
  return eEncoding;
}

/*----------------------------------------------------------------------------*/
/* encoding of flt                                                            */
/*----------------------------------------------------------------------------*/
eMN RowCoder::eEncodingFlt(Datum* pDatum, i8& nBytesEncoded)
{
  eMN eEncoding;
  
  if (pDatum->m_iInt == C_DATUM_NULL) {
    eEncoding = MN_NULL;
    nBytesEncoded = 0;
  }
  else {
    eEncoding = (eMN)IntLib::iByteMagnitude(pDatum->m_iInt);
    nBytesEncoded = eEncoding;
  }
  
  return eEncoding;
}

/*----------------------------------------------------------------------------*/
/* encoding of int                                                            */
/*----------------------------------------------------------------------------*/
eMN RowCoder::eEncodingInt(Datum* pDatum, i8& nBytesEncoded)
{
  eMN eEncoding;
  
  if (pDatum->m_iInt == C_DATUM_NULL) {
    eEncoding = MN_NULL;
    nBytesEncoded = 0;
  }
  else {
    eEncoding = (eMN)IntLib::iByteMagnitude(pDatum->m_iInt);
    nBytesEncoded = eEncoding;
  }
  
  return eEncoding;
}

/*----------------------------------------------------------------------------*/
/* encoding of ivl                                                            */
/*----------------------------------------------------------------------------*/
eMN RowCoder::eEncodingIvl(Datum* pDatum, i8& nBytesEncoded)
{
  eMN eEncoding;
  
  if (pDatum->m_iInt == C_DATUM_NULL) {
    eEncoding = MN_NULL;
    nBytesEncoded = 0;
  }
  else {
    eEncoding = (eMN)IntLib::iByteMagnitude(pDatum->m_iInt);
    nBytesEncoded = eEncoding;
  }
  
  return eEncoding;
}

/*----------------------------------------------------------------------------*/
/* encoding of tme                                                            */
/*----------------------------------------------------------------------------*/
eMN RowCoder::eEncodingTme(Datum* pDatum, i8& nBytesEncoded)
{
  if (pDatum->m_iInt == C_DATUM_NULL) {
    nBytesEncoded = 0;
    return MN_NULL;
  }
  
  eMN eEncoding;
  
  bool bSawNonZero = true;
  
  while (! pDatum->m_yaByte[0]) {
    
    bSawNonZero = false;
    
    for (i8 i_Byte = 0; i_Byte < 7; i_Byte++)
      if ( (pDatum->m_yaByte[i_Byte] = pDatum->m_yaByte[i_Byte+1]) )
        bSawNonZero = true;
    pDatum->m_yaByte[7] = 0;
    
    if (! bSawNonZero)
      break;
  }
  
  eEncoding = (eMN)IntLib::iByteMagnitude(pDatum->m_iInt);
  
  nBytesEncoded = eEncoding;
  
  return eEncoding;
}

/*----------------------------------------------------------------------------*/
/* encoding of tsp                                                            */
/*----------------------------------------------------------------------------*/
eMN RowCoder::eEncodingTsp(Datum* pDatum, i8& nBytesEncoded)
{
  if (pDatum->m_iInt == C_DATUM_NULL) {
    nBytesEncoded = 0;
    return MN_NULL;
  }
  
  eMN eEncoding;
  
//  bool bSawNonZero = true;
//  
//  while (! pDatum->m_yaByte[0]) {
//    
//    bSawNonZero = false;
//    
//    for (i8 i_Byte = 0; i_Byte < 7; i_Byte++)
//      if ( (pDatum->m_yaByte[i_Byte] = pDatum->m_yaByte[i_Byte+1]) )
//        bSawNonZero = true;
//    pDatum->m_yaByte[7] = 0;
//    
//    if (! bSawNonZero)
//      break;
//  }
  
  eEncoding = (eMN)IntLib::iByteMagnitude(pDatum->m_iInt);
  
  nBytesEncoded = eEncoding;
  
  return eEncoding;
}

/*----------------------------------------------------------------------------*/
/* encoding of vch                                                            */
/*----------------------------------------------------------------------------*/
eMN RowCoder::eEncodingVch(Datum* pDatum, i8& nBytesEncoded)
{
  if (pDatum->m_iInt == C_DATUM_NULL) {
    nBytesEncoded = 0;
    return MN_NULL;
  }
  
  eMN eEncoding;
  
  i8 iLen = StrLib::iLength((char*)pDatum->m_xWsp.aArea());
  //RASSERT(iLen <= 8);
  
  if (iLen == 0) {
    eEncoding = MN_1B;
    nBytesEncoded = 0;
  }
  else if (iLen <= 3) {
    eEncoding = (eMN)iLen;
    nBytesEncoded = iLen;
  }
  else {
    eEncoding = MN_SHOCO_2B;
    nBytesEncoded = iLen;
  }
  
  return eEncoding;
}

/*----------------------------------------------------------------------------*/
/* search for duplicate tail line                                             */
/*----------------------------------------------------------------------------*/
i8 RowCoder::iFindTail(RowNub* pNub, TailLine* aFindLine)
{
  static i8 nFound = 0;
  static i8 nNotFound = 0;
  
  for (i8 i_Tail = pNub->iLastTail();
       i_Tail != C_UNKNOWN;
       i_Tail = pNub->iPrevTail(i_Tail)) {
  
//    vTrace("i_Tail %lld\n"_Stz,i_Tail);
  
//  for (i8 i_Tail = pNub->iFirstTail();
//       i_Tail != C_UNKNOWN;
//       i_Tail = pNub->iNextTail(i_Tail)) {
    
    TailLine* aTailLine = pNub->aGetTail(i_Tail);
    i8 nTailUnits = aTailLine->w8_1B(C_TAIL_UNITS_OFFSET);
    //RASSERT(nTailUnits <= 4);

    //MemLib::vWordDump(aFindLine,sizeof(HeadLine));
    //MemLib::vWordDump(aTailLine,sizeof(HeadLine));
    
    if (0 == MemLib::iCompareWords(aTailLine,aFindLine,nTailUnits*sizeof(DoubleWord))) {
      nFound++;
//      if (IntLib::bIsDecaCycle(nFound))
//        vTrace("%lld dupe tail found\n"_Stz,nFound);
      return i_Tail;
    }
    
  }
  
  nNotFound++;
  //  if (IntLib::bIsDecaCycle(nNotFound))
  //    vTrace("%lld dupe tail NOT found\n"_Stz,nNotFound);
  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* calculate free space of a head line                                        */
/*----------------------------------------------------------------------------*/
i8 RowCoder::nFreeSpaceHead(RowNub* pNub, HeadLine* aHeadLine)
{
  //MemLib::vWordDump(aHeadLine,sizeof(HeadLine));
  
  ColumnVec* pColVec = pNub->pDesc()->pColumnVec();
  Coda* pCoda = pNub->pCoda();

  i8 nFreeBytes = nFreeSpaceHead(aHeadLine, pColVec, pCoda);
  
  return nFreeBytes;
}

/*----------------------------------------------------------------------------*/
/* calculate free space of a head line                                        */
/*----------------------------------------------------------------------------*/
i8 RowCoder::nFreeSpaceHead(HeadLine* aHeadLine, ColumnVec* pColVec, Coda* pCoda)
{
  //MemLib::vWordDump(aHeadLine,sizeof(HeadLine));
  
  Column* pLastCol = pColVec->pGet(pColVec->nItems() - 1);
  ASSERT(pLastCol->bIsLastColumn());
  
  i8 i_Offset = 8;
  i8 i_LastColumn = 0;
  
  eMN eEncoding = eGetEncodingExt(pLastCol->iColumnNo(),aHeadLine,pColVec,pCoda,
                                  i_LastColumn,i_Offset);
  
  if (MN_SHOCO_SPLIT == eEncoding)
    return 0;
  
  if (MN_SHOCO_FIT == eEncoding) {
    i8 nCompressed = aHeadLine->w8_1B(i_Offset);
    RASSERT(nCompressed <= 56);
    i_Offset += nCompressed;
  }
  
  eDM eDatumExt = pCoda->m_aeDatum[pLastCol->iColumnNo()];

  i8 nBytes = nDataBytes(eDatumExt, eEncoding);
  
  i8 nFreeBytes = C_BYTES_PER_CACHE_LINE - i_Offset - nBytes;
  
  return nFreeBytes;
}

/*----------------------------------------------------------------------------*/
/* get the extended encoding (and offset) of a column from given head line    */
/*----------------------------------------------------------------------------*/
//! consider passing a eDatum array instead of colum vector

eMN RowCoder::eGetEncodingExt(i8 i_ForCol, HeadLine* aHeadLine,
                              ColumnVec* pColVec, Coda* pCoda, i8& i_PriorColumn_,
                              i8& i_Offset_)
{
  
  if (i_ForCol < i_PriorColumn_) {
    i_PriorColumn_ = 0;
    i_Offset_ = 8;
  }
  
  for (i8 i_Nibble = i_PriorColumn_; i_Nibble < i_ForCol; i_Nibble++) {
    
    eMN eEncoding = (eMN)aHeadLine->w8_1N(i_Nibble);

    Column* pCol  = pColVec->pGet(i_Nibble);
    
    eDM eDatumExt = pCoda->m_aeDatum[i_Nibble];

    i_Offset_ += nDataBytes(eDatumExt,eEncoding);
  }
  
  eMN eEncoding = (eMN)aHeadLine->w8_1N(i_ForCol);
  i_PriorColumn_ = i_ForCol;
  
  return eEncoding;
}

/*----------------------------------------------------------------------------*/
/* get the natural encoding (and offset) of a column from given head line     */
/*----------------------------------------------------------------------------*/
//! consider passing a eDatum array instead of colum vector

eMN RowCoder::eGetEncodingNat(i8 i_ForCol, HeadLine* aFromHead,
                              ColumnVec* pColVec, i8& i_PriorColumn_,
                              i8& i_FromOffset_)
{
  
  if (i_ForCol < i_PriorColumn_) {
    i_PriorColumn_ = 0;
    i_FromOffset_  = 8;
  }
  
  for (i8 i_Column = i_PriorColumn_; i_Column < i_ForCol; i_Column++) {
    
    eMN eEncoding = (eMN)aFromHead->w8_1N(i_Column);
    
    Column* pCol  = pColVec->pGet(i_Column);
    eDM eDatumNat = pCol->eDatumType();
    
    i_FromOffset_ += nDataBytes(eDatumNat,eEncoding);
  }
  
  eMN eEncoding  = (eMN)aFromHead->w8_1N(i_ForCol);
  
  i_PriorColumn_ = i_ForCol;
  
  return eEncoding;
}

/*----------------------------------------------------------------------------*/
/* get the encoding as a string given its datum                               */
/*----------------------------------------------------------------------------*/
Stz* RowCoder::sEncoding(eMN eEncoding, eDM eDatumNat, eDM eDatumExt)
{
  
  eDM eDatum = eDatumExt == DM_NONE ? eDatumNat : eDatumExt;
  
  switch (eDatum) {
      
    case DM_BCD:
    case DM_FLT:
    case DM_INT:
      switch (eEncoding) {
        case MN_NULL:      return("MN_NULL"_Stz); break;
        case MN_1B:        return("MN_1B"_Stz); break;
        case MN_2B:        return("MN_2B"_Stz); break;
        case MN_3B:        return("MN_3B"_Stz); break;
        case MN_4B:        return("MN_4B"_Stz); break;
        case MN_5B:        return("MN_5B"_Stz); break;
        case MN_6B:        return("MN_6B"_Stz); break;
        case MN_7B:        return("MN_7B"_Stz); break;
        case MN_8B:        return("MN_8B"_Stz); break;
        case MN_MINUS_ONE: return("MN_MINUS_ONE"_Stz); break;
        case MN_ZERO:      return("MN_ZERO"_Stz);      break;
        case MN_PLUS_ONE:  return("MN_PLUS_ONE"_Stz);  break;
        default:
          NEVER_REACH;
      }
      break;
      
    case DM_BOL:
      switch (eEncoding) {
        case MN_NULL:  return("MN_NULL"_Stz);  break;
        case MN_FALSE: return("MN_FALSE"_Stz); break;
        case MN_TRUE:  return("MN_TRUE"_Stz);  break;
        default:
          NEVER_REACH;
      }
      break;
      
    case DM_DTE:
      switch (eEncoding) {
        case MN_NULL:         return("MN_NULL"_Stz);         break;
        case MN_CLASSICAL_1B: return("MN_CLASSICAL_1B"_Stz); break;
        case MN_CLASSICAL_2B: return("MN_CLASSICAL_2B"_Stz); break;
        case MN_SILVER_1B:    return("MN_SILVER_1B"_Stz); break;
        case MN_SILVER_2B:    return("MN_SILVER_2B"_Stz); break;
        case MN_MODERN_1B:    return("MN_MODERN_1B"_Stz); break;
        case MN_MODERN_2B:    return("MN_MODERN_2B"_Stz); break;
        default:
          NEVER_REACH;
      }
      break;
      
    
    case DM_IVL:
    case DM_TME:
    case DM_TSP:
      switch (eEncoding) {
        case MN_NULL:      return("MN_NULL"_Stz); break;
        case MN_1B:        return("MN_1B"_Stz); break;
        case MN_2B:        return("MN_2B"_Stz); break;
        case MN_3B:        return("MN_3B"_Stz); break;
        case MN_4B:        return("MN_4B"_Stz); break;
        case MN_5B:        return("MN_5B"_Stz); break;
        case MN_6B:        return("MN_6B"_Stz); break;
        case MN_7B:        return("MN_7B"_Stz); break;
        case MN_8B:        return("MN_8B"_Stz); break;
        default:
          NEVER_REACH;
      }
      break;
      
    case DM_VCH:
      switch (eEncoding) {
        case MN_NULL:        return("MN_NULL"_Stz); break;
        case MN_1B:          return("MN_1B"_Stz); break;
        case MN_2B:          return("MN_2B"_Stz); break;
        case MN_3B:          return("MN_3B"_Stz); break;
        case MN_4B:          return("MN_4B"_Stz); break;
        case MN_5B:          return("MN_5B"_Stz); break;
        case MN_6B:          return("MN_6B"_Stz); break;
        case MN_7B:          return("MN_7B"_Stz); break;
        case MN_8B:          return("MN_8B"_Stz); break;
        case MN_EMPTY:       return("MN_EMPTY"_Stz);     break;
        case MN_SHOCO_FIT:   return("MN_SHOCO_FIT"_Stz); break;
        case MN_SHOCO_SPLIT: return("MN_SHOCO_SPLIT"_Stz); break;
        case MN_SHOCO_2B:    return("MN_SHOCO_2B"_Stz);  break;
        default:
          NEVER_REACH;
      }
      break;
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case DM_ALPHA:
      switch (eEncoding) {
        case MN_NULL:    return("MN_NULL"_Stz);    break;
        case MN_1B:      return("MN_1B"_Stz);      break;
        case MN_2B:      return("MN_2B"_Stz);      break;
        case MN_3B:      return("MN_3B"_Stz);      break;
        case MN_4B:      return("MN_4B"_Stz);      break;
        case MN_5B:      return("MN_5B"_Stz);      break;
        case MN_6B:      return("MN_6B"_Stz);      break;
        case MN_7B:      return("MN_7B"_Stz);      break;
        case MN_8B:      return("MN_8B"_Stz);      break;
        case MN_ALPHA_0: return("MN_ALPHA_0"_Stz); break;
        case MN_ALPHA_1: return("MN_ALPHA_1"_Stz); break;
        case MN_ALPHA_2: return("MN_ALPHA_2"_Stz); break;
        case MN_ALPHA_3: return("MN_ALPHA_3"_Stz); break;
        case MN_ALPHA_4: return("MN_ALPHA_4"_Stz); break;
        case MN_ALPHA_5: return("MN_ALPHA_5"_Stz); break;
        default:
          NEVER_REACH;
      }
      
    case DM_DAYS:
      switch (eEncoding) {
        case MN_NULL:      return("MN_NULL"_Stz); break;
        case MN_1B:        return("MN_1B"_Stz);   break;
        case MN_2B:        return("MN_2B"_Stz);   break;
        case MN_3B:        return("MN_3B"_Stz);   break;
        case MN_4B:        return("MN_4B"_Stz);   break;
        case MN_5B:        return("MN_5B"_Stz);   break;
        case MN_6B:        return("MN_6B"_Stz);   break;
        case MN_7B:        return("MN_7B"_Stz);   break;
        case MN_8B:        return("MN_8B"_Stz);   break;
        case MN_DAYS_PLUS_1B:  return("MN_DAYS_PLUS_1B"_Stz);  break;
        case MN_DAYS_MINUS_1B: return("MN_DAYS_MINUS_1B"_Stz); break;
        default:
          NEVER_REACH;
      }
      
    case DM_DELTA_PLUS:
      switch (eEncoding) {
        case MN_NULL:      return("MN_NULL"_Stz); break;
        case MN_1B:        return("MN_1B"_Stz);   break;
        case MN_2B:        return("MN_2B"_Stz);   break;
        case MN_3B:        return("MN_3B"_Stz);   break;
        case MN_4B:        return("MN_4B"_Stz);   break;
        case MN_5B:        return("MN_5B"_Stz);   break;
        case MN_6B:        return("MN_6B"_Stz);   break;
        case MN_7B:        return("MN_7B"_Stz);   break;
        case MN_8B:        return("MN_8B"_Stz);   break;
        case MN_MINUS_ONE: return("MN_MINUS_ONE"_Stz); break;
        case MN_ZERO:      return("MN_ZERO"_Stz);      break;
        case MN_PLUS_1B:   return("MN_PLUS_1B"_Stz);   break;
        case MN_PLUS_2B:   return("MN_PLUS_2B"_Stz);   break;
        case MN_SAME:      return("MN_SAME"_Stz);      break;
        default:
          NEVER_REACH;
      }
      
    case DM_DIGIT:
      switch (eEncoding) {
        case MN_NULL:     return("MN_NULL"_Stz);    break;
        case MN_DIGIT_0:  return("MN_DIGIT_0"_Stz); break;
        case MN_DIGIT_1:  return("MN_DIGIT_1"_Stz); break;
        case MN_DIGIT_2:  return("MN_DIGIT_2"_Stz); break;
        case MN_DIGIT_3:  return("MN_DIGIT_3"_Stz); break;
        case MN_DIGIT_4:  return("MN_DIGIT_4"_Stz); break;
        case MN_DIGIT_5:  return("MN_DIGIT_5"_Stz); break;
        case MN_DIGIT_6:  return("MN_DIGIT_6"_Stz); break;
        case MN_DIGIT_7:  return("MN_DIGIT_7"_Stz); break;
        case MN_DIGIT_1B: return("MN_DIGIT_1B"_Stz); break;
        case MN_DIGIT_2B: return("MN_DIGIT_2B"_Stz); break;
        case MN_DIGIT_3B: return("MN_DIGIT_3B"_Stz); break;
        case MN_DIGIT_4B: return("MN_DIGIT_4B"_Stz); break;
        case MN_DIGIT_6B: return("MN_DIGIT_6B"_Stz); break;
        case MN_DIGIT_8B: return("MN_DIGIT_8B"_Stz); break;
        default:
          NEVER_REACH;
      }
      
    case DM_TENTH:
      switch (eEncoding) {
        case MN_NULL:     return("MN_NULL"_Stz);  break;
        case MN_TENTH_0:  return("MN_TENTH_0"_Stz); break;
        case MN_TENTH_1:  return("MN_TENTH_1"_Stz); break;
        case MN_TENTH_2:  return("MN_TENTH_2"_Stz); break;
        case MN_TENTH_3:  return("MN_TENTH_3"_Stz); break;
        case MN_TENTH_4:  return("MN_TENTH_4"_Stz); break;
        case MN_TENTH_5:  return("MN_TENTH_5"_Stz); break;
        case MN_TENTH_6:  return("MN_TENTH_6"_Stz); break;
        case MN_TENTH_7:  return("MN_TENTH_7"_Stz); break;
        case MN_TENTH_8:  return("MN_TENTH_8"_Stz); break;
        case MN_TENTH_9:  return("MN_TENTH_9"_Stz); break;
        case MN_TENTH_10: return("MN_TENTH_10"_Stz); break;
        case MN_TENTH_2B: return("MN_TENTH_2B"_Stz); break;
        case MN_TENTH_4B: return("MN_TENTH_4B"_Stz); break;
        case MN_TENTH_8B: return("MN_TENTH_8B"_Stz); break;
        default:
          NEVER_REACH;
      }
  }
  return ""_Stz;
}

/*----------------------------------------------------------------------------*/
/* initialise given cache line                                                */
/*----------------------------------------------------------------------------*/
void RowCoder::vInitHeadLine(HeadLine* aHeadLine)
{
  // set all bytes in the entire cache line to xFF
  aHeadLine->vSetAll(0xFF);
  
  // set all bits in first word of cache line to zero
  aHeadLine->v8B(0,0);
}

/*----------------------------------------------------------------------------*/
/* compute how many cache lines an encoded row occupies                       */
/*----------------------------------------------------------------------------*/
i8 RowCoder::nLines(RowNub* pNub, i8 i_Row)
{
  ColumnVec* pColVec = pNub->pDesc()->pColumnVec();
  Coda* pCoda = pNub->pCoda();
  
  i8 nHeadLines = 1;
  // get no of columns
  i8 nColumns = pColVec->nItems();
  
  i8 i_Offset = 8;
  i8 i_LastColumn = 0;
  
  // for each column
  for (i8 i_Column = 0; i_Column < nColumns; i_Column++) {
    
    // get the column at index
    Column* pCol = pColVec->pGet(i_Column);
    
    if (DM_VCH != pCol->eDatumType())
      continue;
    
    HeadLine* aHeadLine = pNub->aGetHead(i_Row);
    
    eMN eEncoding = eGetEncodingExt(i_Column,aHeadLine,
                                    pColVec,pCoda,
                                    i_LastColumn,i_Offset);
    
    if (MN_SHOCO_2B == eEncoding)
      nHeadLines++;
  }
  
  return nHeadLines;
}

/*----------------------------------------------------------------------------*/
/* restore zero stripped dte                                                  */
/*----------------------------------------------------------------------------*/
void RowCoder::vRestoreDte(Datum* pDatum, eMN eEncoding)
{
  i8 iEra = 0;
  
  if (MN_SILVER_1B == eEncoding || MN_SILVER_2B == eEncoding)
    iEra = C_SILVER_ERA;
  else if (MN_MODERN_1B == eEncoding || MN_MODERN_2B == eEncoding)
    iEra = C_MODERN_ERA;
  else {
    iEra = C_CLASSICAL_ERA;
  }

  vFromBaseDays(pDatum,iEra);
  //pDatum->m_iInt <<= 47;
}

/*----------------------------------------------------------------------------*/
/* restore zero stripped tme                                                  */
/*----------------------------------------------------------------------------*/
void RowCoder::vRestoreTme(Datum* pDatum, eMN eEncoding)
{
  pDatum->m_iInt <<= 40;
}

/*----------------------------------------------------------------------------*/
/* restore zero stripped tsp                                                  */
/*----------------------------------------------------------------------------*/
void RowCoder::vRestoreTsp(Datum* pDatum, eMN eEncoding)
{
  pDatum->m_iInt <<= 24;
}


/*----------------------------------------------------------------------------*/
const static i8 nMonthDaySum[13] =
  //    J  F  M   A   M   J   J   A   S   O   N   D
    {0,31,60,91,121,152,182,213,244,274,305,335,366};


/*----------------------------------------------------------------------------*/
/* compute date from number of days since base date                           */
/*----------------------------------------------------------------------------*/
void RowCoder::vFromBaseDays(Datum* pDatum, i8& iEra)
{
  i8 nYearsMonthsDays = pDatum->m_iDte;

  if (C_SILVER_ERA == iEra)
    nYearsMonthsDays += C_SILVER_DAYS;
  else if (C_MODERN_ERA == iEra)
    nYearsMonthsDays += C_MODERN_DAYS;
  else {
    ASSERT(C_CLASSICAL_ERA == iEra);
  }

  i8 nYears = (nYearsMonthsDays - 1) / 366;
  i8 nMonthsDays = nYearsMonthsDays - (nYears * 366);

  i8 i_Month;
  for (i_Month = 1; i_Month <= 11; i_Month++)
    if (nMonthDaySum[i_Month] >= nMonthsDays)
      break;
  
  
  
  pDatum->xDte.m_uYearDelta = nYears;
  pDatum->xDte.m_uMonth     = i_Month;
  pDatum->xDte.m_uDay       = nMonthsDays - nMonthDaySum[i_Month - 1];
  pDatum->xDte.m_uSpare     = 0;
}

/*----------------------------------------------------------------------------*/
/* compute number of days since base date                                     */
/*----------------------------------------------------------------------------*/
void RowCoder::vIntoBaseDays(Datum* pDatum, i8& iEra)
{
  if (pDatum->xDte.m_uYearDelta < C_SILVER_ERA)
    iEra = C_CLASSICAL_ERA;
  else if (pDatum->xDte.m_uYearDelta < C_MODERN_ERA)
    iEra = C_SILVER_ERA;
  else
    iEra = C_MODERN_ERA;

  i8 nDays = 0;
  
  // year component
  nDays = pDatum->xDte.m_uYearDelta * 366;
  
  if (C_SILVER_ERA == iEra)
    nDays -= C_SILVER_DAYS;
  else if (C_MODERN_ERA == iEra)
    nDays -= C_MODERN_DAYS;
  else {
    ASSERT(C_CLASSICAL_ERA == iEra);
  }

  // month components
  nDays += nMonthDaySum[pDatum->xDte.m_uMonth - 1];
  
  // day component
  nDays += pDatum->xDte.m_uDay;
  
  pDatum->m_iDte = nDays;
}


/*----------------------------------------------------------------------------*/
/* decoding of dte                                                            */
/*----------------------------------------------------------------------------*/
i8 RowCoder::iStripDte(WordView& wTspView, eMN eEncoding)
{
  i8 iDecoded = wTspView.m_iI8;
  iDecoded <<= 40;
  
  return iDecoded;
}

/*----------------------------------------------------------------------------*/
/* decoding of tme                                                            */
/*----------------------------------------------------------------------------*/
i8 RowCoder::iStripTme(WordView& wTspView, eMN eEncoding)
{
  i8 iDecoded = wTspView.m_iI8;
  iDecoded <<= 40;
  
  return iDecoded;
}

/*----------------------------------------------------------------------------*/
/* decoding of tsp                                                            */
/*----------------------------------------------------------------------------*/
i8 RowCoder::iStripTsp(WordView& wTspView, eMN eEncoding)
{
  i8 iDecoded = wTspView.m_iI8;
  iDecoded <<= 24;
  
  return iDecoded;
}

/*----------------------------------------------------------------------------*/
/* get length of tail associate with a head line                              */
/*----------------------------------------------------------------------------*/
i8 RowCoder::nShocoTailSize(RowNub* pNub, HeadLine* aHeadLine)
{
  //MemLib::vWordDump(aHeadLine,sizeof(HeadLine));
  
  ColumnVec* pColVec = pNub->pDesc()->pColumnVec();
  Coda* pCoda = pNub->pCoda();

  Column* pLastCol = pColVec->pGet(pColVec->nItems() - 1);
  ASSERT(pLastCol->bIsLastColumn());
  
  i8 i_Offset = 8;
  i8 i_LastColumn = 0;
  
  eMN eEncoding = eGetEncodingExt(pLastCol->iColumnNo(),aHeadLine,
                                  pColVec,pCoda,
                                  i_LastColumn,i_Offset);
  
  if (MN_SHOCO_2B != eEncoding)
    return C_UNKNOWN;
  
  i8 i_Distance = aHeadLine->w8_2B(i_Offset);
  TailLine* aShocoTail = pNub->aToTail(aHeadLine,i_Distance);

  i8 nCompressed = aShocoTail->w8_1B(C_SHOCO_COMPRESSED_OFFSET);
  RASSERT(nCompressed <= 56);
  i8 nTailSize = nCompressed + 1;
  
  //MemLib::vWordDump(aHeadLine,sizeof(HeadLine));


  return nTailSize;
}

/*----------------------------------------------------------------------------*/
/* get length of tail associate with a head line                              */
/*----------------------------------------------------------------------------*/
void RowCoder::vSetAuxInt(Coda* pIntoCoda, HeadLine* aIntoHead, Column* pCol,
                          eMN eEncoding, i8 i_IntoOffset)
{
  i8 iInt = 0;
  
  switch (eEncoding) {
      
    case MN_NULL: return;
      
    case MN_1B:   iInt = aIntoHead->w8_1B(i_IntoOffset); break;
    case MN_2B:   iInt = aIntoHead->w8_2B(i_IntoOffset); break;
    case MN_3B:   iInt = aIntoHead->w8_3B(i_IntoOffset); break;
    case MN_4B:   iInt = aIntoHead->w8_4B(i_IntoOffset); break;
    case MN_5B:   iInt = aIntoHead->w8_5B(i_IntoOffset); break;
    case MN_6B:   iInt = aIntoHead->w8_6B(i_IntoOffset); break;
    case MN_7B:   iInt = aIntoHead->w8_7B(i_IntoOffset); break;
    case MN_8B:   iInt = aIntoHead->w8_8B(i_IntoOffset); break;
      
    case MN_MINUS_ONE: iInt = -1; break;
    case MN_ZERO:      iInt = 0;  break;
    case MN_PLUS_ONE:  iInt = 1;  break;
      
    default:
      NEVER_REACH;
  }

  pIntoCoda->m_aiAux[pCol->iColumnNo()].m_iInt = iInt;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
