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
  return false;
}

/*----------------------------------------------------------------------------*/
/* extract a boolean from substr                                              */
/*----------------------------------------------------------------------------*/
bool DtmLib::bExtractBol(char** aaSrc, Bol* aBoolean)
{
  return false;
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
  aDate->m_uYear = uField;
  
  // skip - separator
  if ('-' != *(*aaSrc)++)
    return false;
  
  // extract month
  if ( ! StrLib::bExtractU8(aaSrc,&uField) )
    return false;
  aDate->m_uMonth = uField;
  
  // skip - separator
  if ('-' != *(*aaSrc)++) return false;
  
  // extract day
  if ( ! StrLib::bExtractU8(aaSrc,&uField) )
    return false;
  aDate->m_uDay = uField;
  
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
  return false;
}

/*----------------------------------------------------------------------------*/
/* extract an integer from substr                                             */
/*----------------------------------------------------------------------------*/
bool DtmLib::bExtractInt(char** aaSrc, Int* aInteger)
{
  return false;
}

/*----------------------------------------------------------------------------*/
/* extract an interval from substr                                            */
/*----------------------------------------------------------------------------*/
bool DtmLib::bExtractIvl(char** aaSrc, Ivl* aInterval)
{
  return false;
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
  aTime->m_uHour = uField;
  
  // skip : separator
  if (':' != *(*aaSrc)++)
    return true;
  
  // extract minute
  if ( ! StrLib::bExtractU8(aaSrc,&uField) )
    return true;
  aTime->m_uMinute = uField;
  
  // skip : separator
  if (':' != *(*aaSrc)++)
    return true;
  
  // extract second
  if ( ! StrLib::bExtractU8(aaSrc,&uField) )
    return true;
  aTime->m_uSecond = uField;
  
  // extract fraction (assuming milliseconds)
  if ('.' == **aaSrc) {
    (*aaSrc)++;
    if ( StrLib::bExtractU8(aaSrc,&uField) )
      aTime->m_uMilli = uField;
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
  aTimestamp->m_uYear = uField;
  
  // skip - separator
  if ('-' != *(*aaSrc)++)
    return false;
  
  // extract month
  if ( ! StrLib::bExtractU8(aaSrc,&uField) )
    return false;
  aTimestamp->m_uMonth = uField;
  
  // skip - separator
  if ('-' != *(*aaSrc)++) return false;
  
  // extract day
  if ( ! StrLib::bExtractU8(aaSrc,&uField) )
    return false;
  aTimestamp->m_uDay = uField;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - -
  // time portion
  
  // skip T or space separator
  if ( ! ('T' == **aaSrc || ' ' == **aaSrc) )
    return true;
  (*aaSrc)++;
  
  // extract hour
  if ( ! StrLib::bExtractU8(aaSrc,&uField) )
    return true;
  aTimestamp->m_uHour = uField;
  
  // skip : separator
  if (':' != *(*aaSrc)++)
    return true;
  
  // extract minute
  if ( ! StrLib::bExtractU8(aaSrc,&uField) )
    return true;
  aTimestamp->m_uMinute = uField;
  
  // skip : separator
  if (':' != *(*aaSrc)++)
    return true;
  
  // extract second
  if ( ! StrLib::bExtractU8(aaSrc,&uField) )
    return true;
  aTimestamp->m_uSecond = uField;
  
  // extract fraction (assuming milliseconds)
  if ('.' == **aaSrc) {
    (*aaSrc)++;
    if ( StrLib::bExtractU8(aaSrc,&uField) )
      aTimestamp->m_uMilli = uField;
   }
  
  // skip Z separator
  if ('Z' == **aaSrc)
    (*aaSrc)++;
  
  return true;
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
/* is argA < argB?                                                            */
/*----------------------------------------------------------------------------*/
bool DtmLib::bLessThan(eDM eDatum, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bLessThan = false;
  
  switch (eDatum) {
    case DM_BCD: bLessThan = false; break;
    case DM_BOL: bLessThan = false; break;
    case DM_CHR: bLessThan = false; break;
    case DM_DTE: bLessThan = false; break;
    case DM_FLT: bLessThan = false; break;
    case DM_INT: bLessThan = Int::bLessThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_IVL: bLessThan = false; break;
    case DM_TME: bLessThan = false; break;
    case DM_TSP: bLessThan = false; break;
    case DM_VBN: bLessThan = false; break;
    case DM_VCH: bLessThan = false; break;

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
    case DM_BCD: bLessEqual = false; break;
    case DM_BOL: bLessEqual = false; break;
    case DM_CHR: bLessEqual = false; break;
    case DM_DTE: bLessEqual = false; break;
    case DM_FLT: bLessEqual = false; break;
    case DM_INT: bLessEqual = Int::bLessEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_IVL: bLessEqual = false; break;
    case DM_TME: bLessEqual = false; break;
    case DM_TSP: bLessEqual = false; break;
    case DM_VBN: bLessEqual = false; break;
    case DM_VCH: bLessEqual = false; break;
      
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
    case DM_BCD: bEqualTo = false; break;
    case DM_BOL: bEqualTo = false; break;
    case DM_CHR: bEqualTo = false; break;
    case DM_DTE: bEqualTo = false; break;
    case DM_FLT: bEqualTo = false; break;
    case DM_INT: bEqualTo = Int::bEqualTo(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_IVL: bEqualTo = false; break;
    case DM_TME: bEqualTo = false; break;
    case DM_TSP: bEqualTo = false; break;
    case DM_VBN: bEqualTo = false; break;
    case DM_VCH: bEqualTo = false; break;
      
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
    case DM_BCD: bNotEqual = false; break;
    case DM_BOL: bNotEqual = false; break;
    case DM_CHR: bNotEqual = false; break;
    case DM_DTE: bNotEqual = false; break;
    case DM_FLT: bNotEqual = false; break;
    case DM_INT: bNotEqual = Int::bNotEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_IVL: bNotEqual = false; break;
    case DM_TME: bNotEqual = false; break;
    case DM_TSP: bNotEqual = false; break;
    case DM_VBN: bNotEqual = false; break;
    case DM_VCH: bNotEqual = false; break;
      
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
    case DM_BCD: bGreaterEqual = false; break;
    case DM_BOL: bGreaterEqual = false; break;
    case DM_CHR: bGreaterEqual = false; break;
    case DM_DTE: bGreaterEqual = false; break;
    case DM_FLT: bGreaterEqual = false; break;
    case DM_INT: bGreaterEqual = Int::bGreaterEqual(aArgA,nBytesA,aArgB,nBytesB);
                 break;
    case DM_IVL: bGreaterEqual = false; break;
    case DM_TME: bGreaterEqual = false; break;
    case DM_TSP: bGreaterEqual = false; break;
    case DM_VBN: bGreaterEqual = false; break;
    case DM_VCH: bGreaterEqual = false; break;
      
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
    case DM_BCD: bGreaterThan = false; break;
    case DM_BOL: bGreaterThan = false; break;
    case DM_CHR: bGreaterThan = false; break;
    case DM_DTE: bGreaterThan = false; break;
    case DM_FLT: bGreaterThan = false; break;
    case DM_INT: bGreaterThan = Int::bGreaterThan(aArgA,nBytesA,aArgB,nBytesB);
                 break;
    case DM_IVL: bGreaterThan = false; break;
    case DM_TME: bGreaterThan = false; break;
    case DM_TSP: bGreaterThan = false; break;
    case DM_VBN: bGreaterThan = false; break;
    case DM_VCH: bGreaterThan = false; break;
      
    default:
      THROW("DtmLib::bGreaterThan: Invalid eDatum %lld"_Str,eDatum);
  }
  
  return bGreaterThan;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
