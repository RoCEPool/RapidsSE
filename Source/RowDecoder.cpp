/*============================================================================*/
/* RowDecoder.cpp -- Row Decoder                                              */
/*============================================================================*/

#include "Data.h"

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* RowDecoder::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_ROWDECODER);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void RowDecoder::operator delete(void* pAddr)
{
  RowDecoder* pRowDecoder = reinterpret_cast<RowDecoder*>(pAddr);
  
  HeapMem::vFree(pRowDecoder);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowDecoder::RowDecoder(RowTray* pRowTray)
: Object(CN_ROWDECODER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWDECODER,_FOOTER_);
  
  m_pRowTray = pRowTray;
  
  _checkInitialised_(CN_ROWDECODER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWDECODER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RowDecoder::~RowDecoder(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWDECODER,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get decoded datum from cache line                                          */
/*----------------------------------------------------------------------------*/
void RowDecoder::vDecode(i8 i_Row, Column* pCol, w8& wDatum, Stz* szDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWDECODER,_FOOTER_);
  
  eDM eDatum = pCol->eDatumType();
  switch (eDatum) {
    case DM_BCD: vDecodeBcd(i_Row,pCol,wDatum,szDatum); break;
    case DM_BOL: vDecodeBol(i_Row,pCol,wDatum,szDatum); break;
    case DM_DTE: vDecodeDte(i_Row,pCol,wDatum,szDatum); break;
    case DM_FLT: vDecodeFlt(i_Row,pCol,wDatum,szDatum); break;
    case DM_INT: vDecodeInt(i_Row,pCol,wDatum,szDatum); break;
    case DM_IVL: vDecodeIvl(i_Row,pCol,wDatum,szDatum); break;
    case DM_TME: vDecodeTme(i_Row,pCol,wDatum,szDatum); break;
    case DM_TSP: vDecodeTsp(i_Row,pCol,wDatum,szDatum); break;
    case DM_VCH: vDecodeVch(i_Row,pCol,wDatum,szDatum); break;
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* print decoded datum from cache line                                        */
/*----------------------------------------------------------------------------*/
void RowDecoder::vPrint(i8 i_Row, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWDECODER,_FOOTER_);
  
  w8 wDatum;
  s256 sDatum;
  
  Record* pRecord = new Record;
  
  vDecode(i_Row,pCol,wDatum,sDatum);
  
  eDM eDatum = pCol->eDatumType();
  switch (eDatum) {
    case DM_BCD: vAppendBcd(pRecord,pCol,wDatum,sDatum); break;
    case DM_BOL: vAppendBol(pRecord,pCol,wDatum,sDatum); break;
    case DM_DTE: vAppendDte(pRecord,pCol,wDatum,sDatum); break;
    case DM_FLT: vAppendFlt(pRecord,pCol,wDatum,sDatum); break;
    case DM_INT: vAppendInt(pRecord,pCol,wDatum,sDatum); break;
    case DM_IVL: vAppendIvl(pRecord,pCol,wDatum,sDatum); break;
    case DM_TME: vAppendTme(pRecord,pCol,wDatum,sDatum); break;
    case DM_TSP: vAppendTsp(pRecord,pCol,wDatum,sDatum); break;
    case DM_VCH: vAppendVch(pRecord,pCol,wDatum,sDatum); break;
    default:
      NEVER_REACH;
  }

  vTrace("Column[%lld %s %s] : %s\n"_Stz,
         pCol->iColumnNo(),
         pCol->sColumnName().aStz(),
         aDM[pCol->eDatumType()],
         pRecord->szRecStr());
  
  delete pRecord;
}

/*----------------------------------------------------------------------------*/
/* sprint decode row from cache line into string                              */
/*----------------------------------------------------------------------------*/
void RowDecoder::vRecord(Record* pRec, i8 i_Row, ColumnVec* pColVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWDECODER,_FOOTER_);
  
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
    vAppend(pRec,i_Row,pCol);
  }
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* decode and append column datum from cache line                             */
/*----------------------------------------------------------------------------*/
void RowDecoder::vAppend(Record* pRec, i8 i_Row, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWDECODER,_FOOTER_);
  
  w8 wDatum;
  s256 sDatum;
  
  vDecode(i_Row,pCol,wDatum,sDatum);
  
  eDM eDatum = pCol->eDatumType();
  switch (eDatum) {
    case DM_BCD: vAppendBcd(pRec,pCol,wDatum,sDatum); break;
    case DM_BOL: vAppendBol(pRec,pCol,wDatum,sDatum); break;
    case DM_DTE: vAppendDte(pRec,pCol,wDatum,sDatum); break;
    case DM_FLT: vAppendFlt(pRec,pCol,wDatum,sDatum); break;
    case DM_INT: vAppendInt(pRec,pCol,wDatum,sDatum); break;
    case DM_IVL: vAppendIvl(pRec,pCol,wDatum,sDatum); break;
    case DM_TME: vAppendTme(pRec,pCol,wDatum,sDatum); break;
    case DM_TSP: vAppendTsp(pRec,pCol,wDatum,sDatum); break;
    case DM_VCH: vAppendVch(pRec,pCol,wDatum,sDatum); break;
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* append decimal datum to string                                             */
/*----------------------------------------------------------------------------*/
void RowDecoder::vAppendBcd(Record* pRec, Column* pCol, w8 wDatum, Stz* szDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWDECODER,_FOOTER_);
  
  pRec->szRecStr()->vCatSprint("%lld"_Stz,(i8)wDatum);
}

/*----------------------------------------------------------------------------*/
/* append decimal datum to string                                             */
/*----------------------------------------------------------------------------*/
void RowDecoder::vAppendBol(Record* pRec, Column* pCol, w8 wDatum, Stz* szDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWDECODER,_FOOTER_);
  
  pRec->szRecStr()->vConcat(wDatum ? "true"_Stz : "false"_Stz);
}

/*----------------------------------------------------------------------------*/
/* append decimal datum to string                                             */
/*----------------------------------------------------------------------------*/
void RowDecoder::vAppendDte(Record* pRec, Column* pCol, w8 wDatum, Stz* szDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWDECODER,_FOOTER_);
  
  Dte wDte;
  wDte.vAsI8(wDatum);
  
  s32 sDte;
  wDte.vSprint(sDte.aStz());
  
  pRec->szRecStr()->vConcat(sDte.aStz());
}

