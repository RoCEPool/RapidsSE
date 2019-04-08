/*============================================================================*/
/* RowCoder.cpp -- Row Coder                                                  */
/*============================================================================*/

#include "Data.h"

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* copy an encoded row from one tray to another                               */
/*----------------------------------------------------------------------------*/
bool RowCoder::bCopy(RowNub* pIntoNub, RowNub* pFromNub)
{
  static i8 nFolds = 0;
  RASSERT(pIntoNub->pDesc() == pFromNub->pDesc());
  
  i8 nCacheLines = nLines(pFromNub,0);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // check for space in target nub
  if (! pIntoNub->bHasSpace(sizeof(CacheLine) * nCacheLines))
    return false;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  ColumnVec* pColVec = pIntoNub->pDesc()->pColumnVec();

  //! NOTE:: Assuming each row fits in a cacheline + (n * Shoco Lines)

  CacheLine* pIntoHead  = pIntoNub->aAllotHead();
  CacheLine* pFromHead = pFromNub->aGet(0);
  
  MemLib::vCopy(pIntoHead,pFromHead,C_BYTES_PER_CACHE_LINE,CN_ROWBUFFER);
  //MemLib::vWordDump(pIntoHead,sizeof(CacheLine));

  if (1 == nCacheLines)
    return true;

  i8 nColumns = pColVec->nItems();
  i8 i_Offset = C_UNKNOWN;
  
  i8 i_Tail = 0;
  for (i8 i_Column = 0; i_Column < nColumns; i_Column++) {
    
    Column* pCol = pColVec->pGet(i_Column);
    
    if (DM_VCH != pCol->eDatumType())
      continue;
    
    eMN eEncoding = eGetEncoding(pFromHead,pColVec,i_Column,i_Offset);
    
    if (MN_SHOCO_2B != eEncoding)
      continue;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // copy and adjust a shoco tail line
    
    i8 i_Diff = pFromHead->w8_2B(i_Offset);
    CacheLine* pFromTail = pFromHead + i_Diff;
    
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef WITH_SHOCO_FOLDLAST
    if (pCol->bIsLastColumn()) {
      
      i8 nFreeHeadBytes = C_BYTES_PER_CACHE_LINE - i_Offset - 1;
      i8 nCompressed = pFromTail->w8_2B(0);
      
      if (nCompressed <= nFreeHeadBytes) {
        
        //MemLib::vWordDump(pIntoHead,sizeof(CacheLine));
        pIntoHead->v1N(i_Column,MN_SHOCO_FIT);
        //MemLib::vWordDump(pIntoHead,sizeof(CacheLine));
        pIntoHead->v1B(i_Offset,nCompressed);
        //MemLib::vWordDump(pIntoHead,sizeof(CacheLine));

        byte* aFrom = (byte*)pFromTail + C_BYTES_PER_WORD;
        byte* aInto = (byte*)pIntoHead + i_Offset + 1;
        
        MemLib::vCopy(aInto,aFrom,nCompressed,CN_ROWBUFFER);

        //MemLib::vWordDump(pIntoHead,sizeof(CacheLine));

        nFolds++;
        if (IntLib::bIsDecaCycle(nFolds))
          vTrace("%lld folds\n"_Stz,nFolds);
        
        i_Tail++;
        continue;
      }
      
      // fall thru
    }
#endif

    CacheLine* pIntoTail = nullptr;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef WITH_SHOCO_DUPLICATES
    pIntoTail = pFindTail(pIntoNub,pFromTail);
    
    if (pIntoTail) {
      // what gets encoded is the difference in cache lines
      i8 i_RevisedDiff = pIntoTail - pIntoHead;
      
      //MemLib::vWordDump(pIntoHead,sizeof(CacheLine));
      
      pIntoHead->vPut(i_Offset,i_RevisedDiff,MN_2B);
      
      //MemLib::vWordDump(pIntoHead,sizeof(CacheLine));
      
      i_Tail++;
      continue;
    }
#endif

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    pIntoTail = pIntoNub->aAllotTails(1);
    MemLib::vCopy(pIntoTail,pFromTail,C_BYTES_PER_CACHE_LINE,CN_ROWBUFFER);
    
    // what gets encoded is the difference in cache lines
    i8 i_RevisedDiff = pIntoTail - pIntoHead;
    
    //MemLib::vWordDump(pIntoHead,sizeof(CacheLine));
    
    pIntoHead->vPut(i_Offset,i_RevisedDiff,MN_2B);
    
    //MemLib::vWordDump(pIntoHead,sizeof(CacheLine));
    
    i_Tail++;
    continue;
  }
  
  return true;
}
/*----------------------------------------------------------------------------*/
//bool RowCoder::bCopy(RowNub* pIntoNub, RowNub* pFromNub)
//{
//  RASSERT(pIntoNub->pDesc() == pFromNub->pDesc());
//
//  i8 nCacheLines = nLines(pFromNub,0);
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//  // check for space in target nub
//  if (! pIntoNub->bHasSpace(sizeof(CacheLine) * nCacheLines))
//    return false;
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//  ColumnVec* pColVec = pIntoNub->pDesc()->pColumnVec();
//
//  //! NOTE:: Assuming each row fits in a cacheline + (n * Shoco Lines)
//
//  CacheLine* pIntoHead  = pIntoNub->aAllotHead();
//  CacheLine* pIntoTails = pIntoNub->aAllotTails(nCacheLines - 1);
//
//  CacheLine* pFromHead = pFromNub->aGet(0);
//
//  MemLib::vCopy(pIntoHead,pFromHead,C_BYTES_PER_CACHE_LINE,CN_ROWBUFFER);
//  //MemLib::vWordDump(pIntoHead,sizeof(CacheLine));
//
//  if (! pIntoTails)
//    return true;
//
//  i8 nColumns = pColVec->nItems();
//  i8 i_Offset = C_UNKNOWN;
//
//  i8 i_Tail = 0;
//  for (i8 i_Column = 0; i_Column < nColumns; i_Column++) {
//
//    Column* pCol = pColVec->pGet(i_Column);
//
//    if (DM_VCH != pCol->eDatumType())
//      continue;
//
//    eMN eEncoding = eGetEncoding(pFromHead,pColVec,i_Column,i_Offset);
//
//    if (MN_SHOCO_2B != eEncoding)
//      continue;
//
//    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//    // copy and adjust a shoco tail line
//
//    i8 i_Diff = pFromHead->w8_2B(i_Offset);
//    CacheLine* pFromTail = pFromHead + i_Diff;
//
//    MemLib::vCopy(&pIntoTails[i_Tail],pFromTail,
//                  C_BYTES_PER_CACHE_LINE,CN_ROWBUFFER);
//
//    // what gets encoded is the difference in cache lines
//    i8 i_RevisedDiff = &pIntoTails[i_Tail] - pIntoHead;
//
//    //MemLib::vWordDump(pIntoHead,sizeof(CacheLine));
//
//    pIntoHead->vPut(i_Offset,i_RevisedDiff,MN_2B);
//
//    //MemLib::vWordDump(pIntoHead,sizeof(CacheLine));
//
//    i_Tail++;
//  }
//
//  return true;
//}

