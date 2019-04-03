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
void RowCoder::vCopy(CacheLine* pIntoHead, CacheLine* pIntoTails,
                     CacheLine xaFromLine[],
                     ColumnVec* pColVec)
{
  CacheLine* pFromHead = &xaFromLine[0];
  MemLib::vCopy(pIntoHead,pFromHead,C_BYTES_PER_CACHE_LINE,CN_ROWBUFFER);

  if (! pIntoTails)
    return;
  
  i8 nColumns = pColVec->nItems();
  i8 i_Offset = C_UNKNOWN;
  
  i8 i_Tail = 0;
  CacheLine* pFromTails = &xaFromLine[i_Tail];
  for (i8 i_Column = 0; i_Column < nColumns; i_Column++) {
    
    Column* pCol = pColVec->pGet(i_Column);
    
    if (DM_VCH != pCol->eDatumType())
      continue;
    
    eMN eEncoding = eGetEncoding(i_Column,&xaFromLine[0],i_Offset);
    
    if (MN_SHOCO_2B != eEncoding)
      continue;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // copy and adjust a shoco tail line

    MemLib::vCopy(&pIntoTails[i_Tail],&pFromTails[i_Tail],
                  C_BYTES_PER_CACHE_LINE,CN_ROWBUFFER);
    
    // what gets encoded is the difference in cache lines
    i8 iDiff = &pIntoTails[i_Tail] - pIntoHead;
    
    //MemLib::vWordDump(pIntoHead,sizeof(CacheLine));

    pIntoHead->vPut(i_Offset,iDiff,MN_2B);

    //MemLib::vWordDump(pIntoHead,sizeof(CacheLine));

    i_Tail++;
  }

}

