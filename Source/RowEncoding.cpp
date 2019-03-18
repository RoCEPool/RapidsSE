/*============================================================================*/
/* RowEncoding.hpp -- Row Encoding                                            */
/*============================================================================*/

#include "Data.h"

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* decode decimal                                                             */
/*----------------------------------------------------------------------------*/
void RowEncoding::vDecodeBcd(Column* pColumn, CacheLine* aCacheLine, WordView& wDecoded)
{
  i8 i_Column  = pColumn->iColumnNo();
  i8 i_Offset  = C_UNKNOWN;
  i8 iEncoding = iGetEncoding(i_Column,aCacheLine,i_Offset);

  switch (iEncoding) {
      
    case 0: wDecoded.m_iI8 = C_DATUM_NULL;                break;
    case 1: wDecoded.m_iI8 = aCacheLine->w8_1B(i_Offset); break;
    case 2: wDecoded.m_iI8 = aCacheLine->w8_2B(i_Offset); break;
    case 3: wDecoded.m_iI8 = aCacheLine->w8_3B(i_Offset); break;
    case 4: wDecoded.m_iI8 = aCacheLine->w8_4B(i_Offset); break;
    case 5: wDecoded.m_iI8 = aCacheLine->w8_5B(i_Offset); break;
    case 6: wDecoded.m_iI8 = aCacheLine->w8_6B(i_Offset); break;
    case 7: wDecoded.m_iI8 = aCacheLine->w8_7B(i_Offset); break;
    case 8: wDecoded.m_iI8 = aCacheLine->w8_8B(i_Offset); break;
      
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
      
    case 0: wDecoded.m_iI8 = C_DATUM_NULL;                break;
    case 1: wDecoded.m_iI8 = aCacheLine->w8_1B(i_Offset); break;
    case 2: wDecoded.m_iI8 = aCacheLine->w8_2B(i_Offset); break;
    case 3: wDecoded.m_iI8 = aCacheLine->w8_3B(i_Offset); break;
    case 4: wDecoded.m_iI8 = aCacheLine->w8_4B(i_Offset); break;
    case 5: wDecoded.m_iI8 = aCacheLine->w8_5B(i_Offset); break;
    case 6: wDecoded.m_iI8 = aCacheLine->w8_6B(i_Offset); break;
    case 7: wDecoded.m_iI8 = aCacheLine->w8_7B(i_Offset); break;
    case 8: wDecoded.m_iI8 = aCacheLine->w8_8B(i_Offset); break;
      
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
      
    case 0: wDecoded.m_iI8 = C_DATUM_NULL;                break;
    case 1: wDecoded.m_iI8 = aCacheLine->w8_1B(i_Offset); break;
    case 2: wDecoded.m_iI8 = aCacheLine->w8_2B(i_Offset); break;
    case 3: wDecoded.m_iI8 = aCacheLine->w8_3B(i_Offset); break;
    case 4: wDecoded.m_iI8 = aCacheLine->w8_4B(i_Offset); break;
    case 5: wDecoded.m_iI8 = aCacheLine->w8_5B(i_Offset); break;
    case 6: wDecoded.m_iI8 = aCacheLine->w8_6B(i_Offset); break;
    case 7: wDecoded.m_iI8 = aCacheLine->w8_7B(i_Offset); break;
    case 8: wDecoded.m_iI8 = aCacheLine->w8_8B(i_Offset); break;
      
    default:
      NEVER_REACH;
  }
  
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
      
    case 0: wDecoded.m_iI8 = C_DATUM_NULL;                break;
    case 1: wDecoded.m_iI8 = aCacheLine->w8_1B(i_Offset); break;
    case 2: wDecoded.m_iI8 = aCacheLine->w8_2B(i_Offset); break;
    case 3: wDecoded.m_iI8 = aCacheLine->w8_3B(i_Offset); break;
    case 4: wDecoded.m_iI8 = aCacheLine->w8_4B(i_Offset); break;
    case 5: wDecoded.m_iI8 = aCacheLine->w8_5B(i_Offset); break;
    case 6: wDecoded.m_iI8 = aCacheLine->w8_6B(i_Offset); break;
    case 7: wDecoded.m_iI8 = aCacheLine->w8_7B(i_Offset); break;
    case 8: wDecoded.m_iI8 = aCacheLine->w8_8B(i_Offset); break;
      
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
      
    case 0: wDecoded.m_iI8 = C_DATUM_NULL;                break;
    case 1: wDecoded.m_iI8 = aCacheLine->w8_1B(i_Offset); break;
    case 2: wDecoded.m_iI8 = aCacheLine->w8_2B(i_Offset); break;
    case 3: wDecoded.m_iI8 = aCacheLine->w8_3B(i_Offset); break;
    case 4: wDecoded.m_iI8 = aCacheLine->w8_4B(i_Offset); break;
    case 5: wDecoded.m_iI8 = aCacheLine->w8_5B(i_Offset); break;
    case 6: wDecoded.m_iI8 = aCacheLine->w8_6B(i_Offset); break;
    case 7: wDecoded.m_iI8 = aCacheLine->w8_7B(i_Offset); break;
    case 8: wDecoded.m_iI8 = aCacheLine->w8_8B(i_Offset); break;
      
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
      
    case 0: wDecoded.m_iI8 = C_DATUM_NULL;                break;
    case 1: wDecoded.m_iI8 = aCacheLine->w8_1B(i_Offset); break;
    case 2: wDecoded.m_iI8 = aCacheLine->w8_2B(i_Offset); break;
    case 3: wDecoded.m_iI8 = aCacheLine->w8_3B(i_Offset); break;
    case 4: wDecoded.m_iI8 = aCacheLine->w8_4B(i_Offset); break;
    case 5: wDecoded.m_iI8 = aCacheLine->w8_5B(i_Offset); break;
    case 6: wDecoded.m_iI8 = aCacheLine->w8_6B(i_Offset); break;
    case 7: wDecoded.m_iI8 = aCacheLine->w8_7B(i_Offset); break;
    case 8: wDecoded.m_iI8 = aCacheLine->w8_8B(i_Offset); break;
      
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
      
    case 0: wDecoded.m_iI8 = C_DATUM_NULL;                break;
    case 1: wDecoded.m_iI8 = aCacheLine->w8_1B(i_Offset); break;
    case 2: wDecoded.m_iI8 = aCacheLine->w8_2B(i_Offset); break;
    case 3: wDecoded.m_iI8 = aCacheLine->w8_3B(i_Offset); break;
    case 4: wDecoded.m_iI8 = aCacheLine->w8_4B(i_Offset); break;
    case 5: wDecoded.m_iI8 = aCacheLine->w8_5B(i_Offset); break;
    case 6: wDecoded.m_iI8 = aCacheLine->w8_6B(i_Offset); break;
    case 7: wDecoded.m_iI8 = aCacheLine->w8_7B(i_Offset); break;
    case 8: wDecoded.m_iI8 = aCacheLine->w8_8B(i_Offset); break;
      
    default:
      NEVER_REACH;
  }
  
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
      
    case 0: wDecoded.m_iI8 = C_DATUM_NULL;                break;
    case 1: wDecoded.m_iI8 = aCacheLine->w8_1B(i_Offset); break;
    case 2: wDecoded.m_iI8 = aCacheLine->w8_2B(i_Offset); break;
    case 3: wDecoded.m_iI8 = aCacheLine->w8_3B(i_Offset); break;
    case 4: wDecoded.m_iI8 = aCacheLine->w8_4B(i_Offset); break;
    case 5: wDecoded.m_iI8 = aCacheLine->w8_5B(i_Offset); break;
    case 6: wDecoded.m_iI8 = aCacheLine->w8_6B(i_Offset); break;
    case 7: wDecoded.m_iI8 = aCacheLine->w8_7B(i_Offset); break;
    case 8: wDecoded.m_iI8 = aCacheLine->w8_8B(i_Offset); break;
      
    default:
      NEVER_REACH;
  }
  
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
      
    case 0: wDecoded.m_iI8 = C_DATUM_NULL;                break;
    case 1: wDecoded.m_iI8 = aCacheLine->w8_1B(i_Offset); break;
    case 2: wDecoded.m_iI8 = aCacheLine->w8_2B(i_Offset); break;
    case 3: wDecoded.m_iI8 = aCacheLine->w8_3B(i_Offset); break;
    case 4: wDecoded.m_iI8 = aCacheLine->w8_4B(i_Offset); break;
    case 5: wDecoded.m_iI8 = aCacheLine->w8_5B(i_Offset); break;
    case 6: wDecoded.m_iI8 = aCacheLine->w8_6B(i_Offset); break;
    case 7: wDecoded.m_iI8 = aCacheLine->w8_7B(i_Offset); break;
    case 8: wDecoded.m_iI8 = aCacheLine->w8_8B(i_Offset); break;
      
    default:
      NEVER_REACH;
  }
  
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
    iEncoding = RowEncoding::iTspEncoding(wEncoded);
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
    iEncoding = RowEncoding::iTspEncoding(wEncoded);
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
    i_Offset += nDataBytes;
  }
  
  i8 iEncoding = aCacheLine->w8_1N(i_Column);

  return iEncoding;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
