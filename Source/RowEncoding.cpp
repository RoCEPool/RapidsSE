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
  i8 i_Column = pColumn->iColumnNo();
  eDM eDatum  = pColumn->eDatumType();
  
  i8 i_Offset = 8;
  for (i8 i_Nibble = 0; i_Nibble < i_Column; i_Nibble++) {
    i8 nDataBytes = aCacheLine->w8_1N(i_Nibble);
    i_Offset += nDataBytes;
  }
  
  i8 iEncoding = aCacheLine->w8_1N(i_Column);
  
  w8 wVal = C_UNKNOWN;
  
  if (DM_BOL == eDatum) {
    switch (iEncoding) {
      case MN_FALSE: wVal = false; break;
      case MN_TRUE : wVal = true;  break;
      default:
        NEVER_REACH;
    }
    return wVal;
  }
  
  switch (iEncoding) {
      
    case 0: wVal = C_DATUM_NULL;                break;
    case 1: wVal = aCacheLine->w8_1B(i_Offset); break;
    case 2: wVal = aCacheLine->w8_2B(i_Offset); break;
    case 3: wVal = aCacheLine->w8_3B(i_Offset); break;
    case 4: wVal = aCacheLine->w8_4B(i_Offset); break;
    case 5: wVal = aCacheLine->w8_5B(i_Offset); break;
    case 6: wVal = aCacheLine->w8_6B(i_Offset); break;
    case 7: wVal = aCacheLine->w8_7B(i_Offset); break;
    case 8: wVal = aCacheLine->w8_8B(i_Offset); break;
      
    case MN_FALSE: wVal = FALSE;                break;
    case MN_TRUE : wVal = FALSE;                break;
      
    default:
      NEVER_REACH;
  }
  
  
  return wVal;
}

/*----------------------------------------------------------------------------*/
/* decode decimal                                                             */
/*----------------------------------------------------------------------------*/
void RowEncoding::vDecodeBcd(Column* pColumn, CacheLine* aCacheLine, WordView& wDecoded)
{
  i8 i_Column  = pColumn->iColumnNo();
  i8 i_Offset  = C_UNKNOWN;
  i8 iEncoding = iGetEncoding(i_Column,aCacheLine,i_Offset);

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

}

/*----------------------------------------------------------------------------*/
/* decode bolean                                                              */
/*----------------------------------------------------------------------------*/
void RowEncoding::vDecodeBol(Column* pColumn, CacheLine* aCacheLine, WordView& wDecoded)
{
  i8 i_Column  = pColumn->iColumnNo();
  i8 i_Offset  = C_UNKNOWN;
  i8 iEncoding = iGetEncoding(i_Column,aCacheLine,i_Offset);
  
  switch (iEncoding) {
      
    case MN_NULL:  wDecoded.m_iI8 = C_DATUM_NULL; break;
    case MN_FALSE: wDecoded.m_iI8 = 0;            break;
    case MN_TRUE:  wDecoded.m_iI8 = 1;            break;

    default:
      NEVER_REACH;
  }
  
}

/*----------------------------------------------------------------------------*/
/* decode date                                                                */
/*----------------------------------------------------------------------------*/
void RowEncoding::vDecodeDte(Column* pColumn, CacheLine* aCacheLine, WordView& wDecoded)
{
  i8 i_Column  = pColumn->iColumnNo();
  i8 i_Offset  = C_UNKNOWN;
  i8 iEncoding = iGetEncoding(i_Column,aCacheLine,i_Offset);
  
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
}

/*----------------------------------------------------------------------------*/
/* decode float                                                               */
/*----------------------------------------------------------------------------*/
void RowEncoding::vDecodeFlt(Column* pColumn, CacheLine* aCacheLine, WordView& wDecoded)
{
  i8 i_Column  = pColumn->iColumnNo();
  i8 i_Offset  = C_UNKNOWN;
  i8 iEncoding = iGetEncoding(i_Column,aCacheLine,i_Offset);
  
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
  
}

/*----------------------------------------------------------------------------*/
/* decode integer                                                             */
/*----------------------------------------------------------------------------*/
void RowEncoding::vDecodeInt(Column* pColumn, CacheLine* aCacheLine, WordView& wDecoded)
{
  i8 i_Column  = pColumn->iColumnNo();
  i8 i_Offset  = C_UNKNOWN;
  i8 iEncoding = iGetEncoding(i_Column,aCacheLine,i_Offset);
  
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
  
}

