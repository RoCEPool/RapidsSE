/*============================================================================*/
/* CsvValue.cpp -- Csv Value                                                  */
/*============================================================================*/

#include "Records.h"

/*----------------------------------------------------------------------------*/
/*-- Public Static -----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* extract a datum from csv field value                                       */
/*----------------------------------------------------------------------------*/
bool CsvValue::bToDtm(Datum* pDatum, Column* pCol, char** aazCrs, Format* pFmt)
{
  bool b = false;
  
  eDM eDatum = pCol->eDatum();

  switch (eDatum) {
    case DM_BCD: b = bToBcd(pDatum,pCol,aazCrs,pFmt); break;
    case DM_BOL: b = bToBol(pDatum,pCol,aazCrs,pFmt); break;
    case DM_DTE: b = bToDte(pDatum,pCol,aazCrs,pFmt); break;
    case DM_FLT: b = bToFlt(pDatum,pCol,aazCrs,pFmt); break;
    case DM_INT: b = bToInt(pDatum,pCol,aazCrs,pFmt); break;
    case DM_IVL: b = bToIvl(pDatum,pCol,aazCrs,pFmt); break;
    case DM_TME: b = bToTme(pDatum,pCol,aazCrs,pFmt); break;
    case DM_TSP: b = bToTsp(pDatum,pCol,aazCrs,pFmt); break;
    case DM_VCH: b = bToBcd(pDatum,pCol,aazCrs,pFmt); break;
      
    default:
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
             "CsvValue::bToDtm: Invalid eDatum : %lld\n"_Stz,eDatum);
  }
  
  return b;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* extract a decimal datum from csv field value                               */
