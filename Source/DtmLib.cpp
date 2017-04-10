/*============================================================================*/
/* DtmLib.cpp -- Datum Library                                                */
/*============================================================================*/

#include "Datums.h"

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
/* valid date in substr?                                                      */
/*----------------------------------------------------------------------------*/
bool DtmLib::bValidDte(char** aaSrc)
{
  Dte xDte;
  
  bool bValid = bExtractDte(aaSrc,&xDte);
  
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
