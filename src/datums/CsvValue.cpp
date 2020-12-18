/*============================================================================*/
/* CsvValue.cpp -- Csv Value                                                  */
/*============================================================================*/

#include "Datums.h"

/*----------------------------------------------------------------------------*/
/*-- Public Static -----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* extract a datum from csv field value                                       */
/*----------------------------------------------------------------------------*/
bool CsvValue::bToCell(Dtm* aDtm, Column* pCol,
                        char** aazCrs, Format* pFmt)
{
  bool b = false;
  
  eDM eDatum = pCol->eDatum();

  switch (eDatum) {
    case DM_BCD: b = bToCellBcd(aDtm,pCol,aazCrs,pFmt); break;
    case DM_BOL: b = bToCellBol(aDtm,pCol,aazCrs,pFmt); break;
    case DM_DTE: b = bToCellDte(aDtm,pCol,aazCrs,pFmt); break;
    case DM_FLT: b = bToCellFlt(aDtm,pCol,aazCrs,pFmt); break;
    case DM_INT: b = bToCellInt(aDtm,pCol,aazCrs,pFmt); break;
    case DM_IVL: b = bToCellIvl(aDtm,pCol,aazCrs,pFmt); break;
    case DM_TME: b = bToCellTme(aDtm,pCol,aazCrs,pFmt); break;
    case DM_TSP: b = bToCellTsp(aDtm,pCol,aazCrs,pFmt); break;
    case DM_VCH: b = bToCellVch(aDtm,pCol,aazCrs,pFmt); break;
      
    default:
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
             "CsvValue::bToCell: Invalid eDatum : %lld\n"_Stz,eDatum);
  }
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* extract a datum from csv field value                                       */
/*----------------------------------------------------------------------------*/
bool CsvValue::bToDatum(Datum_h* pDatum, Column* pCol,
                        char** aazCrs, Format* pFmt)
{
  bool b = false;
  
  eDM eDatum = pCol->eDatum();

  switch (eDatum) {
    case DM_BCD: b = bToDatumBcd(pDatum,pCol,aazCrs,pFmt); break;
    case DM_BOL: b = bToDatumBol(pDatum,pCol,aazCrs,pFmt); break;
    case DM_DTE: b = bToDatumDte(pDatum,pCol,aazCrs,pFmt); break;
    case DM_FLT: b = bToDatumFlt(pDatum,pCol,aazCrs,pFmt); break;
    case DM_INT: b = bToDatumInt(pDatum,pCol,aazCrs,pFmt); break;
    case DM_IVL: b = bToDatumIvl(pDatum,pCol,aazCrs,pFmt); break;
    case DM_TME: b = bToDatumTme(pDatum,pCol,aazCrs,pFmt); break;
    case DM_TSP: b = bToDatumTsp(pDatum,pCol,aazCrs,pFmt); break;
    case DM_VCH: b = bToDatumVch(pDatum,pCol,aazCrs,pFmt); break;
      
    default:
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
             "CsvValue::bToDatum: Invalid eDatum : %lld\n"_Stz,eDatum);
  }
  
  return b;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* extract a decimal datum from csv field value                               */
