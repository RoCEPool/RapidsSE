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
/* get encoded datum from cache line                                          */
/*----------------------------------------------------------------------------*/
w8 RowDecoder::wDecode(i8 i_Row, Column* pCol, Stz* szBuf)
{
  eDM eDatum = pCol->eDatumType();
  
  WordView wDecoded;
  
  switch (eDatum) {
    case DM_BCD: wDecoded = wDecodeBcd(i_Row,pCol,szBuf); break;
    case DM_BOL: wDecoded = wDecodeBol(i_Row,pCol,szBuf); break;
    case DM_DTE: wDecoded = wDecodeDte(i_Row,pCol,szBuf); break;
    case DM_FLT: wDecoded = wDecodeFlt(i_Row,pCol,szBuf); break;
    case DM_INT: wDecoded = wDecodeInt(i_Row,pCol,szBuf); break;
    case DM_IVL: wDecoded = wDecodeIvl(i_Row,pCol,szBuf); break;
    case DM_TME: wDecoded = wDecodeTme(i_Row,pCol,szBuf); break;
    case DM_TSP: wDecoded = wDecodeTsp(i_Row,pCol,szBuf); break;
    case DM_VCH: wDecoded = wDecodeVch(i_Row,pCol,szBuf); break;
    default:
      NEVER_REACH;
  }

  return wDecoded;
}

/*----------------------------------------------------------------------------*/
/* print decoded datum from cache line                                        */
/*----------------------------------------------------------------------------*/
void RowDecoder::vPrint(i8 i_Row, Column* pCol)
{
  WordView wDatum;
  s256 sBuf;
  
  wDatum.m_wWord = wDecode(i_Row,pCol,sBuf);
  
  vPrint(pCol,wDatum);
}

