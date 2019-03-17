/*============================================================================*/
/* RowEncoding.hpp -- Row Encoding                                            */
/*============================================================================*/

#include "Data.h"

#include "StorageEngine.h"

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
    iEncoding = Datum::iTspEncoding(wEncoded);
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
    iEncoding = Datum::iTspEncoding(wEncoded);
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
    iEncoding = Datum::iTspEncoding(wEncoded);
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