/*----------------------------------------------------------------------------*/
/* get encoded datum from cache line                                          */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecode(CacheLine xaCacheLine[], i8 i_Row,
                       Column* pCol, Datum* pDatum)
{
  eDM eDatum = pCol->eDatumType();
  
  switch (eDatum) {
    case DM_BCD: vDecodeBcd(xaCacheLine,i_Row,pCol,pDatum); break;
    case DM_BOL: vDecodeBol(xaCacheLine,i_Row,pCol,pDatum); break;
    case DM_DTE: vDecodeDte(xaCacheLine,i_Row,pCol,pDatum); break;
    case DM_FLT: vDecodeFlt(xaCacheLine,i_Row,pCol,pDatum); break;
    case DM_INT: vDecodeInt(xaCacheLine,i_Row,pCol,pDatum); break;
    case DM_IVL: vDecodeIvl(xaCacheLine,i_Row,pCol,pDatum); break;
    case DM_TME: vDecodeTme(xaCacheLine,i_Row,pCol,pDatum); break;
    case DM_TSP: vDecodeTsp(xaCacheLine,i_Row,pCol,pDatum); break;
    case DM_VCH: vDecodeVch(xaCacheLine,i_Row,pCol,pDatum); break;
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* encode given datum into cache line                                         */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncode(RowNub* pRowNub, Column* pColumn, Datum* pDatum)
{
  i8 i_Column = pColumn->iColumnNo();
  
  //! don't encode columns 16+                                             (fix)
  if (i_Column >= 16)
    return;
  
  eDM eDatum = pColumn->eDatumType();
  switch (eDatum) {
    case DM_BCD: vEncodeBcd(pRowNub,pColumn,pDatum); return;
    case DM_BOL: vEncodeBol(pRowNub,pColumn,pDatum); return;
    case DM_DTE: vEncodeDte(pRowNub,pColumn,pDatum); return;
    case DM_FLT: vEncodeFlt(pRowNub,pColumn,pDatum); return;
    case DM_INT: vEncodeInt(pRowNub,pColumn,pDatum); return;
    case DM_IVL: vEncodeIvl(pRowNub,pColumn,pDatum); return;
    case DM_TME: vEncodeTme(pRowNub,pColumn,pDatum); return;
    case DM_TSP: vEncodeTsp(pRowNub,pColumn,pDatum); return;
    case DM_VCH: vEncodeVch(pRowNub,pColumn,pDatum); return;
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* compute how many cache lines an encoded row occupies                       */
/*----------------------------------------------------------------------------*/
i8 RowCoder::nLines(CacheLine xaCacheLine[], i8 i_Row, ColumnVec* pColVec)
{
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
    
    eMN eEncoding = eGetEncoding(i_Column,&xaCacheLine[0],i_Offset);
    
    if (MN_SHOCO_2B == eEncoding)
      nCacheLines++;
  }
  
  return nCacheLines;
}

/*----------------------------------------------------------------------------*/
/* print decoded datum from cache line                                        */
/*----------------------------------------------------------------------------*/
void RowCoder::vPrint(CacheLine xaCacheLine[], i8 i_Row, Column* pCol)
{
  Datum oDatum;
  Record* pRec = new Record;
  
  vDecode(xaCacheLine,i_Row,pCol,&oDatum);
  
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
  
  vTrace("Column[%lld %s %s] : %s\n"_Stz,
         pCol->iColumnNo(),
         pCol->sColumnName().aStz(),
         aDM[pCol->eDatumType()],
         pRec->szRecStr());
  
  delete pRec;
}

/*----------------------------------------------------------------------------*/
/* sprint decode row from cache line into string                              */
/*----------------------------------------------------------------------------*/
void RowCoder::vRecord(Record* pRec,
                       CacheLine xaCacheLine[], i8 i_Row, ColumnVec* pColVec)
{
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
    RowCoder::vAppend(pRec,xaCacheLine,i_Row,pCol);
  }
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* decode and append column datum from cache line                             */
/*----------------------------------------------------------------------------*/
void RowCoder::vAppend(Record* pRec, CacheLine xaCacheLine[], i8 i_Row, Column* pCol)
{
  Datum oDatum;
  
  CacheLine* aCacheLine = &xaCacheLine[0];
  
  RowCoder::vDecode(aCacheLine,i_Row,pCol,&oDatum);
  
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
        case MN_SHOCO_CL:    NEVER_REACH; return 0;
        case MN_SHOCO_2B:    NEVER_REACH; return 0;
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
void RowCoder::vDecodeBcd(CacheLine xaCacheLine[], i8 i_Row,
                          Column* pCol, Datum* pDatum)
{
  CacheLine* aCacheLine = &xaCacheLine[i_Row];
  
  i8 i_Offset  = C_UNKNOWN;
  i8 i_Column  = pCol->iColumnNo();
  
  eMN eEncoding = eGetEncoding(i_Column,aCacheLine,i_Offset);
  switch (eEncoding) {
      
    case MN_NULL: pDatum->m_iInt = C_DATUM_NULL;                break;
    case MN_1B:   pDatum->m_iInt = aCacheLine->w8_1B(i_Offset); break;
    case MN_2B:   pDatum->m_iInt = aCacheLine->w8_2B(i_Offset); break;
    case MN_3B:   pDatum->m_iInt = aCacheLine->w8_3B(i_Offset); break;
    case MN_4B:   pDatum->m_iInt = aCacheLine->w8_4B(i_Offset); break;
    case MN_5B:   pDatum->m_iInt = aCacheLine->w8_5B(i_Offset); break;
    case MN_6B:   pDatum->m_iInt = aCacheLine->w8_6B(i_Offset); break;
    case MN_7B:   pDatum->m_iInt = aCacheLine->w8_7B(i_Offset); break;
    case MN_8B:   pDatum->m_iInt = aCacheLine->w8_8B(i_Offset); break;
      
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
void RowCoder::vDecodeBol(CacheLine xaCacheLine[], i8 i_Row,
                          Column* pCol, Datum* pDatum)
{
  CacheLine* aCacheLine = &xaCacheLine[i_Row];
  
  i8 i_Offset  = C_UNKNOWN;
  i8 i_Column  = pCol->iColumnNo();
  
  eMN eEncoding = eGetEncoding(i_Column,aCacheLine,i_Offset);
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
void RowCoder::vDecodeDte(CacheLine xaCacheLine[], i8 i_Row,
                          Column* pCol, Datum* pDatum)
{
  CacheLine* aCacheLine = &xaCacheLine[i_Row];
  
  i8 i_Offset  = C_UNKNOWN;
  i8 i_Column  = pCol->iColumnNo();
  
  eMN eEncoding = eGetEncoding(i_Column,aCacheLine,i_Offset);
  switch (eEncoding) {
      
    case MN_NULL: pDatum->m_iInt = C_DATUM_NULL;                break;
    case MN_1B:   pDatum->m_iInt = aCacheLine->w8_1B(i_Offset); break;
    case MN_2B:   pDatum->m_iInt = aCacheLine->w8_2B(i_Offset); break;
    case MN_3B:   pDatum->m_iInt = aCacheLine->w8_3B(i_Offset); break;
    case MN_4B:   pDatum->m_iInt = aCacheLine->w8_4B(i_Offset); break;
    case MN_5B:   pDatum->m_iInt = aCacheLine->w8_5B(i_Offset); break;
    case MN_6B:   pDatum->m_iInt = aCacheLine->w8_6B(i_Offset); break;
    case MN_7B:   pDatum->m_iInt = aCacheLine->w8_7B(i_Offset); break;
    case MN_8B:   pDatum->m_iInt = aCacheLine->w8_8B(i_Offset); break;
      
    default:
      NEVER_REACH;
  }
  
  vRestoreDte(pDatum,eEncoding);
}

/*----------------------------------------------------------------------------*/
/* decode float from given cache line                                         */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeFlt(CacheLine xaCacheLine[], i8 i_Row,
                          Column* pCol, Datum* pDatum)
{
  CacheLine* aCacheLine = &xaCacheLine[i_Row];
  
  i8 i_Offset  = C_UNKNOWN;
  i8 i_Column  = pCol->iColumnNo();
  
  eMN eEncoding = eGetEncoding(i_Column,aCacheLine,i_Offset);
  switch (eEncoding) {
      
    case MN_NULL: pDatum->m_iInt = C_DATUM_NULL;                break;
    case MN_1B:   pDatum->m_iInt = aCacheLine->w8_1B(i_Offset); break;
    case MN_2B:   pDatum->m_iInt = aCacheLine->w8_2B(i_Offset); break;
    case MN_3B:   pDatum->m_iInt = aCacheLine->w8_3B(i_Offset); break;
    case MN_4B:   pDatum->m_iInt = aCacheLine->w8_4B(i_Offset); break;
    case MN_5B:   pDatum->m_iInt = aCacheLine->w8_5B(i_Offset); break;
    case MN_6B:   pDatum->m_iInt = aCacheLine->w8_6B(i_Offset); break;
    case MN_7B:   pDatum->m_iInt = aCacheLine->w8_7B(i_Offset); break;
    case MN_8B:   pDatum->m_iInt = aCacheLine->w8_8B(i_Offset); break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* decode integer from given cache line                                       */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeInt(CacheLine xaCacheLine[], i8 i_Row,
                          Column* pCol, Datum* pDatum)
{
  CacheLine* aCacheLine = &xaCacheLine[i_Row];
  
  i8 i_Offset  = C_UNKNOWN;
  i8 i_Column  = pCol->iColumnNo();
  
  eMN eEncoding = eGetEncoding(i_Column,aCacheLine,i_Offset);
  switch (eEncoding) {
      
    case MN_NULL: pDatum->m_iInt = C_DATUM_NULL;                break;
    case MN_1B:   pDatum->m_iInt = aCacheLine->w8_1B(i_Offset); break;
    case MN_2B:   pDatum->m_iInt = aCacheLine->w8_2B(i_Offset); break;
    case MN_3B:   pDatum->m_iInt = aCacheLine->w8_3B(i_Offset); break;
    case MN_4B:   pDatum->m_iInt = aCacheLine->w8_4B(i_Offset); break;
    case MN_5B:   pDatum->m_iInt = aCacheLine->w8_5B(i_Offset); break;
    case MN_6B:   pDatum->m_iInt = aCacheLine->w8_6B(i_Offset); break;
    case MN_7B:   pDatum->m_iInt = aCacheLine->w8_7B(i_Offset); break;
    case MN_8B:   pDatum->m_iInt = aCacheLine->w8_8B(i_Offset); break;
      
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
void RowCoder::vDecodeIvl(CacheLine xaCacheLine[], i8 i_Row,
                          Column* pCol, Datum* pDatum)
{
  CacheLine* aCacheLine = &xaCacheLine[i_Row];
  
  i8 i_Offset  = C_UNKNOWN;
  i8 i_Column  = pCol->iColumnNo();
  
  eMN eEncoding = eGetEncoding(i_Column,aCacheLine,i_Offset);
  switch (eEncoding) {
      
    case MN_NULL: pDatum->m_iInt = C_DATUM_NULL;                break;
    case MN_1B:   pDatum->m_iInt = aCacheLine->w8_1B(i_Offset); break;
    case MN_2B:   pDatum->m_iInt = aCacheLine->w8_2B(i_Offset); break;
    case MN_3B:   pDatum->m_iInt = aCacheLine->w8_3B(i_Offset); break;
    case MN_4B:   pDatum->m_iInt = aCacheLine->w8_4B(i_Offset); break;
    case MN_5B:   pDatum->m_iInt = aCacheLine->w8_5B(i_Offset); break;
    case MN_6B:   pDatum->m_iInt = aCacheLine->w8_6B(i_Offset); break;
    case MN_7B:   pDatum->m_iInt = aCacheLine->w8_7B(i_Offset); break;
    case MN_8B:   pDatum->m_iInt = aCacheLine->w8_8B(i_Offset); break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* decode time from given cache line                                          */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeTme(CacheLine xaCacheLine[], i8 i_Row,
                          Column* pCol, Datum* pDatum)
{
  CacheLine* aCacheLine = &xaCacheLine[i_Row];
  
  i8 i_Offset  = C_UNKNOWN;
  i8 i_Column  = pCol->iColumnNo();
  
  eMN eEncoding = eGetEncoding(i_Column,aCacheLine,i_Offset);
  switch (eEncoding) {
      
    case MN_NULL: pDatum->m_iInt = C_DATUM_NULL;                break;
    case MN_1B:   pDatum->m_iInt = aCacheLine->w8_1B(i_Offset); break;
    case MN_2B:   pDatum->m_iInt = aCacheLine->w8_2B(i_Offset); break;
    case MN_3B:   pDatum->m_iInt = aCacheLine->w8_3B(i_Offset); break;
    case MN_4B:   pDatum->m_iInt = aCacheLine->w8_4B(i_Offset); break;
    case MN_5B:   pDatum->m_iInt = aCacheLine->w8_5B(i_Offset); break;
    case MN_6B:   pDatum->m_iInt = aCacheLine->w8_6B(i_Offset); break;
    case MN_7B:   pDatum->m_iInt = aCacheLine->w8_7B(i_Offset); break;
    case MN_8B:   pDatum->m_iInt = aCacheLine->w8_8B(i_Offset); break;
      
    default:
      NEVER_REACH;
  }
  
  vRestoreTme(pDatum,eEncoding);
}

/*----------------------------------------------------------------------------*/
/* decode timestamp from given cache line                                     */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeTsp(CacheLine xaCacheLine[], i8 i_Row,
                          Column* pCol, Datum* pDatum)
{
  CacheLine* aCacheLine = &xaCacheLine[i_Row];
  
  i8 i_Offset  = C_UNKNOWN;
  i8 i_Column  = pCol->iColumnNo();
  
  eMN eEncoding = eGetEncoding(i_Column,aCacheLine,i_Offset);
  switch (eEncoding) {
      
    case MN_NULL: pDatum->m_iInt = C_DATUM_NULL;                break;
    case MN_1B:   pDatum->m_iInt = aCacheLine->w8_1B(i_Offset); break;
    case MN_2B:   pDatum->m_iInt = aCacheLine->w8_2B(i_Offset); break;
    case MN_3B:   pDatum->m_iInt = aCacheLine->w8_3B(i_Offset); break;
    case MN_4B:   pDatum->m_iInt = aCacheLine->w8_4B(i_Offset); break;
    case MN_5B:   pDatum->m_iInt = aCacheLine->w8_5B(i_Offset); break;
    case MN_6B:   pDatum->m_iInt = aCacheLine->w8_6B(i_Offset); break;
    case MN_7B:   pDatum->m_iInt = aCacheLine->w8_7B(i_Offset); break;
    case MN_8B:   pDatum->m_iInt = aCacheLine->w8_8B(i_Offset); break;
      
    default:
      NEVER_REACH;
  }
  
  if (eEncoding == MN_5B)
    vRestoreTsp(pDatum,eEncoding);
}

/*----------------------------------------------------------------------------*/
/* decode varchar from given cache line                                       */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeVch(CacheLine xaCacheLine[], i8 i_Row,
                          Column* pCol, Datum* pDatum)
{
  CacheLine* aCacheLine = &xaCacheLine[i_Row];
  
  i8 i_Offset  = C_UNKNOWN;
  i8 i_Column  = pCol->iColumnNo();
  
  //! needs more work
  eMN eEncoding = eGetEncoding(i_Column,aCacheLine,i_Offset);
  switch (eEncoding) {
      
    case MN_NULL: pDatum->m_iInt = C_DATUM_NULL;                break;
    case MN_1B:   pDatum->m_iInt = aCacheLine->w8_1B(i_Offset); break;
    case MN_2B:   pDatum->m_iInt = aCacheLine->w8_2B(i_Offset); break;
    case MN_3B:   pDatum->m_iInt = aCacheLine->w8_3B(i_Offset); break;
    case MN_4B:   pDatum->m_iInt = aCacheLine->w8_4B(i_Offset); break;
    case MN_5B:   pDatum->m_iInt = aCacheLine->w8_5B(i_Offset); break;
    case MN_6B:   pDatum->m_iInt = aCacheLine->w8_6B(i_Offset); break;
    case MN_7B:   pDatum->m_iInt = aCacheLine->w8_7B(i_Offset); break;
    case MN_8B:   pDatum->m_iInt = aCacheLine->w8_8B(i_Offset); break;
      
    case MN_SHOCO_2B: {
      i8 i_Shoco = aCacheLine->w8_2B(i_Offset);
      CacheLine* aShocoLine = &xaCacheLine[i_Shoco];
      
      i8 iLenCompressed = aShocoLine->w8_1N(0);
      
      s256 sDecomp;
      i8 iLenDecompressed =
      Compress::iFromShoco((char*)pDatum->m_xWsp.aStz(), 255,
                           ((byte*)aShocoLine)+8, iLenCompressed);
      sDecomp.aChar()[iLenDecompressed] = '\0';
      vTrace("Decomp: %s\n"_Stz,sDecomp.aChar());
      
      break;
    }
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* encode decimal                                                             */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeBcd(RowNub* pRowNub, Column* pColumn, Datum* pDatum)
{
  CacheLine* aCacheLine = pRowNub->aGet(0);
  
  i8 i_Column = pColumn->iColumnNo();
  w8 wOriginal = pDatum->m_iBcd;
  
  //! temp
  pDatum->m_iInt = wOriginal;
  
  // hash the column value if part of partition key
  if (pColumn->bIsPartitionKey()) {
    i8 iHash = HshLib::iHash(&pDatum->m_iInt,sizeof(w8));
    pRowNub->vHashInc(iHash);
  }
  
  i8 nBytesEncoded;
  eMN eEncoding = RowCoder::eEncodingBcd(pDatum,nBytesEncoded);
  RASSERT(nBytesEncoded == RowCoder::nDataBytes(eEncoding,DM_BCD));
  
  // set 4bit meta data for column value
  aCacheLine->v1N(i_Column,eEncoding);
  
  // set n-byte data for column value
  if (nBytesEncoded)
    aCacheLine->vPut(pRowNub->i_Offset(),pDatum->m_iInt,eEncoding);
  
  pRowNub->vOffsetInc(nBytesEncoded);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
  RowCoder::vDecode(aCacheLine,0,pColumn,pDatum);
  RASSERT(wOriginal == (w8)pDatum->m_iInt);
}

/*----------------------------------------------------------------------------*/
/* encode boolean                                                             */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeBol(RowNub* pRowNub, Column* pColumn, Datum* pDatum)
{
  CacheLine* aCacheLine = pRowNub->aGet(0);

  i8 i_Column = pColumn->iColumnNo();
  w8 wOriginal = pDatum->m_iBol;
  
  //! temp
  pDatum->m_iInt = wOriginal;
  
  // hash the column value if part of partition key
  if (pColumn->bIsPartitionKey()) {
    i8 iHash = HshLib::iHash(&pDatum->m_iInt,sizeof(w8));
    pRowNub->vHashInc(iHash);
  }
  
  i8 nBytesEncoded;
  eMN eEncoding = RowCoder::eEncodingBol(pDatum,nBytesEncoded);
  RASSERT(nBytesEncoded == RowCoder::nDataBytes(eEncoding,DM_BOL));
  
  // set 4bit meta data for column value
  aCacheLine->v1N(i_Column,eEncoding);
  
  pRowNub->vOffsetInc(nBytesEncoded);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
  RowCoder::vDecode(aCacheLine,0,pColumn,pDatum);
  RASSERT(wOriginal == (w8)pDatum->m_iInt);
}

/*----------------------------------------------------------------------------*/
/* encode date                                                                */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeDte(RowNub* pRowNub, Column* pColumn, Datum* pDatum)
{
  CacheLine* aCacheLine = pRowNub->aGet(0);

  i8 i_Column = pColumn->iColumnNo();
  w8 wOriginal = pDatum->m_iDte;
  
  //! temp
  pDatum->m_iInt = wOriginal;
  
  // hash the column value if part of partition key
  if (pColumn->bIsPartitionKey()) {
    i8 iHash = HshLib::iHash(&pDatum->m_iInt,sizeof(w8));
    pRowNub->vHashInc(iHash);
  }
  
  i8 nBytesEncoded;
  eMN eEncoding = RowCoder::eEncodingDte(pDatum,nBytesEncoded);
  RASSERT(nBytesEncoded == RowCoder::nDataBytes(eEncoding,DM_DTE));
  
  // set 4bit meta data for column value
  aCacheLine->v1N(i_Column,eEncoding);
  
  // set n-byte data for column value
  if (nBytesEncoded)
    aCacheLine->vPut(pRowNub->i_Offset(),pDatum->m_iInt,eEncoding);
  
  pRowNub->vOffsetInc(nBytesEncoded);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
  RowCoder::vDecode(aCacheLine,0,pColumn,pDatum);
  RASSERT(wOriginal == (w8)pDatum->m_iInt);
}

/*----------------------------------------------------------------------------*/
/* encode float                                                               */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeFlt(RowNub* pRowNub, Column* pColumn, Datum* pDatum)
{
  CacheLine* aCacheLine = pRowNub->aGet(0);

  i8 i_Column = pColumn->iColumnNo();
  w8 wOriginal = (w8)pDatum->m_wWord;
  
  //! temp
  pDatum->m_iInt = wOriginal;
  
  // hash the column value if part of partition key
  if (pColumn->bIsPartitionKey()) {
    i8 iHash = HshLib::iHash(&pDatum->m_iInt,sizeof(w8));
    pRowNub->vHashInc(iHash);
  }
  
  i8 nBytesEncoded;
  eMN eEncoding = RowCoder::eEncodingFlt(pDatum,nBytesEncoded);
  RASSERT(nBytesEncoded == RowCoder::nDataBytes(eEncoding,DM_FLT));
  
  // set 4bit meta data for column value
  aCacheLine->v1N(i_Column,eEncoding);
  
  // set n-byte data for column value
  if (nBytesEncoded)
    aCacheLine->vPut(pRowNub->i_Offset(),pDatum->m_iInt,eEncoding);
  
  pRowNub->vOffsetInc(nBytesEncoded);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
  RowCoder::vDecode(aCacheLine,0,pColumn,pDatum);
  RASSERT(wOriginal == (w8)pDatum->m_iInt);
}

/*----------------------------------------------------------------------------*/
/* encode integer                                                             */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeInt(RowNub* pRowNub, Column* pColumn, Datum* pDatum)
{
  CacheLine* aCacheLine = pRowNub->aGet(0);

  i8 i_Column = pColumn->iColumnNo();
  w8 wOriginal = pDatum->m_iInt;
  
  //! temp
  pDatum->m_iInt = wOriginal;
  
  // hash the column value if part of partition key
  if (pColumn->bIsPartitionKey()) {
    i8 iHash = HshLib::iHash(&pDatum->m_iInt,sizeof(w8));
    pRowNub->vHashInc(iHash);
  }
  
  i8 nBytesEncoded;
  eMN eEncoding = RowCoder::eEncodingInt(pDatum,nBytesEncoded);
  RASSERT(nBytesEncoded == RowCoder::nDataBytes(eEncoding,DM_INT));
  
  // set 4bit meta data for column value
  aCacheLine->v1N(i_Column,eEncoding);
  
  // set n-byte data for column value
  if (nBytesEncoded)
    aCacheLine->vPut(pRowNub->i_Offset(),pDatum->m_iInt,eEncoding);
  
  pRowNub->vOffsetInc(nBytesEncoded);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
  RowCoder::vDecode(aCacheLine,0,pColumn,pDatum);
  RASSERT(wOriginal == pDatum->m_iInt);
}

/*----------------------------------------------------------------------------*/
/* encode interval                                                            */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeIvl(RowNub* pRowNub, Column* pColumn, Datum* pDatum)
{
  CacheLine* aCacheLine = pRowNub->aGet(0);

  i8 i_Column = pColumn->iColumnNo();
  w8 wOriginal = pDatum->m_iIvl;
  
  //! temp
  pDatum->m_iInt = wOriginal;
  
  // hash the column value if part of partition key
  if (pColumn->bIsPartitionKey()) {
    i8 iHash = HshLib::iHash(&pDatum->m_iInt,sizeof(w8));
    pRowNub->vHashInc(iHash);
  }
  
  i8 nBytesEncoded;
  eMN eEncoding = RowCoder::eEncodingIvl(pDatum,nBytesEncoded);
  RASSERT(nBytesEncoded == RowCoder::nDataBytes(eEncoding,DM_IVL));
  
  // set 4bit meta data for column value
  aCacheLine->v1N(i_Column,eEncoding);
  
  // set n-byte data for column value
  if (nBytesEncoded)
    aCacheLine->vPut(pRowNub->i_Offset(),pDatum->m_iInt,eEncoding);
  
  pRowNub->vOffsetInc(nBytesEncoded);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
  RowCoder::vDecode(aCacheLine,0,pColumn,pDatum);
  RASSERT(wOriginal == (w8)pDatum->m_iInt);
}

/*----------------------------------------------------------------------------*/
/* encode time                                                                */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeTme(RowNub* pRowNub, Column* pColumn, Datum* pDatum)
{
  CacheLine* aCacheLine = pRowNub->aGet(0);

  i8 i_Column = pColumn->iColumnNo();
  w8 wOriginal = pDatum->m_iTme;
  
  //! temp
  pDatum->m_iInt = wOriginal;
  
  // hash the column value if part of partition key
  if (pColumn->bIsPartitionKey()) {
    i8 iHash = HshLib::iHash(&pDatum->m_iInt,sizeof(w8));
    pRowNub->vHashInc(iHash);
  }
  
  i8 nBytesEncoded;
  eMN eEncoding = RowCoder::eEncodingTme(pDatum,nBytesEncoded);
  RASSERT(nBytesEncoded == RowCoder::nDataBytes(eEncoding,DM_TME));
  
  // set 4bit meta data for column value
  aCacheLine->v1N(i_Column,eEncoding);
  
  // set n-byte data for column value
  if (nBytesEncoded)
    aCacheLine->vPut(pRowNub->i_Offset(),pDatum->m_iInt,eEncoding);
  
  pRowNub->vOffsetInc(nBytesEncoded);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
  RowCoder::vDecode(aCacheLine,0,pColumn,pDatum);
  RASSERT(wOriginal == (w8)pDatum->m_iInt);
}

/*----------------------------------------------------------------------------*/
/* encode timestamp                                                           */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeTsp(RowNub* pRowNub, Column* pColumn, Datum* pDatum)
{
  CacheLine* aCacheLine = pRowNub->aGet(0);

  i8 i_Column = pColumn->iColumnNo();
  w8 wOriginal = pDatum->m_iTsp;
  
  //! temp
  pDatum->m_iInt = wOriginal;
  
  // hash the column value if part of partition key
  if (pColumn->bIsPartitionKey()) {
    i8 iHash = HshLib::iHash(&pDatum->m_iInt,sizeof(w8));
    pRowNub->vHashInc(iHash);
  }
  
  i8 nBytesEncoded;
  eMN eEncoding = RowCoder::eEncodingTsp(pDatum,nBytesEncoded);
  RASSERT(nBytesEncoded == RowCoder::nDataBytes(eEncoding,DM_TSP));
  
  // set 4bit meta data for column value
  aCacheLine->v1N(i_Column,eEncoding);
  
  // set n-byte data for column value
  if (nBytesEncoded)
    aCacheLine->vPut(pRowNub->i_Offset(),pDatum->m_iInt,eEncoding);
  
  pRowNub->vOffsetInc(nBytesEncoded);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
  RowCoder::vDecode(aCacheLine,0,pColumn,pDatum);
  RASSERT(wOriginal == (w8)pDatum->m_iInt);
}

/*----------------------------------------------------------------------------*/
/* encode varchar                                                             */
/*----------------------------------------------------------------------------*/
void RowCoder::vEncodeVch(RowNub* pRowNub, Column* pColumn, Datum* pDatum)
{
  CacheLine* aCacheLine = pRowNub->aGet(0);

  i8 i_Column = pColumn->iColumnNo();
  w8 wOriginal = (w8)pDatum->m_xWsp.aArea();
    
  // hash the column value if part of partition key
  if (pColumn->bIsPartitionKey()) {
    i8 iHash = HshLib::iHash(pDatum->m_xWsp.aArea(),sizeof(w8));
    pRowNub->vHashInc(iHash);
  }
  
  i8 nBytesEncoded;
  eMN eEncoding = RowCoder::eEncodingVch(pDatum,nBytesEncoded);
  //RASSERT(nBytesEncoded == nEncodedBytes(eEncoding,DM_TSP));
  
  // set 4bit meta data for column value
  aCacheLine->v1N(i_Column,eEncoding);
  
  // set n-byte data for column value
  if (eEncoding == MN_SHOCO_2B) {
    // we would encode the shoco version of the string into a cache line
    
    // allot a tail cache line
    i8 i_Shoco = pRowNub->iAllotTail();
    RASSERT(255 == i_Shoco);
    CacheLine* aShocoLine = pRowNub->aGet(i_Shoco);
    
    // set data for column value as the index of the shoco cache line
    aCacheLine->vPut(pRowNub->i_Offset(),i_Shoco,MN_2B);
    
    // compress varchar into tail shoco cache line
    i8 iLenCompressed =
    Compress::iIntoShoco(((byte*)aShocoLine)+8, C_BYTES_PER_CACHE_LINE-8,
                         (char*)pDatum->m_xWsp.aArea(), nBytesEncoded);
    
    // set length of the compressed string encoded into tail shoco cache line
    aShocoLine->v1N(0,iLenCompressed);
    
    //MemLib::vWordDump(aShocoLine,sizeof(CacheLine));
    pRowNub->vOffsetInc(2);

    //    s256 sDecomp;
    //    i8 iLenDecompressed =
    //      Compress::iFromShoco(sDecomp.aChar(), 255,
    //                           ((byte*)aShocoLine)+8, C_BYTES_PER_CACHE_LINE-8);
    //    sDecomp.aChar()[iLenDecompressed] = '\0';
    //    vTrace("Decomp: %s\n"_Stz,sDecomp.aChar());
  }
  else {
    aCacheLine->vPut(pRowNub->i_Offset(),(w8)pDatum->m_xWsp.aArea(),nBytesEncoded);
    pRowNub->vOffsetInc(nBytesEncoded);
  }
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
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
/* encoding of vch                                                            */
/*----------------------------------------------------------------------------*/
eMN RowCoder::eEncodingVch(Datum* pDatum, i8& nBytesEncoded)
{
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
/* get the encoding of of a column from given cache line                      */
/*----------------------------------------------------------------------------*/
eMN RowCoder::eGetEncoding(i8 i_Column, CacheLine* aCacheLine, i8& i_Offset)
{
  i_Offset = 8;
  
  for (i8 i_Nibble = 0; i_Nibble < i_Column; i_Nibble++) {
    i8 nDataBytes = aCacheLine->w8_1N(i_Nibble);
    if (nDataBytes <= MN_8B)
      i_Offset += nDataBytes;
  }
  
  eMN eEncoding = (eMN)aCacheLine->w8_1N(i_Column);
  
  return eEncoding;
}

/*----------------------------------------------------------------------------*/
/* initialise given cache line                                                */
/*----------------------------------------------------------------------------*/
void RowCoder::vInitCacheLine(CacheLine* aCacheLine)
{
  // set all bytes in the entire cache line to xFF
  aCacheLine->vSetAll(0xFF);
  
  // set all bits in first word of cache line to zero
  aCacheLine->v8B(0,0);
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
