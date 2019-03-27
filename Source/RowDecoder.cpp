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
  
  Record* pRec = new Record;
  
  vDecode(i_Row,pCol,wDatum,sDatum);
  
  eDM eDatum = pCol->eDatumType();
  switch (eDatum) {
    case DM_BCD: pRec->vAppendBcd(pCol,wDatum,sDatum); break;
    case DM_BOL: pRec->vAppendBol(pCol,wDatum,sDatum); break;
    case DM_DTE: pRec->vAppendDte(pCol,wDatum,sDatum); break;
    case DM_FLT: pRec->vAppendFlt(pCol,wDatum,sDatum); break;
    case DM_INT: pRec->vAppendInt(pCol,wDatum,sDatum); break;
    case DM_IVL: pRec->vAppendIvl(pCol,wDatum,sDatum); break;
    case DM_TME: pRec->vAppendTme(pCol,wDatum,sDatum); break;
    case DM_TSP: pRec->vAppendTsp(pCol,wDatum,sDatum); break;
    case DM_VCH: pRec->vAppendVch(pCol,wDatum,sDatum); break;
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
    case DM_BCD: pRec->vAppendBcd(pCol,wDatum,sDatum); break;
    case DM_BOL: pRec->vAppendBol(pCol,wDatum,sDatum); break;
    case DM_DTE: pRec->vAppendDte(pCol,wDatum,sDatum); break;
    case DM_FLT: pRec->vAppendFlt(pCol,wDatum,sDatum); break;
    case DM_INT: pRec->vAppendInt(pCol,wDatum,sDatum); break;
    case DM_IVL: pRec->vAppendIvl(pCol,wDatum,sDatum); break;
    case DM_TME: pRec->vAppendTme(pCol,wDatum,sDatum); break;
    case DM_TSP: pRec->vAppendTsp(pCol,wDatum,sDatum); break;
    case DM_VCH: pRec->vAppendVch(pCol,wDatum,sDatum); break;
    default:
      NEVER_REACH;
  }
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
  
  eMN eEncoding = eGetEncoding(i_Column,aCacheLine,i_Offset);
  switch (eEncoding) {
      
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
  
  eMN eEncoding = eGetEncoding(i_Column,aCacheLine,i_Offset);
  switch (eEncoding) {
      
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
  
  eMN eEncoding = eGetEncoding(i_Column,aCacheLine,i_Offset);
  switch (eEncoding) {
      
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
  
  wDatum = wRestoreDte(wDatum,eEncoding);
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
  
  eMN eEncoding = eGetEncoding(i_Column,aCacheLine,i_Offset);
  switch (eEncoding) {
      
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
  
  eMN eEncoding = eGetEncoding(i_Column,aCacheLine,i_Offset);
  switch (eEncoding) {
      
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
  
  eMN eEncoding = eGetEncoding(i_Column,aCacheLine,i_Offset);
  switch (eEncoding) {
      
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
  
  eMN eEncoding = eGetEncoding(i_Column,aCacheLine,i_Offset);
  switch (eEncoding) {
      
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
  
  wDatum = wRestoreTme(wDatum,eEncoding);
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
  
  eMN eEncoding = eGetEncoding(i_Column,aCacheLine,i_Offset);
  switch (eEncoding) {
      
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
  
  if (eEncoding == MN_5B)
    wDatum = wRestoreTsp(wDatum,eEncoding);
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
  eMN eEncoding = eGetEncoding(i_Column,aCacheLine,i_Offset);
  switch (eEncoding) {
      
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
eMN RowDecoder::eGetEncoding(i8 i_Column, CacheLine* aCacheLine, i8& i_Offset)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWDECODER,_FOOTER_);
  
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
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* restore zero stripped dte                                                  */
/*----------------------------------------------------------------------------*/
w8 RowDecoder::wRestoreDte(w8 wDteStripped, eMN eEncoding)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWDECODER,_FOOTER_);
  
  w8 wDte = wDteStripped;
  wDte <<= 40;
  
  return wDte;
}

/*----------------------------------------------------------------------------*/
/* restore zero stripped tme                                                  */
/*----------------------------------------------------------------------------*/
w8 RowDecoder::wRestoreTme(w8 wTmeStripped, eMN eEncoding)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWDECODER,_FOOTER_);
  
  w8 wTme = wTmeStripped;
  wTme <<= 40;
  
  return wTme;
}

/*----------------------------------------------------------------------------*/
/* restore zero stripped tsp                                                  */
/*----------------------------------------------------------------------------*/
w8 RowDecoder::wRestoreTsp(w8 wTspStripped, eMN eEncoding)
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
