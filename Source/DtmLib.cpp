/*============================================================================*/
/* DtmLib.cpp -- Datum Library                                                */
/*============================================================================*/

#include "Datums.h"

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/* extract a decimal from substr                                              */
/*----------------------------------------------------------------------------*/
bool DtmLib::bExtractBcd(char** aaSrc, Bcd* aDecimal)
{
  d8 dResult = 0.0;
  char xBuffer[256];
  char* aChar = xBuffer;
  
  // collect the csv
  while (**aaSrc && '|' != **aaSrc && ',' != **aaSrc)
    *aChar++ = *(*aaSrc)++;
  *aChar++ = '\0';
  
  // find first char
  for (aChar = xBuffer; *aChar && ' ' == *aChar; aChar++)
    ;
  
  // convert
  dResult = atof(aChar);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a boolean from substr                                              */
/*----------------------------------------------------------------------------*/
/*
bool DtmLib::bExtractBol(char** aaSrc, Bol* aBoolean)
{
  bool bResult = false;
  char xBuffer[256];
  char* aChar = xBuffer;
  
  // collect the csv
  while ('|' != **aaSrc && ',' != **aaSrc)
    *aChar++ = *(*aaSrc)++;
  *aChar++ = '\0';

  // find first char
  for (aChar = xBuffer; *aChar && ' ' == *aChar; aChar++)
    ;
  
  // test for true
  if ('t' == *aChar ||
      'T' == *aChar ||
      'y' == *aChar ||
      'Y' == *aChar ||
      '1' == *aChar)
    bResult = true;

  aBoolean->m_bBol = bResult;
  
  return bResult;
}
*/
bool DtmLib::bExtractBol(char** aaSrc, Bol* aBoolean)
{
  bool bResult = false;
    
  // test for true
  if ('t' == *(*aaSrc) ||
      'T' == *(*aaSrc) ||
      'y' == *(*aaSrc) ||
      'Y' == *(*aaSrc) ||
      '1' == *(*aaSrc))
  bResult = true;
  
  aBoolean->m_bBol = bResult;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a date from substr                                     yyyy-mo-ddZ */
/*----------------------------------------------------------------------------*/
bool DtmLib::bExtractDte(char** aaSrc, Dte* aDate)
{
  u8 uField;
  
  aDate->vInitZero();
  
  if ( ! *aaSrc )
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - -
  // date portion
  
  // extract year
  if ( ! StrLib::bExtractU8(aaSrc,&uField) )
    return false;
  aDate->x.m_uYear = uField;
  
  // skip - separator
  if ('-' != *(*aaSrc)++)
    return false;
  
  // extract month
  if ( ! StrLib::bExtractU8(aaSrc,&uField) )
    return false;
  aDate->x.m_uMonth = uField;
  
  // skip - separator
  if ('-' != *(*aaSrc)++) return false;
  
  // extract day
  if ( ! StrLib::bExtractU8(aaSrc,&uField) )
    return false;
  aDate->x.m_uDay = uField;
  
  // skip Z separator
  if ('Z' == **aaSrc)
    (*aaSrc)++;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a float from substr                                                */
/*----------------------------------------------------------------------------*/
bool DtmLib::bExtractFlt(char** aaSrc, Flt* aFloat)
{
  d8 dResult = 0.0;
  char xBuffer[256];
  char* aChar = xBuffer;
  
  // collect the csv
  while (**aaSrc && '|' != **aaSrc && ',' != **aaSrc)
    *aChar++ = *(*aaSrc)++;
  *aChar++ = '\0';
  
  // find first char
  for (aChar = xBuffer; *aChar && ' ' == *aChar; aChar++)
    ;
  
  // convert
  dResult = atof(aChar);
  
  aFloat->m_dFlt = dResult;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract an integer from substr                                             */
/*----------------------------------------------------------------------------*/
bool DtmLib::bExtractInt(char** aaSrc, Int* aInteger)
{
  i8 iResult = 0;
  char xBuffer[256];
  char* aChar = xBuffer;
  
  // skip leading whitespace
  while (' ' == **aaSrc)
    (*aaSrc)++;

  if ( ! **aaSrc || '|' == **aaSrc ) {
    aInteger->vSetNull();
    return true;
  }
  
  // collect the csv
  while (**aaSrc && '\n' != **aaSrc && '\r' != **aaSrc && '|' != **aaSrc && ',' != **aaSrc) {
    if ('|' == **aaSrc)
      return true;
    if ( ! StrLib::bIsDigit(**aaSrc) )
      return false;
    *aChar++ = *(*aaSrc)++;
  }
  *aChar++ = '\0';
  
  // find first char
  for (aChar = xBuffer; *aChar && ' ' == *aChar; aChar++)
    ;
  
  // convert
  iResult = atol(aChar);
  
  aInteger->m_iInt = iResult;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract an interval from substr                                            */
/*----------------------------------------------------------------------------*/
bool DtmLib::bExtractIvl(char** aaSrc, Ivl* aInterval)
{
  u8 uField;
  
  aInterval->vInitZero();
  
  if ( ! *aaSrc )
    return false;
  
  // extract second
  if ( ! StrLib::bExtractU8(aaSrc,&uField) )
    return true;
  aInterval->x.m_uSecond = uField;
  
//  // skip . separator
//  if ('.' != *(*aaSrc)++)
//    return true;
  
  // extract fraction (assuming milliseconds)
  if ('.' == **aaSrc) {
    (*aaSrc)++;
    if ( StrLib::bExtractU8(aaSrc,&uField) )
      aInterval->x.m_uMilli = uField;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a time from substr                                   hh:mi:ss.fffZ */
/*----------------------------------------------------------------------------*/
bool DtmLib::bExtractTme(char** aaSrc, Tme* aTime)
{
  u8 uField;
  
  aTime->vInitZero();
  
  if ( ! *aaSrc )
    return false;
  
  // extract hour
  if ( ! StrLib::bExtractU8(aaSrc,&uField) )
    return true;
  aTime->x.m_uHour = uField;
  
  // skip : separator
  if (':' != *(*aaSrc)++)
    return true;
  
  // extract minute
  if ( ! StrLib::bExtractU8(aaSrc,&uField) )
    return true;
  aTime->x.m_uMinute = uField;
  
  // skip : separator
  if (':' != *(*aaSrc)++)
    return true;
  
  // extract second
  if ( ! StrLib::bExtractU8(aaSrc,&uField) )
    return true;
  aTime->x.m_uSecond = uField;
  
  // extract fraction (assuming milliseconds)
  if ('.' == **aaSrc) {
    (*aaSrc)++;
    if ( StrLib::bExtractU8(aaSrc,&uField) )
      aTime->x.m_uMilli = uField;
  }
  
  // skip Z separator
  if ('Z' == **aaSrc)
    (*aaSrc)++;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a timestamp from substr                   yyyy-mo-ddThh:mi:ss.fffZ */
/*----------------------------------------------------------------------------*/
bool DtmLib::bExtractTsp(char** aaSrc, Tsp* aTimestamp)
{
  // refer: http://en.wikipedia.org/wiki/ISO_8601

  u8 uField;
  
  aTimestamp->vInitZero();
  
  if ( ! *aaSrc )
    return false;

  // - - - - - - - - - - - - - - - - - - - - - - - - -
  // date portion
  
  // extract year
  if ( ! StrLib::bExtractU8(aaSrc,&uField) )
    return false;
  aTimestamp->x.m_uYear = uField;
  
  // skip - separator
  if ('-' != *(*aaSrc)++)
    return false;
  
  // extract month
  if ( ! StrLib::bExtractU8(aaSrc,&uField) )
    return false;
  aTimestamp->x.m_uMonth = uField;
  
  // skip - separator
  if ('-' != *(*aaSrc)++) return false;
  
  // extract day
  if ( ! StrLib::bExtractU8(aaSrc,&uField) )
    return false;
  aTimestamp->x.m_uDay = uField;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - -
  // time portion
  
  // skip T or space separator
  if ( ! ('T' == **aaSrc || ' ' == **aaSrc) )
    return true;
  (*aaSrc)++;
  
  // extract hour
  if ( ! StrLib::bExtractU8(aaSrc,&uField) )
    return true;
  aTimestamp->x.m_uHour = uField;
  
  // skip : separator
  if (':' != *(*aaSrc)++)
    return true;
  
  // extract minute
  if ( ! StrLib::bExtractU8(aaSrc,&uField) )
    return true;
  aTimestamp->x.m_uMinute = uField;
  
  // skip : separator
  if (':' != *(*aaSrc)++)
    return true;
  
  // extract second
  if ( ! StrLib::bExtractU8(aaSrc,&uField) )
    return true;
  aTimestamp->x.m_uSecond = uField;
  
  // extract fraction (assuming milliseconds)
  if ('.' == **aaSrc) {
    (*aaSrc)++;
    if ( StrLib::bExtractU8(aaSrc,&uField) )
      aTimestamp->x.m_uMilli = uField;
   }
  
  // skip Z separator
  if ('Z' == **aaSrc)
    (*aaSrc)++;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract an varchar from substr                                             */
/*----------------------------------------------------------------------------*/
bool DtmLib::bExtractVch(char** aaSrc, Vch* aVch)
{
  char* aChar = (char*)aVch;
  
  // collect the csv
  while ('|' != **aaSrc && ',' != **aaSrc)
    *aChar++ = *(*aaSrc)++;
  *aChar++ = '\0';
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* valid datum?                                                               */
/*----------------------------------------------------------------------------*/
bool DtmLib::bValidateDatum(eDM eDatum, char** aaSrc)
{
  bool bValid = false;
  
  switch (eDatum) {
    case DM_BCD: bValid = bValidBcd(aaSrc); break;
    case DM_BOL: bValid = bValidBol(aaSrc); break;
    case DM_CHR: bValid = bValidVch(aaSrc); break;
    case DM_DTE: bValid = bValidDte(aaSrc); break;
    case DM_FLT: bValid = bValidFlt(aaSrc); break;
    case DM_INT: bValid = bValidInt(aaSrc); break;
    case DM_IVL: bValid = bValidIvl(aaSrc); break;
    case DM_TME: bValid = bValidTme(aaSrc); break;
    case DM_TSP: bValid = bValidTsp(aaSrc); break;
    case DM_VCH: bValid = bValidVch(aaSrc); break;
    default:
      vFatal("DtmLib::bValid: Invalid aDatum argument: %lld\n"_Str,
             eDatum);
  }
  
  return bValid;
}

/*----------------------------------------------------------------------------*/
/* valid decimal in substr?                                                   */
/*----------------------------------------------------------------------------*/
bool DtmLib::bValidBcd(char** aaSrc)
{
  Bcd xBcd;
  
  bool bValid = bExtractBcd(aaSrc,&xBcd);
  
  return bValid;
}

/*----------------------------------------------------------------------------*/
/* valid boolean in substr?                                                   */
/*----------------------------------------------------------------------------*/
bool DtmLib::bValidBol(char** aaSrc)
{
  Bol xBol;
  
  bool bValid = bExtractBol(aaSrc,&xBol);
  
  return bValid;
}

/*----------------------------------------------------------------------------*/
/* valid date in substr?                                                      */
/*----------------------------------------------------------------------------*/
bool DtmLib::bValidDte(char** aaSrc)
{
  Dte xDte;
  
  bool bValid = bExtractDte(aaSrc,&xDte);
  
  return bValid;
}

/*----------------------------------------------------------------------------*/
/* valid float in substr?                                                     */
/*----------------------------------------------------------------------------*/
bool DtmLib::bValidFlt(char** aaSrc)
{
  Flt xFlt;
  
  bool bValid = bExtractFlt(aaSrc,&xFlt);
  
  return bValid;
}

/*----------------------------------------------------------------------------*/
/* valid integer in substr?                                                   */
/*----------------------------------------------------------------------------*/
bool DtmLib::bValidInt(char** aaSrc)
{
  Int xInt;
  
  bool bValid = bExtractInt(aaSrc,&xInt);
  
  return bValid;
}

/*----------------------------------------------------------------------------*/
/* valid interval in substr?                                                  */
/*----------------------------------------------------------------------------*/
bool DtmLib::bValidIvl(char** aaSrc)
{
  Ivl xIvl;
  
  bool bValid = bExtractIvl(aaSrc,&xIvl);
  
  return bValid;
}

/*----------------------------------------------------------------------------*/
/* valid time in substr?                                                      */
/*----------------------------------------------------------------------------*/
bool DtmLib::bValidTme(char** aaSrc)
{
  Tme xTme;
  
  bool bValid = bExtractTme(aaSrc,&xTme);
  
  return bValid;
}

/*----------------------------------------------------------------------------*/
/* valid timestamp in substr?                                                 */
/*----------------------------------------------------------------------------*/
bool DtmLib::bValidTsp(char** aaSrc)
{
  Tsp xTsp;
  
  bool bValid = bExtractTsp(aaSrc,&xTsp);
  
  return bValid;
}

/*----------------------------------------------------------------------------*/
/* valid varchar in substr?                                                   */
/*----------------------------------------------------------------------------*/
bool DtmLib::bValidVch(char** aaSrc)
{
  s48K sBuffer;
  
  Vch* aVch = (Vch*)sBuffer.aStr();
  bool bValid = bExtractVch(aaSrc,aVch);
  
  return bValid;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is argA < argB?                                                            */
/*----------------------------------------------------------------------------*/
bool DtmLib::bLessThan(eDM eDatum, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bLessThan = false;
  
  switch (eDatum) {
    case DM_BCD: bLessThan = Bcd::bLessThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_BOL: bLessThan = Bol::bLessThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_CHR: ASSERT(0); break;
    case DM_DTE: bLessThan = Dte::bLessThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_FLT: bLessThan = Flt::bLessThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_INT: bLessThan = Int::bLessThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_IVL: bLessThan = Ivl::bLessThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TME: bLessThan = Tme::bLessThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TSP: bLessThan = Tsp::bLessThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_VBN: ASSERT(0); break;
    case DM_VCH: bLessThan = Vch::bLessThan(aArgA,nBytesA,aArgB,nBytesB); break;
      
    default:
      THROW("DtmLib::bLessThan: Invalid eDatum %lld"_Str,eDatum);
  }
  
  return bLessThan;
}

/*----------------------------------------------------------------------------*/
/* is argA <= argB?                                                           */
/*----------------------------------------------------------------------------*/
bool DtmLib::bLessEqual(eDM eDatum, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bLessEqual = false;
  
  switch (eDatum) {
    case DM_BCD: bLessEqual = Bcd::bLessEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_BOL: bLessEqual = Bol::bLessEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_CHR: ASSERT(0); break;
    case DM_DTE: bLessEqual = Dte::bLessEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_FLT: bLessEqual = Flt::bLessEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_INT: bLessEqual = Int::bLessEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_IVL: bLessEqual = Ivl::bLessEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TME: bLessEqual = Tme::bLessEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TSP: bLessEqual = Tsp::bLessEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_VBN: ASSERT(0); break;
    case DM_VCH: bLessEqual = Vch::bLessEqual(aArgA,nBytesA,aArgB,nBytesB); break;
      
    default:
      THROW("DtmLib::bLessEqual: Invalid eDatum %lld"_Str,eDatum);
  }
  
  return bLessEqual;
}

/*----------------------------------------------------------------------------*/
/* is argA == argB?                                                           */
/*----------------------------------------------------------------------------*/
bool DtmLib::bEqualTo(eDM eDatum, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bEqualTo = false;
  
  switch (eDatum) {
    case DM_BCD: bEqualTo = Bcd::bEqualTo(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_BOL: bEqualTo = Bol::bEqualTo(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_CHR: ASSERT(0); break;
    case DM_DTE: bEqualTo = Dte::bEqualTo(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_FLT: bEqualTo = Flt::bEqualTo(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_INT: bEqualTo = Int::bEqualTo(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_IVL: bEqualTo = Ivl::bEqualTo(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TME: bEqualTo = Tme::bEqualTo(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TSP: bEqualTo = Tsp::bEqualTo(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_VBN: ASSERT(0); break;
    case DM_VCH: bEqualTo = Vch::bEqualTo(aArgA,nBytesA,aArgB,nBytesB); break;
      
    default:
      THROW("DtmLib::bEqualTo: Invalid eDatum %lld"_Str,eDatum);
  }
  
  return bEqualTo;
}

/*----------------------------------------------------------------------------*/
/* is argA != argB?                                                           */
/*----------------------------------------------------------------------------*/
bool DtmLib::bNotEqual(eDM eDatum, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bNotEqual = false;
  
  switch (eDatum) {
    case DM_BCD: bNotEqual = Bcd::bNotEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_BOL: bNotEqual = Bol::bNotEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_CHR: ASSERT(0); break;
    case DM_DTE: bNotEqual = Dte::bNotEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_FLT: bNotEqual = Flt::bNotEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_INT: bNotEqual = Int::bNotEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_IVL: bNotEqual = Ivl::bNotEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TME: bNotEqual = Tme::bNotEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TSP: bNotEqual = Tsp::bNotEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_VBN: ASSERT(0); break;
    case DM_VCH: bNotEqual = Vch::bNotEqual(aArgA,nBytesA,aArgB,nBytesB); break;
      
    default:
      THROW("DtmLib::bNotEqual: Invalid eDatum %lld"_Str,eDatum);
  }
  
  return bNotEqual;
}

/*----------------------------------------------------------------------------*/
/* is argA >= argB?                                                           */
/*----------------------------------------------------------------------------*/
bool DtmLib::bGreaterEqual(eDM eDatum, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bGreaterEqual = false;
  
  switch (eDatum) {
    case DM_BCD: bGreaterEqual = Bcd::bGreaterEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_BOL: bGreaterEqual = Bol::bGreaterEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_CHR: ASSERT(0); break;
    case DM_DTE: bGreaterEqual = Dte::bGreaterEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_FLT: bGreaterEqual = Flt::bGreaterEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_INT: bGreaterEqual = Int::bGreaterEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_IVL: bGreaterEqual = Ivl::bGreaterEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TME: bGreaterEqual = Tme::bGreaterEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TSP: bGreaterEqual = Tsp::bGreaterEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_VBN: ASSERT(0); break;
    case DM_VCH: bGreaterEqual = Vch::bGreaterEqual(aArgA,nBytesA,aArgB,nBytesB); break;
      
    default:
      THROW("DtmLib::bGreaterEqual: Invalid eDatum %lld"_Str,eDatum);
  }
  
  return bGreaterEqual;
}

/*----------------------------------------------------------------------------*/
/* is argA > argB?                                                            */
/*----------------------------------------------------------------------------*/
bool DtmLib::bGreaterThan(eDM eDatum, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bGreaterThan = false;
  
  switch (eDatum) {
    case DM_BCD: bGreaterThan = Bcd::bGreaterThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_BOL: bGreaterThan = Bol::bGreaterThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_CHR: ASSERT(0); break;
    case DM_DTE: bGreaterThan = Dte::bGreaterThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_FLT: bGreaterThan = Flt::bGreaterThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_INT: bGreaterThan = Int::bGreaterThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_IVL: bGreaterThan = Ivl::bGreaterThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TME: bGreaterThan = Tme::bGreaterThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TSP: bGreaterThan = Tsp::bGreaterThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_VBN: ASSERT(0); break;
    case DM_VCH: bGreaterThan = Vch::bGreaterThan(aArgA,nBytesA,aArgB,nBytesB); default:
      THROW("DtmLib::bGreaterThan: Invalid eDatum %lld"_Str,eDatum);
  }
  
  return bGreaterThan;
}

/*----------------------------------------------------------------------------*/
/* cmp argA with argB (-ve: argA < argB, 0: argA == argB, +ve: argA > argB)   */
/*----------------------------------------------------------------------------*/
i8 DtmLib::iCmp(eDM eDatum, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  i8 iCmp = 0;
  
  switch (eDatum) {
    case DM_BCD: iCmp = Bcd::iCmp(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_BOL: iCmp = Bol::iCmp(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_CHR: ASSERT(0); break;
    case DM_DTE: iCmp = Dte::iCmp(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_FLT: iCmp = Flt::iCmp(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_INT: iCmp = Int::iCmp(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_IVL: iCmp = Ivl::iCmp(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TME: iCmp = Tme::iCmp(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TSP: iCmp = Tsp::iCmp(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_VBN: ASSERT(0); break;
    case DM_VCH: iCmp = Vch::iCmp(aArgA,nBytesA,aArgB,nBytesB); break;
      
    default:
      THROW("DtmLib::iCmp: Invalid eDatum %lld"_Str,eDatum);
  }
  
  return iCmp;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
