/*============================================================================*/
/* RowExtractor.cpp -- RowExtractor                                           */
/*============================================================================*/

#include "Buffers.h"
#include "AM.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Public Static -----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add element to entwrk key                                                  */
/*----------------------------------------------------------------------------*/
void RowExtractor::vAddElement(EntWrk* pEntWrk, Column* pCol,
                               Stz* szSrc, Format* pFmt)
{
  i8 nBytes;

  char* aSrc = (char*)szSrc;
  char* aDst = (char*)pEntWrk->ayElementCrs();

  if (DM_VCH == pCol->eDatum()) {
    nBytes = StrLib::iWordFlip(aDst,aSrc);
    pEntWrk->vIncKeyBytes(nBytes);
    return;
  }

  Datum oDatum;

  bool b = RowExtractor::bExtractDtm(&oDatum,pCol,&aSrc,pFmt);
  ASSERT(b);

  if (oDatum.bIsNull()) {
    nBytes = Null::nCopy((byte*)aDst);
    pEntWrk->vIncKeyBytes(nBytes);
    return;
  }

  nBytes = Dtm::nCopy((byte*)aDst,oDatum.iInt());
  pEntWrk->vIncKeyBytes(nBytes);
}

/*----------------------------------------------------------------------------*/
/*-- Private Static ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* extract key element                                                        */
/*----------------------------------------------------------------------------*/
bool RowExtractor::bExtractDtm(Datum* pDatum, Column* pCol, char** aazCrs, Format* pFmt)
{
  bool b = false;
  
  eDM eDatum = pCol->eDatum();
  
  i8 iBcd, iBol, iInt, iIvl;
  
  d8 dFlt;
  
  i8 iYear, iMonth, iDay, iHour, iMinute, iSecond, iMilli, iMicro;
  
  switch (eDatum) {
    case DM_BCD:
      b = StrLib::bExtractDecimal(aazCrs,iBcd,
                                  pCol->iColumnLength(),
                                  pCol->iColumnScale());
      pDatum->vBcd(iBcd);
      break;
      
    case DM_BOL:
      b = StrLib::bExtractBoolean(aazCrs,iBol);
      pDatum->vBol(iBol);
      break;
      
    case DM_DTE:
      b = StrLib::bExtractDate2(aazCrs, iYear, iMonth, iDay,
                                pFmt->m_eDateComponent,
                                pFmt->m_cDateSeparator);
      if (!b)
        break;
      
      pDatum->vDte(iYear,iMonth,iDay);
      break;
      
    case DM_FLT:
      b = StrLib::bExtractFloat(aazCrs,dFlt);
      pDatum->vFlt(dFlt);
      break;
      
    case DM_INT:
      b = StrLib::bExtractInteger(aazCrs,&iInt, true/*bWithSign*/);
      pDatum->vInt(iInt);
      break;
      
    case DM_IVL:
      b = StrLib::bExtractInterval(aazCrs, iSecond, iMilli);
      iMicro = iMilli * C_1K;
      pDatum->vIvl(iSecond,iMicro);
      break;
      
    case DM_TME:
      b = StrLib::bExtractTime(aazCrs, iHour, iMinute, iSecond, iMicro);
      
      if (!b)
        break;
      
      pDatum->vTme(iHour,iMinute,iSecond,iMicro);
      break;
      
    case DM_TSP:
      b = StrLib::bExtractTimestamp2(aazCrs, iYear, iMonth, iDay,
                                     iHour, iMinute, iSecond, iMicro,
                                     pFmt->m_eDateComponent,
                                     pFmt->m_cDateSeparator);
      
      if (!b)
        break;
      
      pDatum->vTsp(iYear,iMonth,iDay,iHour,iMinute,iSecond,iMicro);
      break;
      
    case DM_VCH: {

      i8 nChars = 0;
      b = StrLib::bExtractVarcharDefault(aazCrs,
                                         pDatum->azChar(),
                                         pCol->iColumnLength(),
                                         nChars);
      pDatum->vStzSize(nChars + 1);
      
      //pDatum->vStzDump();
      
      break;
    }
      
    default:
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
             "RowExtractor::bExtractDtm: Invalid eDatum : %lld\n"_Stz,eDatum);
  }
  
  return b;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is the string enclosed?                                                    */
/*----------------------------------------------------------------------------*/
bool RowExtractor::bIsEnclosedString(char** aazCrs, Format* pFmt)
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
bool RowExtractor::bIsThereFieldData(char* aSrc, Format* pFmt)
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
/* is there data?                                                             */
/*----------------------------------------------------------------------------*/
bool RowExtractor::bIsThereRecordData(char* aSrc, Format* pFmt)
{
  if ( ! *aSrc
      || *aSrc == C_NL
      || *aSrc == C_CR
      )
    return false;
  else
    return true;
}

/*----------------------------------------------------------------------------*/
/* test for some form of null                                                 */
/*----------------------------------------------------------------------------*/
bool RowExtractor::bIsSomeNull(char** aazCrs, Format* pFmt)
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

/*----------------------------------------------------------------------------*/
/* test for valid field end                                                   */
/*----------------------------------------------------------------------------*/
bool RowExtractor::bValidFieldEnd(char* aSrc, Format* pFmt)
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
  // test for explicit RowExtractor terminator
  
  if (pFmt->m_cRecordTerminator) {
    
    // not a valid field end if expected RowExtractor terminator is not seen
    if (*aSrc != pFmt->m_cRecordTerminator)
      return false;
    
    // test we see the RowExtractor end after the RowExtractor termintor
    bool bValidEnd = bValidRecordEnd(aSrc+1,pFmt);
    
    return bValidEnd;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for explicit RowExtractor ends
  
  bool bValidEnd = bValidRecordEnd(aSrc,pFmt);
  
  return bValidEnd;
}

/*----------------------------------------------------------------------------*/
/* test for valid field end                                                   */
/*----------------------------------------------------------------------------*/
bool RowExtractor::bValidFieldEndDefault(char* aSrc, Format* pFmt)
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
  // test for explicit RowExtractor ends
  
  bool bValidEnd = bValidRecordEnd(aSrc,pFmt);
  
  return bValidEnd;
}

/*----------------------------------------------------------------------------*/
/* test for valid record end                                                  */
/*----------------------------------------------------------------------------*/
bool RowExtractor::bValidRecordEnd(char* aSrc, Format* pFmt)
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
