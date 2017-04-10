/*============================================================================*/
/* DtmLib.cpp -- Datum Library                                                */
/*============================================================================*/

#include "Datums.h"

/*---------------------------------------------------------------------------*/
/* extract a timestamp from substr                  yyyy-mo-ddThh:mi:ss.fffZ */
/*---------------------------------------------------------------------------*/
bool DtmLib::bExtractTsp(char** aaSrc, Tsp* aTime)
{
  // refer: http://en.wikipedia.org/wiki/ISO_8601

  i8 iField;
  
  aTime->vInitZero();
  
  if ( ! *aaSrc )
    return false;

  // - - - - - - - - - - - - - - - - - - - - - - - - -
  // date portion
  
  // extract year
  if ( ! StrLib::bExtractI8(aaSrc,&iField) )
    return false;
  aTime->m_uYear = (u8)iField;
  
  // skip - separator
  if ('-' != *(*aaSrc)++)
    return false;
  
  // extract month
  if ( ! StrLib::bExtractI8(aaSrc,&iField) )
    return false;
  aTime->m_uMonth = (u8)iField;
  
  // skip - separator
  if ('-' != *(*aaSrc)++) return false;
  
  // extract day
  if ( ! StrLib::bExtractI8(aaSrc,&iField) )
    return false;
  aTime->m_uDay = (u8)iField;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - -
  // time portion
  
  // skip T or space separator
  if ( ! ('T' == **aaSrc || ' ' == **aaSrc) )
    return true;
  (*aaSrc)++;
  
  // extract hour
  if ( ! StrLib::bExtractI8(aaSrc,&iField) )
    return true;
  aTime->m_uHour = (u8)iField;
  
  // skip : separator
  if (':' != *(*aaSrc)++)
    return true;
  
  // extract minute
  if ( ! StrLib::bExtractI8(aaSrc,&iField) )
    return true;
  aTime->m_uMinute = (u8)iField;
  
  // skip : separator
  if (':' != *(*aaSrc)++)
    return true;
  
  // extract second
  if ( ! StrLib::bExtractI8(aaSrc,&iField) )
    return true;
  aTime->m_uSecond = (u8)iField;
  
  // extract fraction (assuming milliseconds)
  if ('.' == **aaSrc) {
    (*aaSrc)++;
    if ( StrLib::bExtractI8(aaSrc,&iField) )
      aTime->m_uMilli  = (u8)iField;
   }
  
  // skip Z separator
  if ('Z' == **aaSrc)
    (*aaSrc)++;
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
