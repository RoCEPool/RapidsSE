/*============================================================================*/
/* RowCoder.cpp -- Row Coder                                                  */
/*============================================================================*/

#include "Data.h"

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* decode decimal from given cache line                                       */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeBcd(CacheLine xaCacheLine[], i8 i_Row,
                          Column* pCol, Datum* pDatum)
{
  w8 wDatum;
  CacheLine* aCacheLine = &xaCacheLine[i_Row];
  
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
  
  pDatum->vDatum(wDatum);
}

/*----------------------------------------------------------------------------*/
/* decode boolean from given cache line                                       */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeBol(CacheLine xaCacheLine[], i8 i_Row,
                          Column* pCol, Datum* pDatum)
{
  w8 wDatum;
  CacheLine* aCacheLine = &xaCacheLine[i_Row];
  
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
  pDatum->vDatum(wDatum);
}

/*----------------------------------------------------------------------------*/
/* decode date from given cache line                                          */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeDte(CacheLine xaCacheLine[], i8 i_Row,
                          Column* pCol, Datum* pDatum)
{
  w8 wDatum;
  CacheLine* aCacheLine = &xaCacheLine[i_Row];
  
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
  pDatum->vDatum(wDatum);
}

/*----------------------------------------------------------------------------*/
/* decode float from given cache line                                         */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeFlt(CacheLine xaCacheLine[], i8 i_Row,
                          Column* pCol, Datum* pDatum)
{
  w8 wDatum;
  CacheLine* aCacheLine = &xaCacheLine[i_Row];
  
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
  pDatum->vDatum(wDatum);
}

/*----------------------------------------------------------------------------*/
/* decode integer from given cache line                                       */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeInt(CacheLine xaCacheLine[], i8 i_Row,
                          Column* pCol, Datum* pDatum)
{
  w8 wDatum;
  CacheLine* aCacheLine = &xaCacheLine[i_Row];
  
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
  pDatum->vDatum(wDatum);
}

/*----------------------------------------------------------------------------*/
/* decode interval from given cache line                                      */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeIvl(CacheLine xaCacheLine[], i8 i_Row,
                          Column* pCol, Datum* pDatum)
{
  w8 wDatum;
  CacheLine* aCacheLine = &xaCacheLine[i_Row];
  
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
  pDatum->vDatum(wDatum);
}

/*----------------------------------------------------------------------------*/
/* decode time from given cache line                                          */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeTme(CacheLine xaCacheLine[], i8 i_Row,
                          Column* pCol, Datum* pDatum)
{
  w8 wDatum;
  CacheLine* aCacheLine = &xaCacheLine[i_Row];
  
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
  pDatum->vDatum(wDatum);
}

/*----------------------------------------------------------------------------*/
/* decode timestamp from given cache line                                     */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeTsp(CacheLine xaCacheLine[], i8 i_Row,
                          Column* pCol, Datum* pDatum)
{
  w8 wDatum;
  CacheLine* aCacheLine = &xaCacheLine[i_Row];
  
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
  pDatum->vDatum(wDatum);
}

/*----------------------------------------------------------------------------*/
/* decode varchar from given cache line                                       */
/*----------------------------------------------------------------------------*/
void RowCoder::vDecodeVch(CacheLine xaCacheLine[], i8 i_Row,
                          Column* pCol, Datum* pDatum)
{
  w8 wDatum;
  CacheLine* aCacheLine = &xaCacheLine[i_Row];
  
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
      CacheLine* aShocoLine = &xaCacheLine[i_Shoco];
      
      i8 iLenCompressed = aShocoLine->w8_1N(0);
      
      s256 sDecomp;
      i8 iLenDecompressed =
      Compress::iFromShoco(pDatum->szDatum()->aChar(), 255,
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
/* return number of bytes of encoding                                         */
/*----------------------------------------------------------------------------*/
i8 RowCoder::nEncodedBytes(i8 iEncoding, eDM eDatum)
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
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* restore zero stripped dte                                                  */
/*----------------------------------------------------------------------------*/
w8 RowCoder::wRestoreDte(w8 wDteStripped, eMN eEncoding)
{
  w8 wDte = wDteStripped;
  wDte <<= 40;
  
  return wDte;
}

/*----------------------------------------------------------------------------*/
/* restore zero stripped tme                                                  */
/*----------------------------------------------------------------------------*/
w8 RowCoder::wRestoreTme(w8 wTmeStripped, eMN eEncoding)
{
  w8 wTme = wTmeStripped;
  wTme <<= 40;
  
  return wTme;
}

/*----------------------------------------------------------------------------*/
/* restore zero stripped tsp                                                  */
/*----------------------------------------------------------------------------*/
w8 RowCoder::wRestoreTsp(w8 wTspStripped, eMN eEncoding)
{
  w8 wTsp = wTspStripped;
  wTsp <<= 24;
  
  return wTsp;
}

/*----------------------------------------------------------------------------*/
/* decoding of dte                                                            */
/*----------------------------------------------------------------------------*/
i8 RowCoder::iDteDecoding(WordView& wTspView, i8 iEncoding)
{
  i8 iDecoded = wTspView.m_iI8;
  iDecoded <<= 40;
  
  return iDecoded;
}

/*----------------------------------------------------------------------------*/
/* decoding of tme                                                            */
/*----------------------------------------------------------------------------*/
i8 RowCoder::iTmeDecoding(WordView& wTspView, i8 iEncoding)
{
  i8 iDecoded = wTspView.m_iI8;
  iDecoded <<= 40;
  
  return iDecoded;
}

/*----------------------------------------------------------------------------*/
/* decoding of tsp                                                            */
/*----------------------------------------------------------------------------*/
i8 RowCoder::iTspDecoding(WordView& wTspView, i8 iEncoding)
{
  i8 iDecoded = wTspView.m_iI8;
  iDecoded <<= 24;
  
  return iDecoded;
}

/*----------------------------------------------------------------------------*/
/* encoding of bcd                                                            */
/*----------------------------------------------------------------------------*/
i8 RowCoder::iBcdEncoding(WordView& wBcdView, i8& nBytesEncoded)
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
i8 RowCoder::iBolEncoding(WordView& wBolView, i8& nBytesEncoded)
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
i8 RowCoder::iDteEncoding(WordView& wDteView, i8& nBytesEncoded)
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
i8 RowCoder::iFltEncoding(WordView& wFltView, i8& nBytesEncoded)
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
i8 RowCoder::iIntEncoding(WordView& wIntView, i8& nBytesEncoded)
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
i8 RowCoder::iIvlEncoding(WordView& wIvlView, i8& nBytesEncoded)
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
i8 RowCoder::iTmeEncoding(WordView& wTmeView, i8& nBytesEncoded)
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
i8 RowCoder::iTspEncoding(WordView& wTspView, i8& nBytesEncoded)
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
i8 RowCoder::iVchEncoding(WordView& wVchView, i8& nBytesEncoded)
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
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
