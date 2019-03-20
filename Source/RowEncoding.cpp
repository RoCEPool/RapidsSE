/*============================================================================*/
/* RowEncoding.hpp -- Row Encoding                                            */
/*============================================================================*/

#include "Data.h"

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get encoded datum from cache line                                          */
/*----------------------------------------------------------------------------*/
w8 RowEncoding::wDecode(Column* pColumn, CacheLine* aCacheLine)
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
/* decode decimal from given cache line                                       */
/*----------------------------------------------------------------------------*/
w8 RowEncoding::wDecodeBcd(Column* pColumn, CacheLine* aCacheLine)
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
w8 RowEncoding::wDecodeBol(Column* pColumn, CacheLine* aCacheLine)
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
w8 RowEncoding::wDecodeDte(Column* pColumn, CacheLine* aCacheLine)
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
w8 RowEncoding::wDecodeFlt(Column* pColumn, CacheLine* aCacheLine)
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
w8 RowEncoding::wDecodeInt(Column* pColumn, CacheLine* aCacheLine)
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
w8 RowEncoding::wDecodeIvl(Column* pColumn, CacheLine* aCacheLine)
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
w8 RowEncoding::wDecodeTme(Column* pColumn, CacheLine* aCacheLine)
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
w8 RowEncoding::wDecodeTsp(Column* pColumn, CacheLine* aCacheLine)
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
w8 RowEncoding::wDecodeVch(Column* pColumn, CacheLine* aCacheLine)
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
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* begin encoding row into new cache line                                     */
/*----------------------------------------------------------------------------*/
void RowEncoding::vEncodeBegin(CacheLine* aCacheLine, i8& i_CacheOffset)
{
  aCacheLine->vSetAll(0xFF);
  aCacheLine->v8B(0,0);

  i_CacheOffset = 8;
}