/*----------------------------------------------------------------------------*/
/* append decimal datum to string                                             */
/*----------------------------------------------------------------------------*/
void RowDecoder::vAppendFlt(Record* pRec, Column* pCol, w8 wDatum, Stz* szDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWDECODER,_FOOTER_);
  
  pRec->szRecStr()->vCatSprint("%g"_Stz,(d8)wDatum);
}

/*----------------------------------------------------------------------------*/
/* append decimal datum to string                                             */
/*----------------------------------------------------------------------------*/
void RowDecoder::vAppendInt(Record* pRec, Column* pCol, w8 wDatum, Stz* szDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWDECODER,_FOOTER_);
  
  pRec->szRecStr()->vCatSprint("%lld"_Stz,(i8)wDatum);
}

/*----------------------------------------------------------------------------*/
/* append decimal datum to string                                             */
/*----------------------------------------------------------------------------*/
void RowDecoder::vAppendIvl(Record* pRec, Column* pCol, w8 wDatum, Stz* szDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWDECODER,_FOOTER_);
  
  pRec->szRecStr()->vCatSprint("%lld"_Stz,(i8)wDatum);
}

/*----------------------------------------------------------------------------*/
/* append decimal datum to string                                             */
/*----------------------------------------------------------------------------*/
void RowDecoder::vAppendTme(Record* pRec, Column* pCol, w8 wDatum, Stz* szDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWDECODER,_FOOTER_);
  
  Tme wTme;
  wTme.vAsI8((i8)wDatum);
  
  s32 sTme;
  wTme.vSprint(sTme.aStz());
  
  pRec->szRecStr()->vConcat(sTme.aStz());
}