/*----------------------------------------------------------------------------*/
/* decode interval                                                            */
/*----------------------------------------------------------------------------*/
void RowEncoding::vDecodeIvl(Column* pColumn, CacheLine* aCacheLine, WordView& wDecoded)
{
  i8 i_Column  = pColumn->iColumnNo();
  i8 i_Offset  = C_UNKNOWN;
  i8 iEncoding = iGetEncoding(i_Column,aCacheLine,i_Offset);
  
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
  
}

/*----------------------------------------------------------------------------*/
/* decode time                                                                */
/*----------------------------------------------------------------------------*/
void RowEncoding::vDecodeTme(Column* pColumn, CacheLine* aCacheLine, WordView& wDecoded)
{
  i8 i_Column  = pColumn->iColumnNo();
  i8 i_Offset  = C_UNKNOWN;
  i8 iEncoding = iGetEncoding(i_Column,aCacheLine,i_Offset);
  
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

}

/*----------------------------------------------------------------------------*/
/* decode timestamp                                                           */
/*----------------------------------------------------------------------------*/
void RowEncoding::vDecodeTsp(Column* pColumn, CacheLine* aCacheLine, WordView& wDecoded)
{
  i8 i_Column  = pColumn->iColumnNo();
  i8 i_Offset  = C_UNKNOWN;
  i8 iEncoding = iGetEncoding(i_Column,aCacheLine,i_Offset);
  
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
  
}