/*----------------------------------------------------------------------------*/
/* set given datum in cache line                                              */
/*----------------------------------------------------------------------------*/
void RowEncoding::vEncode(Column* pColumn, byte* aDatum,
                          CacheLine* aCacheLine, i8& i_CacheOffset)
{
  i8 i_Column = pColumn->iColumnNo();
  eDM eDatum  = pColumn->eDatumType();
  
  // don't encode columns 16+
  if (i_Column >= 16)
    return;
  
  switch (eDatum) {
      
    case DM_BCD: {
      //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
      
      WordView wEncoded;
      i8 iEncoding;
      wEncoded.m_iI8 = RowEncoding::wEncodeBcd(pColumn,aDatum,iEncoding,aCacheLine,i_CacheOffset);
      
      w8 wVal = *(i8*)aDatum;
      //aCacheLine->v8B(8 + i_Column * 8,wVal);
      
      // set n-byte data for column value
      //aCacheLine->vPut(i_CacheOffset,aDatum,iEncoding);
      //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
      
      //i_CacheOffset += iEncoding;
      
      WordView wDecoded;
      wDecoded.m_wWord = RowEncoding::wDecode(pColumn,aCacheLine);
      //vTrace("i_Column %lld Bcd got: %lld\n"_Stz,i_Column,wDecoded.m_iI8);
      //vPrint(pColumn,wDecoded);
      RASSERT(wVal == wDecoded.m_iI8);
      
      return;
    }
      
    case DM_BOL: {
      //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
      
      WordView wEncoded;
      i8 iEncoding;
      wEncoded.m_iI8 = RowEncoding::wEncodeBol(pColumn,aDatum,iEncoding,aCacheLine,i_CacheOffset);

      w8 wVal = *(i8*)aDatum;
      
      WordView wDecoded;
      wDecoded.m_wWord = RowEncoding::wDecode(pColumn,aCacheLine);
      //vTrace("i_Column %lld Bol got: %lld\n"_Stz,i_Column,wDecoded.m_iI8);
      //vPrint(pColumn,wDecoded);
      RASSERT(wVal == wDecoded.m_iI8);
      
      return;
    }
      
    case DM_DTE: {
      //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
      
      WordView wEncoded;
      i8 iEncoding;
      wEncoded.m_iI8 = RowEncoding::wEncodeDte(pColumn,aDatum,iEncoding,aCacheLine,i_CacheOffset);

      w8 wVal = *(i8*)aDatum;
      //aCacheLine->v8B(8 + i_Column * 8,wVal);
      
      // set n-byte data for column value
      //aCacheLine->vPut(i_CacheOffset,(byte*)&wEncoded.m_iI8,iEncoding);
      //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
      
      //i_CacheOffset += iEncoding;
      
      WordView wDecoded;
      wDecoded.m_wWord = RowEncoding::wDecode(pColumn,aCacheLine);
      Dte wDte;
      wDte.vAsI8(wDecoded.m_iI8);
      
      s32 sDte;
      wDte.vSprint(sDte.aStz());
      
      //vTrace("i_Column %lld Dte got: %s\n"_Stz,i_Column,sDte.aStz());
      //vPrint(pColumn,wDecoded);
      RASSERT(wVal == wDecoded.m_iI8);
      
      
      return;
    }
      
    case DM_FLT: {
      //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
      
      WordView wEncoded;
      i8 iEncoding;
      wEncoded.m_iI8 = RowEncoding::wEncodeFlt(pColumn,aDatum,iEncoding,aCacheLine,i_CacheOffset);

      
      w8 wVal = *(i8*)aDatum;
      //aCacheLine->v8B(8 + i_Column * 8,wVal);
      
      // set n-byte data for column value
      //aCacheLine->vPut(i_CacheOffset,aDatum,iEncoding);
      //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
      
      //i_CacheOffset += iEncoding;
      
      WordView wDecoded;
      wDecoded.m_wWord = RowEncoding::wDecode(pColumn,aCacheLine);
      //vTrace("i_Column %lld Flt got: %llf\n"_Stz,i_Column,wDecoded.m_dF8);
      //vPrint(pColumn,wDecoded);
      RASSERT(wVal == wDecoded.m_iI8);
      
      return;
    }
      
    case DM_INT: {
      
      //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
      
      WordView wEncoded;
      i8 iEncoding;
      wEncoded.m_iI8 = RowEncoding::wEncodeInt(pColumn,aDatum,iEncoding,aCacheLine,i_CacheOffset);
      
      w8 wVal = *(i8*)aDatum;
      //aCacheLine->v8B(8 + i_Column * 8,wVal);
      
      // set n-byte data for column value
      //aCacheLine->vPut(i_CacheOffset,aDatum,iEncoding);
      //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
      
      //i_CacheOffset += iEncoding;
      
      WordView wDecoded;
      wDecoded.m_wWord = RowEncoding::wDecode(pColumn,aCacheLine);
      //vTrace("i_Column %lld Int got: %lld\n"_Stz,i_Column,wDecoded.m_iI8);
      //vPrint(pColumn,wDecoded);
      RASSERT(wVal == wDecoded.m_iI8);
      
      return;
    }
      
    case DM_IVL: {
      
      //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
      
      WordView wEncoded;
      i8 iEncoding;
      wEncoded.m_iI8 = RowEncoding::wEncodeIvl(pColumn,aDatum,iEncoding,aCacheLine,i_CacheOffset);

      w8 wVal = *(i8*)aDatum;
      //aCacheLine->v8B(8 + i_Column * 8,wVal);
      
      // set n-byte data for column value
      //aCacheLine->vPut(i_CacheOffset,aDatum,iEncoding);
      //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
      
      //i_CacheOffset += iEncoding;
      
      WordView wDecoded;
      wDecoded.m_wWord = RowEncoding::wDecode(pColumn,aCacheLine);
      //vTrace("i_Column %lld Ivl got: %lld\n"_Stz,i_Column,wDecoded.m_iI8);
      //vPrint(pColumn,wDecoded);
      RASSERT(wVal == wDecoded.m_iI8);
      
      return;
    }
      
    case DM_TME: {
      //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
      
      WordView wEncoded;
      i8 iEncoding;
      wEncoded.m_iI8 = RowEncoding::wEncodeTme(pColumn,aDatum,iEncoding,aCacheLine,i_CacheOffset);
      
      w8 wVal = *(i8*)aDatum;
      //aCacheLine->v8B(8 + i_Column * 8,wVal);
      
      // set n-byte data for column value
      //aCacheLine->vPut(i_CacheOffset,(byte*)&wEncoded.m_iI8,iEncoding);
      //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
      
      //i_CacheOffset += iEncoding;
      
      WordView wDecoded;
      wDecoded.m_wWord = RowEncoding::wDecode(pColumn,aCacheLine);
      Tme wTme;
      wTme.vAsI8(wDecoded.m_iI8);
      
      s32 sTme;
      wTme.vSprint(sTme.aStz());
      
      //vTrace("i_Column %lld Tme got: %s\n"_Stz,i_Column,sTme.aStz());
      //vPrint(pColumn,wDecoded);
      RASSERT(wVal == wDecoded.m_iI8);
      return;
    }
      
    case DM_TSP: {
      //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
      
      WordView wEncoded;
      i8 iEncoding;
      wEncoded.m_iI8 = RowEncoding::wEncodeTsp(pColumn,aDatum,iEncoding,aCacheLine,i_CacheOffset);
      
      w8 wVal = *(i8*)aDatum;
      //aCacheLine->v8B(8 + i_Column * 8,wVal);
      
      // set n-byte data for column value
      //aCacheLine->vPut(i_CacheOffset,(byte*)&wEncoded.m_iI8,iEncoding);
      //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
      
      //i_CacheOffset += iEncoding;
      
      WordView wDecoded;
      wDecoded.m_wWord = RowEncoding::wDecode(pColumn,aCacheLine);
      Tsp wTsp;
      wTsp.vAsI8(wDecoded.m_iI8);
      
      s32 sTsp;
      wTsp.vSprint(sTsp.aStz());
      
      //vTrace("i_Column %lld Tsp got: %s\n"_Stz,i_Column,sTsp.aStz());
      //vPrint(pColumn,wDecoded);
      RASSERT(wVal == wDecoded.m_iI8);
      
      return;
    }
      
    case DM_VCH: {
    
      WordView wEncoded;
      i8 iEncoding;
      wEncoded.m_iI8 = RowEncoding::wEncodeVch(pColumn,aDatum,iEncoding,aCacheLine,i_CacheOffset);

      //aCacheLine->vPut(i_CacheOffset,aDatum,iEncoding);
      //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
      
      //i_CacheOffset += iEncoding;
      
      WordView wDecoded;
      wDecoded.m_wWord = RowEncoding::wDecode(pColumn,aCacheLine);
//      vTrace("i_Column %lld Vch got: "_Stz,i_Column);
//      for (i8 i_Char = 0; i_Char < iEncoding; i_Char++)
//        vTrace("%c"_Stz,wDecoded.m_yaByte[i_Char]);
//      vTrace("\n"_Stz);
      
      //vPrint(pColumn,wDecoded);
      return;
    }
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* encode decimal                                                             */
/*----------------------------------------------------------------------------*/
w8 RowEncoding::wEncodeBcd(Column* pColumn,byte* aDatum, i8& iEncoding,
                           CacheLine* aCacheLine, i8& i_CacheOffset)
{
  i8 i_Column = pColumn->iColumnNo();
  w8 wVal = *(i8*)aDatum;
  
  WordView wEncoded;
  wEncoded.m_wWord = wVal;

  if (wVal == (u8)C_DATUM_NULL)
    iEncoding = MN_NULL;
  else
    iEncoding = IntLib::iByteMagnitude(wVal);
  
  // set 4bit meta data for column value
  aCacheLine->v1N(i_Column,iEncoding);

  // set n-byte data for column value
  aCacheLine->vPut(i_CacheOffset,aDatum,iEncoding);

  i_CacheOffset += iEncoding;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
  WordView wDecoded;
  wDecoded.m_wWord = RowEncoding::wDecode(pColumn,aCacheLine);
  RASSERT(wVal == wDecoded.m_iI8);

  return wEncoded.m_wWord;
}

/*----------------------------------------------------------------------------*/
/* encode boolean                                                             */
/*----------------------------------------------------------------------------*/
w8 RowEncoding::wEncodeBol(Column* pColumn,byte* aDatum, i8& iEncoding,
                           CacheLine* aCacheLine, i8& i_CacheOffset)
{
  i8 i_Column = pColumn->iColumnNo();
  w8 wVal = *(i8*)aDatum;
  
  WordView wEncoded;
  wEncoded.m_wWord = wVal;
  
  if (wVal == (u8)C_DATUM_NULL)
    iEncoding = MN_NULL;
  else if (wVal)
    iEncoding = MN_TRUE;
  else
    iEncoding = MN_FALSE;
  
  // set 4bit meta data for column value
  aCacheLine->v1N(i_Column,iEncoding);
  
  i_CacheOffset += 0;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
  WordView wDecoded;
  wDecoded.m_wWord = RowEncoding::wDecode(pColumn,aCacheLine);
  RASSERT(wVal == wDecoded.m_iI8);
  

  
  
  
  return wEncoded.m_wWord;
}

/*----------------------------------------------------------------------------*/
/* encode date                                                                */
/*----------------------------------------------------------------------------*/
w8 RowEncoding::wEncodeDte(Column* pColumn,byte* aDatum, i8& iEncoding,
                           CacheLine* aCacheLine, i8& i_CacheOffset)
{
  i8 i_Column = pColumn->iColumnNo();
  w8 wVal = *(i8*)aDatum;
  
  WordView wEncoded;
  wEncoded.m_wWord = wVal;

  if (wVal == (u8)C_DATUM_NULL)
    iEncoding = MN_NULL;
  else {
    wEncoded.m_iI8 = wVal;
    iEncoding = RowEncoding::iDteEncoding(wEncoded);
  }

  // set 4bit meta data for column value
  aCacheLine->v1N(i_Column,iEncoding);
  
  // set n-byte data for column value
  aCacheLine->vPut(i_CacheOffset,(byte*)&wEncoded.m_iI8,iEncoding);

  i_CacheOffset += iEncoding;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
  WordView wDecoded;
  wDecoded.m_wWord = RowEncoding::wDecode(pColumn,aCacheLine);
  RASSERT(wVal == wDecoded.m_iI8);
  

  
  
  return wEncoded.m_wWord;
}

/*----------------------------------------------------------------------------*/
/* encode float                                                               */
/*----------------------------------------------------------------------------*/
w8 RowEncoding::wEncodeFlt(Column* pColumn,byte* aDatum, i8& iEncoding,
                           CacheLine* aCacheLine, i8& i_CacheOffset)
{
  i8 i_Column = pColumn->iColumnNo();
  w8 wVal = *(i8*)aDatum;
  
  WordView wEncoded;
  wEncoded.m_wWord = wVal;
  
  if (wVal == (u8)C_DATUM_NULL)
    iEncoding = MN_NULL;
  else
    iEncoding = IntLib::iByteMagnitude(wVal);
  
  // set 4bit meta data for column value
  aCacheLine->v1N(i_Column,iEncoding);
  
  // set n-byte data for column value
  aCacheLine->vPut(i_CacheOffset,aDatum,iEncoding);

  i_CacheOffset += iEncoding;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
  WordView wDecoded;
  wDecoded.m_wWord = RowEncoding::wDecode(pColumn,aCacheLine);
  RASSERT(wVal == wDecoded.m_iI8);
  

  
  
  return wEncoded.m_wWord;
}

/*----------------------------------------------------------------------------*/
/* encode integer                                                             */
/*----------------------------------------------------------------------------*/
w8 RowEncoding::wEncodeInt(Column* pColumn,byte* aDatum, i8& iEncoding,
                           CacheLine* aCacheLine, i8& i_CacheOffset)
{
  i8 i_Column = pColumn->iColumnNo();
  w8 wVal = *(i8*)aDatum;
  
  WordView wEncoded;
  wEncoded.m_wWord = wVal;
  
  if (wVal == (u8)C_DATUM_NULL)
    iEncoding = MN_NULL;
  else
    iEncoding = IntLib::iByteMagnitude(wVal);

  // set 4bit meta data for column value
  aCacheLine->v1N(i_Column,iEncoding);
  
  // set n-byte data for column value
  aCacheLine->vPut(i_CacheOffset,aDatum,iEncoding);

  i_CacheOffset += iEncoding;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
  WordView wDecoded;
  wDecoded.m_wWord = RowEncoding::wDecode(pColumn,aCacheLine);
  RASSERT(wVal == wDecoded.m_iI8);
  

  
  
  return wEncoded.m_wWord;
}

/*----------------------------------------------------------------------------*/
/* encode interval                                                            */
/*----------------------------------------------------------------------------*/
w8 RowEncoding::wEncodeIvl(Column* pColumn,byte* aDatum, i8& iEncoding,
                           CacheLine* aCacheLine, i8& i_CacheOffset)
{
  i8 i_Column = pColumn->iColumnNo();
  w8 wVal = *(i8*)aDatum;
  
  WordView wEncoded;
  wEncoded.m_wWord = wVal;

  if (wVal == (u8)C_DATUM_NULL)
    iEncoding = MN_NULL;
  else
    iEncoding = IntLib::iByteMagnitude(wVal);
  
  // set 4bit meta data for column value
  aCacheLine->v1N(i_Column,iEncoding);
  
  // set n-byte data for column value
  aCacheLine->vPut(i_CacheOffset,aDatum,iEncoding);

  i_CacheOffset += iEncoding;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
  WordView wDecoded;
  wDecoded.m_wWord = RowEncoding::wDecode(pColumn,aCacheLine);
  RASSERT(wVal == wDecoded.m_iI8);
  

  
  
  return wEncoded.m_wWord;
}

/*----------------------------------------------------------------------------*/
/* encode time                                                                */
/*----------------------------------------------------------------------------*/
w8 RowEncoding::wEncodeTme(Column* pColumn,byte* aDatum, i8& iEncoding,
                           CacheLine* aCacheLine, i8& i_CacheOffset)
{
  i8 i_Column = pColumn->iColumnNo();
  w8 wVal = *(i8*)aDatum;
  
  WordView wEncoded;
  wEncoded.m_wWord = wVal;

  iEncoding = C_UNKNOWN;
  
  if (wVal == (u8)C_DATUM_NULL)
    iEncoding = MN_NULL;
  else {
    wEncoded.m_iI8 = wVal;
    iEncoding = RowEncoding::iTmeEncoding(wEncoded);
  }
  
  // set 4bit meta data for column value
  aCacheLine->v1N(i_Column,iEncoding);
  
  // set n-byte data for column value
  aCacheLine->vPut(i_CacheOffset,(byte*)&wEncoded.m_iI8,iEncoding);

  i_CacheOffset += iEncoding;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
  WordView wDecoded;
  wDecoded.m_wWord = RowEncoding::wDecode(pColumn,aCacheLine);
  RASSERT(wVal == wDecoded.m_iI8);
  

  
  
  return wEncoded.m_wWord;
}

/*----------------------------------------------------------------------------*/
/* encode timestamp                                                           */
/*----------------------------------------------------------------------------*/
w8 RowEncoding::wEncodeTsp(Column* pColumn,byte* aDatum, i8& iEncoding,
                           CacheLine* aCacheLine, i8& i_CacheOffset)
{
  i8 i_Column = pColumn->iColumnNo();
  w8 wVal = *(i8*)aDatum;
  
  WordView wEncoded;
  wEncoded.m_wWord = wVal;

  iEncoding = C_UNKNOWN;
  
  if (wVal == (u8)C_DATUM_NULL)
    iEncoding = MN_NULL;
  else {
    wEncoded.m_iI8 = wVal;
    iEncoding = RowEncoding::iTspEncoding(wEncoded);
  }
  
  // set 4bit meta data for column value
  aCacheLine->v1N(i_Column,iEncoding);
  
  // set n-byte data for column value
  aCacheLine->vPut(i_CacheOffset,(byte*)&wEncoded.m_iI8,iEncoding);

  i_CacheOffset += iEncoding;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
  WordView wDecoded;
  wDecoded.m_wWord = RowEncoding::wDecode(pColumn,aCacheLine);
  RASSERT(wVal == wDecoded.m_iI8);
  

  
  
  return wEncoded.m_wWord;
}

/*----------------------------------------------------------------------------*/
/* encode varchar                                                             */
/*----------------------------------------------------------------------------*/
w8 RowEncoding::wEncodeVch(Column* pColumn,byte* aDatum, i8& iEncoding,
                           CacheLine* aCacheLine, i8& i_CacheOffset)
{
  i8 i_Column = pColumn->iColumnNo();
  w8 wVal = *(i8*)aDatum;
  
  WordView wEncoded;
  wEncoded.m_wWord = wVal;
  
  iEncoding = C_UNKNOWN;
  
  i8 iLen = StrLib::iLength((char*)aDatum);
  RASSERT(iLen <= 8);
  
  iEncoding = iLen;
  
  // set 4bit meta data for column value
  aCacheLine->v1N(i_Column,iEncoding);
  
  // set n-byte data for column value
  aCacheLine->vPut(i_CacheOffset,aDatum,iEncoding);

  i_CacheOffset += iEncoding;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  

  
  
  return wEncoded.m_wWord;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* print decoded datum from cache line                                        */
/*----------------------------------------------------------------------------*/
void RowEncoding::vPrint(Column* pColumn, CacheLine* aCacheLine)
{
  WordView wDatum;
  
  wDatum.m_wWord = wDecode(pColumn,aCacheLine);
  vPrint(pColumn,wDatum);
}

/*----------------------------------------------------------------------------*/
/* print decoded datum from cache line                                        */
/*----------------------------------------------------------------------------*/
void RowEncoding::vPrint(Column* pColumn, WordView& wDatum)
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
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* decoding of dte                                                            */
/*----------------------------------------------------------------------------*/
i8 RowEncoding::iDteDecoding(WordView& wTspView, i8 iEncoding)
{
  i8 iDecoded = wTspView.m_iI8;
  iDecoded <<= 40;
  
  return iDecoded;
}

/*----------------------------------------------------------------------------*/
/* decoding of tme                                                            */
/*----------------------------------------------------------------------------*/
i8 RowEncoding::iTmeDecoding(WordView& wTspView, i8 iEncoding)
{
  i8 iDecoded = wTspView.m_iI8;
  iDecoded <<= 40;
  
  return iDecoded;
}

/*----------------------------------------------------------------------------*/
/* decoding of tsp                                                            */
/*----------------------------------------------------------------------------*/
i8 RowEncoding::iTspDecoding(WordView& wTspView, i8 iEncoding)
{
  i8 iDecoded = wTspView.m_iI8;
  iDecoded <<= 24;
  
  return iDecoded;
}

/*----------------------------------------------------------------------------*/
/* encoding of dte                                                            */
/*----------------------------------------------------------------------------*/
i8 RowEncoding::iDteEncoding(WordView& wTspView)
{
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
  
  i8 iEncoding = IntLib::iByteMagnitude(wTspView.m_iI8);
  
  return iEncoding;
  
}

/*----------------------------------------------------------------------------*/
/* encoding of tme                                                            */
/*----------------------------------------------------------------------------*/
i8 RowEncoding::iTmeEncoding(WordView& wTspView)
{
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
  
  i8 iEncoding = IntLib::iByteMagnitude(wTspView.m_iI8);
  
  return iEncoding;
  
}

/*----------------------------------------------------------------------------*/
/* encoding of tsp                                                            */
/*----------------------------------------------------------------------------*/
i8 RowEncoding::iTspEncoding(WordView& wTspView)
{
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
  
  i8 iEncoding = IntLib::iByteMagnitude(wTspView.m_iI8);
  
  return iEncoding;
  
}

/*----------------------------------------------------------------------------*/
/* encoding of tsp                                                            */
/*----------------------------------------------------------------------------*/
i8 RowEncoding::iGetEncoding(i8 i_Column, CacheLine* aCacheLine, i8& i_Offset)
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
i8 RowEncoding::nEncodedBytes(i8 iEncoding, eDM eDatum)
{
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
        case MN_SHOCOL_CL:   NEVER_REACH; return 0;
        case MN_SHOCOL_2B:   NEVER_REACH; return 0;
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
