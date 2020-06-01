/*============================================================================*/
/* CsvField.cpp -- Csv Field                                                  */
/*============================================================================*/

#include "Records.h"

/*----------------------------------------------------------------------------*/
/*-- Public Static -----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse csv field and extract next datum                                     */
/*----------------------------------------------------------------------------*/
bool CsvField::bExtractDtm(Datum* pDatum, Column* pCol,
                              char** aazCrs, Format* pFmt)
{
  eDM eDatum = pCol->eDatum();
  
  // - - - -
  // leading
  
  bool bSawEnclosedLeading;
  if (DM_VCH == eDatum)
    bSawEnclosedLeading = false;
  else
    bSawEnclosedLeading =
      StrLib::bSkipLeading(aazCrs,pFmt->m_cEnclosedBy_Leading);
  
  // - - - -
  // null
  
  bool bSkipTrailingSpaces = pCol->eDatum() == DM_VCH ? false : true;
  if (CsvValue::bIsSomeNull(aazCrs,pFmt,bSkipTrailingSpaces)) {
    if ( ! pCol->bIsNullable() ) {
      // a null was detected in a non-nullable column
      return false;
    }
    pDatum->vSetNull();
    goto L_TRAILING;
  }
  
  // - - - -
  // value
  
  bool b;
  switch (eDatum) {
    case DM_BCD: b = CsvValue::bToBcd(pDatum,pCol,aazCrs,pFmt); break;
    case DM_BOL: b = CsvValue::bToBol(pDatum,pCol,aazCrs,pFmt); break;
    case DM_DTE: b = CsvValue::bToDte(pDatum,pCol,aazCrs,pFmt); break;
    case DM_FLT: b = CsvValue::bToFlt(pDatum,pCol,aazCrs,pFmt); break;
    case DM_INT: b = CsvValue::bToInt(pDatum,pCol,aazCrs,pFmt); break;
    case DM_IVL: b = CsvValue::bToIvl(pDatum,pCol,aazCrs,pFmt); break;
    case DM_TME: b = CsvValue::bToTme(pDatum,pCol,aazCrs,pFmt); break;
    case DM_TSP: b = CsvValue::bToTsp(pDatum,pCol,aazCrs,pFmt); break;
    case DM_VCH: b = CsvValue::bToVch(pDatum,pCol,aazCrs,pFmt); break;
    default:
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
             "KeyBuilder::bExtract: Invalid eDatum : %lld\n"_Stz,eDatum);
  }
  
  if (! b)
    return false;
  
  // - - - - -
  // trailing
  
L_TRAILING:
  
  bool bSawEnclosedTrailing =
  StrLib::bSkipTrailing(aazCrs,bSawEnclosedLeading,pFmt->m_cEnclosedBy_Trailing);
  if (! bSawEnclosedTrailing)
    return false;
  
  // should see a valid field end
  if ( ! bValidFieldEnd(*aazCrs,pFmt) )
    return false;
  
  
  
  
  // move past the field delimiter
  //   (but not past the KeyBuilder terminator or KeyBuilder ends)
  
  // - - - -
  
  if (pCol->bIsLastColumn()) {
    // do not expect to see a field delimiter
    if ( pFmt->m_cFieldDelimiter &&
        pFmt->m_cRecordTerminator &&
        pFmt->m_cFieldDelimiter != pFmt->m_cRecordTerminator &&
        *(*aazCrs) == pFmt->m_cFieldDelimiter ) {
      // saw a field delimiter after last field (incorrect)
      return false;
    }
    /*
     // did not see a field delimiter (correct)
     if ( pFmt->m_cFieldDelimiter &&
     *(*aazCrs) == pFmt->m_cFieldDelimiter )
     (*aazCrs)++;
     */
  }
  else {
    // expect to see a field delimiter
    if ( pFmt->m_cFieldDelimiter &&
        *(*aazCrs) == pFmt->m_cFieldDelimiter )
      // saw a field delimiter after non-last field (correct)
      (*aazCrs)++;
    else
      // did not see a field delimiter after non-last field (incorrect)
      return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* test for valid field end                                                   */
/*----------------------------------------------------------------------------*/
bool CsvField::bValidFieldEnd(char* aSrc, Format* pFmt)
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
bool CsvField::bValidRecordEnd(char* aSrc, Format* pFmt)
{
  switch (pFmt->m_eRecordEnd) {

    case RE_NL: // test for \n
      if (C_NL == *aSrc)
        return true;
      else
        return false;

    case RE_CR: // test for \r
      if (C_CR == *aSrc)
        return true;
      else
        return false;

    case RE_CRNL: // test for \r\n
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
