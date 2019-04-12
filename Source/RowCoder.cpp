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

  for (i8 i_Tail = pNub->iFirstTail();
       i_Tail != C_UNKNOWN;
       i_Tail = pNub->iNextTail(i_Tail)) {
    
    TailLine* aTail = pNub->aGetTail(i_Tail);
    
    i8 iLenTail = aTail->w8_1B(0);
    iSumTail += iLenTail;
    nTails++;
    
    if (iLenTail < iMinTail)
      iMinTail = iLenTail;
    if (iLenTail > iMaxTail)
      iMaxTail = iLenTail;
  }
  
  for (i8 i_Head = 0; i_Head < pNub->nHeads();  i_Head++) {
    
    HeadLine* aHead = pNub->aGetHead(i_Head);
    
    i8 nFreeHead = nFreeSpaceHead(pNub,aHead);
    iSumHead += nFreeHead;
    nHeads++;
    
    if (nFreeHead < iMinHead)
      iMinHead = nFreeHead;
    if (nFreeHead > iMaxHead)
      iMaxHead = nFreeHead;
  }

  vTrace("Heads %lld Tails %lld; "
         "Avg head free %lldB Min %lldB Max %lldB; "
         "Avg tail use %lldB Min %lldB Max %lldB\n"_Stz,
         
         pNub->nHeads(),
         pNub->nTails(),

         nHeads ? iSumHead/nHeads : 0,
         iMinHead,
         iMaxHead,

         nTails ? iSumTail/nTails : 0,
         iMinTail,
         iMaxTail);
}

