/*============================================================================*/
/* Datum.cpp -- datum                                                         */
/*============================================================================*/

#include "Datums.h"

#include "StorageEngine.h"

static const i8 C_CACHELINES = C_64KB;

CacheLine xaCacheLine[C_CACHELINES];

i8 i_CurrentCacheLine = -1;
i8 i_CacheOffset = -1;

/*----------------------------------------------------------------------------*/
/* begin encoding into new cache line                                         */
/*----------------------------------------------------------------------------*/
void Datum::vBegin(void)
{
  i_CurrentCacheLine++;
  i_CacheOffset = 8;

  CacheLine* aCacheLine = &xaCacheLine[i_CurrentCacheLine];
  aCacheLine->vSetAll(0xFF);
  aCacheLine->v8B(0,0);
}

/*----------------------------------------------------------------------------*/
/* set given datum in cache line                                              */
/*----------------------------------------------------------------------------*/
void Datum::vSet(Column* pColumn, i8 i_Column, eDM eDatum, byte* aDatum)
{
  // don't encode columns 16+
  if (i_Column >= 16)
    return;

  w8 wVal;
  i8 iEncoding;
  WordView wEncoded;
  
  CacheLine* aCacheLine = &xaCacheLine[i_CurrentCacheLine];
  
  if (DM_BCD == eDatum) {
    //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
    
    iEncoding = RowEncoding::iEncode(pColumn,aDatum,wEncoded);
    
    // set 4bit meta data for column value
    aCacheLine->v1N(i_Column,iEncoding);
    //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
    
    wVal = *(i8*)aDatum;
    //aCacheLine->v8B(8 + i_Column * 8,wVal);
    
    // set n-byte data for column value
    vPut(i_CacheOffset,aDatum,iEncoding);
    MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
    
    i_CacheOffset += iEncoding;
    
    w8 wGot = wGet(i_Column,eDatum);
    vTrace("i_Column %lld got: %lld\n"_Stz,i_Column,wGot);
    
    WordView wDecoded;
    RowEncoding::vDecodeBcd(pColumn,aCacheLine,wDecoded);
    vTrace("i_Column %lld Bcd got: %lld\n"_Stz,i_Column,wDecoded.m_iI8);
    RASSERT(wVal == wDecoded.m_iI8);
    
    return;
  }
  
  if (DM_BOL == eDatum) {
    //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
    
    iEncoding = RowEncoding::iEncode(pColumn,aDatum,wEncoded);
    
    // set 4bit meta data for column value
    aCacheLine->v1N(i_Column,iEncoding);
    //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
    wVal = *(i8*)aDatum;

    w8 wGot = wGet(i_Column,eDatum);
    vTrace("i_Column %lld got: %lld\n"_Stz,i_Column,wGot);
    
    WordView wDecoded;
    RowEncoding::vDecodeBol(pColumn,aCacheLine,wDecoded);
    vTrace("i_Column %lld Bol got: %lld\n"_Stz,i_Column,wDecoded.m_iI8);
    RASSERT(wVal == wDecoded.m_iI8);
    
    return;
  }
  
  if (DM_DTE == eDatum) {
    //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
    
    iEncoding = RowEncoding::iEncode(pColumn,aDatum,wEncoded);
    
    // set 4bit meta data for column value
    aCacheLine->v1N(i_Column,iEncoding);
    //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
    
    wVal = *(i8*)aDatum;
    //aCacheLine->v8B(8 + i_Column * 8,wVal);
    
    // set n-byte data for column value
    vPut(i_CacheOffset,(byte*)&wEncoded.m_iI8,iEncoding);
    MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
    
    i_CacheOffset += iEncoding;
    
    w8 wGot = wGet(i_Column,eDatum);
    vTrace("i_Column %lld got: %lld\n"_Stz,i_Column,wGot);
    
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
  
  if (DM_FLT == eDatum) {
    //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
    
    iEncoding = RowEncoding::iEncode(pColumn,aDatum,wEncoded);
    
    // set 4bit meta data for column value
    aCacheLine->v1N(i_Column,iEncoding);
    //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
    
    wVal = *(i8*)aDatum;
    //aCacheLine->v8B(8 + i_Column * 8,wVal);
    
    // set n-byte data for column value
    vPut(i_CacheOffset,aDatum,iEncoding);
    MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
    
    i_CacheOffset += iEncoding;
    
    w8 wGot = wGet(i_Column,eDatum);
    vTrace("i_Column %lld got: %lld\n"_Stz,i_Column,wGot);
    
    WordView wDecoded;
    RowEncoding::vDecodeFlt(pColumn,aCacheLine,wDecoded);
    vTrace("i_Column %lld Flt got: %llf\n"_Stz,i_Column,wDecoded.m_dF8);
    RASSERT(wVal == wDecoded.m_iI8);
    
    return;
  }
  
  if (DM_INT == eDatum) {
    //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
    
    iEncoding = RowEncoding::iEncode(pColumn,aDatum,wEncoded);
    
    // set 4bit meta data for column value
    aCacheLine->v1N(i_Column,iEncoding);
    //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));

    wVal = *(i8*)aDatum;
    //aCacheLine->v8B(8 + i_Column * 8,wVal);

    // set n-byte data for column value
    vPut(i_CacheOffset,aDatum,iEncoding);
    MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
    
    i_CacheOffset += iEncoding;
    
    WordView wDecoded;
    RowEncoding::vDecodeInt(pColumn,aCacheLine,wDecoded);
    vTrace("i_Column %lld Int got: %lld\n"_Stz,i_Column,wDecoded.m_iI8);
    RASSERT(wVal == wDecoded.m_iI8);

    return;
  }
  
  if (DM_IVL == eDatum) {
    //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
    
    iEncoding = RowEncoding::iEncode(pColumn,aDatum,wEncoded);
    
    // set 4bit meta data for column value
    aCacheLine->v1N(i_Column,iEncoding);
    //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
    
    wVal = *(i8*)aDatum;
    //aCacheLine->v8B(8 + i_Column * 8,wVal);
    
    // set n-byte data for column value
    vPut(i_CacheOffset,aDatum,iEncoding);
    MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
    
    i_CacheOffset += iEncoding;
    
    w8 wGot = wGet(i_Column,eDatum);
    vTrace("i_Column %lld got: %lld\n"_Stz,i_Column,wGot);
    
    WordView wDecoded;
    RowEncoding::vDecodeIvl(pColumn,aCacheLine,wDecoded);
    vTrace("i_Column %lld Ivl got: %lld\n"_Stz,i_Column,wDecoded.m_iI8);
    RASSERT(wVal == wDecoded.m_iI8);
    
    return;
  }
  
  if (DM_TME == eDatum) {
    //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
    
    iEncoding = RowEncoding::iEncode(pColumn,aDatum,wEncoded);
    
    // set 4bit meta data for column value
    aCacheLine->v1N(i_Column,iEncoding);
    //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
    
    wVal = *(i8*)aDatum;
    //aCacheLine->v8B(8 + i_Column * 8,wVal);
    
    // set n-byte data for column value
    vPut(i_CacheOffset,(byte*)&wEncoded.m_iI8,iEncoding);
    MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
    
    i_CacheOffset += iEncoding;
    
    w8 wGot = wGet(i_Column,eDatum);
    vTrace("i_Column %lld got: %lld\n"_Stz,i_Column,wGot);
    
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
  
  if (DM_TSP == eDatum) {
    //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
    
    iEncoding = RowEncoding::iEncode(pColumn,aDatum,wEncoded);
    
    // set 4bit meta data for column value
    aCacheLine->v1N(i_Column,iEncoding);
    //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
    
    wVal = *(i8*)aDatum;
    //aCacheLine->v8B(8 + i_Column * 8,wVal);
    
    // set n-byte data for column value
    vPut(i_CacheOffset,(byte*)&wEncoded.m_iI8,iEncoding);
    MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
    
    i_CacheOffset += iEncoding;
    
    w8 wGot = wGet(i_Column,eDatum);
    vTrace("i_Column %lld Tsp got: %lld\n"_Stz,i_Column,wGot);
    
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
  
  if (DM_VCH == eDatum) {
    
    iEncoding = RowEncoding::iEncode(pColumn,aDatum,wEncoded);

    // set 4bit meta data for column value
    aCacheLine->v1N(i_Column,iEncoding);
    //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
    
    vPut(i_CacheOffset,aDatum,iEncoding);
    MemLib::vWordDump(aCacheLine,sizeof(CacheLine));

    i_CacheOffset += iEncoding;
    
    w8 wGot = wGet(i_Column,eDatum);
    vTrace("i_Column %lld got: "_Stz,i_Column);
    
    WordView wView;
    wView.m_iI8 = wGot;
    
    for (i8 i_Char = 0; i_Char < iEncoding; i_Char++)
      vTrace("%c"_Stz,wView.m_yaByte[i_Char]);
    vTrace("\n"_Stz);

    WordView wDecoded;
    RowEncoding::vDecodeVch(pColumn,aCacheLine,wDecoded);
    vTrace("i_Column %lld Vch got: "_Stz,i_Column);
    for (i8 i_Char = 0; i_Char < iEncoding; i_Char++)
      vTrace("%c"_Stz,wDecoded.m_yaByte[i_Char]);
    vTrace("\n"_Stz);

    return;
  }
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get encoded datum from cache line                                          */
/*----------------------------------------------------------------------------*/
w8 Datum::wGet(i8 i_Column, eDM eDatum)
{
  CacheLine* aCacheLine = &xaCacheLine[i_CurrentCacheLine];

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
/* put encoded datam into cache line                                          */
/*----------------------------------------------------------------------------*/
void Datum::vPut(i8 i_Offset, byte* aDatum, i8 iEncoding)
{
  w8 wVal = *(i8*)aDatum;
  CacheLine* aCacheLine = &xaCacheLine[i_CurrentCacheLine];

  switch (iEncoding) {
    
    case 0: return;
    case 1: aCacheLine->v1B(i_Offset,wVal); return;
    case 2: aCacheLine->v2B(i_Offset,wVal); return;
    case 3: aCacheLine->v3B(i_Offset,wVal); return;
    case 4: aCacheLine->v4B(i_Offset,wVal); return;
    case 5: aCacheLine->v5B(i_Offset,wVal); return;
    case 6: aCacheLine->v6B(i_Offset,wVal); return;
    case 7: aCacheLine->v7B(i_Offset,wVal); return;
    case 8: aCacheLine->v8B(i_Offset,wVal); return;

    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* put encoded datam into cache line                                          */
/*----------------------------------------------------------------------------*/
#ifdef WITH_ROWBUFFER
//ALKA's versions

/*----------------------------------------------------------------------------*/
/* set given datum in cache line                                              */
/*----------------------------------------------------------------------------*/
void Datum::vSet(i8 i_Column, eDM eDatum, byte* aDatum, CacheLine* aCacheLine, i8 &i_CacheOffset)
{
  // don't encode columns 16+
  if (i_Column >= 16)
    return;
  
  w8 wVal;
  i8 iEncoding;
  
  if (DM_INT == eDatum) {
    //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
    
    iEncoding = RowEncoding::iEncode(pColumn,aDatum);
    
    aCacheLine->v1N(i_Column,iEncoding);
    //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
    
    wVal = *(i8*)aDatum;
    //aCacheLine->v8B(8 + i_Column * 8,wVal);
    vPut(i_CacheOffset,aDatum,iEncoding, aCacheLine);
    
    MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
    
    i_CacheOffset += iEncoding;
  }
  
}

/*----------------------------------------------------------------------------*/
/* get encoded datum from cache line                                          */
/*----------------------------------------------------------------------------*/
w8 Datum::wGet(i8 i_Column, eDM eDatum, CacheLine* aCacheLine)
{
  i8 i_Offset = 8;
  for (i8 i_Nibble = 0; i_Nibble < i_Column; i_Nibble++) {
    i8 nDataBytes = aCacheLine->w8_1N(i_Nibble);
    i_Offset += nDataBytes;
  }
  
  i8 iEncoding = aCacheLine->w8_1N(i_Column);
  
  w8 wVal = C_UNKNOWN;
  
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
      
    default:
      NEVER_REACH;
  }
  
  
  return wVal;
}

void Datum::vPut(i8 i_Offset, byte* aDatum, i8 iEncoding, CacheLine* aCacheLine)
{
  w8 wVal = *(i8*)aDatum;
  //CacheLine* aCacheLine = &xaCacheLine[i_CurrentCacheLine];
  
  switch (iEncoding) {
      
    case 0: return;
    case 1: aCacheLine->v1B(i_Offset,wVal); return;
    case 2: aCacheLine->v2B(i_Offset,wVal); return;
    case 3: aCacheLine->v3B(i_Offset,wVal); return;
    case 4: aCacheLine->v4B(i_Offset,wVal); return;
    case 5: aCacheLine->v5B(i_Offset,wVal); return;
    case 6: aCacheLine->v6B(i_Offset,wVal); return;
    case 7: aCacheLine->v7B(i_Offset,wVal); return;
    case 8: aCacheLine->v8B(i_Offset,wVal); return;
      
    default:
      NEVER_REACH;
  }
}
#endif

/*----------------------------------------------------------------------------*/
/* get encoded datum from cache line                                          */
/*----------------------------------------------------------------------------*/
i8 Datum::iTspEncoding(WordView& wTspView)
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