/*----------------------------------------------------------------------------*/
/* decode varchar                                                             */
/*----------------------------------------------------------------------------*/
void RowEncoding::vDecodeVch(Column* pColumn, CacheLine* aCacheLine, WordView& wDecoded)
{
  i8 i_Column  = pColumn->iColumnNo();
  i8 i_Offset  = C_UNKNOWN;
  i8 iEncoding = iGetEncoding(i_Column,aCacheLine,i_Offset);
  
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
      i8 iEncoding = RowEncoding::iEncode(pColumn,aDatum,wEncoded);
      
      // set 4bit meta data for column value
      aCacheLine->v1N(i_Column,iEncoding);
      //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
      
      w8 wVal = *(i8*)aDatum;
      //aCacheLine->v8B(8 + i_Column * 8,wVal);
      
      // set n-byte data for column value
      aCacheLine->vPut(i_CacheOffset,aDatum,iEncoding);
      MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
      
      i_CacheOffset += iEncoding;
      
      WordView wDecoded;
      RowEncoding::vDecodeBcd(pColumn,aCacheLine,wDecoded);
      vTrace("i_Column %lld Bcd got: %lld\n"_Stz,i_Column,wDecoded.m_iI8);
      RASSERT(wVal == wDecoded.m_iI8);
      
      return;
    }
      
    case DM_BOL: {
      //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
      
      WordView wEncoded;
      i8 iEncoding = RowEncoding::iEncode(pColumn,aDatum,wEncoded);
      
      // set 4bit meta data for column value
      aCacheLine->v1N(i_Column,iEncoding);
      //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
      w8 wVal = *(i8*)aDatum;
      
      WordView wDecoded;
      RowEncoding::vDecodeBol(pColumn,aCacheLine,wDecoded);
      vTrace("i_Column %lld Bol got: %lld\n"_Stz,i_Column,wDecoded.m_iI8);
      RASSERT(wVal == wDecoded.m_iI8);
      
      return;
    }
      
    case DM_DTE: {
      //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
      
      WordView wEncoded;
      i8 iEncoding = RowEncoding::iEncode(pColumn,aDatum,wEncoded);
      
      // set 4bit meta data for column value
      aCacheLine->v1N(i_Column,iEncoding);
      //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
      
      w8 wVal = *(i8*)aDatum;
      //aCacheLine->v8B(8 + i_Column * 8,wVal);
      
      // set n-byte data for column value
      aCacheLine->vPut(i_CacheOffset,(byte*)&wEncoded.m_iI8,iEncoding);
      MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
      
      i_CacheOffset += iEncoding;
      
      WordView wDecoded;
      RowEncoding::vDecodeDte(pColumn,aCacheLine,wDecoded);
      Dte wDte;
      wDte.vAsI8(wDecoded.m_iI8);
      
      s32 sDte;
      wDte.vSprint(sDte.aStz());
      
      vTrace("i_Column %lld Dte got: %s\n"_Stz,i_Column,sDte.aStz());
      RASSERT(wVal == wDecoded.m_iI8);
      
      
      return;
    }
      
    case DM_FLT: {
      //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
      
      WordView wEncoded;
      i8 iEncoding = RowEncoding::iEncode(pColumn,aDatum,wEncoded);
      
      // set 4bit meta data for column value
      aCacheLine->v1N(i_Column,iEncoding);
      //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
      
      w8 wVal = *(i8*)aDatum;
      //aCacheLine->v8B(8 + i_Column * 8,wVal);
      
      // set n-byte data for column value
      aCacheLine->vPut(i_CacheOffset,aDatum,iEncoding);
      MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
      
      i_CacheOffset += iEncoding;
      
      WordView wDecoded;
      RowEncoding::vDecodeFlt(pColumn,aCacheLine,wDecoded);
      vTrace("i_Column %lld Flt got: %llf\n"_Stz,i_Column,wDecoded.m_dF8);
      RASSERT(wVal == wDecoded.m_iI8);
      
      return;
    }
      
    case DM_INT: {
      
      //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
      
      WordView wEncoded;
      i8 iEncoding = RowEncoding::iEncode(pColumn,aDatum,wEncoded);
      
      // set 4bit meta data for column value
      aCacheLine->v1N(i_Column,iEncoding);
      //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
      
      w8 wVal = *(i8*)aDatum;
      //aCacheLine->v8B(8 + i_Column * 8,wVal);
      
      // set n-byte data for column value
      aCacheLine->vPut(i_CacheOffset,aDatum,iEncoding);
      MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
      
      i_CacheOffset += iEncoding;
      
      WordView wDecoded;
      RowEncoding::vDecodeInt(pColumn,aCacheLine,wDecoded);
      vTrace("i_Column %lld Int got: %lld\n"_Stz,i_Column,wDecoded.m_iI8);
      RASSERT(wVal == wDecoded.m_iI8);
      
      return;
    }
      
    case DM_IVL: {
      
      //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
      
      WordView wEncoded;
      i8 iEncoding = RowEncoding::iEncode(pColumn,aDatum,wEncoded);
      
      // set 4bit meta data for column value
      aCacheLine->v1N(i_Column,iEncoding);
      //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
      
      w8 wVal = *(i8*)aDatum;
      //aCacheLine->v8B(8 + i_Column * 8,wVal);
      
      // set n-byte data for column value
      aCacheLine->vPut(i_CacheOffset,aDatum,iEncoding);
      MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
      
      i_CacheOffset += iEncoding;
      
      WordView wDecoded;
      RowEncoding::vDecodeIvl(pColumn,aCacheLine,wDecoded);
      vTrace("i_Column %lld Ivl got: %lld\n"_Stz,i_Column,wDecoded.m_iI8);
      RASSERT(wVal == wDecoded.m_iI8);
      
      return;
    }
      
    case DM_TME: {
      //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
      
      WordView wEncoded;
      i8 iEncoding = RowEncoding::iEncode(pColumn,aDatum,wEncoded);
      
      // set 4bit meta data for column value
      aCacheLine->v1N(i_Column,iEncoding);
      //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
      
      w8 wVal = *(i8*)aDatum;
      //aCacheLine->v8B(8 + i_Column * 8,wVal);
      
      // set n-byte data for column value
      aCacheLine->vPut(i_CacheOffset,(byte*)&wEncoded.m_iI8,iEncoding);
      MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
      
      i_CacheOffset += iEncoding;
      
      WordView wDecoded;
      RowEncoding::vDecodeTme(pColumn,aCacheLine,wDecoded);
      Tme wTme;
      wTme.vAsI8(wDecoded.m_iI8);
      
      s32 sTme;
      wTme.vSprint(sTme.aStz());
      
      vTrace("i_Column %lld Tme got: %s\n"_Stz,i_Column,sTme.aStz());
      RASSERT(wVal == wDecoded.m_iI8);
      return;
    }
      
    case DM_TSP: {
      //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
      
      WordView wEncoded;
      i8 iEncoding = RowEncoding::iEncode(pColumn,aDatum,wEncoded);
      
      // set 4bit meta data for column value
      aCacheLine->v1N(i_Column,iEncoding);
      //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
      
      w8 wVal = *(i8*)aDatum;
      //aCacheLine->v8B(8 + i_Column * 8,wVal);
      
      // set n-byte data for column value
      aCacheLine->vPut(i_CacheOffset,(byte*)&wEncoded.m_iI8,iEncoding);
      MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
      
      i_CacheOffset += iEncoding;
      
      WordView wDecoded;
      RowEncoding::vDecodeTsp(pColumn,aCacheLine,wDecoded);
      Tsp wTsp;
      wTsp.vAsI8(wDecoded.m_iI8);
      
      s32 sTsp;
      wTsp.vSprint(sTsp.aStz());
      
      vTrace("i_Column %lld Tsp got: %s\n"_Stz,i_Column,sTsp.aStz());
      RASSERT(wVal == wDecoded.m_iI8);
      
      return;
    }
      
    case DM_VCH: {
    
      WordView wEncoded;
      i8 iEncoding = RowEncoding::iEncode(pColumn,aDatum,wEncoded);
      
      // set 4bit meta data for column value
      aCacheLine->v1N(i_Column,iEncoding);
      //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
      
      aCacheLine->vPut(i_CacheOffset,aDatum,iEncoding);
      MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
      
      i_CacheOffset += iEncoding;
      
      WordView wDecoded;
      RowEncoding::vDecodeVch(pColumn,aCacheLine,wDecoded);
      vTrace("i_Column %lld Vch got: "_Stz,i_Column);
      for (i8 i_Char = 0; i_Char < iEncoding; i_Char++)
        vTrace("%c"_Stz,wDecoded.m_yaByte[i_Char]);
      vTrace("\n"_Stz);
      
      return;
    }
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* encode given datum before setting in cache line                            */
/*----------------------------------------------------------------------------*/
i8 RowEncoding::iEncode(Column* pColumn, byte* aDatum, WordView& wEncoded)
{
  eDM eDatum = pColumn->eDatumType();
  
  w8 wVal = *(i8*)aDatum;
  
  i8 iEncoding = -1;
  
  switch (eDatum) {
      
    case DM_BCD:
      if (wVal == (u8)C_DATUM_NULL)
        iEncoding = MN_NULL;
      else
        iEncoding = IntLib::iByteMagnitude(wVal);
      break;
      
    case DM_BOL:
      if (wVal == (u8)C_DATUM_NULL)
        iEncoding = MN_NULL;
      else if (wVal)
        iEncoding = MN_TRUE;
      else
        iEncoding = MN_FALSE;
      break;
      
    case DM_DTE:
      if (wVal == (u8)C_DATUM_NULL)
        iEncoding = MN_NULL;
      else {
        wEncoded.m_iI8 = wVal;
        iEncoding = iTspEncoding(wEncoded);
      }
      break;
      
    case DM_FLT:
      if (wVal == (u8)C_DATUM_NULL)
        iEncoding = MN_NULL;
      else
        iEncoding = IntLib::iByteMagnitude(wVal);
      break;
      
    case DM_INT:
      if (wVal == (u8)C_DATUM_NULL)
        iEncoding = MN_NULL;
      else
        iEncoding = IntLib::iByteMagnitude(wVal);
      break;
      
    case DM_IVL:
      if (wVal == (u8)C_DATUM_NULL)
        iEncoding = MN_NULL;
      else
        iEncoding = IntLib::iByteMagnitude(wVal);
      break;
      
    case DM_TME:
      if (wVal == (u8)C_DATUM_NULL)
        iEncoding = MN_NULL;
      else {
        wEncoded.m_iI8 = wVal;
        iEncoding = iTspEncoding(wEncoded);
      }
      break;
      
    case DM_TSP:
      if (wVal == (u8)C_DATUM_NULL)
        iEncoding = MN_NULL;
      else {
        wEncoded.m_iI8 = wVal;
        iEncoding = iTspEncoding(wEncoded);
      }
      break;
      
    case DM_VCH: {
      i8 iLen = StrLib::iLength((char*)aDatum);
      RASSERT(iLen <= 8);
      iEncoding = iLen;
      break;
    }
      
    default:
      NEVER_REACH;
  }
  
  return iEncoding;
}

