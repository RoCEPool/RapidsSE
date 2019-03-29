/*============================================================================*/
/* RowCoder.cpp -- Row Coder                                                  */
/*============================================================================*/

#include "Data.h"

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* initialise given cache line                                                */
/*----------------------------------------------------------------------------*/
void RowCoder::vCacheLineInit(CacheLine* aCacheLine)
{
  // set all bytes in the entire cache line to xFF
  aCacheLine->vSetAll(0xFF);
  
  // set all bits in first word of cache line to zero
  aCacheLine->v8B(0,0);
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
      
    case MN_NULL: pDatum->m_iDatum = C_DATUM_NULL;                break;
    case MN_1B:   pDatum->m_iDatum = aCacheLine->w8_1B(i_Offset); break;
    case MN_2B:   pDatum->m_iDatum = aCacheLine->w8_2B(i_Offset); break;
    case MN_3B:   pDatum->m_iDatum = aCacheLine->w8_3B(i_Offset); break;
    case MN_4B:   pDatum->m_iDatum = aCacheLine->w8_4B(i_Offset); break;
    case MN_5B:   pDatum->m_iDatum = aCacheLine->w8_5B(i_Offset); break;
    case MN_6B:   pDatum->m_iDatum = aCacheLine->w8_6B(i_Offset); break;
    case MN_7B:   pDatum->m_iDatum = aCacheLine->w8_7B(i_Offset); break;
    case MN_8B:   pDatum->m_iDatum = aCacheLine->w8_8B(i_Offset); break;
      
    case MN_MINUS_ONE: pDatum->m_iDatum = (i8)-1; break;
    case MN_ZERO:      pDatum->m_iDatum = (i8)0;  break;
    case MN_PLUS_ONE:  pDatum->m_iDatum = (i8)1;  break;
      
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
      
    case MN_NULL:  pDatum->m_iDatum = C_DATUM_NULL; break;
    case MN_FALSE: pDatum->m_iDatum = (i8)0;        break;
    case MN_TRUE:  pDatum->m_iDatum = (i8)1;        break;
      
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
      
    case MN_NULL: pDatum->m_iDatum = C_DATUM_NULL;                break;
    case MN_1B:   pDatum->m_iDatum = aCacheLine->w8_1B(i_Offset); break;
    case MN_2B:   pDatum->m_iDatum = aCacheLine->w8_2B(i_Offset); break;
    case MN_3B:   pDatum->m_iDatum = aCacheLine->w8_3B(i_Offset); break;
    case MN_4B:   pDatum->m_iDatum = aCacheLine->w8_4B(i_Offset); break;
    case MN_5B:   pDatum->m_iDatum = aCacheLine->w8_5B(i_Offset); break;
    case MN_6B:   pDatum->m_iDatum = aCacheLine->w8_6B(i_Offset); break;
    case MN_7B:   pDatum->m_iDatum = aCacheLine->w8_7B(i_Offset); break;
    case MN_8B:   pDatum->m_iDatum = aCacheLine->w8_8B(i_Offset); break;
      
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
      
    case MN_NULL: pDatum->m_iDatum = C_DATUM_NULL;                break;
    case MN_1B:   pDatum->m_iDatum = aCacheLine->w8_1B(i_Offset); break;
    case MN_2B:   pDatum->m_iDatum = aCacheLine->w8_2B(i_Offset); break;
    case MN_3B:   pDatum->m_iDatum = aCacheLine->w8_3B(i_Offset); break;
    case MN_4B:   pDatum->m_iDatum = aCacheLine->w8_4B(i_Offset); break;
    case MN_5B:   pDatum->m_iDatum = aCacheLine->w8_5B(i_Offset); break;
    case MN_6B:   pDatum->m_iDatum = aCacheLine->w8_6B(i_Offset); break;
    case MN_7B:   pDatum->m_iDatum = aCacheLine->w8_7B(i_Offset); break;
    case MN_8B:   pDatum->m_iDatum = aCacheLine->w8_8B(i_Offset); break;
      
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
      
    case MN_NULL: pDatum->m_iDatum = C_DATUM_NULL;                break;
    case MN_1B:   pDatum->m_iDatum = aCacheLine->w8_1B(i_Offset); break;
    case MN_2B:   pDatum->m_iDatum = aCacheLine->w8_2B(i_Offset); break;
    case MN_3B:   pDatum->m_iDatum = aCacheLine->w8_3B(i_Offset); break;
    case MN_4B:   pDatum->m_iDatum = aCacheLine->w8_4B(i_Offset); break;
    case MN_5B:   pDatum->m_iDatum = aCacheLine->w8_5B(i_Offset); break;
    case MN_6B:   pDatum->m_iDatum = aCacheLine->w8_6B(i_Offset); break;
    case MN_7B:   pDatum->m_iDatum = aCacheLine->w8_7B(i_Offset); break;
    case MN_8B:   pDatum->m_iDatum = aCacheLine->w8_8B(i_Offset); break;
      
    case MN_MINUS_ONE: pDatum->m_iDatum = (i8)-1; break;
    case MN_ZERO:      pDatum->m_iDatum = (i8)0;  break;
    case MN_PLUS_ONE:  pDatum->m_iDatum = (i8)1;  break;
      
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
      
    case MN_NULL: pDatum->m_iDatum = C_DATUM_NULL;                break;
    case MN_1B:   pDatum->m_iDatum = aCacheLine->w8_1B(i_Offset); break;
    case MN_2B:   pDatum->m_iDatum = aCacheLine->w8_2B(i_Offset); break;
    case MN_3B:   pDatum->m_iDatum = aCacheLine->w8_3B(i_Offset); break;
    case MN_4B:   pDatum->m_iDatum = aCacheLine->w8_4B(i_Offset); break;
    case MN_5B:   pDatum->m_iDatum = aCacheLine->w8_5B(i_Offset); break;
    case MN_6B:   pDatum->m_iDatum = aCacheLine->w8_6B(i_Offset); break;
    case MN_7B:   pDatum->m_iDatum = aCacheLine->w8_7B(i_Offset); break;
    case MN_8B:   pDatum->m_iDatum = aCacheLine->w8_8B(i_Offset); break;
      
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
      
    case MN_NULL: pDatum->m_iDatum = C_DATUM_NULL;                break;
    case MN_1B:   pDatum->m_iDatum = aCacheLine->w8_1B(i_Offset); break;
    case MN_2B:   pDatum->m_iDatum = aCacheLine->w8_2B(i_Offset); break;
    case MN_3B:   pDatum->m_iDatum = aCacheLine->w8_3B(i_Offset); break;
    case MN_4B:   pDatum->m_iDatum = aCacheLine->w8_4B(i_Offset); break;
    case MN_5B:   pDatum->m_iDatum = aCacheLine->w8_5B(i_Offset); break;
    case MN_6B:   pDatum->m_iDatum = aCacheLine->w8_6B(i_Offset); break;
    case MN_7B:   pDatum->m_iDatum = aCacheLine->w8_7B(i_Offset); break;
    case MN_8B:   pDatum->m_iDatum = aCacheLine->w8_8B(i_Offset); break;
      
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
      
    case MN_NULL: pDatum->m_iDatum = C_DATUM_NULL;                break;
    case MN_1B:   pDatum->m_iDatum = aCacheLine->w8_1B(i_Offset); break;
    case MN_2B:   pDatum->m_iDatum = aCacheLine->w8_2B(i_Offset); break;
    case MN_3B:   pDatum->m_iDatum = aCacheLine->w8_3B(i_Offset); break;
    case MN_4B:   pDatum->m_iDatum = aCacheLine->w8_4B(i_Offset); break;
    case MN_5B:   pDatum->m_iDatum = aCacheLine->w8_5B(i_Offset); break;
    case MN_6B:   pDatum->m_iDatum = aCacheLine->w8_6B(i_Offset); break;
    case MN_7B:   pDatum->m_iDatum = aCacheLine->w8_7B(i_Offset); break;
    case MN_8B:   pDatum->m_iDatum = aCacheLine->w8_8B(i_Offset); break;
      
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
      
    case MN_NULL: pDatum->m_iDatum = C_DATUM_NULL;                break;
    case MN_1B:   pDatum->m_iDatum = aCacheLine->w8_1B(i_Offset); break;
    case MN_2B:   pDatum->m_iDatum = aCacheLine->w8_2B(i_Offset); break;
    case MN_3B:   pDatum->m_iDatum = aCacheLine->w8_3B(i_Offset); break;
    case MN_4B:   pDatum->m_iDatum = aCacheLine->w8_4B(i_Offset); break;
    case MN_5B:   pDatum->m_iDatum = aCacheLine->w8_5B(i_Offset); break;
    case MN_6B:   pDatum->m_iDatum = aCacheLine->w8_6B(i_Offset); break;
    case MN_7B:   pDatum->m_iDatum = aCacheLine->w8_7B(i_Offset); break;
    case MN_8B:   pDatum->m_iDatum = aCacheLine->w8_8B(i_Offset); break;
      
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
/* encoding of bcd                                                            */
/*----------------------------------------------------------------------------*/
i8 RowCoder::iEncodingBcd(Datum* pDatum, i8& nBytesEncoded)
{
  i8 iEncoding;
  
  if (pDatum->m_iDatum == C_DATUM_NULL) {
    iEncoding = MN_NULL;
    nBytesEncoded = 0;
  }
  else {
    iEncoding = IntLib::iByteMagnitude(pDatum->m_iDatum);
    nBytesEncoded = iEncoding;
  }
  
  return iEncoding;
}

/*----------------------------------------------------------------------------*/
/* encoding of bol                                                            */
/*----------------------------------------------------------------------------*/
i8 RowCoder::iEncodingBol(Datum* pDatum, i8& nBytesEncoded)
{
  i8 iEncoding;
  
  if (pDatum->m_iDatum == C_DATUM_NULL)
    iEncoding = MN_NULL;
  else if (pDatum->m_iDatum)
    iEncoding = MN_TRUE;
  else
    iEncoding = MN_FALSE;
  
  nBytesEncoded = 0;
  
  return iEncoding;
}

/*----------------------------------------------------------------------------*/
/* encoding of dte                                                            */
/*----------------------------------------------------------------------------*/
i8 RowCoder::iEncodingDte(Datum* pDatum, i8& nBytesEncoded)
{
  i8 iEncoding;
  
  if (pDatum->m_iDatum == C_DATUM_NULL) {
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
  
  iEncoding = IntLib::iByteMagnitude(pDatum->m_iDatum);
  
  nBytesEncoded = iEncoding;
  
  return iEncoding;
}

/*----------------------------------------------------------------------------*/
/* encoding of flt                                                            */
/*----------------------------------------------------------------------------*/
i8 RowCoder::iEncodingFlt(Datum* pDatum, i8& nBytesEncoded)
{
  i8 iEncoding;
  
  if (pDatum->m_iDatum == C_DATUM_NULL) {
    iEncoding = MN_NULL;
    nBytesEncoded = 0;
  }
  else {
    iEncoding = IntLib::iByteMagnitude(pDatum->m_iDatum);
    nBytesEncoded = iEncoding;
  }
  
  return iEncoding;
}

/*----------------------------------------------------------------------------*/
/* encoding of int                                                            */
/*----------------------------------------------------------------------------*/
i8 RowCoder::iEncodingInt(Datum* pDatum, i8& nBytesEncoded)
{
  i8 iEncoding;
  
  if (pDatum->m_iDatum == C_DATUM_NULL) {
    iEncoding = MN_NULL;
    nBytesEncoded = 0;
  }
  else {
    iEncoding = IntLib::iByteMagnitude(pDatum->m_iDatum);
    nBytesEncoded = iEncoding;
  }
  
  return iEncoding;
}

/*----------------------------------------------------------------------------*/
/* encoding of ivl                                                            */
/*----------------------------------------------------------------------------*/
i8 RowCoder::iEncodingIvl(Datum* pDatum, i8& nBytesEncoded)
{
  i8 iEncoding;
  
  if (pDatum->m_iDatum == C_DATUM_NULL) {
    iEncoding = MN_NULL;
    nBytesEncoded = 0;
  }
  else {
    iEncoding = IntLib::iByteMagnitude(pDatum->m_iDatum);
    nBytesEncoded = iEncoding;
  }
  
  return iEncoding;
}

/*----------------------------------------------------------------------------*/
/* encoding of tme                                                            */
/*----------------------------------------------------------------------------*/
i8 RowCoder::iEncodingTme(Datum* pDatum, i8& nBytesEncoded)
{
  if (pDatum->m_iDatum == C_DATUM_NULL) {
    nBytesEncoded = 0;
    return MN_NULL;
  }
  
  i8 iEncoding;
  
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
  
  iEncoding = IntLib::iByteMagnitude(pDatum->m_iDatum);
  
  nBytesEncoded = iEncoding;
  
  return iEncoding;
}

/*----------------------------------------------------------------------------*/
/* encoding of tsp                                                            */
/*----------------------------------------------------------------------------*/
i8 RowCoder::iEncodingTsp(Datum* pDatum, i8& nBytesEncoded)
{
  if (pDatum->m_iDatum == C_DATUM_NULL) {
    nBytesEncoded = 0;
    return MN_NULL;
  }
  
  i8 iEncoding;
  
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
  
  iEncoding = IntLib::iByteMagnitude(pDatum->m_iDatum);
  
  nBytesEncoded = iEncoding;
  
  return iEncoding;
}

/*----------------------------------------------------------------------------*/
/* encoding of vch                                                            */
/*----------------------------------------------------------------------------*/
i8 RowCoder::iEncodingVch(Datum* pDatum, i8& nBytesEncoded)
{
  i8 iEncoding;
  
  i8 iLen = StrLib::iLength((char*)pDatum->m_ayDatum);
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
/* print decoded datum from cache line                                        */
/*----------------------------------------------------------------------------*/
void RowCoder::vPrint(CacheLine xaCacheLine[], i8 i_Row, Column* pCol)
{
  Datum* pDatum = new Datum;
  Record* pRec = new Record;
  
  vDecode(xaCacheLine,i_Row,pCol,pDatum);
  
  eDM eDatum = pCol->eDatumType();
  switch (eDatum) {
    case DM_BCD: pRec->vAppendBcd(pCol,pDatum); break;
    case DM_BOL: pRec->vAppendBol(pCol,pDatum); break;
    case DM_DTE: pRec->vAppendDte(pCol,pDatum); break;
    case DM_FLT: pRec->vAppendFlt(pCol,pDatum); break;
    case DM_INT: pRec->vAppendInt(pCol,pDatum); break;
    case DM_IVL: pRec->vAppendIvl(pCol,pDatum); break;
    case DM_TME: pRec->vAppendTme(pCol,pDatum); break;
    case DM_TSP: pRec->vAppendTsp(pCol,pDatum); break;
    case DM_VCH: pRec->vAppendVch(pCol,pDatum); break;
    default:
      NEVER_REACH;
  }
  
  vTrace("Column[%lld %s %s] : %s\n"_Stz,
         pCol->iColumnNo(),
         pCol->sColumnName().aStz(),
         aDM[pCol->eDatumType()],
         pRec->szRecStr());
  
  delete pRec;
  delete pDatum;
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
/* restore zero stripped dte                                                  */
/*----------------------------------------------------------------------------*/
void RowCoder::vRestoreDte(Datum* pDatum, eMN eEncoding)
{
  pDatum->m_iDatum <<= 40;
}

/*----------------------------------------------------------------------------*/
/* restore zero stripped tme                                                  */
/*----------------------------------------------------------------------------*/
void RowCoder::vRestoreTme(Datum* pDatum, eMN eEncoding)
{
  pDatum->m_iDatum <<= 40;
}

/*----------------------------------------------------------------------------*/
/* restore zero stripped tsp                                                  */
/*----------------------------------------------------------------------------*/
void RowCoder::vRestoreTsp(Datum* pDatum, eMN eEncoding)
{
  pDatum->m_iDatum <<= 24;
}

/*----------------------------------------------------------------------------*/
/* decoding of dte                                                            */
/*----------------------------------------------------------------------------*/
i8 RowCoder::iStripDte(WordView& wTspView, i8 iEncoding)
{
  i8 iDecoded = wTspView.m_iI8;
  iDecoded <<= 40;
  
  return iDecoded;
}

/*----------------------------------------------------------------------------*/
/* decoding of tme                                                            */
/*----------------------------------------------------------------------------*/
i8 RowCoder::iStripTme(WordView& wTspView, i8 iEncoding)
{
  i8 iDecoded = wTspView.m_iI8;
  iDecoded <<= 40;
  
  return iDecoded;
}

/*----------------------------------------------------------------------------*/
/* decoding of tsp                                                            */
/*----------------------------------------------------------------------------*/
i8 RowCoder::iStripTsp(WordView& wTspView, i8 iEncoding)
{
  i8 iDecoded = wTspView.m_iI8;
  iDecoded <<= 24;
  
  return iDecoded;
}

/*----------------------------------------------------------------------------*/
/* print row decoded datum from cache line                                    */
/*----------------------------------------------------------------------------*/
void RowCoder::vString(s48K& sString, ColumnVec* pColVec, CacheLine* aCacheLine)
{
  Datum* pDatum = new Datum;
  
  // get no of columns
  i8 nColumns = pColVec->nItems();
  
  // for each column
  for (i8 i_Column = 0; i_Column < nColumns; i_Column++) {
    
    // get the column at index
    Column* pColumn = pColVec->pGet(i_Column);
    
    // not the first column
    if (i_Column == 0)
      sString.vConcat(""_Stz);
    else
      sString.vConcat(","_Stz);
    
    // copy the data for the column into aDst
    eDM eDatum = pColumn->eDatumType();
    RowCoder::vDecode(aCacheLine,0,pColumn,pDatum);
    
    // check for nulls
    if (pDatum->m_iDatum == C_DATUM_NULL) {
      
      // NULL Floats ?
      RASSERT(eDatum != DM_FLT);
      
      sString.vCatSprint("null"_Stz);
      continue;
    }
    
    switch (eDatum) {
      case DM_BCD: {
        sString.vCatSprint("%lld"_Stz, pDatum->m_iDatum);
        break;
      }
      case DM_BOL: {
        sString.vConcat(pDatum->m_iDatum ? "true"_Stz : "false"_Stz);
        break;
      }
      case DM_DTE: {
        Dte wDte;
        wDte.vAsI8(pDatum->m_iDatum);
        s32 sDte;
        wDte.vSprint(sDte.aStz());
        sString.vConcat(sDte.aStz());
        break;
      }
      case DM_FLT: {
        sString.vCatSprint("%g"_Stz, pDatum->m_dDatum);
        break;
      }
      case DM_INT: {
        sString.vCatSprint("%lld"_Stz, pDatum->m_iDatum);
        break;
      }
      case DM_IVL: {
        sString.vCatSprint("%lld"_Stz, pDatum->m_iDatum);
        break;
      }
      case DM_TME: {
        Tme wTme;
        wTme.vAsI8(pDatum->m_iDatum);
        s32 sTme;
        wTme.vSprint(sTme.aStz());
        sString.vConcat(sTme.aStz());
        break;
      }
      case DM_TSP: {
        Tsp wTsp;
        wTsp.vAsI8(pDatum->m_iDatum);
        s32 sTsp;
        wTsp.vSprint(sTsp.aStz());
        sString.vConcat(sTsp.aStz());
        break;
      }
      case DM_VCH: {
        s256 sStr;
        Vch::vSprintQuotes(sStr, pDatum->m_sDatum);
        sString.vConcat(sStr.aStz());
        break;
      }
      default:
        NEVER_REACH;
    }
    
  }
  
  delete pDatum;
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* decode and append column datum from cache line                             */
/*----------------------------------------------------------------------------*/
void RowCoder::vAppend(Record* pRec, CacheLine xaCacheLine[], i8 i_Row, Column* pCol)
{
  Datum* pDatum = new Datum;
  
  CacheLine* aCacheLine = &xaCacheLine[0];
  
  RowCoder::vDecode(aCacheLine,i_Row,pCol,pDatum);
  
  eDM eDatum = pCol->eDatumType();
  switch (eDatum) {
    case DM_BCD: pRec->vAppendBcd(pCol,pDatum); break;
    case DM_BOL: pRec->vAppendBol(pCol,pDatum); break;
    case DM_DTE: pRec->vAppendDte(pCol,pDatum); break;
    case DM_FLT: pRec->vAppendFlt(pCol,pDatum); break;
    case DM_INT: pRec->vAppendInt(pCol,pDatum); break;
    case DM_IVL: pRec->vAppendIvl(pCol,pDatum); break;
    case DM_TME: pRec->vAppendTme(pCol,pDatum); break;
    case DM_TSP: pRec->vAppendTsp(pCol,pDatum); break;
    case DM_VCH: pRec->vAppendVch(pCol,pDatum); break;
    default:
      NEVER_REACH;
  }
  
  delete pDatum;
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