/*----------------------------------------------------------------------------*/
/* print row decoded datum from cache line                                    */
/*----------------------------------------------------------------------------*/
void RowDecoder::vString(i8 i_Row, ColumnVec* pColVec, s48K& sStr)
{
  WordView wDatum;
  s256 sBuf;
  
  // get no of columns
  i8 nColumns = pColVec->nItems();
  
  // for each column
  for (i8 i_Column = 0; i_Column < nColumns - 1; i_Column++) {
    
    // get the column at index
    Column* pCol = pColVec->pGet(i_Column);
    
    // comma sepate column values
    if (i_Column)
      sStr.vConcat(","_Stz);
    
    eDM eDatum = pCol->eDatumType();
    wDatum = wDecode(i_Row,pCol,sBuf.aStz());
    
    switch (eDatum) {
      case DM_BCD: vStringBcd(pCol,wDatum,sBuf,sStr); break;
      case DM_BOL: vStringBol(pCol,wDatum,sBuf,sStr); break;
      case DM_DTE: vStringDte(pCol,wDatum,sBuf,sStr); break;
      case DM_FLT: vStringFlt(pCol,wDatum,sBuf,sStr); break;
      case DM_INT: vStringInt(pCol,wDatum,sBuf,sStr); break;
      case DM_IVL: vStringIvl(pCol,wDatum,sBuf,sStr); break;
      case DM_TME: vStringTme(pCol,wDatum,sBuf,sStr); break;
      case DM_TSP: vStringTsp(pCol,wDatum,sBuf,sStr); break;
      case DM_VCH: vStringVch(pCol,wDatum,sBuf,sStr); break;
      default:
        NEVER_REACH;
    }
    
  }
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* decode decimal from given cache line                                       */
/*----------------------------------------------------------------------------*/
w8 RowDecoder::wDecodeBcd(i8 i_Row, Column* pCol, Stz* szBuf)
{
  WordView wDecoded;
  
  CacheLine* aCacheLine = m_pRowTray->aGet(0);
  
  i8 i_Offset  = C_UNKNOWN;
  i8 i_Column  = pCol->iColumnNo();
  i8 iEncoding = iGetEncoding(i_Column,aCacheLine,i_Offset);

  switch (iEncoding) {
      
    case MN_NULL: wDecoded = C_DATUM_NULL;                break;
    case MN_1B:   wDecoded = aCacheLine->w8_1B(i_Offset); break;
    case MN_2B:   wDecoded = aCacheLine->w8_2B(i_Offset); break;
    case MN_3B:   wDecoded = aCacheLine->w8_3B(i_Offset); break;
    case MN_4B:   wDecoded = aCacheLine->w8_4B(i_Offset); break;
    case MN_5B:   wDecoded = aCacheLine->w8_5B(i_Offset); break;
    case MN_6B:   wDecoded = aCacheLine->w8_6B(i_Offset); break;
    case MN_7B:   wDecoded = aCacheLine->w8_7B(i_Offset); break;
    case MN_8B:   wDecoded = aCacheLine->w8_8B(i_Offset); break;
      
    case MN_MINUS_ONE: wDecoded = (i8)-1; break;
    case MN_ZERO:      wDecoded = (i8)0;  break;
    case MN_PLUS_ONE:  wDecoded = (i8)1;  break;
      
    default:
      NEVER_REACH;
  }
  
  return wDecoded;
}

/*----------------------------------------------------------------------------*/
/* decode boolean from given cache line                                       */
/*----------------------------------------------------------------------------*/
w8 RowDecoder::wDecodeBol(i8 i_Row, Column* pCol, Stz* szBuf)
{
  WordView wDecoded;
  
  CacheLine* aCacheLine = m_pRowTray->aGet(i_Row);
  
  i8 i_Offset  = C_UNKNOWN;
  i8 i_Column  = pCol->iColumnNo();
  i8 iEncoding = iGetEncoding(i_Column,aCacheLine,i_Offset);
  
  switch (iEncoding) {
      
    case MN_NULL:  wDecoded = C_DATUM_NULL; break;
    case MN_FALSE: wDecoded = (i8)0;        break;
    case MN_TRUE:  wDecoded = (i8)1;        break;
      
    default:
      NEVER_REACH;
  }
  
  return wDecoded;
}

/*----------------------------------------------------------------------------*/
/* decode date from given cache line                                          */
/*----------------------------------------------------------------------------*/
w8 RowDecoder::wDecodeDte(i8 i_Row, Column* pCol, Stz* szBuf)
{
  WordView wDecoded;
  
  CacheLine* aCacheLine = m_pRowTray->aGet(i_Row);
  
  i8 i_Offset  = C_UNKNOWN;
  i8 i_Column  = pCol->iColumnNo();
  i8 iEncoding = iGetEncoding(i_Column,aCacheLine,i_Offset);
  
  switch (iEncoding) {
      
    case MN_NULL: wDecoded = C_DATUM_NULL;                break;
    case MN_1B:   wDecoded = aCacheLine->w8_1B(i_Offset); break;
    case MN_2B:   wDecoded = aCacheLine->w8_2B(i_Offset); break;
    case MN_3B:   wDecoded = aCacheLine->w8_3B(i_Offset); break;
    case MN_4B:   wDecoded = aCacheLine->w8_4B(i_Offset); break;
    case MN_5B:   wDecoded = aCacheLine->w8_5B(i_Offset); break;
    case MN_6B:   wDecoded = aCacheLine->w8_6B(i_Offset); break;
    case MN_7B:   wDecoded = aCacheLine->w8_7B(i_Offset); break;
    case MN_8B:   wDecoded = aCacheLine->w8_8B(i_Offset); break;
      
    default:
      NEVER_REACH;
  }
  
  //if (iEncoding == MN_5B)
  wDecoded = iDteDecoding(wDecoded,iEncoding);
  
  return wDecoded;
}

/*----------------------------------------------------------------------------*/
/* decode float from given cache line                                         */
/*----------------------------------------------------------------------------*/
w8 RowDecoder::wDecodeFlt(i8 i_Row, Column* pCol, Stz* szBuf)
{
  WordView wDecoded;
  
  CacheLine* aCacheLine = m_pRowTray->aGet(i_Row);
  
  i8 i_Offset  = C_UNKNOWN;
  i8 i_Column  = pCol->iColumnNo();
  i8 iEncoding = iGetEncoding(i_Column,aCacheLine,i_Offset);
  
  switch (iEncoding) {
      
    case MN_NULL: wDecoded = C_DATUM_NULL;                break;
    case MN_1B:   wDecoded = aCacheLine->w8_1B(i_Offset); break;
    case MN_2B:   wDecoded = aCacheLine->w8_2B(i_Offset); break;
    case MN_3B:   wDecoded = aCacheLine->w8_3B(i_Offset); break;
    case MN_4B:   wDecoded = aCacheLine->w8_4B(i_Offset); break;
    case MN_5B:   wDecoded = aCacheLine->w8_5B(i_Offset); break;
    case MN_6B:   wDecoded = aCacheLine->w8_6B(i_Offset); break;
    case MN_7B:   wDecoded = aCacheLine->w8_7B(i_Offset); break;
    case MN_8B:   wDecoded = aCacheLine->w8_8B(i_Offset); break;
      
    default:
      NEVER_REACH;
  }
  
  return wDecoded;
}

/*----------------------------------------------------------------------------*/
/* decode integer from given cache line                                       */
/*----------------------------------------------------------------------------*/
w8 RowDecoder::wDecodeInt(i8 i_Row, Column* pCol, Stz* szBuf)
{
  WordView wDecoded;
  
  CacheLine* aCacheLine = m_pRowTray->aGet(i_Row);
  
  i8 i_Offset  = C_UNKNOWN;
  i8 i_Column  = pCol->iColumnNo();
  i8 iEncoding = iGetEncoding(i_Column,aCacheLine,i_Offset);
  
  switch (iEncoding) {
      
    case MN_NULL: wDecoded = C_DATUM_NULL;                break;
    case MN_1B:   wDecoded = aCacheLine->w8_1B(i_Offset); break;
    case MN_2B:   wDecoded = aCacheLine->w8_2B(i_Offset); break;
    case MN_3B:   wDecoded = aCacheLine->w8_3B(i_Offset); break;
    case MN_4B:   wDecoded = aCacheLine->w8_4B(i_Offset); break;
    case MN_5B:   wDecoded = aCacheLine->w8_5B(i_Offset); break;
    case MN_6B:   wDecoded = aCacheLine->w8_6B(i_Offset); break;
    case MN_7B:   wDecoded = aCacheLine->w8_7B(i_Offset); break;
    case MN_8B:   wDecoded = aCacheLine->w8_8B(i_Offset); break;
      
    case MN_MINUS_ONE: wDecoded = (i8)-1; break;
    case MN_ZERO:      wDecoded = (i8)0;  break;
    case MN_PLUS_ONE:  wDecoded = (i8)1;  break;
      
    default:
      NEVER_REACH;
  }
  
  return wDecoded;
}

/*----------------------------------------------------------------------------*/
/* decode interval from given cache line                                      */
/*----------------------------------------------------------------------------*/
w8 RowDecoder::wDecodeIvl(i8 i_Row, Column* pCol, Stz* szBuf)
{
  WordView wDecoded;
  
  CacheLine* aCacheLine = m_pRowTray->aGet(i_Row);
  
  i8 i_Offset  = C_UNKNOWN;
  i8 i_Column  = pCol->iColumnNo();
  i8 iEncoding = iGetEncoding(i_Column,aCacheLine,i_Offset);
  
  switch (iEncoding) {
      
    case MN_NULL: wDecoded = C_DATUM_NULL;                break;
    case MN_1B:   wDecoded = aCacheLine->w8_1B(i_Offset); break;
    case MN_2B:   wDecoded = aCacheLine->w8_2B(i_Offset); break;
    case MN_3B:   wDecoded = aCacheLine->w8_3B(i_Offset); break;
    case MN_4B:   wDecoded = aCacheLine->w8_4B(i_Offset); break;
    case MN_5B:   wDecoded = aCacheLine->w8_5B(i_Offset); break;
    case MN_6B:   wDecoded = aCacheLine->w8_6B(i_Offset); break;
    case MN_7B:   wDecoded = aCacheLine->w8_7B(i_Offset); break;
    case MN_8B:   wDecoded = aCacheLine->w8_8B(i_Offset); break;
      
    default:
      NEVER_REACH;
  }
  
  return wDecoded;
}

/*----------------------------------------------------------------------------*/
/* decode time from given cache line                                          */
/*----------------------------------------------------------------------------*/
w8 RowDecoder::wDecodeTme(i8 i_Row, Column* pCol, Stz* szBuf)
{
  WordView wDecoded;
  
  CacheLine* aCacheLine = m_pRowTray->aGet(i_Row);
  
  i8 i_Offset  = C_UNKNOWN;
  i8 i_Column  = pCol->iColumnNo();
  i8 iEncoding = iGetEncoding(i_Column,aCacheLine,i_Offset);
  
  switch (iEncoding) {
      
    case MN_NULL: wDecoded = C_DATUM_NULL;                break;
    case MN_1B:   wDecoded = aCacheLine->w8_1B(i_Offset); break;
    case MN_2B:   wDecoded = aCacheLine->w8_2B(i_Offset); break;
    case MN_3B:   wDecoded = aCacheLine->w8_3B(i_Offset); break;
    case MN_4B:   wDecoded = aCacheLine->w8_4B(i_Offset); break;
    case MN_5B:   wDecoded = aCacheLine->w8_5B(i_Offset); break;
    case MN_6B:   wDecoded = aCacheLine->w8_6B(i_Offset); break;
    case MN_7B:   wDecoded = aCacheLine->w8_7B(i_Offset); break;
    case MN_8B:   wDecoded = aCacheLine->w8_8B(i_Offset); break;
      
    default:
      NEVER_REACH;
  }
  
  wDecoded = iTmeDecoding(wDecoded,iEncoding);
  
  return wDecoded;
}

/*----------------------------------------------------------------------------*/
/* decode timestamp from given cache line                                     */
/*----------------------------------------------------------------------------*/
w8 RowDecoder::wDecodeTsp(i8 i_Row, Column* pCol, Stz* szBuf)
{
  WordView wDecoded;
  
  CacheLine* aCacheLine = m_pRowTray->aGet(i_Row);
  
  i8 i_Offset  = C_UNKNOWN;
  i8 i_Column  = pCol->iColumnNo();
  i8 iEncoding = iGetEncoding(i_Column,aCacheLine,i_Offset);
  
  switch (iEncoding) {
      
    case MN_NULL: wDecoded = C_DATUM_NULL;                break;
    case MN_1B:   wDecoded = aCacheLine->w8_1B(i_Offset); break;
    case MN_2B:   wDecoded = aCacheLine->w8_2B(i_Offset); break;
    case MN_3B:   wDecoded = aCacheLine->w8_3B(i_Offset); break;
    case MN_4B:   wDecoded = aCacheLine->w8_4B(i_Offset); break;
    case MN_5B:   wDecoded = aCacheLine->w8_5B(i_Offset); break;
    case MN_6B:   wDecoded = aCacheLine->w8_6B(i_Offset); break;
    case MN_7B:   wDecoded = aCacheLine->w8_7B(i_Offset); break;
    case MN_8B:   wDecoded = aCacheLine->w8_8B(i_Offset); break;
      
    default:
      NEVER_REACH;
  }
  
  if (iEncoding == MN_5B)
    wDecoded = iTspDecoding(wDecoded,iEncoding);
  
  return wDecoded;
}

/*----------------------------------------------------------------------------*/
/* decode varchar from given cache line                                       */
/*----------------------------------------------------------------------------*/
w8 RowDecoder::wDecodeVch(i8 i_Row, Column* pCol, Stz* szBuf)
{
  WordView wDecoded;
  
  CacheLine* aCacheLine = m_pRowTray->aGet(i_Row);
  
  i8 i_Offset  = C_UNKNOWN;
  i8 i_Column  = pCol->iColumnNo();
  i8 iEncoding = iGetEncoding(i_Column,aCacheLine,i_Offset);
  
  switch (iEncoding) {
      
    case MN_NULL: wDecoded = C_DATUM_NULL;                break;
    case MN_1B:   wDecoded = aCacheLine->w8_1B(i_Offset); break;
    case MN_2B:   wDecoded = aCacheLine->w8_2B(i_Offset); break;
    case MN_3B:   wDecoded = aCacheLine->w8_3B(i_Offset); break;
    case MN_4B:   wDecoded = aCacheLine->w8_4B(i_Offset); break;
    case MN_5B:   wDecoded = aCacheLine->w8_5B(i_Offset); break;
    case MN_6B:   wDecoded = aCacheLine->w8_6B(i_Offset); break;
    case MN_7B:   wDecoded = aCacheLine->w8_7B(i_Offset); break;
    case MN_8B:   wDecoded = aCacheLine->w8_8B(i_Offset); break;
      
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
  
  return wDecoded;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append decimal datum to string                                             */
/*----------------------------------------------------------------------------*/
void RowDecoder::vStringBcd(Column* pCol, w8 wDatum, Stz* szBuf, s48K& sStr)
{
  WordView wView(wDatum);
  sStr.vCatSprint("%lld"_Stz,wView.m_iI8);
}

/*----------------------------------------------------------------------------*/
/* append decimal datum to string                                             */
/*----------------------------------------------------------------------------*/
void RowDecoder::vStringBol(Column* pCol, w8 wDatum, Stz* szBuf, s48K& sStr)
{
  WordView wView(wDatum);
  sStr.vConcat(wView.m_iI8 == 1 ? "true"_Stz : "false"_Stz);
}

/*----------------------------------------------------------------------------*/
/* append decimal datum to string                                             */
/*----------------------------------------------------------------------------*/
void RowDecoder::vStringDte(Column* pCol, w8 wDatum, Stz* szBuf, s48K& sStr)
{
  WordView wView(wDatum);
  
  Dte wDte;
  wDte.vAsI8(wView.m_iI8);
  
  s32 sDte;
  wDte.vSprint(sDte.aStz());
  
  sStr.vConcat(sDte.aStz());
}

/*----------------------------------------------------------------------------*/
/* append decimal datum to string                                             */
/*----------------------------------------------------------------------------*/
void RowDecoder::vStringFlt(Column* pCol, w8 wDatum, Stz* szBuf, s48K& sStr)
{
  WordView wView(wDatum);
  sStr.vCatSprint("%g"_Stz,wView.m_dF8);
}

/*----------------------------------------------------------------------------*/
/* append decimal datum to string                                             */
/*----------------------------------------------------------------------------*/
void RowDecoder::vStringInt(Column* pCol, w8 wDatum, Stz* szBuf, s48K& sStr)
{
  WordView wView(wDatum);
  sStr.vCatSprint("%lld"_Stz,wView.m_iI8);
}

/*----------------------------------------------------------------------------*/
/* append decimal datum to string                                             */
/*----------------------------------------------------------------------------*/
void RowDecoder::vStringIvl(Column* pCol, w8 wDatum, Stz* szBuf, s48K& sStr)
{
  WordView wView(wDatum);
  sStr.vCatSprint("%lld"_Stz,wView.m_iI8);
}

/*----------------------------------------------------------------------------*/
/* append decimal datum to string                                             */
/*----------------------------------------------------------------------------*/
void RowDecoder::vStringTme(Column* pCol, w8 wDatum, Stz* szBuf, s48K& sStr)
{
  WordView wView(wDatum);
  
  Tme wTme;
  wTme.vAsI8(wView.m_iI8);
  
  s32 sTme;
  wTme.vSprint(sTme.aStz());
  
  sStr.vConcat(sTme.aStz());
}

/*----------------------------------------------------------------------------*/
/* append decimal datum to string                                             */
/*----------------------------------------------------------------------------*/
void RowDecoder::vStringTsp(Column* pCol, w8 wDatum, Stz* szBuf, s48K& sStr)
{
  WordView wView(wDatum);
  
  Tsp wTsp;
  wTsp.vAsI8(wView.m_iI8);
  
  s32 sTsp;
  wTsp.vSprint(sTsp.aStz());
  
  sStr.vConcat(sTsp.aStz());
}

/*----------------------------------------------------------------------------*/
/* append decimal datum to string                                             */
/*----------------------------------------------------------------------------*/
void RowDecoder::vStringVch(Column* pCol, w8 wDatum, Stz* szBuf, s48K& sStr)
{
  s256 sBuf;
  
  Vch::vSprintQuotes(sBuf,(Stz*)&wDatum);
  sStr.vConcat(sBuf.aStz());
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* decoding of dte                                                            */
/*----------------------------------------------------------------------------*/
i8 RowDecoder::iDteDecoding(WordView& wDteView, i8 iEncoding)
{
  i8 iDecoded = wDteView.m_iI8;
  iDecoded <<= 40;
  
  return iDecoded;
}

/*----------------------------------------------------------------------------*/
/* decoding of tme                                                            */
/*----------------------------------------------------------------------------*/
i8 RowDecoder::iTmeDecoding(WordView& wTmeView, i8 iEncoding)
{
  i8 iDecoded = wTmeView.m_iI8;
  iDecoded <<= 40;
  
  return iDecoded;
}

/*----------------------------------------------------------------------------*/
/* decoding of tsp                                                            */
/*----------------------------------------------------------------------------*/
i8 RowDecoder::iTspDecoding(WordView& wTspView, i8 iEncoding)
{
  i8 iDecoded = wTspView.m_iI8;
  iDecoded <<= 24;
  
  return iDecoded;
}

/*----------------------------------------------------------------------------*/
/* encoding of tsp                                                            */
/*----------------------------------------------------------------------------*/
i8 RowDecoder::iGetEncoding(i8 i_Column, CacheLine* aCacheLine, i8& i_Offset)
{
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
/* print decoded datum from cache line                                        */
/*----------------------------------------------------------------------------*/
void RowDecoder::vPrint(Column* pCol, WordView& wDatum)
{
  eDM eDatum = pCol->eDatumType();
  
  switch (eDatum) {
    case DM_BCD: {
      vTrace("Column[%lld %s] Bcd : %lld\n"_Stz,
             pCol->iColumnNo(),pCol->sColumnName().aStz(),wDatum.m_iI8);
      break;
    }
    case DM_BOL: {
      vTrace("Column[%lld %s] Bol : %lld\n"_Stz,
             pCol->iColumnNo(),pCol->sColumnName().aStz(),wDatum.m_iI8);
      break;
    }
    case DM_DTE: {
      Dte wDte;
      wDte.vAsI8(wDatum.m_iI8);
      s32 sDte;
      wDte.vSprint(sDte.aStz());
      vTrace("Column[%lld %s] Dte : %s\n"_Stz,
             pCol->iColumnNo(),pCol->sColumnName().aStz(),sDte.aStz());
      break;
    }
    case DM_FLT: {
      vTrace("Column[%lld %s] Bcd : %llf\n"_Stz,
             pCol->iColumnNo(),pCol->sColumnName().aStz(),wDatum.m_dF8);
      break;
    }
    case DM_INT: {
      vTrace("Column[%lld %s] Int : %lld\n"_Stz,
             pCol->iColumnNo(),pCol->sColumnName().aStz(),wDatum.m_iI8);
      break;
    }
    case DM_IVL: {
      vTrace("Column[%lld %s] Ivl : %lld\n"_Stz,
             pCol->iColumnNo(),pCol->sColumnName().aStz(),wDatum.m_iI8);
      break;
    }
    case DM_TME: {
      Tme wTme;
      wTme.vAsI8(wDatum.m_iI8);
      s32 sTme;
      wTme.vSprint(sTme.aStz());
      vTrace("Column[%lld %s] Tme : %s\n"_Stz,
             pCol->iColumnNo(),pCol->sColumnName().aStz(),sTme.aStz());
      break;
    }
    case DM_TSP: {
      Tsp wTsp;
      wTsp.vAsI8(wDatum.m_iI8);
      s32 sTsp;
      wTsp.vSprint(sTsp.aStz());
      vTrace("Column[%lld %s] Tsp : %s\n"_Stz,
             pCol->iColumnNo(),pCol->sColumnName().aStz(),sTsp.aStz());
      break;
    }
    case DM_VCH: {
      vTrace("Column[%lld %s] Vch : "_Stz,
             pCol->iColumnNo(),pCol->sColumnName().aStz());
      for (i8 i_Char = 0; i_Char < 8; i_Char++) {
        if (! wDatum.m_yaByte[i_Char])
        break;
        vTrace("%c"_Stz,wDatum.m_yaByte[i_Char]);
      }
      vTrace("\n"_Stz);
      break;
    }
    default:
    NEVER_REACH;
  }
  
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