/*----------------------------------------------------------------------------*/
/* encode decimal                                                             */
/*----------------------------------------------------------------------------*/
i8 RowEncoding::iEncodeBcd(Column* pColumn,byte* aDatum, WordView& wEncoded)
{
  w8 wVal = *(i8*)aDatum;
  
  i8 iEncoding = C_UNKNOWN;
  
  if (wVal == (u8)C_DATUM_NULL)
    iEncoding = MN_NULL;
  else
    iEncoding = IntLib::iByteMagnitude(wVal);

  return iEncoding;
}

/*----------------------------------------------------------------------------*/
/* encode boolean                                                             */
/*----------------------------------------------------------------------------*/
i8 RowEncoding::iEncodeBol(Column* pColumn,byte* aDatum, WordView& wEncoded)
{
  w8 wVal = *(i8*)aDatum;
  
  i8 iEncoding = C_UNKNOWN;
  
  if (wVal == (u8)C_DATUM_NULL)
    iEncoding = MN_NULL;
  else if (wVal)
    iEncoding = MN_TRUE;
  else
    iEncoding = MN_FALSE;
  
  return iEncoding;
}

/*----------------------------------------------------------------------------*/
/* encode date                                                                */
/*----------------------------------------------------------------------------*/
i8 RowEncoding::iEncodeDte(Column* pColumn,byte* aDatum, WordView& wEncoded)
{
  w8 wVal = *(i8*)aDatum;
  
  i8 iEncoding = C_UNKNOWN;

  if (wVal == (u8)C_DATUM_NULL)
    iEncoding = MN_NULL;
  else {
    wEncoded.m_iI8 = wVal;
    iEncoding = RowEncoding::iDteEncoding(wEncoded);
  }

  return iEncoding;
}