/*----------------------------------------------------------------------------*/
bool CsvValue::bToBcd(Datum* pDatum, Column* pCol, char** aazCrs, Format* pFmt)
{
  SN_Bcd snBcd;
  
  try {
    pDatum->vBcd(snBcd.i(aazCrs,pCol,pFmt));
  }
  catch (Exception xException) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a boolean datum from csv field value                               */
/*----------------------------------------------------------------------------*/
bool CsvValue::bToBol(Datum* pDatum, Column* pCol, char** aazCrs, Format* pFmt)
{
  SN_Bol snBol;
  
  try {
    pDatum->vBol(snBol.i(aazCrs,pCol,pFmt));
  }
  catch (Exception xException) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a date datum from csv field value                      yyyy-mo-ddZ */
/*----------------------------------------------------------------------------*/
bool CsvValue::bToDte(Datum* pDatum, Column* pCol, char** aazCrs, Format* pFmt)
{
  SN_Dte snDte;
  
  try {
    pDatum->vDte(snDte.i(aazCrs,pCol,pFmt));
  }
  catch (Exception xException) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a float datum from csv field value                                 */
/*----------------------------------------------------------------------------*/
bool CsvValue::bToFlt(Datum* pDatum, Column* pCol, char** aazCrs, Format* pFmt)
{
  SN_Flt snFlt;
  
  try {
    pDatum->vFlt(snFlt.d(aazCrs,pCol,pFmt));
  }
  catch (Exception xException) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract an integer datum from csv field value                              */
/*----------------------------------------------------------------------------*/
bool CsvValue::bToInt(Datum* pDatum, Column* pCol, char** aazCrs, Format* pFmt)
{
  SN_Int snInt;
  
  try {
    pDatum->vInt(snInt.i(aazCrs,pCol,pFmt));
  }
  catch (Exception xException) {
    return false;
  }
  
  // ignore any trailing digits (after optional '.')
  if ('.' == **aazCrs) {
    
    // skip the '.' and any digits
    (*aazCrs)++;
    StrLib::vSkipDigits(aazCrs);
  }
  
  return true;
}


/*----------------------------------------------------------------------------*/
/* extract an interval datum from csv field value                             */
/*----------------------------------------------------------------------------*/
bool CsvValue::bToIvl(Datum* pDatum, Column* pCol, char** aazCrs, Format* pFmt)
{
  SN_Ivl snIvl;
  
  try {
    pDatum->vIvl(snIvl.i(aazCrs,pCol,pFmt));
  }
  catch (Exception xException) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a time datum from csv field value                    hh:mi:ss.fffZ */
/*----------------------------------------------------------------------------*/
bool CsvValue::bToTme(Datum* pDatum, Column* pCol, char** aazCrs, Format* pFmt)
{
  SN_Tsp snTsp;
  
  try {
    pDatum->vTsp(snTsp.i(aazCrs,pCol,pFmt));
  }
  catch (Exception xException) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a timestamp datum from csv field value    yyyy-mo-ddThh:mi:ss.fffZ */
/*----------------------------------------------------------------------------*/
bool CsvValue::bToTsp(Datum* pDatum, Column* pCol, char** aazCrs, Format* pFmt)
{
  SN_Tsp snTsp;
  
  try {
    pDatum->vTsp(snTsp.i(aazCrs,pCol,pFmt));
  }
  catch (Exception xException) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a varchar datum from csv field value                               */
/*----------------------------------------------------------------------------*/
bool CsvValue::bToVch(Datum* pDatum, Column* pCol, char** aazCrs, Format* pFmt)
{
  SN_Vch snVch;
  bool bSawEnclosedLeading = false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for enclosed
  bSawEnclosedLeading = bIsEnclosedString(aazCrs,pFmt);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // varchar field value processing
  
  try {
    
    i8 nChars = 0;
    * pDatum = snVch.sz(aazCrs,pCol,pFmt,bSawEnclosedLeading,nChars);

    //pDatum->vStzDump();

  }
  catch (Exception xException) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is the string enclosed?                                                    */
/*----------------------------------------------------------------------------*/
bool CsvValue::bIsEnclosedString(char** aazCrs, Format* pFmt)
{
  if (! pFmt->m_cEnclosedBy_Leading)
    return false;

  char *aStart = *aazCrs;

  // find first non-space character
  while (' ' == **aazCrs)
    (*aazCrs)++;

  // is it an enclosed varchar?
  if (**aazCrs == pFmt->m_cEnclosedBy_Leading) {
    (*aazCrs)++;
    return true;
  }
  else {
    *(aazCrs) = aStart;
    return false;
  }
}

/*----------------------------------------------------------------------------*/
/* is there data?                                                             */
/*----------------------------------------------------------------------------*/
bool CsvValue::bIsThereFieldData(char* aSrc, Format* pFmt)
{
  if ( ! *aSrc
      || *aSrc == pFmt->m_cFieldDelimiter
      || *aSrc == C_NL
      || *aSrc == C_CR
      )
    return false;
  else
    return true;
}

///*----------------------------------------------------------------------------*/
///* is there data?                                                             */
///*----------------------------------------------------------------------------*/
//bool CsvValue::bIsThereRecordData(char* aSrc, Format* pFmt)
//{
//  if ( ! *aSrc
//      || *aSrc == C_NL
//      || *aSrc == C_CR
//      )
//    return false;
//  else
//    return true;
//}
//
/*----------------------------------------------------------------------------*/
/* test for some form of null                                                 */
/*----------------------------------------------------------------------------*/
bool CsvValue::bIsSomeNull(char** aazCrs, Format* pFmt)
{
  /*
   * This function scans aazCrs and attempts to identify the text form of a null.
   * There are three text forms of null: null, \N or '\N'.
   * If a null (and only a null) is detected and
   *  a) the column is nullable, then bValidNull is set to true and the
   *     function returns true; or
   *  b) the column is not nullable, then bValidNull is set to false and the
   *     function returns true.
   * If no form of null is detected then bValidNull is set to false and the
   * function returns false.
   * Importantly a valid null is one of the three forms of null -- there can be
   * no further characters after it. Eg: "nulla" or "null a" are not
   * correct forms of null.
   */

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test if there is any field data

  if ( ! bIsThereFieldData(*aazCrs, pFmt)) {
    return true;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for null, \N, '\N' or [\N]

  char* azStart = *aazCrs;

  if ( (**aazCrs == 'N' || **aazCrs == 'n') && (0 == StrLib::iCompare(*aazCrs+1, "ULL", 3, C_NO_CASE)) ) {
    // skip over token null
    *aazCrs = (*aazCrs) + 4;
  }
  else if (**aazCrs     == pFmt->m_cEscapeChar &&
           *(*aazCrs+1) == 'N') {
    // skip over \N
    *aazCrs = (*aazCrs) + 2;
  }
  else if (**aazCrs     == pFmt->m_cEnclosedBy_Leading &&
           *(*aazCrs+1) == pFmt->m_cEscapeChar &&
           *(*aazCrs+2) == 'N'                        &&
           *(*aazCrs+3) == pFmt->m_cEnclosedBy_Trailing) {
    // skip over '\N' or [\N]
    *aazCrs = (*aazCrs) + 4;
  }
  else if (**aazCrs    == pFmt->m_cEnclosedBy_Leading &&
           ((*(*aazCrs+1) == 'N' || *(*aazCrs+1) == 'n') && (0 == StrLib::iCompare(*aazCrs+2, "ULL", 3, C_NO_CASE))) &&
           *(*aazCrs+5) == pFmt->m_cEnclosedBy_Trailing) {
    // skip over 'NULL'
    *aazCrs = (*aazCrs) + 6;
  }
  else {
    // no null detected
    return false;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // check the column is nullable

  return true;
}

///*----------------------------------------------------------------------------*/
///* test for valid field end                                                   */
///*----------------------------------------------------------------------------*/
//bool CsvValue::bValidFieldEnd(char* aSrc, Format* pFmt)
//{
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // test for end of data
//
//  // valid field end if we've run out of data
//  if (! *aSrc)
//    return true;
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // test for explicit field separator
//
//  // valid field end if we've encounter the field delimiter
//  if (*aSrc == pFmt->m_cFieldDelimiter)
//    return true;
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // test for explicit CsvValue terminator
//
//  if (pFmt->m_cRecordTerminator) {
//
//    // not a valid field end if expected CsvValue terminator is not seen
//    if (*aSrc != pFmt->m_cRecordTerminator)
//      return false;
//
//    // test we see the CsvValue end after the CsvValue termintor
//    bool bValidEnd = bValidRecordEnd(aSrc+1,pFmt);
//
//    return bValidEnd;
//  }
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // test for explicit CsvValue ends
//
//  bool bValidEnd = bValidRecordEnd(aSrc,pFmt);
//
//  return bValidEnd;
//}
//
///*----------------------------------------------------------------------------*/
///* test for valid field end                                                   */
///*----------------------------------------------------------------------------*/
//bool CsvValue::bValidFieldEndDefault(char* aSrc, Format* pFmt)
//{
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // test for end of data
//
//  // valid field end if we've run out of data
//  if (! *aSrc)
//    return true;
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // test for explicit field separator
//
//  // valid field end if we've encounter the field delimiter
//  if (*aSrc == pFmt->m_cFieldDelimiter)
//    return true;
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // test for explicit CsvValue ends
//
//  bool bValidEnd = bValidRecordEnd(aSrc,pFmt);
//
//  return bValidEnd;
//}
//
///*----------------------------------------------------------------------------*/
///* test for valid record end                                                  */
///*----------------------------------------------------------------------------*/
//bool CsvValue::bValidRecordEnd(char* aSrc, Format* pFmt)
//{
//  switch (pFmt->m_eRecordEnd) {
//
//    case RE_NL: // test for \n
//      if (C_NL == *aSrc)
//        return true;
//      else
//        return false;
//
//    case RE_CR: // test for \r
//      if (C_CR == *aSrc)
//        return true;
//      else
//        return false;
//
//    case RE_CRNL: // test for \r\n
//      if (C_CR == *aSrc && C_NL == *(aSrc+1))
//        return true;
//      else
//        return false;
//
//    default: // otherwise not a field end
//      NEVER_REACH;
//      return false;
//  }
//}
//

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