/*----------------------------------------------------------------------------*/
/* append decimal datum to string                                             */
/*----------------------------------------------------------------------------*/
void RowDecoder::vAppendTsp(Record* pRec, Column* pCol, w8 wDatum, Stz* szDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWDECODER,_FOOTER_);
  
  Tsp wTsp;
  wTsp.vAsI8((i8)wDatum);
  
  s32 sTsp;
  wTsp.vSprint(sTsp.aStz());
  
  pRec->szRecStr()->vConcat(sTsp.aStz());
}

/*----------------------------------------------------------------------------*/
/* append decimal datum to string                                             */
/*----------------------------------------------------------------------------*/
void RowDecoder::vAppendVch(Record* pRec, Column* pCol, w8 wDatum, Stz* szDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWDECODER,_FOOTER_);
  
  s256 sBuf;
  
  Vch::vSprintQuotes(sBuf,szDatum);
  pRec->szRecStr()->vConcat(sBuf.aStz());
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* decode decimal from given cache line                                       */
/*----------------------------------------------------------------------------*/
void RowDecoder::vDecodeBcd(i8 i_Row, Column* pCol, w8& wDatum, Stz* szDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWDECODER,_FOOTER_);
  
  CacheLine* aCacheLine = m_pRowTray->aGet(0);
  
  i8 i_Offset  = C_UNKNOWN;
  i8 i_Column  = pCol->iColumnNo();
  
  i8 iEncoding = iGetEncoding(i_Column,aCacheLine,i_Offset);
  switch (iEncoding) {
      
    case MN_NULL: wDatum = C_DATUM_NULL;                break;
    case MN_1B:   wDatum = aCacheLine->w8_1B(i_Offset); break;
    case MN_2B:   wDatum = aCacheLine->w8_2B(i_Offset); break;
    case MN_3B:   wDatum = aCacheLine->w8_3B(i_Offset); break;
    case MN_4B:   wDatum = aCacheLine->w8_4B(i_Offset); break;
    case MN_5B:   wDatum = aCacheLine->w8_5B(i_Offset); break;
    case MN_6B:   wDatum = aCacheLine->w8_6B(i_Offset); break;
    case MN_7B:   wDatum = aCacheLine->w8_7B(i_Offset); break;
    case MN_8B:   wDatum = aCacheLine->w8_8B(i_Offset); break;
      
    case MN_MINUS_ONE: wDatum = (i8)-1; break;
    case MN_ZERO:      wDatum = (i8)0;  break;
    case MN_PLUS_ONE:  wDatum = (i8)1;  break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* decode boolean from given cache line                                       */
/*----------------------------------------------------------------------------*/
void RowDecoder::vDecodeBol(i8 i_Row, Column* pCol, w8& wDatum, Stz* szDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWDECODER,_FOOTER_);
  
  CacheLine* aCacheLine = m_pRowTray->aGet(i_Row);
  
  i8 i_Offset  = C_UNKNOWN;
  i8 i_Column  = pCol->iColumnNo();
  
  i8 iEncoding = iGetEncoding(i_Column,aCacheLine,i_Offset);
  switch (iEncoding) {
      
    case MN_NULL:  wDatum = C_DATUM_NULL; break;
    case MN_FALSE: wDatum = (i8)0;        break;
    case MN_TRUE:  wDatum = (i8)1;        break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* decode date from given cache line                                          */
/*----------------------------------------------------------------------------*/
void RowDecoder::vDecodeDte(i8 i_Row, Column* pCol, w8& wDatum, Stz* szDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWDECODER,_FOOTER_);
  
  CacheLine* aCacheLine = m_pRowTray->aGet(i_Row);
  
  i8 i_Offset  = C_UNKNOWN;
  i8 i_Column  = pCol->iColumnNo();
  
  i8 iEncoding = iGetEncoding(i_Column,aCacheLine,i_Offset);
  switch (iEncoding) {
      
    case MN_NULL: wDatum = C_DATUM_NULL;                break;
    case MN_1B:   wDatum = aCacheLine->w8_1B(i_Offset); break;
    case MN_2B:   wDatum = aCacheLine->w8_2B(i_Offset); break;
    case MN_3B:   wDatum = aCacheLine->w8_3B(i_Offset); break;
    case MN_4B:   wDatum = aCacheLine->w8_4B(i_Offset); break;
    case MN_5B:   wDatum = aCacheLine->w8_5B(i_Offset); break;
    case MN_6B:   wDatum = aCacheLine->w8_6B(i_Offset); break;
    case MN_7B:   wDatum = aCacheLine->w8_7B(i_Offset); break;
    case MN_8B:   wDatum = aCacheLine->w8_8B(i_Offset); break;
      
    default:
      NEVER_REACH;
  }
  
  wDatum = wRestoreDte(wDatum,iEncoding);
}

/*----------------------------------------------------------------------------*/
/* decode float from given cache line                                         */
/*----------------------------------------------------------------------------*/
void RowDecoder::vDecodeFlt(i8 i_Row, Column* pCol, w8& wDatum, Stz* szDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWDECODER,_FOOTER_);
  
  CacheLine* aCacheLine = m_pRowTray->aGet(i_Row);
  
  i8 i_Offset  = C_UNKNOWN;
  i8 i_Column  = pCol->iColumnNo();
  
  i8 iEncoding = iGetEncoding(i_Column,aCacheLine,i_Offset);
  switch (iEncoding) {
      
    case MN_NULL: wDatum = C_DATUM_NULL;                break;
    case MN_1B:   wDatum = aCacheLine->w8_1B(i_Offset); break;
    case MN_2B:   wDatum = aCacheLine->w8_2B(i_Offset); break;
    case MN_3B:   wDatum = aCacheLine->w8_3B(i_Offset); break;
    case MN_4B:   wDatum = aCacheLine->w8_4B(i_Offset); break;
    case MN_5B:   wDatum = aCacheLine->w8_5B(i_Offset); break;
    case MN_6B:   wDatum = aCacheLine->w8_6B(i_Offset); break;
    case MN_7B:   wDatum = aCacheLine->w8_7B(i_Offset); break;
    case MN_8B:   wDatum = aCacheLine->w8_8B(i_Offset); break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* decode integer from given cache line                                       */
/*----------------------------------------------------------------------------*/
void RowDecoder::vDecodeInt(i8 i_Row, Column* pCol, w8& wDatum, Stz* szDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWDECODER,_FOOTER_);
  
  CacheLine* aCacheLine = m_pRowTray->aGet(i_Row);
  
  i8 i_Offset  = C_UNKNOWN;
  i8 i_Column  = pCol->iColumnNo();
  
  i8 iEncoding = iGetEncoding(i_Column,aCacheLine,i_Offset);
  switch (iEncoding) {
      
    case MN_NULL: wDatum = C_DATUM_NULL;                break;
    case MN_1B:   wDatum = aCacheLine->w8_1B(i_Offset); break;
    case MN_2B:   wDatum = aCacheLine->w8_2B(i_Offset); break;
    case MN_3B:   wDatum = aCacheLine->w8_3B(i_Offset); break;
    case MN_4B:   wDatum = aCacheLine->w8_4B(i_Offset); break;
    case MN_5B:   wDatum = aCacheLine->w8_5B(i_Offset); break;
    case MN_6B:   wDatum = aCacheLine->w8_6B(i_Offset); break;
    case MN_7B:   wDatum = aCacheLine->w8_7B(i_Offset); break;
    case MN_8B:   wDatum = aCacheLine->w8_8B(i_Offset); break;
      
    case MN_MINUS_ONE: wDatum = (i8)-1; break;
    case MN_ZERO:      wDatum = (i8)0;  break;
    case MN_PLUS_ONE:  wDatum = (i8)1;  break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* decode interval from given cache line                                      */
/*----------------------------------------------------------------------------*/
void RowDecoder::vDecodeIvl(i8 i_Row, Column* pCol, w8& wDatum, Stz* szDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWDECODER,_FOOTER_);
  
  CacheLine* aCacheLine = m_pRowTray->aGet(i_Row);
  
  i8 i_Offset  = C_UNKNOWN;
  i8 i_Column  = pCol->iColumnNo();
  
  i8 iEncoding = iGetEncoding(i_Column,aCacheLine,i_Offset);
  switch (iEncoding) {
      
    case MN_NULL: wDatum = C_DATUM_NULL;                break;
    case MN_1B:   wDatum = aCacheLine->w8_1B(i_Offset); break;
    case MN_2B:   wDatum = aCacheLine->w8_2B(i_Offset); break;
    case MN_3B:   wDatum = aCacheLine->w8_3B(i_Offset); break;
    case MN_4B:   wDatum = aCacheLine->w8_4B(i_Offset); break;
    case MN_5B:   wDatum = aCacheLine->w8_5B(i_Offset); break;
    case MN_6B:   wDatum = aCacheLine->w8_6B(i_Offset); break;
    case MN_7B:   wDatum = aCacheLine->w8_7B(i_Offset); break;
    case MN_8B:   wDatum = aCacheLine->w8_8B(i_Offset); break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* decode time from given cache line                                          */
/*----------------------------------------------------------------------------*/
void RowDecoder::vDecodeTme(i8 i_Row, Column* pCol, w8& wDatum, Stz* szDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWDECODER,_FOOTER_);
  
  CacheLine* aCacheLine = m_pRowTray->aGet(i_Row);
  
  i8 i_Offset  = C_UNKNOWN;
  i8 i_Column  = pCol->iColumnNo();
  
  i8 iEncoding = iGetEncoding(i_Column,aCacheLine,i_Offset);
  switch (iEncoding) {
      
    case MN_NULL: wDatum = C_DATUM_NULL;                break;
    case MN_1B:   wDatum = aCacheLine->w8_1B(i_Offset); break;
    case MN_2B:   wDatum = aCacheLine->w8_2B(i_Offset); break;
    case MN_3B:   wDatum = aCacheLine->w8_3B(i_Offset); break;
    case MN_4B:   wDatum = aCacheLine->w8_4B(i_Offset); break;
    case MN_5B:   wDatum = aCacheLine->w8_5B(i_Offset); break;
    case MN_6B:   wDatum = aCacheLine->w8_6B(i_Offset); break;
    case MN_7B:   wDatum = aCacheLine->w8_7B(i_Offset); break;
    case MN_8B:   wDatum = aCacheLine->w8_8B(i_Offset); break;
      
    default:
      NEVER_REACH;
  }
  
  wDatum = wRestoreTme(wDatum,iEncoding);
}

/*----------------------------------------------------------------------------*/
/* decode timestamp from given cache line                                     */
/*----------------------------------------------------------------------------*/
void RowDecoder::vDecodeTsp(i8 i_Row, Column* pCol, w8& wDatum, Stz* szDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWDECODER,_FOOTER_);
  
  CacheLine* aCacheLine = m_pRowTray->aGet(i_Row);
  
  i8 i_Offset  = C_UNKNOWN;
  i8 i_Column  = pCol->iColumnNo();
  
  i8 iEncoding = iGetEncoding(i_Column,aCacheLine,i_Offset);
  switch (iEncoding) {
      
    case MN_NULL: wDatum = C_DATUM_NULL;                break;
    case MN_1B:   wDatum = aCacheLine->w8_1B(i_Offset); break;
    case MN_2B:   wDatum = aCacheLine->w8_2B(i_Offset); break;
    case MN_3B:   wDatum = aCacheLine->w8_3B(i_Offset); break;
    case MN_4B:   wDatum = aCacheLine->w8_4B(i_Offset); break;
    case MN_5B:   wDatum = aCacheLine->w8_5B(i_Offset); break;
    case MN_6B:   wDatum = aCacheLine->w8_6B(i_Offset); break;
    case MN_7B:   wDatum = aCacheLine->w8_7B(i_Offset); break;
    case MN_8B:   wDatum = aCacheLine->w8_8B(i_Offset); break;
      
    default:
      NEVER_REACH;
  }
  
  if (iEncoding == MN_5B)
    wDatum = wRestoreTsp(wDatum,iEncoding);
}

/*----------------------------------------------------------------------------*/
/* decode varchar from given cache line                                       */
/*----------------------------------------------------------------------------*/
void RowDecoder::vDecodeVch(i8 i_Row, Column* pCol, w8& wDatum, Stz* szDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWDECODER,_FOOTER_);
  
  CacheLine* aCacheLine = m_pRowTray->aGet(i_Row);
  
  i8 i_Offset  = C_UNKNOWN;
  i8 i_Column  = pCol->iColumnNo();
  
  //! needs more work
  i8 iEncoding = iGetEncoding(i_Column,aCacheLine,i_Offset);
  switch (iEncoding) {
      
    case MN_NULL: wDatum = C_DATUM_NULL;                break;
    case MN_1B:   wDatum = aCacheLine->w8_1B(i_Offset); break;
    case MN_2B:   wDatum = aCacheLine->w8_2B(i_Offset); break;
    case MN_3B:   wDatum = aCacheLine->w8_3B(i_Offset); break;
    case MN_4B:   wDatum = aCacheLine->w8_4B(i_Offset); break;
    case MN_5B:   wDatum = aCacheLine->w8_5B(i_Offset); break;
    case MN_6B:   wDatum = aCacheLine->w8_6B(i_Offset); break;
    case MN_7B:   wDatum = aCacheLine->w8_7B(i_Offset); break;
    case MN_8B:   wDatum = aCacheLine->w8_8B(i_Offset); break;
      
    case MN_SHOCO_2B: {
      i8 i_Shoco = aCacheLine->w8_2B(i_Offset);
      CacheLine* aShocoLine = m_pRowTray->aGet(i_Shoco);

      i8 iLenCompressed = aShocoLine->w8_1N(0);
      
      s256 sDecomp;
      i8 iLenDecompressed =
        Compress::iFromShoco(sDecomp.aChar(), 255,
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
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get the encoding of of a column from given cache line                      */
/*----------------------------------------------------------------------------*/
i8 RowDecoder::iGetEncoding(i8 i_Column, CacheLine* aCacheLine, i8& i_Offset)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWDECODER,_FOOTER_);
  
  i_Offset = 8;
  
  for (i8 i_Nibble = 0; i_Nibble < i_Column; i_Nibble++) {
    i8 nDataBytes = aCacheLine->w8_1N(i_Nibble);
    if (nDataBytes <= MN_8B)
      i_Offset += nDataBytes;
  }
  
  i8 iEncoding = aCacheLine->w8_1N(i_Column);
  
  return iEncoding;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* restore zero stripped dte                                                  */
/*----------------------------------------------------------------------------*/
w8 RowDecoder::wRestoreDte(w8 wDteStripped, i8 iEncoding)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWDECODER,_FOOTER_);
  
  w8 wDte = wDteStripped;
  wDte <<= 40;
  
  return wDte;
}

/*----------------------------------------------------------------------------*/
/* restore zero stripped tme                                                  */
/*----------------------------------------------------------------------------*/
w8 RowDecoder::wRestoreTme(w8 wTmeStripped, i8 iEncoding)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWDECODER,_FOOTER_);
  
  w8 wTme = wTmeStripped;
  wTme <<= 40;
  
  return wTme;
}

/*----------------------------------------------------------------------------*/
/* restore zero stripped tsp                                                  */
/*----------------------------------------------------------------------------*/
w8 RowDecoder::wRestoreTsp(w8 wTspStripped, i8 iEncoding)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWDECODER,_FOOTER_);
  
  w8 wTsp = wTspStripped;
  wTsp <<= 24;
  
  return wTsp;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* RowDecoder::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWDECODER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