/*----------------------------------------------------------------------------*/
/* encode float                                                               */
/*----------------------------------------------------------------------------*/
i8 RowEncoding::iEncodeFlt(Column* pColumn,byte* aDatum, WordView& wEncoded)
{
  w8 wVal = *(i8*)aDatum;
  
  i8 iEncoding = C_UNKNOWN;
  
  if (wVal == (u8)C_DATUM_NULL)
    iEncoding = MN_NULL;
  else
    iEncoding = IntLib::iByteMagnitude(wVal);
  
  return iEncoding;
}

/*----------------------------------------------------------------------------*/
/* encode integer                                                             */
/*----------------------------------------------------------------------------*/
i8 RowEncoding::iEncodeInt(Column* pColumn,byte* aDatum, WordView& wEncoded)
{
  w8 wVal = *(i8*)aDatum;
  
  i8 iEncoding = C_UNKNOWN;
  
  if (wVal == (u8)C_DATUM_NULL)
    iEncoding = MN_NULL;
  else
    iEncoding = IntLib::iByteMagnitude(wVal);

  return iEncoding;
}

/*----------------------------------------------------------------------------*/
/* encode interval                                                            */
/*----------------------------------------------------------------------------*/
i8 RowEncoding::iEncodeIvl(Column* pColumn,byte* aDatum, WordView& wEncoded)
{
  w8 wVal = *(i8*)aDatum;
  
  i8 iEncoding = C_UNKNOWN;
  
  if (wVal == (u8)C_DATUM_NULL)
    iEncoding = MN_NULL;
  else
    iEncoding = IntLib::iByteMagnitude(wVal);
  
  return iEncoding;
}

/*----------------------------------------------------------------------------*/
/* encode time                                                                */
/*----------------------------------------------------------------------------*/
i8 RowEncoding::iEncodeTme(Column* pColumn,byte* aDatum, WordView& wEncoded)
{
  w8 wVal = *(i8*)aDatum;
  
  i8 iEncoding = C_UNKNOWN;
  
  if (wVal == (u8)C_DATUM_NULL)
    iEncoding = MN_NULL;
  else {
    wEncoded.m_iI8 = wVal;
    iEncoding = RowEncoding::iTmeEncoding(wEncoded);
  }
  
  return iEncoding;
}

/*----------------------------------------------------------------------------*/
/* encode timestamp                                                           */
/*----------------------------------------------------------------------------*/
i8 RowEncoding::iEncodeTsp(Column* pColumn,byte* aDatum, WordView& wEncoded)
{
  w8 wVal = *(i8*)aDatum;
  
  i8 iEncoding = C_UNKNOWN;
  
  if (wVal == (u8)C_DATUM_NULL)
    iEncoding = MN_NULL;
  else {
    wEncoded.m_iI8 = wVal;
    iEncoding = RowEncoding::iTspEncoding(wEncoded);
  }
  
  return iEncoding;
}

/*----------------------------------------------------------------------------*/
/* encode varchar                                                             */
/*----------------------------------------------------------------------------*/
i8 RowEncoding::iEncodeVch(Column* pColumn,byte* aDatum, WordView& wEncoded)
{
  i8 iEncoding = C_UNKNOWN;
  
  i8 iLen = StrLib::iLength((char*)aDatum);
  RASSERT(iLen <= 8);
  
  iEncoding = iLen;
  
  return iEncoding;
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