/*----------------------------------------------------------------------------*/
/* copy an encoded row from one tray to another                               */
/*----------------------------------------------------------------------------*/
bool RowCoder::bCopy(RowNub* pIntoNub, RowNub* pFromNub)
{
  static i8 nCalls = 0;
  nCalls++;
  
  static i8 nFolds = 0;
  RASSERT(pIntoNub->pDesc() == pFromNub->pDesc());
  
  i8 nHeadLines = nLines(pFromNub,0);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // check for space in target nub
  if (! pIntoNub->bHasSpace(sizeof(HeadLine) * nHeadLines))
    return false;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  ColumnVec* pColVec = pIntoNub->pDesc()->pColumnVec();

  //! NOTE:: Assuming each row fits in a cacheline + (n * Shoco Lines)

  i8 i_IntoHead = pIntoNub->iAllotHead();
  HeadLine* pIntoHead = pIntoNub->aGetHead(i_IntoHead);
  HeadLine* pFromHead = pFromNub->aGetHead(0);
  
  MemLib::vCopy(pIntoHead,pFromHead,sizeof(HeadLine),CN_ROWBUFFER);
  //MemLib::vWordDump(pIntoHead,sizeof(HeadLine));

  if (1 == nHeadLines)
    return true;

  i8 nColumns = pColVec->nItems();
  i8 i_Offset = C_UNKNOWN;
  
  for (i8 i_Column = 0; i_Column < nColumns; i_Column++) {
    
    Column* pCol = pColVec->pGet(i_Column);
    
    if (DM_VCH != pCol->eDatumType())
      continue;
    
    eMN eEncoding = eGetEncoding(pFromHead,pColVec,i_Column,i_Offset);
    
    if (MN_SHOCO_2B != eEncoding)
      continue;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // copy shoco tail line from nub
    
    i8 i_Shoco = pFromHead->w8_2B(i_Offset);
    TailLine* pFromTail = pFromNub->aGetTail(i_Shoco);
    
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef WITH_SHOCO_FOLDLAST
    if (pCol->bIsLastColumn()) {
      
      i8 nFreeHeadBytes = C_BYTES_PER_CACHE_LINE - i_Offset - 1;
      i8 nCompressed = pFromTail->w8_1B(0);
      
      if (nCompressed <= nFreeHeadBytes) {
        
        //if (82 == nCalls)
        //  MemLib::vWordDump(pIntoHead,sizeof(HeadLine));
        pIntoHead->v1N(i_Column,MN_SHOCO_FIT);
        //if (82 == nCalls)
        //  MemLib::vWordDump(pIntoHead,sizeof(HeadLine));
        pIntoHead->v1B(i_Offset,nCompressed);
        //if (82 == nCalls)
        //  MemLib::vWordDump(pIntoHead,sizeof(HeadLine));

        byte* aFrom = (byte*)pFromTail + 1;
        byte* aInto = (byte*)pIntoHead + i_Offset + 1;
        
        MemLib::vCopy(aInto,aFrom,nCompressed,CN_ROWBUFFER);
        //if (82 == nCalls)
        //  MemLib::vWordDump(pIntoHead,sizeof(HeadLine));

        nFolds++;
        //if (IntLib::bIsDecaCycle(nFolds))
        //  vTrace("%lld folds\n"_Stz,nFolds);
        
        continue;
      }
      
      // fall thru
    }
#endif

    TailLine* pIntoTail = nullptr;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef WITH_SHOCO_DUPLICATES
    i8 i_IntoTail = iFindTail(pIntoNub,pFromTail);
    
    if (C_UNKNOWN != i_IntoTail) {

      pIntoTail = pIntoNub->aGetTail(i_IntoTail);
    
      // what gets encoded is the unit difference between head and tail
      i8 i_TailDistance = pIntoNub->iUnitDiff(i_IntoHead,i_IntoTail);
      
      //MemLib::vWordDump(pIntoHead,sizeof(HeadLine));
      
      pIntoHead->vPut(i_Offset,i_TailDistance,MN_2B);
      
      //MemLib::vWordDump(pIntoHead,sizeof(HeadLine));
      
      continue;
    }
#endif

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    i_IntoTail = pIntoNub->iAllotTail(sizeof(TailLine));
    pIntoTail = pIntoNub->aGetTail(i_IntoTail);
    MemLib::vCopy(pIntoTail,pFromTail,sizeof(TailLine),CN_ROWBUFFER);
    
    // what gets encoded is the difference in cache lines
    i8 i_TailDistance = pIntoNub->iUnitDiff(i_IntoHead,i_IntoTail);

    //MemLib::vWordDump(pIntoHead,sizeof(HeadLine));
    
    pIntoHead->vPut(i_Offset,i_TailDistance,MN_2B);
    
    //MemLib::vWordDump(pIntoHead,sizeof(HeadLine));
    
    continue;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get encoded datum from cache line                                          */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecode(Datum* pDatum, RowNub* pNub, i8 i_Row, Column* pCol)
{
  HeadLine* aHeadLine = pNub->aGetHead(i_Row);
  
  ColumnVec* pColVec = pNub->pDesc()->pColumnVec();

  vDecode(pDatum,aHeadLine,pColVec,pCol);
}

/*----------------------------------------------------------------------------*/
/* get encoded datum from cache line                                          */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecode(Datum* pDatum, HeadLine* aHeadLine,
                       ColumnVec* pColVec, Column* pCol)
{
  eDM eDatum = pCol->eDatumType();
  
  switch (eDatum) {
    case DM_BCD: vDecodeBcd(pDatum,aHeadLine,pColVec,pCol); break;
    case DM_BOL: vDecodeBol(pDatum,aHeadLine,pColVec,pCol); break;
    case DM_DTE: vDecodeDte(pDatum,aHeadLine,pColVec,pCol); break;
    case DM_FLT: vDecodeFlt(pDatum,aHeadLine,pColVec,pCol); break;
    case DM_INT: vDecodeInt(pDatum,aHeadLine,pColVec,pCol); break;
    case DM_IVL: vDecodeIvl(pDatum,aHeadLine,pColVec,pCol); break;
    case DM_TME: vDecodeTme(pDatum,aHeadLine,pColVec,pCol); break;
    case DM_TSP: vDecodeTsp(pDatum,aHeadLine,pColVec,pCol); break;
    case DM_VCH: vDecodeVch(pDatum,aHeadLine,pColVec,pCol); break;
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
/* print decoded datum from cache line                                        */
/*----------------------------------------------------------------------------*/
void RowCoder::vPrint(RowNub* pNub, i8 i_Row, Column* pCol)
{
  Datum oDatum;
  HV_Rec hvRec;
  
  i8 i_Offset;
  eMN eEncoding = eGetEncoding(pNub->aGetHead(i_Row), pNub->pDesc()->pColumnVec(),
                               pCol->iColumnNo(),i_Offset);
  
  vDecode(&oDatum,pNub,i_Row,pCol);
  
  eDM eDatum = pCol->eDatumType();
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
  
  vTrace("Column[%lld %s %s] (e%lld,@%lld) : %s\n"_Stz,
         pCol->iColumnNo(),
         pCol->sColumnName().aStz(),
         aDM[pCol->eDatumType()],
         
         eEncoding,
         i_Offset,
         
         hvRec->szRecStr());
  
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
  
  // get no of columns
  i8 nColumns = pColVec->nItems();
  
  // for each column
  for (i8 i_Column = 0; i_Column < nColumns; i_Column++) {
    
    // get the column at index
    Column* pCol = pColVec->pGet(i_Column);
    
    // comma separate column values
    if (i_Column)
      pRec->szRecStr()->vConcat(","_Stz);
    
    // append column value
    vAppend(pRec,aHeadLine,pColVec,pCol);
  }
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* decode and append column datum from cache line                             */
/*----------------------------------------------------------------------------*/
void RowCoder::vAppend(Record* pRec, HeadLine* aHeadLine,
                       ColumnVec* pColVec, Column* pCol)
{
  Datum oDatum;
  
  vDecode(&oDatum,aHeadLine,pColVec,pCol);
  
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
/* return number of data bytes of encoding                                    */
/*----------------------------------------------------------------------------*/

//! this may be better done as a 2d array
i8 RowCoder::nDataBytes(HeadLine* aHeadLine, Column* pCol, eMN eEncoding, eDM eDatum)
{
  if (eEncoding == MN_NULL)
    return 0;
  
  if (eEncoding <= MN_8B)
    return eEncoding;
  
  if (MN_OVERFLOW == eEncoding)
    return 0;
  
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
        case MN_MINUS_ONE: return 0;
        case MN_ZERO:      return 0;
        case MN_PLUS_ONE:  return 0;
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
        case MN_SHOCO_FIT: {
          //aHeadLine->w8_1B(i_Offset);
          return 1;
        }
        case MN_SHOCO_2B:
          return 2;
        default:
          NEVER_REACH;
      }
      break;
      
    default:
      NEVER_REACH;
  }
  
}

/*----------------------------------------------------------------------------*/
/* decode decimal from given cache line                                       */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeBcd(Datum* pDatum, HeadLine* aHeadLine,
                          ColumnVec* pColVec, Column* pCol)
{
  i8 i_Offset  = C_UNKNOWN;
  i8 i_Column  = pCol->iColumnNo();
  
  eMN eEncoding = eGetEncoding(aHeadLine,pColVec,i_Column,i_Offset);
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
      
    default:
      NEVER_REACH;
  }
 }

/*----------------------------------------------------------------------------*/
/* decode boolean from given cache line                                       */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeBol(Datum* pDatum, HeadLine* aHeadLine,
                          ColumnVec* pColVec, Column* pCol)
{
  i8 i_Offset  = C_UNKNOWN;
  i8 i_Column  = pCol->iColumnNo();
  
  eMN eEncoding = eGetEncoding(aHeadLine,pColVec,i_Column,i_Offset);
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
                          ColumnVec* pColVec, Column* pCol)
{
  i8 i_Offset  = C_UNKNOWN;
  i8 i_Column  = pCol->iColumnNo();
  
  eMN eEncoding = eGetEncoding(aHeadLine,pColVec,i_Column,i_Offset);
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
                          ColumnVec* pColVec, Column* pCol)
{
  i8 i_Offset  = C_UNKNOWN;
  i8 i_Column  = pCol->iColumnNo();
  
  eMN eEncoding = eGetEncoding(aHeadLine,pColVec,i_Column,i_Offset);
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
                          ColumnVec* pColVec, Column* pCol)
{
  i8 i_Offset  = C_UNKNOWN;
  i8 i_Column  = pCol->iColumnNo();
  
  eMN eEncoding = eGetEncoding(aHeadLine,pColVec,i_Column,i_Offset);
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
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* decode interval from given cache line                                      */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeIvl(Datum* pDatum, HeadLine* aHeadLine,
                          ColumnVec* pColVec, Column* pCol)
{
  i8 i_Offset  = C_UNKNOWN;
  i8 i_Column  = pCol->iColumnNo();
  
  eMN eEncoding = eGetEncoding(aHeadLine,pColVec,i_Column,i_Offset);
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
                          ColumnVec* pColVec, Column* pCol)
{
  i8 i_Offset  = C_UNKNOWN;
  i8 i_Column  = pCol->iColumnNo();
  
  eMN eEncoding = eGetEncoding(aHeadLine,pColVec,i_Column,i_Offset);
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
                          ColumnVec* pColVec, Column* pCol)
{
  i8 i_Offset  = C_UNKNOWN;
  i8 i_Column  = pCol->iColumnNo();
  
  eMN eEncoding = eGetEncoding(aHeadLine,pColVec,i_Column,i_Offset);
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
                          ColumnVec* pColVec, Column* pCol)
{
  i8 i_Offset  = C_UNKNOWN;
  i8 i_Column  = pCol->iColumnNo();
  
  //! needs more work
  eMN eEncoding = eGetEncoding(aHeadLine,pColVec,i_Column,i_Offset);
  
  if (MN_NULL == eEncoding)
    pDatum->m_iInt = C_DATUM_NULL;

  else if (eEncoding <= MN_8B)
    vDecodeVchSmall(pDatum,aHeadLine,pCol,eEncoding,i_Offset);
  
  else if (MN_SHOCO_2B == eEncoding)
    vDecodeVchShoco(pDatum,aHeadLine,pCol,eEncoding,i_Offset);
  
  else if (MN_SHOCO_FIT == eEncoding)
    vDecodeVchShocoFit(pDatum,aHeadLine,pCol,eEncoding,i_Offset);

  else
    NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* decode varchar from given cache line                                       */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeVchShoco(Datum* pDatum, HeadLine* aHeadLine,
                               Column* pCol, eMN eEncoding, i8 i_Offset)
{
  i8 i_TailDistance = aHeadLine->w8_2B(i_Offset);
  TailLine* aShocoLine = RowNub::aToTail(aHeadLine,i_TailDistance);
  
  //MemLib::vWordDump(aShocoLine,sizeof(HeadLine));
  
  i8 nCompressed = aShocoLine->w8_1B(0);
  
  i8 nDecompressed =
    Compress::iFromShoco((char*)pDatum->m_xWsp.aStz(),
                         255,
                         ((byte*)aShocoLine)+1,
                         nCompressed);
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
  RASSERT(nBytesEncoded == nDataBytes(aHeadLine,pCol,eEncoding,DM_BCD));
  
  // set 4bit meta data for column value
  aHeadLine->v1N(i_Column,eEncoding);
  
  // set n-byte data for column value
  if (nBytesEncoded)
    aHeadLine->vPut(pNub->i_Offset(),pDatum->m_iInt,eEncoding);
  
  pNub->vOffsetInc(nBytesEncoded);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
  vDecode(pDatum,pNub,0,pCol);
  RASSERT(wOriginal == (w8)pDatum->m_iInt);
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
  RASSERT(nBytesEncoded == nDataBytes(aHeadLine,pCol,eEncoding,DM_BOL));
  
  // set 4bit meta data for column value
  aHeadLine->v1N(i_Column,eEncoding);
  
  pNub->vOffsetInc(nBytesEncoded);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
  vDecode(pDatum,pNub,0,pCol);
  RASSERT(wOriginal == (w8)pDatum->m_iInt);
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
  RASSERT(nBytesEncoded == nDataBytes(aHeadLine,pCol,eEncoding,DM_DTE));
  
  // set 4bit meta data for column value
  aHeadLine->v1N(i_Column,eEncoding);
  
  // set n-byte data for column value
  if (nBytesEncoded)
    aHeadLine->vPut(pNub->i_Offset(),pDatum->m_iInt,nBytesEncoded);
  
  pNub->vOffsetInc(nBytesEncoded);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
  vDecode(pDatum,pNub,0,pCol);
//  vTrace("-- %lld-%lld-%lld\n"_Stz,
//         pDatum->xDte.m_uYearDelta,pDatum->xDte.m_uMonth,pDatum->xDte.m_uDay);
  RASSERT(wOriginal == (w8)pDatum->m_iInt);
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
  RASSERT(nBytesEncoded == nDataBytes(aHeadLine,pCol,eEncoding,DM_FLT));
  
  // set 4bit meta data for column value
  aHeadLine->v1N(i_Column,eEncoding);
  
  // set n-byte data for column value
  if (nBytesEncoded)
    aHeadLine->vPut(pNub->i_Offset(),pDatum->m_iInt,eEncoding);
  
  pNub->vOffsetInc(nBytesEncoded);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
  vDecode(pDatum,pNub,0,pCol);
  RASSERT(wOriginal == (w8)pDatum->m_iInt);
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
  RASSERT(nBytesEncoded == nDataBytes(aHeadLine,pCol,eEncoding,DM_INT));
  
  // set 4bit meta data for column value
  aHeadLine->v1N(i_Column,eEncoding);
  
  // set n-byte data for column value
  if (nBytesEncoded)
    aHeadLine->vPut(pNub->i_Offset(),pDatum->m_iInt,eEncoding);
  
  pNub->vOffsetInc(nBytesEncoded);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
  vDecode(pDatum,pNub,0,pCol);
  RASSERT(wOriginal == (w8)pDatum->m_iInt);
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
  RASSERT(nBytesEncoded == nDataBytes(aHeadLine,pCol,eEncoding,DM_IVL));
  
  // set 4bit meta data for column value
  aHeadLine->v1N(i_Column,eEncoding);
  
  // set n-byte data for column value
  if (nBytesEncoded)
    aHeadLine->vPut(pNub->i_Offset(),pDatum->m_iInt,eEncoding);
  
  pNub->vOffsetInc(nBytesEncoded);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
  vDecode(pDatum,pNub,0,pCol);
  RASSERT(wOriginal == (w8)pDatum->m_iInt);
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
  RASSERT(nBytesEncoded == nDataBytes(aHeadLine,pCol,eEncoding,DM_TME));
  
  // set 4bit meta data for column value
  aHeadLine->v1N(i_Column,eEncoding);
  
  // set n-byte data for column value
  if (nBytesEncoded)
    aHeadLine->vPut(pNub->i_Offset(),pDatum->m_iInt,eEncoding);
  
  pNub->vOffsetInc(nBytesEncoded);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
  vDecode(pDatum,pNub,0,pCol);
  RASSERT(wOriginal == (w8)pDatum->m_iInt);
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
  RASSERT(nBytesEncoded == nDataBytes(aHeadLine,pCol,eEncoding,DM_TSP));
  
  // set 4bit meta data for column value
  aHeadLine->v1N(i_Column,eEncoding);
  
  // set n-byte data for column value
  if (nBytesEncoded)
    aHeadLine->vPut(pNub->i_Offset(),pDatum->m_iInt,eEncoding);
  
  pNub->vOffsetInc(nBytesEncoded);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
  vDecode(pDatum,pNub,0,pCol);
  RASSERT(wOriginal == (w8)pDatum->m_iInt);
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
      aHeadLine->v1N(i_Column,MN_1B);
      i8 i_Offset = pNub->i_Offset();
      aHeadLine->v1B(i_Offset,0);
      pNub->vOffsetInc(1);
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
  HeadLine* aHeadLine = pNub->aGetHead(0);
  
  // we would encode the shoco version of the string into a cache line
  
  // allot a tail cache line
  i8 i_Shoco = pNub->iAllotTail(sizeof(TailLine));
  //RASSERT(255 == i_Shoco);
  TailLine* aShocoLine = pNub->aGetTail(i_Shoco);
  
  // set data for column value as the index of the shoco cache line
  i8 i_Offset = pNub->i_Offset();
  aHeadLine->vPut(i_Offset,i_Shoco,MN_2B);
  pNub->vOffsetInc(2);

  // compress varchar into tail shoco cache line
  i8 nCompressed =
    Compress::iIntoShoco((byte*)aShocoLine + 1,
                         C_BYTES_PER_CACHE_LINE - 1,
                         (char*)pDatum->m_xWsp.aArea(),
                         pDatum->m_nChars);
  
  // set length of the compressed string encoded into tail shoco cache line
  aShocoLine->v1B(0,nCompressed);
  
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
  
  aHeadLine->vPut(pNub->i_Offset(),
                   *(w8*)pDatum->m_xWsp.aArea(),
                   pDatum->m_nChars);
  
  pNub->vOffsetInc(pDatum->m_nChars);
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
  else if (iLen <= 8) {
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
  
//  for (i8 i_Tail = pNub->iLastTail();
//       i_Tail != C_UNKNOWN;
//       i_Tail = pNub->iPrevTail(i_Tail)) {
  
//    vTrace("i_Tail %lld\n"_Stz,i_Tail);
  
  for (i8 i_Tail = pNub->iFirstTail();
       i_Tail != C_UNKNOWN;
       i_Tail = pNub->iNextTail(i_Tail)) {
    
    TailLine* aTailLine = pNub->aGetTail(i_Tail);
    
    //MemLib::vWordDump(aFindLine,sizeof(HeadLine));
    //MemLib::vWordDump(aTailLine,sizeof(HeadLine));
    
    if (0 == MemLib::iCompareWords(aTailLine,aFindLine,sizeof(TailLine))) {
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
  
  Column* pLastCol = pColVec->pGet(pColVec->nItems() - 1);
  ASSERT(pLastCol->bIsLastColumn());
  
  i8 i_Offset = 0;
  eMN eEncoding = eGetEncoding(aHeadLine,pColVec,pLastCol->iColumnNo(),i_Offset);
  
  if (MN_SHOCO_FIT == eEncoding) {
    i8 nCompressed = aHeadLine->w8_1B(i_Offset);
    i_Offset += nCompressed;
  }

  i8 nFreeBytes = C_BYTES_PER_CACHE_LINE - i_Offset;
  
  return nFreeBytes;
}

/*----------------------------------------------------------------------------*/
/* get the encoding of of a column from given cache line                      */
/*----------------------------------------------------------------------------*/

//! consider passing a eDatum array instead of colum vector
eMN RowCoder::eGetEncoding(HeadLine* aHeadLine, ColumnVec* pColVec,
                           i8 i_Col, i8& i_Offset)
{
  i_Offset = 8;
  
  for (i8 i_Nibble = 0; i_Nibble < i_Col; i_Nibble++) {
    
    eMN eEncoding = (eMN)aHeadLine->w8_1N(i_Nibble);

    Column* pCol  = pColVec->pGet(i_Nibble);
    eDM eDatum    = pCol->eDatumType();
    
    i_Offset += nDataBytes(aHeadLine,pCol,eEncoding,eDatum);
    
  }
  
  eMN eEncoding = (eMN)aHeadLine->w8_1N(i_Col);
  
//  if (MN_SHOCO_FIT == eEncoding) {
//    i8 nCompressed = aHeadLine->w8_2B(i_Offset);
//    i_Offset += nCompressed;
//  }

  return eEncoding;
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
  
  i8 nHeadLines = 1;
  // get no of columns
  i8 nColumns = pColVec->nItems();
  
  i8 i_Offset = C_UNKNOWN;
  
  // for each column
  for (i8 i_Column = 0; i_Column < nColumns; i_Column++) {
    
    // get the column at index
    Column* pCol = pColVec->pGet(i_Column);
    
    if (DM_VCH != pCol->eDatumType())
      continue;
    
    HeadLine* aHeadLine = pNub->aGetHead(i_Row);
    eMN eEncoding = eGetEncoding(aHeadLine,pColVec,i_Column,i_Offset);
    
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
