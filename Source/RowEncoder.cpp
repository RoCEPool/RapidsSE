/*============================================================================*/
/* RowEncoder.hpp -- Row Encoder                                              */
/*============================================================================*/

#include "Data.h"

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* RowEncoder::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_ROWENCODER);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void RowEncoder::operator delete(void* pAddr)
{
  RowEncoder* pRowEncoder = reinterpret_cast<RowEncoder*>(pAddr);
  
  HeapMem::vFree(pRowEncoder);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowEncoder::RowEncoder(RowParts* pRowParts)
: Object(CN_ROWENCODER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODER,_FOOTER_);
  
  m_i_CacheOffset = 8;
  
  m_pRowParts = pRowParts;
  
  m_pRowParts->vReset();
  
  _checkInitialised_(CN_ROWENCODER);
  
  i8 i_Head = m_pRowParts->iAllotHead();
  RASSERT(0 == i_Head);
  
  CacheLine* aCacheLine = m_pRowParts->aGet(i_Head);
  aCacheLine->vSetAll(0xFF);
  aCacheLine->v8B(0,0);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RowEncoder::~RowEncoder(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODER,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* print row decoded datum from cache line                                    */
/*----------------------------------------------------------------------------*/
void RowEncoder::vAsString(s48K& sDataString, ColumnVec* pColumnVec, CacheLine* aCacheLine)
{
  WordView wDatum;
  
  // get no of columns
  i8 nColumns = pColumnVec->nItems();
  
  // for each column
  for (i8 i_Column = 0; i_Column < nColumns; i_Column++) {
    
    // get the column at index
    Column* pColumn = pColumnVec->pGet(i_Column);
    
    // not the first column
    if (i_Column == 0)
      sDataString.vConcat(""_Stz);
    else
      sDataString.vConcat(","_Stz);
    
    // copy the data for the column into aDst
    eDM eDatum = pColumn->eDatumType();
    wDatum.m_wWord = wDecode(pColumn,aCacheLine);
    
    // check for nulls
    if (wDatum.m_iI8 == C_DATUM_NULL) {

      // NULL Floats ?
      RASSERT(eDatum != DM_FLT);

      sDataString.vCatSprint("null"_Stz);
      return;
    }
    
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
w8 RowEncoder::wDecode(Column* pColumn, CacheLine* aCacheLine)
{
  eDM eDatum  = pColumn->eDatumType();
  
  WordView wDecoded;
  
  switch (eDatum) {
    case DM_BCD: wDecoded.m_wWord = wDecodeBcd(pColumn,aCacheLine); break;
    case DM_BOL: wDecoded.m_wWord = wDecodeBol(pColumn,aCacheLine); break;
    case DM_DTE: wDecoded.m_wWord = wDecodeDte(pColumn,aCacheLine); break;
    case DM_FLT: wDecoded.m_wWord = wDecodeFlt(pColumn,aCacheLine); break;
    case DM_INT: wDecoded.m_wWord = wDecodeInt(pColumn,aCacheLine); break;
    case DM_IVL: wDecoded.m_wWord = wDecodeIvl(pColumn,aCacheLine); break;
    case DM_TME: wDecoded.m_wWord = wDecodeTme(pColumn,aCacheLine); break;
    case DM_TSP: wDecoded.m_wWord = wDecodeTsp(pColumn,aCacheLine); break;
    case DM_VCH: wDecoded.m_wWord = wDecodeVch(pColumn,aCacheLine); break;
    default:
      NEVER_REACH;
  }
  
  return wDecoded.m_wWord;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* encode given datum into cache line                                         */
/*----------------------------------------------------------------------------*/
void RowEncoder::vEncodeDatum(Column* pColumn, byte* aDatum)
{
  i8 i_Column = pColumn->iColumnNo();
  eDM eDatum  = pColumn->eDatumType();
  
  // don't encode columns 16+
  if (i_Column >= 16)
    return;
  
  switch (eDatum) {
    case DM_BCD: vEncodeBcd(pColumn,aDatum); return;
    case DM_BOL: vEncodeBol(pColumn,aDatum); return;
    case DM_DTE: vEncodeDte(pColumn,aDatum); return;
    case DM_FLT: vEncodeFlt(pColumn,aDatum); return;
    case DM_INT: vEncodeInt(pColumn,aDatum); return;
    case DM_IVL: vEncodeIvl(pColumn,aDatum); return;
    case DM_TME: vEncodeTme(pColumn,aDatum); return;
    case DM_TSP: vEncodeTsp(pColumn,aDatum); return;
    case DM_VCH: vEncodeVch(pColumn,aDatum); return;
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* print decoded datum from cache line                                        */
/*----------------------------------------------------------------------------*/
void RowEncoder::vPrint(Column* pColumn, CacheLine* aCacheLine)
{
  WordView wDatum;
  
  wDatum.m_wWord = wDecode(pColumn,aCacheLine);
  vPrint(pColumn,wDatum);
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* decode decimal from given cache line                                       */
/*----------------------------------------------------------------------------*/
w8 RowEncoder::wDecodeBcd(Column* pColumn, CacheLine* aCacheLine)
{
  i8 i_Column  = pColumn->iColumnNo();
  i8 i_Offset  = C_UNKNOWN;
  i8 iEncoding = iGetEncoding(i_Column,aCacheLine,i_Offset);
  
  WordView wDecoded;
  
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
w8 RowEncoder::wDecodeBol(Column* pColumn, CacheLine* aCacheLine)
{
  i8 i_Column  = pColumn->iColumnNo();
  i8 i_Offset  = C_UNKNOWN;
  i8 iEncoding = iGetEncoding(i_Column,aCacheLine,i_Offset);
  
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
w8 RowEncoder::wDecodeDte(Column* pColumn, CacheLine* aCacheLine)
{
  i8 i_Column  = pColumn->iColumnNo();
  i8 i_Offset  = C_UNKNOWN;
  i8 iEncoding = iGetEncoding(i_Column,aCacheLine,i_Offset);
  
  WordView wDecoded;
  
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
w8 RowEncoder::wDecodeFlt(Column* pColumn, CacheLine* aCacheLine)
{
  i8 i_Column  = pColumn->iColumnNo();
  i8 i_Offset  = C_UNKNOWN;
  i8 iEncoding = iGetEncoding(i_Column,aCacheLine,i_Offset);
  
  WordView wDecoded;
  
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
w8 RowEncoder::wDecodeInt(Column* pColumn, CacheLine* aCacheLine)
{
  i8 i_Column  = pColumn->iColumnNo();
  i8 i_Offset  = C_UNKNOWN;
  i8 iEncoding = iGetEncoding(i_Column,aCacheLine,i_Offset);
  
  WordView wDecoded;
  
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
w8 RowEncoder::wDecodeIvl(Column* pColumn, CacheLine* aCacheLine)
{
  i8 i_Column  = pColumn->iColumnNo();
  i8 i_Offset  = C_UNKNOWN;
  i8 iEncoding = iGetEncoding(i_Column,aCacheLine,i_Offset);
  
  WordView wDecoded;
  
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
w8 RowEncoder::wDecodeTme(Column* pColumn, CacheLine* aCacheLine)
{
  i8 i_Column  = pColumn->iColumnNo();
  i8 i_Offset  = C_UNKNOWN;
  i8 iEncoding = iGetEncoding(i_Column,aCacheLine,i_Offset);
  
  WordView wDecoded;
  
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
w8 RowEncoder::wDecodeTsp(Column* pColumn, CacheLine* aCacheLine)
{
  i8 i_Column  = pColumn->iColumnNo();
  i8 i_Offset  = C_UNKNOWN;
  i8 iEncoding = iGetEncoding(i_Column,aCacheLine,i_Offset);
  
  WordView wDecoded;
  
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
w8 RowEncoder::wDecodeVch(Column* pColumn, CacheLine* aCacheLine)
{
  i8 i_Column  = pColumn->iColumnNo();
  i8 i_Offset  = C_UNKNOWN;
  i8 iEncoding = iGetEncoding(i_Column,aCacheLine,i_Offset);
  
  WordView wDecoded;
  
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
/* encode decimal                                                             */
/*----------------------------------------------------------------------------*/
void RowEncoder::vEncodeBcd(Column* pColumn, byte* aDatum)
{
  CacheLine* aCacheLine = m_pRowParts->aGet(0);
  
  i8 i_Column = pColumn->iColumnNo();
  w8 wVal = *(i8*)aDatum;
  
  WordView wEncoded;
  wEncoded.m_wWord = wVal;
  
  i8 nBytesEncoded;
  i8 iEncoding = iBcdEncoding(wEncoded,nBytesEncoded);
  RASSERT(nBytesEncoded == nEncodedBytes(iEncoding,DM_BCD));

  // set 4bit meta data for column value
  aCacheLine->v1N(i_Column,iEncoding);
  
  // set n-byte data for column value
  if (nBytesEncoded)
    aCacheLine->vPut(m_i_CacheOffset,aDatum,iEncoding);
  
  m_i_CacheOffset += nBytesEncoded;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
  WordView wDecoded;
  wDecoded.m_wWord = RowEncoder::wDecode(pColumn,aCacheLine);
  RASSERT(wVal == wDecoded.m_iI8);
}

/*----------------------------------------------------------------------------*/
/* encode boolean                                                             */
/*----------------------------------------------------------------------------*/
void RowEncoder::vEncodeBol(Column* pColumn, byte* aDatum)
{
  CacheLine* aCacheLine = m_pRowParts->aGet(0);
  
  i8 i_Column = pColumn->iColumnNo();
  w8 wVal = *(i8*)aDatum;
  
  WordView wEncoded;
  wEncoded.m_wWord = wVal;
  
  i8 nBytesEncoded;
  i8 iEncoding = iBolEncoding(wEncoded,nBytesEncoded);
  RASSERT(nBytesEncoded == nEncodedBytes(iEncoding,DM_BOL));

  // set 4bit meta data for column value
  aCacheLine->v1N(i_Column,iEncoding);
  
  m_i_CacheOffset += nBytesEncoded;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
  WordView wDecoded;
  wDecoded.m_wWord = RowEncoder::wDecode(pColumn,aCacheLine);
  RASSERT(wVal == wDecoded.m_iI8);
}

/*----------------------------------------------------------------------------*/
/* encode date                                                                */
/*----------------------------------------------------------------------------*/
void RowEncoder::vEncodeDte(Column* pColumn, byte* aDatum)
{
  CacheLine* aCacheLine = m_pRowParts->aGet(0);
  
  i8 i_Column = pColumn->iColumnNo();
  w8 wVal = *(i8*)aDatum;
  
  WordView wEncoded;
  wEncoded.m_wWord = wVal;
  
  i8 nBytesEncoded;
  i8 iEncoding = RowEncoder::iDteEncoding(wEncoded,nBytesEncoded);
  RASSERT(nBytesEncoded == nEncodedBytes(iEncoding,DM_DTE));

  // set 4bit meta data for column value
  aCacheLine->v1N(i_Column,iEncoding);
  
  // set n-byte data for column value
  if (nBytesEncoded)
    aCacheLine->vPut(m_i_CacheOffset,(byte*)&wEncoded.m_iI8,iEncoding);
  
  m_i_CacheOffset += nBytesEncoded;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
  WordView wDecoded;
  wDecoded.m_wWord = RowEncoder::wDecode(pColumn,aCacheLine);
  RASSERT(wVal == wDecoded.m_iI8);
}

/*----------------------------------------------------------------------------*/
/* encode float                                                               */
/*----------------------------------------------------------------------------*/
void RowEncoder::vEncodeFlt(Column* pColumn, byte* aDatum)
{
  CacheLine* aCacheLine = m_pRowParts->aGet(0);
  
  i8 i_Column = pColumn->iColumnNo();
  w8 wVal = *(i8*)aDatum;
  
  WordView wEncoded;
  wEncoded.m_wWord = wVal;
  
  i8 nBytesEncoded;
  i8 iEncoding = iFltEncoding(wEncoded,nBytesEncoded);
  RASSERT(nBytesEncoded == nEncodedBytes(iEncoding,DM_FLT));

  // set 4bit meta data for column value
  aCacheLine->v1N(i_Column,iEncoding);
  
  // set n-byte data for column value
  if (nBytesEncoded)
    aCacheLine->vPut(m_i_CacheOffset,aDatum,iEncoding);
  
  m_i_CacheOffset += nBytesEncoded;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
  WordView wDecoded;
  wDecoded.m_wWord = RowEncoder::wDecode(pColumn,aCacheLine);
  RASSERT(wVal == wDecoded.m_iI8);
}

/*----------------------------------------------------------------------------*/
/* encode integer                                                             */
/*----------------------------------------------------------------------------*/
void RowEncoder::vEncodeInt(Column* pColumn, byte* aDatum)
{
  CacheLine* aCacheLine = m_pRowParts->aGet(0);
  
  i8 i_Column = pColumn->iColumnNo();
  w8 wVal = *(i8*)aDatum;
  
  WordView wEncoded;
  wEncoded.m_wWord = wVal;
  
  i8 nBytesEncoded;
  i8 iEncoding = iIntEncoding(wEncoded,nBytesEncoded);
  RASSERT(nBytesEncoded == nEncodedBytes(iEncoding,DM_INT));

  // set 4bit meta data for column value
  aCacheLine->v1N(i_Column,iEncoding);
  
  // set n-byte data for column value
  if (nBytesEncoded)
    aCacheLine->vPut(m_i_CacheOffset,aDatum,iEncoding);
  
  m_i_CacheOffset += nBytesEncoded;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
  WordView wDecoded;
  wDecoded.m_wWord = RowEncoder::wDecode(pColumn,aCacheLine);
  RASSERT(wVal == wDecoded.m_iI8);
}

/*----------------------------------------------------------------------------*/
/* encode interval                                                            */
/*----------------------------------------------------------------------------*/
void RowEncoder::vEncodeIvl(Column* pColumn, byte* aDatum)
{
  CacheLine* aCacheLine = m_pRowParts->aGet(0);
  
  i8 i_Column = pColumn->iColumnNo();
  w8 wVal = *(i8*)aDatum;
  
  WordView wEncoded;
  wEncoded.m_wWord = wVal;
  
  i8 nBytesEncoded;
  i8 iEncoding = iIvlEncoding(wEncoded,nBytesEncoded);
  RASSERT(nBytesEncoded == nEncodedBytes(iEncoding,DM_IVL));

  // set 4bit meta data for column value
  aCacheLine->v1N(i_Column,iEncoding);
  
  // set n-byte data for column value
  if (nBytesEncoded)
    aCacheLine->vPut(m_i_CacheOffset,aDatum,iEncoding);
  
  m_i_CacheOffset += nBytesEncoded;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
  WordView wDecoded;
  wDecoded.m_wWord = RowEncoder::wDecode(pColumn,aCacheLine);
  RASSERT(wVal == wDecoded.m_iI8);
}

/*----------------------------------------------------------------------------*/
/* encode time                                                                */
/*----------------------------------------------------------------------------*/
void RowEncoder::vEncodeTme(Column* pColumn, byte* aDatum)
{
  CacheLine* aCacheLine = m_pRowParts->aGet(0);
  
  i8 i_Column = pColumn->iColumnNo();
  w8 wVal = *(i8*)aDatum;
  
  WordView wEncoded;
  wEncoded.m_wWord = wVal;
  
  i8 nBytesEncoded;
  i8 iEncoding = iTmeEncoding(wEncoded,nBytesEncoded);
  RASSERT(nBytesEncoded == nEncodedBytes(iEncoding,DM_TME));

  // set 4bit meta data for column value
  aCacheLine->v1N(i_Column,iEncoding);
  
  // set n-byte data for column value
  if (nBytesEncoded)
    aCacheLine->vPut(m_i_CacheOffset,(byte*)&wEncoded.m_iI8,iEncoding);
  
  m_i_CacheOffset += nBytesEncoded;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
  WordView wDecoded;
  wDecoded.m_wWord = RowEncoder::wDecode(pColumn,aCacheLine);
  RASSERT(wVal == wDecoded.m_iI8);
}

/*----------------------------------------------------------------------------*/
/* encode timestamp                                                           */
/*----------------------------------------------------------------------------*/
void RowEncoder::vEncodeTsp(Column* pColumn, byte* aDatum)
{
  CacheLine* aCacheLine = m_pRowParts->aGet(0);
  
  i8 i_Column = pColumn->iColumnNo();
  w8 wVal = *(i8*)aDatum;
  
  WordView wEncoded;
  wEncoded.m_wWord = wVal;
  
  i8 nBytesEncoded;
  i8 iEncoding = iTspEncoding(wEncoded,nBytesEncoded);
  RASSERT(nBytesEncoded == nEncodedBytes(iEncoding,DM_TSP));

  // set 4bit meta data for column value
  aCacheLine->v1N(i_Column,iEncoding);
  
  // set n-byte data for column value
  if (nBytesEncoded)
    aCacheLine->vPut(m_i_CacheOffset,(byte*)&wEncoded.m_iI8,iEncoding);
  
  m_i_CacheOffset += nBytesEncoded;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
  WordView wDecoded;
  wDecoded.m_wWord = RowEncoder::wDecode(pColumn,aCacheLine);
  RASSERT(wVal == wDecoded.m_iI8);
}

/*----------------------------------------------------------------------------*/
/* encode varchar                                                             */
/*----------------------------------------------------------------------------*/
void RowEncoder::vEncodeVch(Column* pColumn, byte* aDatum)
{
  CacheLine* aCacheLine = m_pRowParts->aGet(0);
  
  i8 i_Column = pColumn->iColumnNo();
  w8 wVal = (w8)aDatum;
  
  WordView wEncoded;
  wEncoded.m_wWord = wVal;
  
  i8 nBytesEncoded;
  i8 iEncoding = iVchEncoding(wEncoded,nBytesEncoded);
  //RASSERT(nBytesEncoded == nEncodedBytes(iEncoding,DM_TSP));
  
  // set 4bit meta data for column value
  aCacheLine->v1N(i_Column,iEncoding);
  
  // set n-byte data for column value
  if (iEncoding == MN_SHOCO_2B) {
    // we would encode the shoco version of the string into a cache line
    
    // allot a tail cache line
    i8 i_Tail = m_pRowParts->iAllotTail();
    CacheLine* aShocoLine = m_pRowParts->aGet(i_Tail);

    // set data for column value as the index of the shoco cache line
    aCacheLine->vPut(m_i_CacheOffset,(byte*)&i_Tail,MN_2B);
    
    // compress varchar into tail shoco cache line
    i8 iLenCompressed =
      Compress::iIntoShoco(((byte*)aShocoLine)+8, C_BYTES_PER_CACHE_LINE-8,
                           (char*)aDatum, nBytesEncoded);

    // set length of the compressed string encoded into tail shoco cache line
    aShocoLine->v1N(0,iLenCompressed);
    
    MemLib::vWordDump(aShocoLine,sizeof(CacheLine));
    m_i_CacheOffset += 2;

//    s256 sDecomp;
//    i8 iLenDecompressed =
//      Compress::iFromShoco(sDecomp.aChar(), 255,
//                           ((byte*)aShocoLine)+8, C_BYTES_PER_CACHE_LINE-8);
//    sDecomp.aChar()[iLenDecompressed] = '\0';
//    vTrace("Decomp: %s\n"_Stz,sDecomp.aChar());
  }
  else {
    aCacheLine->vPut(m_i_CacheOffset,aDatum,nBytesEncoded);
    m_i_CacheOffset += nBytesEncoded;
  }


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
}

/*----------------------------------------------------------------------------*/
/* decoding of dte                                                            */
/*----------------------------------------------------------------------------*/
i8 RowEncoder::iDteDecoding(WordView& wTspView, i8 iEncoding)
{
  i8 iDecoded = wTspView.m_iI8;
  iDecoded <<= 40;
  
  return iDecoded;
}

/*----------------------------------------------------------------------------*/
/* decoding of tme                                                            */
/*----------------------------------------------------------------------------*/
i8 RowEncoder::iTmeDecoding(WordView& wTspView, i8 iEncoding)
{
  i8 iDecoded = wTspView.m_iI8;
  iDecoded <<= 40;
  
  return iDecoded;
}

/*----------------------------------------------------------------------------*/
/* decoding of tsp                                                            */
/*----------------------------------------------------------------------------*/
i8 RowEncoder::iTspDecoding(WordView& wTspView, i8 iEncoding)
{
  i8 iDecoded = wTspView.m_iI8;
  iDecoded <<= 24;
  
  return iDecoded;
}

/*----------------------------------------------------------------------------*/
/* encoding of bcd                                                            */
/*----------------------------------------------------------------------------*/
i8 RowEncoder::iBcdEncoding(WordView& wBcdView, i8& nBytesEncoded)
{
  i8 iEncoding;
  
  if (wBcdView.m_iI8 == C_DATUM_NULL) {
    iEncoding = MN_NULL;
    nBytesEncoded = 0;
  }
  else {
    iEncoding = IntLib::iByteMagnitude(wBcdView.m_iI8);
    nBytesEncoded = iEncoding;
  }
  
  return iEncoding;
}

/*----------------------------------------------------------------------------*/
/* encoding of bol                                                            */
/*----------------------------------------------------------------------------*/
i8 RowEncoder::iBolEncoding(WordView& wBolView, i8& nBytesEncoded)
{
  i8 iEncoding;
  
  if (wBolView.m_iI8 == C_DATUM_NULL)
    iEncoding = MN_NULL;
  else if (wBolView.m_iI8)
    iEncoding = MN_TRUE;
  else
    iEncoding = MN_FALSE;

  nBytesEncoded = 0;
  
  return iEncoding;
}

/*----------------------------------------------------------------------------*/
/* encoding of dte                                                            */
/*----------------------------------------------------------------------------*/
i8 RowEncoder::iDteEncoding(WordView& wDteView, i8& nBytesEncoded)
{
  i8 iEncoding;
  
  if (wDteView.m_iI8 == C_DATUM_NULL) {
    nBytesEncoded = 0;
    return MN_NULL;
  }
  
  bool bSawNonZero = true;
  
  while (! wDteView.m_yaByte[0]) {
    
    bSawNonZero = false;
    
    for (i8 i_Byte = 0; i_Byte < 7; i_Byte++)
      if ( (wDteView.m_yaByte[i_Byte] = wDteView.m_yaByte[i_Byte+1]) )
        bSawNonZero = true;
    wDteView.m_yaByte[7] = 0;
    
    if (! bSawNonZero)
      break;
  }
  
  iEncoding = IntLib::iByteMagnitude(wDteView.m_iI8);
  
  nBytesEncoded = iEncoding;
  
  return iEncoding;
}

/*----------------------------------------------------------------------------*/
/* encoding of flt                                                            */
/*----------------------------------------------------------------------------*/
i8 RowEncoder::iFltEncoding(WordView& wFltView, i8& nBytesEncoded)
{
  i8 iEncoding;
 
  if (wFltView.m_iI8 == C_DATUM_NULL) {
    iEncoding = MN_NULL;
    nBytesEncoded = 0;
  }
  else {
    iEncoding = IntLib::iByteMagnitude(wFltView.m_iI8);
    nBytesEncoded = iEncoding;
  }
  
  return iEncoding;
}

/*----------------------------------------------------------------------------*/
/* encoding of int                                                            */
/*----------------------------------------------------------------------------*/
i8 RowEncoder::iIntEncoding(WordView& wIntView, i8& nBytesEncoded)
{
  i8 iEncoding;
  
  if (wIntView.m_iI8 == C_DATUM_NULL) {
    iEncoding = MN_NULL;
    nBytesEncoded = 0;
  }
  else {
    iEncoding = IntLib::iByteMagnitude(wIntView.m_iI8);
    nBytesEncoded = iEncoding;
  }
  
  return iEncoding;
}

/*----------------------------------------------------------------------------*/
/* encoding of ivl                                                            */
/*----------------------------------------------------------------------------*/
i8 RowEncoder::iIvlEncoding(WordView& wIvlView, i8& nBytesEncoded)
{
  i8 iEncoding;

  if (wIvlView  .m_iI8 == C_DATUM_NULL) {
    iEncoding = MN_NULL;
    nBytesEncoded = 0;
  }
  else {
    iEncoding = IntLib::iByteMagnitude(wIvlView.m_iI8);
    nBytesEncoded = iEncoding;
  }
  
  return iEncoding;
}

/*----------------------------------------------------------------------------*/
/* encoding of tme                                                            */
/*----------------------------------------------------------------------------*/
i8 RowEncoder::iTmeEncoding(WordView& wTmeView, i8& nBytesEncoded)
{
  if (wTmeView.m_iI8 == C_DATUM_NULL) {
    nBytesEncoded = 0;
    return MN_NULL;
  }
  
  i8 iEncoding;
  
  bool bSawNonZero = true;
  
  while (! wTmeView.m_yaByte[0]) {
    
    bSawNonZero = false;
    
    for (i8 i_Byte = 0; i_Byte < 7; i_Byte++)
      if ( (wTmeView.m_yaByte[i_Byte] = wTmeView.m_yaByte[i_Byte+1]) )
        bSawNonZero = true;
    wTmeView.m_yaByte[7] = 0;
    
    if (! bSawNonZero)
      break;
  }
  
  iEncoding = IntLib::iByteMagnitude(wTmeView.m_iI8);
  
  nBytesEncoded = iEncoding;

  return iEncoding;
}

/*----------------------------------------------------------------------------*/
/* encoding of tsp                                                            */
/*----------------------------------------------------------------------------*/
i8 RowEncoder::iTspEncoding(WordView& wTspView, i8& nBytesEncoded)
{
  if (wTspView.m_iI8 == C_DATUM_NULL) {
    nBytesEncoded = 0;
    return MN_NULL;
  }
  
  i8 iEncoding;
  
  bool bSawNonZero = true;
  
  while (! wTspView.m_yaByte[0]) {
    
    bSawNonZero = false;
    
    for (i8 i_Byte = 0; i_Byte < 7; i_Byte++)
      if ( (wTspView.m_yaByte[i_Byte] = wTspView.m_yaByte[i_Byte+1]) )
        bSawNonZero = true;
    wTspView.m_yaByte[7] = 0;
    
    if (! bSawNonZero)
      break;
  }
  
  iEncoding = IntLib::iByteMagnitude(wTspView.m_iI8);
  
  nBytesEncoded = iEncoding;
  
  return iEncoding;
}

/*----------------------------------------------------------------------------*/
/* encoding of vch                                                            */
/*----------------------------------------------------------------------------*/
i8 RowEncoder::iVchEncoding(WordView& wVchView, i8& nBytesEncoded)
{
  i8 iEncoding;
  
  i8 iLen = StrLib::iLength((char*)wVchView.m_wWord);
  //RASSERT(iLen <= 8);
  
  if (iLen <= 8) {
    iEncoding = iLen;
    nBytesEncoded = iLen;
  }
  else {
    iEncoding = MN_SHOCO_2B;
    nBytesEncoded = iLen;
  }

  return iEncoding;
}

/*----------------------------------------------------------------------------*/
/* encoding of tsp                                                            */
/*----------------------------------------------------------------------------*/
i8 RowEncoder::iGetEncoding(i8 i_Column, CacheLine* aCacheLine, i8& i_Offset)
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
/* return number of bytes of encoding                                         */
/*----------------------------------------------------------------------------*/
i8 RowEncoder::nEncodedBytes(i8 iEncoding, eDM eDatum)
{
  if (iEncoding == MN_NULL)
    return 0;
  
  if (iEncoding <= MN_8B)
    return iEncoding;
  
  if (MN_OVERFLOW == iEncoding)
    return 0;
  
  switch (eDatum) {
      
    case DM_BCD:
      switch (iEncoding) {
        case MN_MINUS_ONE: return 0;
        case MN_ZERO:      return 0;
        case MN_PLUS_ONE:  return 0;
        default:
          NEVER_REACH;
      }
     break;
      
    case DM_BOL:
      switch (iEncoding) {
        case MN_FALSE: return 0;
        case MN_TRUE:  return 0;
        default:
          NEVER_REACH;
      }
      break;
      
    case DM_DTE:
      switch (iEncoding) {
        case MN_DIFF_1B: return 1;
        case MN_DIFF_2B: return 2;
        case MN_DIFF_3B: return 3;
        case MN_DIFF_4B: return 4;
        default:
          NEVER_REACH;
      }
      break;

    case DM_FLT:
      switch (iEncoding) {
        case MN_MINUS_ONE: return 0;
        case MN_ZERO:      return 0;
        case MN_PLUS_ONE:  return 0;
        default:
          NEVER_REACH;
      }
      break;
      
    case DM_INT:
      switch (iEncoding) {
        case MN_MINUS_ONE: return 0;
        case MN_ZERO:      return 0;
        case MN_PLUS_ONE:  return 0;
        default:
          NEVER_REACH;
      }
      break;
      
    case DM_IVL:
      switch (iEncoding) {
        case MN_DIFF_1B: return 1;
        case MN_DIFF_2B: return 2;
        case MN_DIFF_3B: return 3;
        case MN_DIFF_4B: return 4;
        default:
          NEVER_REACH;
      }
      break;
      
    case DM_TME:
      switch (iEncoding) {
        case MN_DIFF_1B: return 1;
        case MN_DIFF_2B: return 2;
        case MN_DIFF_3B: return 3;
        case MN_DIFF_4B: return 4;
        default:
          NEVER_REACH;
      }
      break;
      
    case DM_TSP:
      switch (iEncoding) {
        case MN_DIFF_1B: return 1;
        case MN_DIFF_2B: return 2;
        case MN_DIFF_3B: return 3;
        case MN_DIFF_4B: return 4;
        default:
          NEVER_REACH;
      }
      break;
      
    case DM_VCH:
      switch (iEncoding) {
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
/* print decoded datum from cache line                                        */
/*----------------------------------------------------------------------------*/
void RowEncoder::vPrint(Column* pColumn, WordView& wDatum)
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
Footer* RowEncoder::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