/*----------------------------------------------------------------------------*/
/* get encoded datum from cache line                                          */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecode(Datum* pDatum, RowNub* pNub, i8 i_Row, Column* pCol)
{
  CacheLine* aHeadLine = pNub->aGet(i_Row);
  
  ColumnVec* pColVec = pNub->pDesc()->pColumnVec();

  vDecode(pDatum,aHeadLine,pColVec,pCol);
}

/*----------------------------------------------------------------------------*/
/* get encoded datum from cache line                                          */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecode(Datum* pDatum, CacheLine* aHeadLine,
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
  eMN eEncoding = eGetEncoding(pNub->aGet(i_Row), pNub->pDesc()->pColumnVec(),
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
  CacheLine* aHeadLine = pNub->aGet(i_Row);

  ColumnVec* pColVec = pNub->pDesc()->pColumnVec();
  
  vRecord(pRec,aHeadLine,pColVec);
}

/*----------------------------------------------------------------------------*/
/* sprint decode row from cache line into string                              */
/*----------------------------------------------------------------------------*/
void RowCoder::vRecord(Record* pRec, CacheLine* aHeadLine, ColumnVec* pColVec)
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
void RowCoder::vAppend(Record* pRec, CacheLine* aHeadLine,
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
i8 RowCoder::nDataBytes(eMN eEncoding, eDM eDatum)
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
        case MN_DIFF_1B: return 1;
        case MN_DIFF_2B: return 2;
        case MN_DIFF_3B: return 3;
        case MN_DIFF_4B: return 4;
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
        case MN_DIFF_1B: return 1;
        case MN_DIFF_2B: return 2;
        case MN_DIFF_3B: return 3;
        case MN_DIFF_4B: return 4;
        default:
          NEVER_REACH;
      }
      break;
      
    case DM_TME:
      switch (eEncoding) {
        case MN_DIFF_1B: return 1;
        case MN_DIFF_2B: return 2;
        case MN_DIFF_3B: return 3;
        case MN_DIFF_4B: return 4;
        default:
          NEVER_REACH;
      }
      break;
      
    case DM_TSP:
      switch (eEncoding) {
        case MN_DIFF_1B: return 1;
        case MN_DIFF_2B: return 2;
        case MN_DIFF_3B: return 3;
        case MN_DIFF_4B: return 4;
        default:
          NEVER_REACH;
      }
      break;
      
    case DM_VCH:
      switch (eEncoding) {
        case MN_COL_ARRAY:   NEVER_REACH; return 0;
        case MN_SHOCO_FIT:   return 2;
        case MN_SHOCO_2B:    return 2;
        case MN_LOCAL_HMAP:  NEVER_REACH; return 0;
        case MN_GLOBAL_HMAP: NEVER_REACH; return 0;
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
void RowCoder::vDecodeBcd(Datum* pDatum, CacheLine* aHeadLine,
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
void RowCoder::vDecodeBol(Datum* pDatum, CacheLine* aHeadLine,
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
void RowCoder::vDecodeDte(Datum* pDatum, CacheLine* aHeadLine,
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
  
  vRestoreDte(pDatum,eEncoding);
}

/*----------------------------------------------------------------------------*/
/* decode float from given cache line                                         */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeFlt(Datum* pDatum, CacheLine* aHeadLine,
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
void RowCoder::vDecodeInt(Datum* pDatum, CacheLine* aHeadLine,
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
void RowCoder::vDecodeIvl(Datum* pDatum, CacheLine* aHeadLine,
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
void RowCoder::vDecodeTme(Datum* pDatum, CacheLine* aHeadLine,
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
void RowCoder::vDecodeTsp(Datum* pDatum, CacheLine* aHeadLine,
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
void RowCoder::vDecodeVch(Datum* pDatum, CacheLine* aHeadLine,
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
void RowCoder::vDecodeVchShoco(Datum* pDatum, CacheLine* aHeadLine,
                               Column* pCol, eMN eEncoding, i8 i_Offset)
{
  i8 i_Delta = aHeadLine->w8_2B(i_Offset);
  CacheLine* aShocoLine = aHeadLine + i_Delta;
  //MemLib::vWordDump(aShocoLine,sizeof(CacheLine));
  
  i8 nCompressed = aShocoLine->w8_1B(0);
  
  i8 nDecompressed =
    Compress::iFromShoco((char*)pDatum->m_xWsp.aStz(),
                         255,
                         ((byte*)aShocoLine)+8,
                         nCompressed);
  ASSERT(nDecompressed);
}

/*----------------------------------------------------------------------------*/
/* decode varchar from given cache line                                       */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeVchShocoFit(Datum* pDatum, CacheLine* aHeadLine,
                               Column* pCol, eMN eEncoding, i8 i_Offset)
{
  i8 nCompressed = aHeadLine->w8_1B(i_Offset);
  
  i8 nDecompressed =
    Compress::iFromShoco((char*)pDatum->m_xWsp.aStz(),
                         255,
                         ((byte*)aHeadLine)+i_Offset+2,
                         nCompressed);
  ASSERT(nDecompressed);
}

/*----------------------------------------------------------------------------*/
/* decode varchar from given cache line                                       */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeVchSmall(Datum* pDatum, CacheLine* aHeadLine,
                               Column* pCol, eMN eEncoding, i8 i_Offset)
{
  switch (eEncoding) {
      
    case MN_1B:
      pDatum->m_iInt = aHeadLine->w8_1B(i_Offset);
      *(i8*)pDatum->m_xWsp.aStz() = pDatum->m_iInt;
      ((char*)pDatum->m_xWsp.aStz())[1] = '\0';
      break;
    case MN_2B:
      pDatum->m_iInt = aHeadLine->w8_2B(i_Offset);
      *(i8*)pDatum->m_xWsp.aStz() = pDatum->m_iInt;
      ((char*)pDatum->m_xWsp.aStz())[2] = '\0';
      break;
    case MN_3B:
      pDatum->m_iInt = aHeadLine->w8_3B(i_Offset);
      *(i8*)pDatum->m_xWsp.aStz() = pDatum->m_iInt;
      ((char*)pDatum->m_xWsp.aStz())[3] = '\0';
      break;
    case MN_4B:
      pDatum->m_iInt = aHeadLine->w8_4B(i_Offset);
      *(i8*)pDatum->m_xWsp.aStz() = pDatum->m_iInt;
      ((char*)pDatum->m_xWsp.aStz())[4] = '\0';
      break;
    case MN_5B:
      pDatum->m_iInt = aHeadLine->w8_5B(i_Offset);
      *(i8*)pDatum->m_xWsp.aStz() = pDatum->m_iInt;
      ((char*)pDatum->m_xWsp.aStz())[5] = '\0';
      break;
    case MN_6B:
      pDatum->m_iInt = aHeadLine->w8_6B(i_Offset);
      *(i8*)pDatum->m_xWsp.aStz() = pDatum->m_iInt;
      ((char*)pDatum->m_xWsp.aStz())[6] = '\0';
      break;
    case MN_7B:
      pDatum->m_iInt = aHeadLine->w8_7B(i_Offset);
      *(i8*)pDatum->m_xWsp.aStz() = pDatum->m_iInt;
      ((char*)pDatum->m_xWsp.aStz())[7] = '\0';
      break;
    case MN_8B:
      pDatum->m_iInt = aHeadLine->w8_8B(i_Offset);
      *(i8*)pDatum->m_xWsp.aStz() = pDatum->m_iInt;
      ((char*)pDatum->m_xWsp.aStz())[8] = '\0';
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
  CacheLine* aHeadLine = pNub->aGet(0);
  
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
  RASSERT(nBytesEncoded == nDataBytes(eEncoding,DM_BCD));
  
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
  CacheLine* aHeadLine = pNub->aGet(0);

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
  RASSERT(nBytesEncoded == nDataBytes(eEncoding,DM_BOL));
  
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
  CacheLine* aHeadLine = pNub->aGet(0);

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
  eMN eEncoding = eEncodingDte(pDatum,nBytesEncoded);
  RASSERT(nBytesEncoded == nDataBytes(eEncoding,DM_DTE));
  
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
/* encode float                                                               */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeFlt(RowNub* pNub, Column* pCol, Datum* pDatum)
{
  CacheLine* aHeadLine = pNub->aGet(0);

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
  RASSERT(nBytesEncoded == nDataBytes(eEncoding,DM_FLT));
  
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
  CacheLine* aHeadLine = pNub->aGet(0);

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
  RASSERT(nBytesEncoded == nDataBytes(eEncoding,DM_INT));
  
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
  CacheLine* aHeadLine = pNub->aGet(0);

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
  RASSERT(nBytesEncoded == nDataBytes(eEncoding,DM_IVL));
  
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
  CacheLine* aHeadLine = pNub->aGet(0);

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
  RASSERT(nBytesEncoded == nDataBytes(eEncoding,DM_TME));
  
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
  CacheLine* aHeadLine = pNub->aGet(0);

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
  RASSERT(nBytesEncoded == nDataBytes(eEncoding,DM_TSP));
  
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
  CacheLine* aHeadLine = pNub->aGet(0);

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
  
  // set n-byte data for column value
  if (pDatum->m_nChars) {
    if (eEncoding == MN_SHOCO_2B)
      vEncodeVchShoco(pNub,pCol,pDatum);
    else
      vEncodeVchSmall(pNub,pCol,pDatum);
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
  CacheLine* aHeadLine = pNub->aGet(0);
  
  // we would encode the shoco version of the string into a cache line
  
  // allot a tail cache line
  i8 i_Shoco = pNub->iAllotTail();
  //RASSERT(255 == i_Shoco);
  CacheLine* aShocoLine = pNub->aGet(i_Shoco);
  
  // set data for column value as the index of the shoco cache line
  i8 i_Offset = pNub->i_Offset();
  aHeadLine->vPut(i_Offset,i_Shoco,MN_2B);
  pNub->vOffsetInc(2);

  // compress varchar into tail shoco cache line
  i8 nCompressed =
    Compress::iIntoShoco((byte*)aShocoLine + C_BYTES_PER_WORD,
                         C_BYTES_PER_CACHE_LINE - C_BYTES_PER_WORD,
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
    
  
  
  //MemLib::vWordDump(aShocoLine,sizeof(CacheLine));
  
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
  CacheLine* aHeadLine = pNub->aGet(0);
  
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
  
  if (iLen <= 8) {
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
CacheLine* RowCoder::pFindTail(RowNub* pNub, CacheLine* aFindLine)
{
  static i8 nFound = 0;
  static i8 nNotFound = 0;

  i8 nTails = pNub->nTails();
  
  if (! nTails) {
    nNotFound++;
//    if (IntLib::bIsDecaCycle(nNotFound))
//      vTrace("%lld dupe tail NOT found\n"_Stz,nNotFound);
    return nullptr;
  }
  
  i8 i_Tail = pNub->nMaxItems() - 1;
  
  while (nTails--) {
    
    CacheLine* aTailLine = pNub->aGet(i_Tail);
    
    //MemLib::vWordDump(aFindLine,sizeof(CacheLine));
    //MemLib::vWordDump(aTailLine,sizeof(CacheLine));

    if (0 == MemLib::iCompareWords(aTailLine,aFindLine,sizeof(CacheLine))) {
      nFound++;
//      if (IntLib::bIsDecaCycle(nFound))
//        vTrace("%lld dupe tail found\n"_Stz,nFound);
      return aTailLine;
    }
    
    i_Tail--;
  }
  
  nNotFound++;
//  if (IntLib::bIsDecaCycle(nNotFound))
//    vTrace("%lld dupe tail NOT found\n"_Stz,nNotFound);
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get the encoding of of a column from given cache line                      */
/*----------------------------------------------------------------------------*/

//! consider passing a eDatum array instead of colum vector
eMN RowCoder::eGetEncoding(CacheLine* aHeadLine, ColumnVec* pColVec,
                           i8 i_Col, i8& i_Offset)
{
  i_Offset = 8;
  
  for (i8 i_Nibble = 0; i_Nibble < i_Col; i_Nibble++) {
    
    eMN eEncoding = (eMN)aHeadLine->w8_1N(i_Nibble);

    Column* pCol  = pColVec->pGet(i_Nibble);
    eDM eDatum    = pCol->eDatumType();
    
    i_Offset += nDataBytes(eEncoding,eDatum);
    
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
void RowCoder::vInitCacheLine(CacheLine* aHeadLine)
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
  
  i8 nCacheLines = 1;
  // get no of columns
  i8 nColumns = pColVec->nItems();
  
  i8 i_Offset = C_UNKNOWN;
  
  // for each column
  for (i8 i_Column = 0; i_Column < nColumns; i_Column++) {
    
    // get the column at index
    Column* pCol = pColVec->pGet(i_Column);
    
    if (DM_VCH != pCol->eDatumType())
      continue;
    
    CacheLine* aHeadLine = pNub->aGet(i_Row);
    eMN eEncoding = eGetEncoding(aHeadLine,pColVec,i_Column,i_Offset);
    
    if (MN_SHOCO_2B == eEncoding)
      nCacheLines++;
  }
  
  return nCacheLines;
}

/*----------------------------------------------------------------------------*/
/* restore zero stripped dte                                                  */
/*----------------------------------------------------------------------------*/
void RowCoder::vRestoreDte(Datum* pDatum, eMN eEncoding)
{
  pDatum->m_iInt <<= 40;
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
