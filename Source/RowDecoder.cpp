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
/* print row decoded datum from cache line                                    */
/*----------------------------------------------------------------------------*/
void RowDecoder::vAsString(s48K& sDataString, ColumnVec* pColumnVec)
{
  WordView wDatum;
  
  CacheLine* aCacheLine = m_pRowTray->aGet(0);
  
  // get no of columns
  i8 nColumns = pColumnVec->nItems();
  
  // for each column
  for (i8 i_Column = 0; i_Column < nColumns - 1; i_Column++) {
    
    // get the column at index
    Column* pColumn = pColumnVec->pGet(i_Column);
    
    // not the first column
    if (i_Column == 0)
      sDataString.vConcat(""_Stz);
    else
      sDataString.vConcat(","_Stz);
    
    // copy the data for the column into aDst
    eDM eDatum = pColumn->eDatumType();
    wDatum.m_wWord = wDecode(pColumn);
    
    switch (eDatum) {
      case DM_BCD: {
        sDataString.vCatSprint("%lld"_Stz, wDatum.m_iI8);
        break;
      }
      case DM_BOL: {
        sDataString.vConcat(wDatum.m_iI8 == 1 ? "true"_Stz : "false"_Stz);
        break;
      }
      case DM_DTE: {
        Dte wDte;
        wDte.vAsI8(wDatum.m_iI8);
        s32 sDte;
        wDte.vSprint(sDte.aStz());
        sDataString.vConcat(sDte.aStz());
        break;
      }
      case DM_FLT: {
        sDataString.vCatSprint("%g"_Stz, wDatum.m_dF8);
        break;
      }
      case DM_INT: {
        sDataString.vCatSprint("%lld"_Stz, wDatum.m_iI8);
        break;
      }
      case DM_IVL: {
        sDataString.vCatSprint("%lld"_Stz, wDatum.m_iI8);
        break;
      }
      case DM_TME: {
        Tme wTme;
        wTme.vAsI8(wDatum.m_iI8);
        s32 sTme;
        wTme.vSprint(sTme.aStz());
        sDataString.vConcat(sTme.aStz());
        break;
      }
      case DM_TSP: {
        Tsp wTsp;
        wTsp.vAsI8(wDatum.m_iI8);
        s32 sTsp;
        wTsp.vSprint(sTsp.aStz());
        sDataString.vConcat(sTsp.aStz());
        break;
      }
      case DM_VCH: {
        s256 sStr;
        Vch::vSprintQuotes(sStr, (Stz*)&wDatum);
        sDataString.vConcat(sStr.aStz());
        break;
      }
      default:
      NEVER_REACH;
    }
    
  }
}

/*----------------------------------------------------------------------------*/
/* get encoded datum from cache line                                          */
/*----------------------------------------------------------------------------*/
w8 RowDecoder::wDecode(Column* pColumn)
{
  i8 i_Column = pColumn->iColumnNo();
  eDM eDatum  = pColumn->eDatumType();
  
  CacheLine* aCacheLine = m_pRowTray->aGet(0);
  
  i8 i_Offset  = C_UNKNOWN;
  i8 iEncoding = iGetEncoding(i_Column,aCacheLine,i_Offset);
  
  WordView wDecoded;
  
  switch (eDatum) {
    case DM_BCD: wDecoded.m_wWord = wDecodeBcd(pColumn,iEncoding,i_Offset); break;
    case DM_BOL: wDecoded.m_wWord = wDecodeBol(pColumn,iEncoding,i_Offset); break;
    case DM_DTE: wDecoded.m_wWord = wDecodeDte(pColumn,iEncoding,i_Offset); break;
    case DM_FLT: wDecoded.m_wWord = wDecodeFlt(pColumn,iEncoding,i_Offset); break;
    case DM_INT: wDecoded.m_wWord = wDecodeInt(pColumn,iEncoding,i_Offset); break;
    case DM_IVL: wDecoded.m_wWord = wDecodeIvl(pColumn,iEncoding,i_Offset); break;
    case DM_TME: wDecoded.m_wWord = wDecodeTme(pColumn,iEncoding,i_Offset); break;
    case DM_TSP: wDecoded.m_wWord = wDecodeTsp(pColumn,iEncoding,i_Offset); break;
    case DM_VCH: wDecoded.m_wWord = wDecodeVch(pColumn,iEncoding,i_Offset); break;
    default:
      NEVER_REACH;
  }

  return wDecoded.m_wWord;
}