/*----------------------------------------------------------------------------*/
bool CsvValue::bToCellBcd(Dtm* aDtm, Column* pCol,
                           char** aazCrs, Format* pFmt)
{
  SN_Bcd snBcd;
  
  try {
    snBcd.i(aazCrs,pCol->iColumnLength(), pCol->iColumnScale(),pFmt);
  }
  catch (Exception xException) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a boolean datum from csv field value                               */
/*----------------------------------------------------------------------------*/
bool CsvValue::bToCellBol(Dtm* aDtm, Column* pCol,
                           char** aazCrs, Format* pFmt)
{
  SN_Bol snBol;
  
  try {
    snBol.i(aazCrs,pFmt);
  }
  catch (Exception xException) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a date datum from csv field value                      yyyy-mo-ddZ */
/*----------------------------------------------------------------------------*/
bool CsvValue::bToCellDte(Dtm* aDtm, Column* pCol,
                           char** aazCrs, Format* pFmt)
{
  SN_Dte snDte;
  
  try {
    snDte.i(aazCrs,pFmt);
  }
  catch (Exception xException) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a float datum from csv field value                                 */
/*----------------------------------------------------------------------------*/
bool CsvValue::bToCellFlt(Dtm* aDtm, Column* pCol,
                           char** aazCrs, Format* pFmt)
{
  SN_Flt snFlt;
  
  try {
    snFlt.d(aazCrs,pFmt);
  }
  catch (Exception xException) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract an integer datum from csv field value                              */
/*----------------------------------------------------------------------------*/
bool CsvValue::bToCellInt(Dtm* aDtm, Column* pCol,
                           char** aazCrs, Format* pFmt)
{
  SN_Int snInt;
  
  try {
    snInt.i(aazCrs,pFmt);
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
bool CsvValue::bToCellIvl(Dtm* aDtm, Column* pCol,
                           char** aazCrs, Format* pFmt)
{
  SN_Ivl snIvl;
  
  try {
    snIvl.i(aazCrs,pFmt);
  }
  catch (Exception xException) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a time datum from csv field value                    hh:mi:ss.fffZ */
/*----------------------------------------------------------------------------*/
bool CsvValue::bToCellTme(Dtm* aDtm, Column* pCol,
                           char** aazCrs, Format* pFmt)
{
  SN_Tme snTme;
  
  try {
    snTme.i(aazCrs,pFmt);
  }
  catch (Exception xException) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a timestamp datum from csv field value    yyyy-mo-ddThh:mi:ss.fffZ */
/*----------------------------------------------------------------------------*/
bool CsvValue::bToCellTsp(Dtm* aDtm, Column* pCol,
                           char** aazCrs, Format* pFmt)
{
  SN_Tsp snTsp;
  
  try {
    snTsp.i(aazCrs,pFmt);
  }
  catch (Exception xException) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a varchar datum from csv field value                               */
/*----------------------------------------------------------------------------*/
bool CsvValue::bToCellVch(Dtm* aDtm, Column* pCol,
                           char** aazCrs, Format* pFmt)
{
  NEVER_REACH; // not supported yet
  
  return true;
}



/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* extract a decimal datum from csv field value                               */
/*----------------------------------------------------------------------------*/
bool CsvValue::bToDatumBcd(Datum_h* pDatum, Column* pCol,
                           char** aazCrs, Format* pFmt)
{
  SN_Bcd snBcd;
  
  try {
    pDatum->vBcd(snBcd.i(aazCrs,pCol->iColumnLength(), pCol->iColumnScale(),pFmt));
  }
  catch (Exception xException) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a boolean datum from csv field value                               */
/*----------------------------------------------------------------------------*/
bool CsvValue::bToDatumBol(Datum_h* pDatum, Column* pCol,
                           char** aazCrs, Format* pFmt)
{
  SN_Bol snBol;
  
  try {
    pDatum->vBol(snBol.i(aazCrs,pFmt));
  }
  catch (Exception xException) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a date datum from csv field value                      yyyy-mo-ddZ */
/*----------------------------------------------------------------------------*/
bool CsvValue::bToDatumDte(Datum_h* pDatum, Column* pCol,
                           char** aazCrs, Format* pFmt)
{
  SN_Dte snDte;
  
  try {
    pDatum->vDte(snDte.i(aazCrs,pFmt));
  }
  catch (Exception xException) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a float datum from csv field value                                 */
/*----------------------------------------------------------------------------*/
bool CsvValue::bToDatumFlt(Datum_h* pDatum, Column* pCol,
                           char** aazCrs, Format* pFmt)
{
  SN_Flt snFlt;
  
  try {
    pDatum->vFlt(snFlt.d(aazCrs,pFmt));
  }
  catch (Exception xException) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract an integer datum from csv field value                              */
/*----------------------------------------------------------------------------*/
bool CsvValue::bToDatumInt(Datum_h* pDatum, Column* pCol,
                           char** aazCrs, Format* pFmt)
{
  SN_Int snInt;
  
  try {
    pDatum->vInt(snInt.i(aazCrs,pFmt));
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
bool CsvValue::bToDatumIvl(Datum_h* pDatum, Column* pCol,
                           char** aazCrs, Format* pFmt)
{
  SN_Ivl snIvl;
  
  try {
    pDatum->vIvl(snIvl.i(aazCrs,pFmt));
  }
  catch (Exception xException) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a time datum from csv field value                    hh:mi:ss.fffZ */
/*----------------------------------------------------------------------------*/
bool CsvValue::bToDatumTme(Datum_h* pDatum, Column* pCol,
                           char** aazCrs, Format* pFmt)
{
  SN_Tme snTme;
  
  try {
    pDatum->vTme(snTme.i(aazCrs,pFmt));
  }
  catch (Exception xException) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a timestamp datum from csv field value    yyyy-mo-ddThh:mi:ss.fffZ */
/*----------------------------------------------------------------------------*/
bool CsvValue::bToDatumTsp(Datum_h* pDatum, Column* pCol,
                           char** aazCrs, Format* pFmt)
{
  SN_Tsp snTsp;
  
  try {
    pDatum->vTsp(snTsp.i(aazCrs,pFmt));
  }
  catch (Exception xException) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a varchar datum from csv field value                               */
/*----------------------------------------------------------------------------*/
bool CsvValue::bToDatumVch(Datum_h* pDatum, Column* pCol,
                           char** aazCrs, Format* pFmt)
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
    * pDatum = snVch.sz(aazCrs,pCol->iColumnLength(),pFmt,bSawEnclosedLeading,nChars);

    //pDatum->vStzDump();

  }
  catch (Exception xException) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* test for some form of null                                                 */
/*----------------------------------------------------------------------------*/
bool CsvValue::bIsSomeNull(char** aazCrs, Format* pFmt, bool bSkipTrailingSpaces)
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
  
  char* aStart = *aazCrs;
  
  if ( ! bIsThereFieldData(*aazCrs, pFmt)) {
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for null, \N, '\N' or [\N]
  
  if ( (**aazCrs == 'N' || **aazCrs == 'n') &&
      (0 == StrLib::iCompare(*aazCrs+1, "ULL", 3, C_NO_CASE))) {
      
    // skip over token null
    *aazCrs = (*aazCrs) + 4;
  }
  else if (**aazCrs     == pFmt->m_cEscapeChar &&
           *(*aazCrs+1) == 'N') {
    // skip over \N
    *aazCrs = (*aazCrs) + 2;
  }
  else if (**aazCrs     == pFmt->m_cEnclosedBy_Leading  &&
           *(*aazCrs+1) == pFmt->m_cEscapeChar          &&
           *(*aazCrs+2) == 'N'                          &&
           *(*aazCrs+3) == pFmt->m_cEnclosedBy_Trailing) {
    // skip over '\N' or [\N]
    *aazCrs = (*aazCrs) + 4;
  }
  else if (**aazCrs    == pFmt->m_cEnclosedBy_Leading    &&
           ((*(*aazCrs+1) == 'N' || *(*aazCrs+1) == 'n') &&
            (0 == StrLib::iCompare(*aazCrs+2, "ULL", 3, C_NO_CASE))) &&
           *(*aazCrs+5) == pFmt->m_cEnclosedBy_Trailing) {
    // skip over 'NULL'
    *aazCrs = (*aazCrs) + 6;
  }
  else {
    // no null detected
    return false;
  }
  
  // optionally skip trailing spaces
  if (bSkipTrailingSpaces)
    StrLib::vSkipSpaces(aazCrs);
  
  // check it was "null" only
  if (! bValidFieldEnd(*aazCrs,pFmt)) {
    // no well formed null detected
    *(aazCrs) = aStart;
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* sprint into target                                                         */
/*----------------------------------------------------------------------------*/
void CsvValue::vInto(Stz *szTgt, void *avSrc, Column *pCol)
{
  eDM eDatum = pCol->eDatum();
  char* az   = (char*)szTgt;
  i8 iValue  = *(i8*)avSrc;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  if (C_DATUM_NULL == iValue) {
    StrLib::vCopy(az,(char*)C_NULL);
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  switch (eDatum) {
      
    case DM_BCD: {
      SP_Bcd SP_Bcd;
      StrLib::vCopy(az,(char*)SP_Bcd.sz(iValue));
      break;
    }
      
    case DM_BOL: {
      SP_Bol SP_Bol;
      StrLib::vCopy(az,(char*)SP_Bol.sz(iValue));
      break;
    }
      
    case DM_DTE: {
      SP_Dte SP_Dte;
      StrLib::vCopy(az,(char*)SP_Dte.sz(iValue));
      break;
    }
      
    case DM_FLT: {
      SP_Flt SP_Flt;
      StrLib::vCopy(az,(char*)SP_Flt.sz(*(d8*)avSrc));
      break;
    }
      
    case DM_INT: {
      SP_Int spInt;
      StrLib::vCopy(az,(char*)spInt.sz(iValue));
      break;
    }
      
    case DM_IVL: {
      SP_Ivl spIvl;
      StrLib::vCopy(az,(char*)spIvl.sz(iValue));
      break;
    }
      
    case DM_TME: {
      SP_Tme SP_Tme;
      StrLib::vCopy(az,(char*)SP_Tme.sz(iValue));
      break;
    }
      
    case DM_TSP: {
      SP_Tsp SP_Tsp;
      StrLib::vCopy(az,(char*)SP_Tsp.sz(iValue));
      break;
    }
      
    case DM_VCH: {
      SP_Vch SP_Vch;
      StrLib::vCopy(az,(char*)SP_Vch.sz((char*)avSrc));
      break;
    }
      
    default:
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
             "Sprint::Into: Invalid eDatum: %lld\n"_Stz,
             eDatum);
  }
  
}

/*----------------------------------------------------------------------------*/
/*-- Private Static ----------------------------------------------------------*/
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

/*----------------------------------------------------------------------------*/
/* test for valid field end                                                   */
/*----------------------------------------------------------------------------*/
bool CsvValue::bValidFieldEnd(char* aSrc, Format* pFmt)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for end of data
  
  // valid field end if we've run out of data
  if (! *aSrc)
    return true;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for explicit field separator
  
  // valid field end if we've encounter the field delimiter
  if (*aSrc == pFmt->m_cFieldDelimiter)
    return true;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for explicit CsvValue terminator
  
  if (pFmt->m_cRecordTerminator) {
    
    // not a valid field end if expected CsvValue terminator is not seen
    if (*aSrc != pFmt->m_cRecordTerminator)
      return false;
    
    // test we see the CsvValue end after the CsvValue termintor
    bool bValidEnd = bValidRecordEnd(aSrc+1,pFmt);
    
    return bValidEnd;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for explicit CsvValue ends
  
  bool bValidEnd = bValidRecordEnd(aSrc,pFmt);
  
  return bValidEnd;
}

/*----------------------------------------------------------------------------*/
/* test for valid record end                                                  */
/*----------------------------------------------------------------------------*/
bool CsvValue::bValidRecordEnd(char* aSrc, Format* pFmt)
{
  switch (pFmt->m_eLineEnd) {
      
    case LE_NL: // test for \n
      if (C_NL == *aSrc)
        return true;
      else
        return false;
      
    case LE_CR: // test for \r
      if (C_CR == *aSrc)
        return true;
      else
        return false;
      
    case LE_CRNL: // test for \r\n
      if (C_CR == *aSrc && C_NL == *(aSrc+1))
        return true;
      else
        return false;
      
    default: // otherwise not a field end
      NEVER_REACH;
      return false;
  }
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