/*----------------------------------------------------------------------------*/
/* print decoded datum from cache line                                        */
/*----------------------------------------------------------------------------*/
void RowDecoder::vPrint(Column* pColumn)
{
  WordView wDatum;
  
  wDatum.m_wWord = wDecode(pColumn);
  vPrint(pColumn,wDatum);
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* decode decimal from given cache line                                       */
/*----------------------------------------------------------------------------*/
w8 RowDecoder::wDecodeBcd(Column* pColumn, i8 iEncoding, i8 i_Offset)
{
  WordView wDecoded;
  
  CacheLine* aCacheLine = m_pRowTray->aGet(0);
  
  switch (iEncoding) {
      
    case MN_NULL: wDecoded.m_iI8 = C_DATUM_NULL;                break;
    case MN_1B:   wDecoded.m_iI8 = aCacheLine->w8_1B(i_Offset); break;
    case MN_2B:   wDecoded.m_iI8 = aCacheLine->w8_2B(i_Offset); break;
    case MN_3B:   wDecoded.m_iI8 = aCacheLine->w8_3B(i_Offset); break;
    case MN_4B:   wDecoded.m_iI8 = aCacheLine->w8_4B(i_Offset); break;
    case MN_5B:   wDecoded.m_iI8 = aCacheLine->w8_5B(i_Offset); break;
    case MN_6B:   wDecoded.m_iI8 = aCacheLine->w8_6B(i_Offset); break;
    case MN_7B:   wDecoded.m_iI8 = aCacheLine->w8_7B(i_Offset); break;
    case MN_8B:   wDecoded.m_iI8 = aCacheLine->w8_8B(i_Offset); break;
      
    case MN_MINUS_ONE: wDecoded.m_iI8 = -1; break;
    case MN_ZERO:      wDecoded.m_iI8 = 0;  break;
    case MN_PLUS_ONE:  wDecoded.m_iI8 = 1;  break;
      
    default:
      NEVER_REACH;
  }
  
  return wDecoded.m_wWord;
}

/*----------------------------------------------------------------------------*/
/* decode boolean from given cache line                                       */
/*----------------------------------------------------------------------------*/
w8 RowDecoder::wDecodeBol(Column* pColumn, i8 iEncoding, i8 i_Offset)
{
  WordView wDecoded;
  
  switch (iEncoding) {
      
    case MN_NULL:  wDecoded.m_iI8 = C_DATUM_NULL; break;
    case MN_FALSE: wDecoded.m_iI8 = 0;            break;
    case MN_TRUE:  wDecoded.m_iI8 = 1;            break;
      
    default:
      NEVER_REACH;
  }
  
  return wDecoded.m_wWord;
}

/*----------------------------------------------------------------------------*/
/* decode date from given cache line                                          */
/*----------------------------------------------------------------------------*/
w8 RowDecoder::wDecodeDte(Column* pColumn, i8 iEncoding, i8 i_Offset)
{
  WordView wDecoded;
  
  CacheLine* aCacheLine = m_pRowTray->aGet(0);
  
  switch (iEncoding) {
      
    case MN_NULL: wDecoded.m_iI8 = C_DATUM_NULL;                break;
    case MN_1B:   wDecoded.m_iI8 = aCacheLine->w8_1B(i_Offset); break;
    case MN_2B:   wDecoded.m_iI8 = aCacheLine->w8_2B(i_Offset); break;
    case MN_3B:   wDecoded.m_iI8 = aCacheLine->w8_3B(i_Offset); break;
    case MN_4B:   wDecoded.m_iI8 = aCacheLine->w8_4B(i_Offset); break;
    case MN_5B:   wDecoded.m_iI8 = aCacheLine->w8_5B(i_Offset); break;
    case MN_6B:   wDecoded.m_iI8 = aCacheLine->w8_6B(i_Offset); break;
    case MN_7B:   wDecoded.m_iI8 = aCacheLine->w8_7B(i_Offset); break;
    case MN_8B:   wDecoded.m_iI8 = aCacheLine->w8_8B(i_Offset); break;
      
    default:
      NEVER_REACH;
  }
  
  //if (iEncoding == MN_5B)
  wDecoded.m_iI8 = iDteDecoding(wDecoded,iEncoding);
  
  return wDecoded.m_wWord;
}

/*----------------------------------------------------------------------------*/
/* decode float from given cache line                                         */
/*----------------------------------------------------------------------------*/
w8 RowDecoder::wDecodeFlt(Column* pColumn, i8 iEncoding, i8 i_Offset)
{
  WordView wDecoded;
  
  CacheLine* aCacheLine = m_pRowTray->aGet(0);
  
  switch (iEncoding) {
      
    case MN_NULL: wDecoded.m_iI8 = C_DATUM_NULL;                break;
    case MN_1B:   wDecoded.m_iI8 = aCacheLine->w8_1B(i_Offset); break;
    case MN_2B:   wDecoded.m_iI8 = aCacheLine->w8_2B(i_Offset); break;
    case MN_3B:   wDecoded.m_iI8 = aCacheLine->w8_3B(i_Offset); break;
    case MN_4B:   wDecoded.m_iI8 = aCacheLine->w8_4B(i_Offset); break;
    case MN_5B:   wDecoded.m_iI8 = aCacheLine->w8_5B(i_Offset); break;
    case MN_6B:   wDecoded.m_iI8 = aCacheLine->w8_6B(i_Offset); break;
    case MN_7B:   wDecoded.m_iI8 = aCacheLine->w8_7B(i_Offset); break;
    case MN_8B:   wDecoded.m_iI8 = aCacheLine->w8_8B(i_Offset); break;
      
    default:
      NEVER_REACH;
  }
  
  return wDecoded.m_wWord;
}

/*----------------------------------------------------------------------------*/
/* decode integer from given cache line                                       */
/*----------------------------------------------------------------------------*/
w8 RowDecoder::wDecodeInt(Column* pColumn, i8 iEncoding, i8 i_Offset)
{
  WordView wDecoded;
  
  CacheLine* aCacheLine = m_pRowTray->aGet(0);
  
  switch (iEncoding) {
      
    case MN_NULL: wDecoded.m_iI8 = C_DATUM_NULL;                break;
    case MN_1B:   wDecoded.m_iI8 = aCacheLine->w8_1B(i_Offset); break;
    case MN_2B:   wDecoded.m_iI8 = aCacheLine->w8_2B(i_Offset); break;
    case MN_3B:   wDecoded.m_iI8 = aCacheLine->w8_3B(i_Offset); break;
    case MN_4B:   wDecoded.m_iI8 = aCacheLine->w8_4B(i_Offset); break;
    case MN_5B:   wDecoded.m_iI8 = aCacheLine->w8_5B(i_Offset); break;
    case MN_6B:   wDecoded.m_iI8 = aCacheLine->w8_6B(i_Offset); break;
    case MN_7B:   wDecoded.m_iI8 = aCacheLine->w8_7B(i_Offset); break;
    case MN_8B:   wDecoded.m_iI8 = aCacheLine->w8_8B(i_Offset); break;
      
    case MN_MINUS_ONE: wDecoded.m_iI8 = -1; break;
    case MN_ZERO:      wDecoded.m_iI8 = 0;  break;
    case MN_PLUS_ONE:  wDecoded.m_iI8 = 1;  break;
      
    default:
      NEVER_REACH;
  }
  
  return wDecoded.m_wWord;
}

/*----------------------------------------------------------------------------*/
/* decode interval from given cache line                                      */
/*----------------------------------------------------------------------------*/
w8 RowDecoder::wDecodeIvl(Column* pColumn, i8 iEncoding, i8 i_Offset)
{
  WordView wDecoded;
  
  CacheLine* aCacheLine = m_pRowTray->aGet(0);
  
  switch (iEncoding) {
      
    case MN_NULL: wDecoded.m_iI8 = C_DATUM_NULL;                break;
    case MN_1B:   wDecoded.m_iI8 = aCacheLine->w8_1B(i_Offset); break;
    case MN_2B:   wDecoded.m_iI8 = aCacheLine->w8_2B(i_Offset); break;
    case MN_3B:   wDecoded.m_iI8 = aCacheLine->w8_3B(i_Offset); break;
    case MN_4B:   wDecoded.m_iI8 = aCacheLine->w8_4B(i_Offset); break;
    case MN_5B:   wDecoded.m_iI8 = aCacheLine->w8_5B(i_Offset); break;
    case MN_6B:   wDecoded.m_iI8 = aCacheLine->w8_6B(i_Offset); break;
    case MN_7B:   wDecoded.m_iI8 = aCacheLine->w8_7B(i_Offset); break;
    case MN_8B:   wDecoded.m_iI8 = aCacheLine->w8_8B(i_Offset); break;
      
    default:
      NEVER_REACH;
  }
  
  return wDecoded.m_wWord;
}

/*----------------------------------------------------------------------------*/
/* decode time from given cache line                                          */
/*----------------------------------------------------------------------------*/
w8 RowDecoder::wDecodeTme(Column* pColumn, i8 iEncoding, i8 i_Offset)
{
  WordView wDecoded;
  
  CacheLine* aCacheLine = m_pRowTray->aGet(0);
  
  switch (iEncoding) {
      
    case MN_NULL: wDecoded.m_iI8 = C_DATUM_NULL;                break;
    case MN_1B:   wDecoded.m_iI8 = aCacheLine->w8_1B(i_Offset); break;
    case MN_2B:   wDecoded.m_iI8 = aCacheLine->w8_2B(i_Offset); break;
    case MN_3B:   wDecoded.m_iI8 = aCacheLine->w8_3B(i_Offset); break;
    case MN_4B:   wDecoded.m_iI8 = aCacheLine->w8_4B(i_Offset); break;
    case MN_5B:   wDecoded.m_iI8 = aCacheLine->w8_5B(i_Offset); break;
    case MN_6B:   wDecoded.m_iI8 = aCacheLine->w8_6B(i_Offset); break;
    case MN_7B:   wDecoded.m_iI8 = aCacheLine->w8_7B(i_Offset); break;
    case MN_8B:   wDecoded.m_iI8 = aCacheLine->w8_8B(i_Offset); break;
      
    default:
      NEVER_REACH;
  }
  
  wDecoded.m_iI8 = iTmeDecoding(wDecoded,iEncoding);
  
  return wDecoded.m_wWord;
}

/*----------------------------------------------------------------------------*/
/* decode timestamp from given cache line                                     */
/*----------------------------------------------------------------------------*/
w8 RowDecoder::wDecodeTsp(Column* pColumn, i8 iEncoding, i8 i_Offset)
{
  WordView wDecoded;
  
  CacheLine* aCacheLine = m_pRowTray->aGet(0);
  
  switch (iEncoding) {
      
    case MN_NULL: wDecoded.m_iI8 = C_DATUM_NULL;                break;
    case MN_1B:   wDecoded.m_iI8 = aCacheLine->w8_1B(i_Offset); break;
    case MN_2B:   wDecoded.m_iI8 = aCacheLine->w8_2B(i_Offset); break;
    case MN_3B:   wDecoded.m_iI8 = aCacheLine->w8_3B(i_Offset); break;
    case MN_4B:   wDecoded.m_iI8 = aCacheLine->w8_4B(i_Offset); break;
    case MN_5B:   wDecoded.m_iI8 = aCacheLine->w8_5B(i_Offset); break;
    case MN_6B:   wDecoded.m_iI8 = aCacheLine->w8_6B(i_Offset); break;
    case MN_7B:   wDecoded.m_iI8 = aCacheLine->w8_7B(i_Offset); break;
    case MN_8B:   wDecoded.m_iI8 = aCacheLine->w8_8B(i_Offset); break;
      
    default:
      NEVER_REACH;
  }
  
  if (iEncoding == MN_5B)
    wDecoded.m_iI8 = iTspDecoding(wDecoded,iEncoding);
  
  return wDecoded.m_wWord;
}

/*----------------------------------------------------------------------------*/
/* decode varchar from given cache line                                       */
/*----------------------------------------------------------------------------*/
w8 RowDecoder::wDecodeVch(Column* pColumn, i8 iEncoding, i8 i_Offset)
{
  WordView wDecoded;
  
  CacheLine* aCacheLine = m_pRowTray->aGet(0);
  
  switch (iEncoding) {
      
    case MN_NULL: wDecoded.m_iI8 = C_DATUM_NULL;                break;
    case MN_1B:   wDecoded.m_iI8 = aCacheLine->w8_1B(i_Offset); break;
    case MN_2B:   wDecoded.m_iI8 = aCacheLine->w8_2B(i_Offset); break;
    case MN_3B:   wDecoded.m_iI8 = aCacheLine->w8_3B(i_Offset); break;
    case MN_4B:   wDecoded.m_iI8 = aCacheLine->w8_4B(i_Offset); break;
    case MN_5B:   wDecoded.m_iI8 = aCacheLine->w8_5B(i_Offset); break;
    case MN_6B:   wDecoded.m_iI8 = aCacheLine->w8_6B(i_Offset); break;
    case MN_7B:   wDecoded.m_iI8 = aCacheLine->w8_7B(i_Offset); break;
    case MN_8B:   wDecoded.m_iI8 = aCacheLine->w8_8B(i_Offset); break;
      
    default:
      NEVER_REACH;
  }
  
  return wDecoded.m_wWord;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* decoding of dte                                                            */
/*----------------------------------------------------------------------------*/
i8 RowDecoder::iDteDecoding(WordView& wTspView, i8 iEncoding)
{
  i8 iDecoded = wTspView.m_iI8;
  iDecoded <<= 40;
  
  return iDecoded;
}

/*----------------------------------------------------------------------------*/
/* decoding of tme                                                            */
/*----------------------------------------------------------------------------*/
i8 RowDecoder::iTmeDecoding(WordView& wTspView, i8 iEncoding)
{
  i8 iDecoded = wTspView.m_iI8;
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
void RowDecoder::vPrint(Column* pColumn, WordView& wDatum)
{
  eDM eDatum = pColumn->eDatumType();
  
  switch (eDatum) {
    case DM_BCD: {
      vTrace("Column[%lld %s] Bcd : %lld\n"_Stz,
             pColumn->iColumnNo(),pColumn->sColumnName().aStz(),wDatum.m_iI8);
      break;
    }
    case DM_BOL: {
      vTrace("Column[%lld %s] Bol : %lld\n"_Stz,
             pColumn->iColumnNo(),pColumn->sColumnName().aStz(),wDatum.m_iI8);
      break;
    }
    case DM_DTE: {
      Dte wDte;
      wDte.vAsI8(wDatum.m_iI8);
      s32 sDte;
      wDte.vSprint(sDte.aStz());
      vTrace("Column[%lld %s] Dte : %s\n"_Stz,
             pColumn->iColumnNo(),pColumn->sColumnName().aStz(),sDte.aStz());
      break;
    }
    case DM_FLT: {
      vTrace("Column[%lld %s] Bcd : %llf\n"_Stz,
             pColumn->iColumnNo(),pColumn->sColumnName().aStz(),wDatum.m_dF8);
      break;
    }
    case DM_INT: {
      vTrace("Column[%lld %s] Int : %lld\n"_Stz,
             pColumn->iColumnNo(),pColumn->sColumnName().aStz(),wDatum.m_iI8);
      break;
    }
    case DM_IVL: {
      vTrace("Column[%lld %s] Ivl : %lld\n"_Stz,
             pColumn->iColumnNo(),pColumn->sColumnName().aStz(),wDatum.m_iI8);
      break;
    }
    case DM_TME: {
      Tme wTme;
      wTme.vAsI8(wDatum.m_iI8);
      s32 sTme;
      wTme.vSprint(sTme.aStz());
      vTrace("Column[%lld %s] Tme : %s\n"_Stz,
             pColumn->iColumnNo(),pColumn->sColumnName().aStz(),sTme.aStz());
      break;
    }
    case DM_TSP: {
      Tsp wTsp;
      wTsp.vAsI8(wDatum.m_iI8);
      s32 sTsp;
      wTsp.vSprint(sTsp.aStz());
      vTrace("Column[%lld %s] Tsp : %s\n"_Stz,
             pColumn->iColumnNo(),pColumn->sColumnName().aStz(),sTsp.aStz());
      break;
    }
    case DM_VCH: {
      vTrace("Column[%lld %s] Vch : "_Stz,
             pColumn->iColumnNo(),pColumn->sColumnName().aStz());
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
