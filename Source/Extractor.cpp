/*============================================================================*/
/* Extractor.cpp -- Extractor                                                 */
/*============================================================================*/

#include "Data.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* extract                                                                    */
/*----------------------------------------------------------------------------*/
bool Extractor::bExtract(Datum* pDatum, char** aaSrc, Column* pCol, Formatters* pFmt)
{
  bool b = false;
  eDM eDatum = pCol->eDatumType();
  
  if (pFmt->m_bDefault) {
    switch (eDatum) {
      case DM_BCD: b = bExtractBcdDefault(pDatum,aaSrc,pCol,pFmt); break;
      case DM_BOL: b = bExtractBolDefault(pDatum,aaSrc,pCol,pFmt); break;
      case DM_CHR: b = bExtractVchDefault(pDatum,aaSrc,pCol,pFmt); break;
      case DM_DTE: b = bExtractDteDefault(pDatum,aaSrc,pCol,pFmt); break;
      case DM_FLT: b = bExtractFltDefault(pDatum,aaSrc,pCol,pFmt); break;
      case DM_INT: b = bExtractIntDefault(pDatum,aaSrc,pCol,pFmt); break;
      case DM_IVL: b = bExtractIvlDefault(pDatum,aaSrc,pCol,pFmt); break;
      case DM_TME: b = bExtractTmeDefault(pDatum,aaSrc,pCol,pFmt); break;
      case DM_TSP: b = bExtractTspDefault(pDatum,aaSrc,pCol,pFmt); break;
      case DM_VCH: b = bExtractVchDefault(pDatum,aaSrc,pCol,pFmt); break;
      default:
        vFatal("Extractor::bExtract: Invalid eDatum : %lld\n"_Stz,eDatum);
    }
  }
  else {
    switch (eDatum) {
      case DM_BCD: b = bExtractBcdSupplied(pDatum,aaSrc,pCol,pFmt); break;
      case DM_BOL: b = bExtractBolSupplied(pDatum,aaSrc,pCol,pFmt); break;
      case DM_CHR: b = bExtractVchSupplied(pDatum,aaSrc,pCol,pFmt); break;
      case DM_DTE: b = bExtractDteSupplied(pDatum,aaSrc,pCol,pFmt); break;
      case DM_FLT: b = bExtractFltSupplied(pDatum,aaSrc,pCol,pFmt); break;
      case DM_INT: b = bExtractIntSupplied(pDatum,aaSrc,pCol,pFmt); break;
      case DM_IVL: b = bExtractIvlSupplied(pDatum,aaSrc,pCol,pFmt); break;
      case DM_TME: b = bExtractTmeSupplied(pDatum,aaSrc,pCol,pFmt); break;
      case DM_TSP: b = bExtractTspSupplied(pDatum,aaSrc,pCol,pFmt); break;
      case DM_VCH: b = bExtractVchSupplied(pDatum,aaSrc,pCol,pFmt); break;
      default:
        vFatal("Extractor::bExtract: Invalid eDatum : %lld\n"_Stz,eDatum);
    }
  }
  
  //pDatum->m_ayDatum = (byte*)szDst->aChar();
  
  // move past the field delimiter
  //   (but not past the Extractor terminator or Extractor ends)
  
  if (pCol->bIsLastColumn()) {
    // do not expect to see a field delimiter
    if ( pFmt->m_cFieldDelimiter &&
        pFmt->m_cRecordTerminator &&
        pFmt->m_cFieldDelimiter != pFmt->m_cRecordTerminator &&
        *(*aaSrc) == pFmt->m_cFieldDelimiter ) {
      // saw a field delimiter after last field (incorrect)
      b = false;
    }
    /*
     // did not see a field delimiter (correct)
     if ( pFormatters->m_cFieldDelimiter &&
     *(*aaSrc) == pFormatters->m_cFieldDelimiter )
     (*aaSrc)++;
     */
  }
  else {
    // expect to see a field delimiter
    if ( pFmt->m_cFieldDelimiter &&
        *(*aaSrc) == pFmt->m_cFieldDelimiter )
      // saw a field delimiter after non-last field (correct)
      (*aaSrc)++;
    else
      // did not see a field delimiter after non-last field (incorrect)
      b = false;
  }
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* extract                                                                    */
/*----------------------------------------------------------------------------*/
bool Extractor::bExtractDatum(Datum* pDatum, char** aaSrc, Column* pCol, Formatters* pFmt)
{
  bool b = false;
  eDM eDatum = pCol->eDatumType();
  
  i8 iYear, iMonth, iDay;
  i8 iHour, iMinute, iSecond, iMicro, iMilli;
  
  switch (eDatum) {
    case DM_BCD:
       b = StrLib::bExtractDecimal(aaSrc,pDatum->m_iBcd,
                                   pCol->iColumnLength(),
                                   pCol->iColumnScale());
       break;
      
    case DM_BOL:
      b = StrLib::bExtractBoolean(aaSrc,pDatum->m_iBol);
      break;
     
    case DM_CHR:
       b = bExtractVchDefault(pDatum,aaSrc,pCol,pFmt);
       break;
      
    case DM_DTE:
      i8 iYear, iMonth, iDay;
      b = StrLib::bExtractDate2(aaSrc, iYear, iMonth, iDay,
                                pFmt->m_eDateComponent,
                                pFmt->m_cDateSeparator);
       if (!b)
         break;
      
       pDatum->xDte.m_uYearDelta  = iYear - C_BASE_YEAR;
       pDatum->xDte.m_uMonth = iMonth;
       pDatum->xDte.m_uDay   = iDay;
       pDatum->xDte.m_uSpare = 0;
      break;
     
    case DM_FLT:
      b = StrLib::bExtractFloat(aaSrc,pDatum->m_dFlt);
      break;
      
    case DM_INT:
      b = StrLib::bExtractInteger(aaSrc,&pDatum->m_iInt, true/*bWithSign*/);
      break;
      
    case DM_IVL:
      b = StrLib::bExtractInterval(aaSrc, iSecond, iMilli);
      break;
      
    case DM_TME:
      b = StrLib::bExtractTime(aaSrc, iHour, iMinute, iSecond, iMicro);
      
      if (!b)
        break;
      
      pDatum->xTme.m_uHour   = iHour;
      pDatum->xTme.m_uMinute = iMinute;
      pDatum->xTme.m_uSecond = iSecond;
      pDatum->xTme.m_uMicro  = iMicro;
      pDatum->xTme.m_uSpare  = 0;
      
      break;
      
    case DM_TSP:
      b = StrLib::bExtractTimestamp2(aaSrc, iYear, iMonth, iDay,
                                     iHour, iMinute, iSecond, iMicro,
                                     pFmt->m_eDateComponent,
                                     pFmt->m_cDateSeparator);
      
      if (!b)
        break;
      
      pDatum->xTsp.m_uYearDelta   = iYear - C_BASE_YEAR;
      pDatum->xTsp.m_uMonth  = iMonth;
      pDatum->xTsp.m_uDay    = iDay;
      pDatum->xTsp.m_uHour   = iHour;
      pDatum->xTsp.m_uMinute = iMinute;
      pDatum->xTsp.m_uSecond = iSecond;
      pDatum->xTsp.m_uMicro  = iMicro;
      pDatum->xTsp.m_uSpare  = 0;
      break;
      
     case DM_VCH:
       b = StrLib::bExtractVarcharDefault(aaSrc,
                                          pDatum->m_sVch.aChar(),
                                          pCol->iColumnLength(),
                                          pDatum->m_nChars);
       break;
      
      default:
        vFatal("Extractor::bExtractDatum: Invalid eDatum : %lld\n"_Stz,eDatum);
  }
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* extract a decimal field from csv substr                                    */
/*----------------------------------------------------------------------------*/
bool Extractor::bExtractBcdSupplied(Datum* pDatum, char** aaSrc,
                                 Column* pColumn, Formatters* pFormatters)
{
  pDatum->m_eDatum = DM_BCD;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  bool bSawEnclosedLeading =
  StrLib::bSkipLeading(aaSrc,pFormatters->m_cEnclosedBy_Leading);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (bTestForNull(pColumn,aaSrc,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    pDatum->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // decimal field processing
  
  if ( ! StrLib::bExtractDecimal(aaSrc,pDatum->m_iBcd,
                                 pColumn->iColumnLength(),pColumn->iColumnScale()) )
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  bool bSawEnclosedTrailing =
  StrLib::bSkipTrailing(aaSrc,bSawEnclosedLeading,pFormatters->m_cEnclosedBy_Trailing);
  if (! bSawEnclosedTrailing)
    return false;
  
  // should see a valid field end
  if ( ! bValidFieldEnd(*aaSrc,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a decimal field from csv substr                                    */
/*----------------------------------------------------------------------------*/
bool Extractor::bExtractBcdDefault(Datum* pDatum, char** aaSrc,
                                Column* pColumn, Formatters* pFormatters)
{
  pDatum->m_eDatum = DM_BCD;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  StrLib::vSkipSpaces(aaSrc);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (bTestForNullDefault(pColumn,aaSrc,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    pDatum->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // decimal field processing
  
  if ( ! StrLib::bExtractDecimal(aaSrc,pDatum->m_iBcd,
                                 pColumn->iColumnLength(),pColumn->iColumnScale()) )
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  StrLib::vSkipSpaces(aaSrc);
  
  // should see a valid field end
  if ( ! bValidFieldEndDefault(*aaSrc,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a boolean field from csv substr                                    */
/*----------------------------------------------------------------------------*/
bool Extractor::bExtractBolSupplied(Datum* pDatum, char** aaSrc,
                                 Column* pColumn, Formatters* pFormatters)
{
  pDatum->m_eDatum = DM_BOL;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  bool bSawEnclosedLeading =
  StrLib::bSkipLeading(aaSrc,pFormatters->m_cEnclosedBy_Leading);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (bTestForNull(pColumn,aaSrc,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    pDatum->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // boolean field processing
  
  if ( ! StrLib::bExtractBoolean(aaSrc,pDatum->m_iBol) )
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  bool bSawEnclosedTrailing =
  StrLib::bSkipTrailing(aaSrc,bSawEnclosedLeading,pFormatters->m_cEnclosedBy_Trailing);
  if (! bSawEnclosedTrailing)
    return false;
  
  // should see a valid field end
  if ( ! bValidFieldEnd(*aaSrc,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a boolean field from csv substr                                    */
/*----------------------------------------------------------------------------*/
bool Extractor::bExtractBolDefault(Datum* pDatum, char** aaSrc,
                                Column* pColumn, Formatters* pFormatters)
{
  pDatum->m_eDatum = DM_BOL;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  StrLib::vSkipSpaces(aaSrc);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (bTestForNullDefault(pColumn,aaSrc,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    pDatum->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // boolean field processing
  
  if ( ! StrLib::bExtractBoolean(aaSrc,pDatum->m_iBol) )
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  StrLib::vSkipSpaces(aaSrc);
  
  // should see a valid field end
  if ( ! bValidFieldEndDefault(*aaSrc,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a date field from csv substr                           yyyy-mo-ddZ */
/*----------------------------------------------------------------------------*/
bool Extractor::bExtractDteSupplied(Datum* pDatum, char** aaSrc,
                                 Column* pColumn, Formatters* pFormatters)
{
  // refer: http://en.wikipedia.org/wiki/ISO_8601
  
  pDatum->m_eDatum = DM_DTE;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  bool bSawEnclosedLeading =
  StrLib::bSkipLeading(aaSrc,pFormatters->m_cEnclosedBy_Leading);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (bTestForNull(pColumn,aaSrc,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    pDatum->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // date field processing
  
  i8 iYear, iMonth, iDay;
  
  if ( ! StrLib::bExtractDate2(aaSrc, iYear, iMonth, iDay,
                               pFormatters->m_eDateComponent,
                               pFormatters->m_cDateSeparator) )
    return false;
  
  pDatum->xDte.m_uYearDelta  = iYear - C_BASE_YEAR;
  pDatum->xDte.m_uMonth = iMonth;
  pDatum->xDte.m_uDay   = iDay;
  pDatum->xDte.m_uSpare = 0;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  bool bSawEnclosedTrailing =
  StrLib::bSkipTrailing(aaSrc,bSawEnclosedLeading,pFormatters->m_cEnclosedBy_Trailing);
  if (! bSawEnclosedTrailing)
    return false;
  
  // should see a valid field end
  if ( ! bValidFieldEnd(*aaSrc,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a date field from csv substr                           yyyy-mo-ddZ */
/*----------------------------------------------------------------------------*/
bool Extractor::bExtractDteDefault(Datum* pDatum, char** aaSrc,
                                Column* pColumn, Formatters* pFormatters)
{
  // refer: http://en.wikipedia.org/wiki/ISO_8601
  
  pDatum->m_eDatum = DM_DTE;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  StrLib::vSkipSpaces(aaSrc);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (bTestForNullDefault(pColumn,aaSrc,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    pDatum->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // date field processing
  
  i8 iYear, iMonth, iDay;
  
  if ( ! StrLib::bExtractDate2(aaSrc, iYear, iMonth, iDay,
                               pFormatters->m_eDateComponent,
                               pFormatters->m_cDateSeparator) )
    return false;
  
  pDatum->xDte.m_uYearDelta  = iYear - C_BASE_YEAR;
  pDatum->xDte.m_uMonth = iMonth;
  pDatum->xDte.m_uDay   = iDay;
  pDatum->xDte.m_uSpare = 0;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  StrLib::vSkipSpaces(aaSrc);
  
  // should see a valid field end
  if ( ! bValidFieldEndDefault(*aaSrc,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a float field from csv substr                                      */
/*----------------------------------------------------------------------------*/
bool Extractor::bExtractFltSupplied(Datum* pDatum, char** aaSrc,
                                 Column* pColumn, Formatters* pFormatters)
{
  pDatum->m_eDatum = DM_FLT;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  bool bSawEnclosedLeading =
  StrLib::bSkipLeading(aaSrc,pFormatters->m_cEnclosedBy_Leading);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (bTestForNull(pColumn,aaSrc,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    pDatum->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // float field processing
  
  if ( ! StrLib::bExtractFloat(aaSrc,pDatum->m_dFlt) )
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  bool bSawEnclosedTrailing =
  StrLib::bSkipTrailing(aaSrc,bSawEnclosedLeading,pFormatters->m_cEnclosedBy_Trailing);
  if (! bSawEnclosedTrailing)
    return false;
  
  // should see a valid field end
  if ( ! bValidFieldEnd(*aaSrc,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a float field from csv substr                                      */
/*----------------------------------------------------------------------------*/
bool Extractor::bExtractFltDefault(Datum* pDatum, char** aaSrc,
                                Column* pColumn, Formatters* pFormatters)
{
  pDatum->m_eDatum = DM_FLT;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  StrLib::vSkipSpaces(aaSrc);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (bTestForNullDefault(pColumn,aaSrc,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    pDatum->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // float field processing
  
  if ( ! StrLib::bExtractFloat(aaSrc,pDatum->m_dFlt) )
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  StrLib::vSkipSpaces(aaSrc);
  
  // should see a valid field end
  if ( ! bValidFieldEndDefault(*aaSrc,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract an integer field from csv substr                                   */
/*----------------------------------------------------------------------------*/
bool Extractor::bExtractIntSupplied(Datum* pDatum, char** aaSrc,
                                 Column* pColumn, Formatters* pFormatters)
{
  pDatum->m_eDatum = DM_INT;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  bool bSawEnclosedLeading =
  StrLib::bSkipLeading(aaSrc,pFormatters->m_cEnclosedBy_Leading);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (bTestForNull(pColumn,aaSrc,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    pDatum->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // integer field processing
  
  if ( ! StrLib::bExtractInteger(aaSrc,&pDatum->m_iInt,
                                 true/*bWithSign*/) )
    return false;
  
  // ignore any trailing digits (after optional '.')
  if ('.' == **aaSrc) {
    
    // skip the '.' and any digits
    (*aaSrc)++;
    StrLib::vSkipDigits(aaSrc);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  bool bSawEnclosedTrailing =
  StrLib::bSkipTrailing(aaSrc,bSawEnclosedLeading,pFormatters->m_cEnclosedBy_Trailing);
  if (! bSawEnclosedTrailing)
    return false;
  
  // should see a valid field end
  if ( ! bValidFieldEnd(*aaSrc,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract an integer field from csv substr                                   */
/*----------------------------------------------------------------------------*/
bool Extractor::bExtractIntDefault(Datum* pDatum, char** aaSrc,
                                Column* pColumn, Formatters* pFormatters)
{
  pDatum->m_eDatum = DM_INT;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  StrLib::vSkipSpaces(aaSrc);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (bTestForNullDefault(pColumn,aaSrc,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    pDatum->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // integer field processing
  
  if ( ! StrLib::bExtractInteger(aaSrc,&pDatum->m_iInt,
                                 true/*bWithSign*/) )
    return false;
  
  // ignore any trailing digits (after optional '.')
  if ('.' == **aaSrc) {
    
    // skip the '.' and any digits
    (*aaSrc)++;
    StrLib::vSkipDigits(aaSrc);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  StrLib::vSkipSpaces(aaSrc);
  
  // should see a valid field end
  if ( ! bValidFieldEndDefault(*aaSrc,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract an interval field from csv substr                                  */
/*----------------------------------------------------------------------------*/
bool Extractor::bExtractIvlSupplied(Datum* pDatum, char** aaSrc,
                                 Column* pColumn, Formatters* pFormatters)
{
  pDatum->m_eDatum = DM_IVL;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  bool bSawEnclosedLeading =
  StrLib::bSkipLeading(aaSrc,pFormatters->m_cEnclosedBy_Leading);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (bTestForNull(pColumn,aaSrc,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    pDatum->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // interval field processing
  
  i8 iSecond, iMilli;
  
  if ( ! StrLib::bExtractInterval(aaSrc, iSecond, iMilli) )
    return false;
  
  pDatum->xIvl.m_uSecond = iSecond;
  pDatum->xIvl.m_uMilli  = iMilli;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  bool bSawEnclosedTrailing =
  StrLib::bSkipTrailing(aaSrc,bSawEnclosedLeading,pFormatters->m_cEnclosedBy_Trailing);
  if (! bSawEnclosedTrailing)
    return false;
  
  // should see a valid field end
  if ( ! bValidFieldEnd(*aaSrc,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract an interval field from csv substr                                  */
/*----------------------------------------------------------------------------*/
bool Extractor::bExtractIvlDefault(Datum* pDatum, char** aaSrc,
                                Column* pColumn, Formatters* pFormatters)
{
  pDatum->m_eDatum = DM_IVL;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  StrLib::vSkipSpaces(aaSrc);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (bTestForNullDefault(pColumn,aaSrc,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    pDatum->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // interval field processing
  
  i8 iSecond, iMilli;
  
  if ( ! StrLib::bExtractInterval(aaSrc, iSecond, iMilli) )
    return false;
  
  pDatum->xIvl.m_uSecond = iSecond;
  pDatum->xIvl.m_uMilli  = iMilli;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  StrLib::vSkipSpaces(aaSrc);
  
  // should see a valid field end
  if ( ! bValidFieldEndDefault(*aaSrc,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a time field from csv substr                         hh:mi:ss.fffZ */
/*----------------------------------------------------------------------------*/
bool Extractor::bExtractTmeSupplied(Datum* pDatum, char** aaSrc,
                                 Column* pColumn, Formatters* pFormatters)
{
  // refer: http://en.wikipedia.org/wiki/ISO_8601
  
  pDatum->m_eDatum = DM_TME;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  bool bSawEnclosedLeading =
  StrLib::bSkipLeading(aaSrc,pFormatters->m_cEnclosedBy_Leading);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (bTestForNull(pColumn,aaSrc,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    pDatum->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // time field processing
  
  i8 iHour, iMinute, iSecond, iMicro;
  
  if ( ! StrLib::bExtractTime(aaSrc, iHour, iMinute, iSecond, iMicro) )
    return false;
  
  pDatum->xTme.m_uHour   = iHour;
  pDatum->xTme.m_uMinute = iMinute;
  pDatum->xTme.m_uSecond = iSecond;
  pDatum->xTme.m_uMicro  = iMicro;
  pDatum->xTme.m_uSpare  = 0;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  bool bSawEnclosedTrailing =
  StrLib::bSkipTrailing(aaSrc,bSawEnclosedLeading,pFormatters->m_cEnclosedBy_Trailing);
  if (! bSawEnclosedTrailing)
    return false;
  
  // should see a valid field end
  if ( ! bValidFieldEnd(*aaSrc,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a time field from csv substr                         hh:mi:ss.fffZ */
/*----------------------------------------------------------------------------*/
bool Extractor::bExtractTmeDefault(Datum* pDatum, char** aaSrc,
                                Column* pColumn, Formatters* pFormatters)
{
  // refer: http://en.wikipedia.org/wiki/ISO_8601
  
  pDatum->m_eDatum = DM_TME;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  StrLib::vSkipSpaces(aaSrc);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (bTestForNullDefault(pColumn,aaSrc,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    pDatum->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // time field processing
  
  i8 iHour, iMinute, iSecond, iMicro;
  
  if ( ! StrLib::bExtractTime(aaSrc, iHour, iMinute, iSecond, iMicro) )
    return false;
  
  pDatum->xTme.m_uHour   = iHour;
  pDatum->xTme.m_uMinute = iMinute;
  pDatum->xTme.m_uSecond = iSecond;
  pDatum->xTme.m_uMicro  = iMicro;
  pDatum->xTme.m_uSpare  = 0;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  StrLib::vSkipSpaces(aaSrc);
  
  // should see a valid field end
  if ( ! bValidFieldEndDefault(*aaSrc,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a timestamp field from csv substr         yyyy-mo-ddThh:mi:ss.fffZ */
/*----------------------------------------------------------------------------*/
bool Extractor::bExtractTspSupplied(Datum* pDatum, char** aaSrc,
                                 Column* pColumn, Formatters* pFormatters)
{
  // refer: http://en.wikipedia.org/wiki/ISO_8601
  
  pDatum->m_eDatum = DM_TSP;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  bool bSawEnclosedLeading =
  StrLib::bSkipLeading(aaSrc,pFormatters->m_cEnclosedBy_Leading);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (bTestForNull(pColumn,aaSrc,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    pDatum->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // timestamp field processing
  
  i8 iYear, iMonth, iDay;
  i8 iHour, iMinute, iSecond, iMicro;
  
  if ( ! StrLib::bExtractTimestamp2(aaSrc, iYear, iMonth, iDay,
                                    iHour, iMinute, iSecond, iMicro,
                                    pFormatters->m_eDateComponent,
                                    pFormatters->m_cDateSeparator) )
    return false;
  
  pDatum->xTsp.m_uYearDelta   = iYear - C_BASE_YEAR;
  pDatum->xTsp.m_uMonth  = iMonth;
  pDatum->xTsp.m_uDay    = iDay;
  pDatum->xTsp.m_uHour   = iHour;
  pDatum->xTsp.m_uMinute = iMinute;
  pDatum->xTsp.m_uSecond = iSecond;
  pDatum->xTsp.m_uMicro  = iMicro;
  pDatum->xTsp.m_uSpare  = 0;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  bool bSawEnclosedTrailing =
  StrLib::bSkipTrailing(aaSrc,bSawEnclosedLeading,pFormatters->m_cEnclosedBy_Trailing);
  if (! bSawEnclosedTrailing)
    return false;
  
  // should see a valid field end
  if ( ! bValidFieldEnd(*aaSrc,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a timestamp field from csv substr         yyyy-mo-ddThh:mi:ss.fffZ */
/*----------------------------------------------------------------------------*/
bool Extractor::bExtractTspDefault(Datum* pDatum, char** aaSrc,
                                Column* pColumn, Formatters* pFormatters)
{
  // refer: http://en.wikipedia.org/wiki/ISO_8601
  
  pDatum->m_eDatum = DM_TSP;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  StrLib::vSkipSpaces(aaSrc);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (bTestForNullDefault(pColumn,aaSrc,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    pDatum->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // timestamp field processing
  
  i8 iYear, iMonth, iDay;
  i8 iHour, iMinute, iSecond, iMicro;
  
  if ( ! StrLib::bExtractTimestamp2(aaSrc, iYear, iMonth, iDay,
                                    iHour, iMinute, iSecond, iMicro,
                                    pFormatters->m_eDateComponent,
                                    pFormatters->m_cDateSeparator) )
    return false;
  
  pDatum->xTsp.m_uYearDelta   = iYear - C_BASE_YEAR;
  pDatum->xTsp.m_uMonth  = iMonth;
  pDatum->xTsp.m_uDay    = iDay;
  pDatum->xTsp.m_uHour   = iHour;
  pDatum->xTsp.m_uMinute = iMinute;
  pDatum->xTsp.m_uSecond = iSecond;
  pDatum->xTsp.m_uMicro  = iMicro;
  pDatum->xTsp.m_uSpare  = 0;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  StrLib::vSkipSpaces(aaSrc);
  
  // should see a valid field end
  if ( ! bValidFieldEndDefault(*aaSrc,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a varchar field from csv substr                                    */
/*----------------------------------------------------------------------------*/
bool Extractor::bExtractVchSupplied(Datum* pDatum, char** aaSrc,
                                 Column* pColumn, Formatters* pFormatters)
{
  pDatum->m_eDatum = DM_VCH;
  
 // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for null
  
  bool bValidNull = false;
  if (bTestForNull(pColumn,aaSrc,pFormatters,bValidNull,false)) {
    if ( ! bValidNull )
      return false;
    pDatum->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for enclosed
  bool bSawEnclosedLeading = bIsEnclosed(aaSrc,pFormatters);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // varchar field processing
  
  i8 iColumnLength = pColumn->iColumnLength();
  i8 nChars;
  if ( ! StrLib::bExtractVarchar(aaSrc,
                                 pDatum->m_sVch.aChar(),
                                 iColumnLength,
                                 bSawEnclosedLeading,
                                 pFormatters->m_cEnclosedBy_Leading,
                                 pFormatters->m_cEnclosedBy_Trailing,
                                 pFormatters->m_cEscapeChar,
                                 pFormatters->m_cFieldDelimiter,
                                 pFormatters->m_cRecordTerminator,
                                 pFormatters->m_bAllowNonPrintable,
                                 pDatum->m_nChars) )
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // shoco test
  
  //  char caCompressed[1024];
  //  i8 iLen = StrLib::iLength((char*)aVch);
  //  i8 iLenCompressed = shoco_compress((char*)aVch, iLen,
  //                                     caCompressed, sizeof(caCompressed));
  //  vTrace("%2lld -> %2lld: %s\n"_Stz,iLen,iLenCompressed,(char*)aVch);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  bool bSawEnclosedTrailing =
  StrLib::bSkipTrailing(aaSrc,bSawEnclosedLeading,
                        pFormatters->m_cEnclosedBy_Trailing);
  if (! bSawEnclosedTrailing)
    return false;
  
  // should see a valid field end
  if ( ! bValidFieldEnd(*aaSrc,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a varchar field from csv substr                                    */
/*----------------------------------------------------------------------------*/
bool Extractor::bExtractVchDefault(Datum* pDatum, char** aaSrc,
                                Column* pColumn, Formatters* pFormatters)
{
  pDatum->m_eDatum = DM_VCH;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for null
  
  bool bValidNull = false;
  if (bTestForNullDefault(pColumn,aaSrc,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    pDatum->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for enclosed
  //bool bSawEnclosedLeading = bIsEnclosed(aaSrc,pFormatters);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // varchar field processing
  
  i8 iColumnLength = pColumn->iColumnLength();
  if ( ! StrLib::bExtractVarcharDefault(aaSrc,
                                        pDatum->m_sVch.aChar(),
                                        iColumnLength,
                                        pDatum->m_nChars) )
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  StrLib::vSkipSpaces(aaSrc);
  
  //bool bSawEnclosedTrailing =
  //StrLib::bSkipTrailing(aaSrc,bSawEnclosedLeading,pFormatters->m_cEnclosedBy_Trailing);
  //if (! bSawEnclosedTrailing)
  //  return false;
  
  // should see a valid field end
  if ( ! bValidFieldEndDefault(*aaSrc,pFormatters) )
    return false;
  
  return true;
}



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is the string enclosed?                                                    */
/*----------------------------------------------------------------------------*/
bool Extractor::bIsEnclosed(char** aaSrc, Formatters* pFormatters)
{
  if (! pFormatters->m_cEnclosedBy_Leading)
    return false;
  
  char *aStart = *aaSrc;
  
  // find first non-space character
  while (' ' == **aaSrc)
    (*aaSrc)++;
  
  // is it an enclosed varchar?
  if (**aaSrc == pFormatters->m_cEnclosedBy_Leading) {
    (*aaSrc)++;
    return true;
  }
  else {
    *(aaSrc) = aStart;
    return false;
  }
}

/*----------------------------------------------------------------------------*/
/* is there data?                                                             */
/*----------------------------------------------------------------------------*/
bool Extractor::bIsThereFieldData(char* aSrc, Formatters* pFormatters)
{
  if ( ! *aSrc
      || *aSrc == pFormatters->m_cFieldDelimiter
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
bool Extractor::bIsThereRecordData(char* aSrc, Formatters* pFormatters)
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
/* test for null token                                                        */
/*----------------------------------------------------------------------------*/
bool Extractor::bTestForNull(Column* pColumn, char** aaSrc,
                          Formatters* pFormatters,
                          bool& bValidNull,
                          bool bSkipTrailingSpaces)
{
  /*
   * This function scans aaSrc and attempts to identify the text form of a null.
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
  
  if ( ! bIsThereFieldData(*aaSrc, pFormatters)) {
    if ( ! pColumn->bIsNullable() ) {
      // a null was detected in a non-nullable column
      bValidNull = false;
      return true;
    }
    // a null was detected in a nullable column
    bValidNull = true;
    return true;
  }
  
  //if ( ! bIsThereFieldData(*aaSrc, pFormatters))
  //  return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for null, \N, '\N' or [\N]
  
  char* aStart = *aaSrc;
  
  if ( (**aaSrc == 'N' || **aaSrc == 'n') && (0 == StrLib::iCompare(*aaSrc+1, "ULL", 3, C_NO_CASE)) ) {
    // skip over token null
    *aaSrc = (*aaSrc) + 4;
  }
  else if (**aaSrc     == pFormatters->m_cEscapeChar &&
           *(*aaSrc+1) == 'N') {
    // skip over \N
    *aaSrc = (*aaSrc) + 2;
  }
  else if (**aaSrc     == pFormatters->m_cEnclosedBy_Leading &&
           *(*aaSrc+1) == pFormatters->m_cEscapeChar &&
           *(*aaSrc+2) == 'N'                        &&
           *(*aaSrc+3) == pFormatters->m_cEnclosedBy_Trailing) {
    // skip over '\N' or [\N]
    *aaSrc = (*aaSrc) + 4;
  }
  else if (**aaSrc    == pFormatters->m_cEnclosedBy_Leading &&
           ((*(*aaSrc+1) == 'N' || *(*aaSrc+1) == 'n') && (0 == StrLib::iCompare(*aaSrc+2, "ULL", 3, C_NO_CASE))) &&
           *(*aaSrc+5) == pFormatters->m_cEnclosedBy_Trailing) {
    // skip over 'NULL'
    *aaSrc = (*aaSrc) + 6;
  }
  else {
    // no null detected
    bValidNull = false; // no null, so neither valid or invalid
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  // optionally skip trailing spaces
  if (bSkipTrailingSpaces)
    StrLib::vSkipSpaces(aaSrc);
  
  // check it was "null" only
  if (! bValidFieldEnd(*aaSrc,pFormatters)) {
    // no well formed null detected
    *(aaSrc) = aStart;
    bValidNull = false; // no null, so neither valid or invalid
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // check the column is nullable
  
  if ( ! pColumn->bIsNullable() ) {
    // a null was detected in a non-nullable column
    bValidNull = false;
    return true;
  }
  
  // a null was detected in a nullable column
  bValidNull = true;
  return true;
}

/*----------------------------------------------------------------------------*/
/* test for null token                                                        */
/*----------------------------------------------------------------------------*/
bool Extractor::bTestForNullDefault(Column* pColumn, char** aaSrc,
                                 Formatters* pFormatters, bool& bValidNull,
                                 bool bSkipTrailingSpaces)
{
  /*
   * This function scans aaSrc and attempts to identify the text form of a null.
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
  
  if ( ! bIsThereFieldData(*aaSrc, pFormatters)) {
    if ( ! pColumn->bIsNullable() ) {
      // a null was detected in a non-nullable column
      bValidNull = false;
      return true;
    }
    // a null was detected in a nullable column
    bValidNull = true;
    return true;
  }
  
  //if ( ! bIsThereFieldData(*aaSrc, pFormatters))
  //  return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for null, \N, '\N' or [\N]
  
  char* aStart = *aaSrc;
  
  if ( (**aaSrc == 'N' || **aaSrc == 'n') && (0 == StrLib::iCompare(*aaSrc+1, "ULL", 3, C_NO_CASE)) ) {
    // skip over token null
    *aaSrc = (*aaSrc) + 4;
  }
  else if (**aaSrc     == pFormatters->m_cEscapeChar &&
           *(*aaSrc+1) == 'N') {
    // skip over \N
    *aaSrc = (*aaSrc) + 2;
  }
  else {
    // no null detected
    bValidNull = false; // no null, so neither valid or invalid
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  // optionally skip trailing spaces
  if (bSkipTrailingSpaces)
    StrLib::vSkipSpaces(aaSrc);
  // check it was "null" only
  if (! bValidFieldEndDefault(*aaSrc,pFormatters)) {
    // no well formed null detected
    *(aaSrc) = aStart;
    bValidNull = false; // no null, so neither valid or invalid
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // check the column is nullable
  
  if ( ! pColumn->bIsNullable() ) {
    // a null was detected in a non-nullable column
    bValidNull = false;
    return true;
  }
  
  // a null was detected in a nullable column
  bValidNull = true;
  return true;
}

/*----------------------------------------------------------------------------*/
/* test for valid field end                                                   */
/*----------------------------------------------------------------------------*/
bool Extractor::bValidFieldEnd(char* aSrc, Formatters* pFormatters)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for end of data
  
  // valid field end if we've run out of data
  if (! *aSrc)
    return true;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for explicit field separator
  
  // valid field end if we've encounter the field delimiter
  if (*aSrc == pFormatters->m_cFieldDelimiter)
    return true;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for explicit Extractor terminator
  
  if (pFormatters->m_cRecordTerminator) {
    
    // not a valid field end if expected Extractor terminator is not seen
    if (*aSrc != pFormatters->m_cRecordTerminator)
      return false;
    
    // test we see the Extractor end after the Extractor termintor
    bool bValidEnd = bValidRecordEnd(aSrc+1,pFormatters);
    
    return bValidEnd;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for explicit Extractor ends
  
  bool bValidEnd = bValidRecordEnd(aSrc,pFormatters);
  
  return bValidEnd;
}

/*----------------------------------------------------------------------------*/
/* test for valid field end                                                   */
/*----------------------------------------------------------------------------*/
bool Extractor::bValidFieldEndDefault(char* aSrc, Formatters* pFormatters)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for end of data
  
  // valid field end if we've run out of data
  if (! *aSrc)
    return true;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for explicit field separator
  
  // valid field end if we've encounter the field delimiter
  if (*aSrc == pFormatters->m_cFieldDelimiter)
    return true;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for explicit Extractor ends
  
  bool bValidEnd = bValidRecordEnd(aSrc,pFormatters);
  
  return bValidEnd;
}

/*----------------------------------------------------------------------------*/
/* test for valid record end                                                  */
/*----------------------------------------------------------------------------*/
bool Extractor::bValidRecordEnd(char* aSrc, Formatters* pFormatters)
{
  switch (pFormatters->m_eRecordEnd) {
      
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




/*----------------------------------------------------------------------------*/
/* add element to key                                                         */
/*----------------------------------------------------------------------------*/
void Extractor::vAddElement(EntWrk* pEntWrk, Stz* aSrc,
                            Column* pCol, Formatters* pFormatters)
{
  
  bool b;
  i8 nBytes;
  
  char* pDst = (char*)pEntWrk->ayElementCrs();
  char* pSrc = (char*)aSrc;
  Datum xDatum;
  
  if (pCol->eDatumType() != DM_VCH) {
    //b = Record::bExtractDatum(&xDatum,&pSrc,pCol,pFormatters);
    b = Extractor::bExtractDatum(&xDatum,&pSrc,pCol,pFormatters);
    RASSERT(b);
  }
  
  switch(pCol->eDatumType()) {
      
    case DM_BCD:
      nBytes = Bcd::nCopy((char*)pDst,(char*)&xDatum.m_iBcd);
      break;
      
    case DM_BOL:
      nBytes = Bol::nCopy((char*)pDst,(char*)&xDatum.m_iBol);
      break;
      
    case DM_DTE:
      nBytes = Dte::nCopy((char*)pDst,(char*)&xDatum.m_iDte);
      break;
      
    case DM_INT:
      nBytes = Int::nCopy(pDst,(void*)&xDatum.m_iDte);
      break;
      
    case DM_IVL:
      nBytes = Ivl::nCopy(pDst,(void*)&xDatum.m_iIvl);
      break;
      
    case DM_FLT:
      nBytes = Flt::nCopy(pDst,(void*)&xDatum.m_dFlt);
      break;
      
    case DM_TME:
      nBytes = Tme::nCopy(pDst,&xDatum.m_iTme);
      break;
      
    case DM_TSP:
      nBytes = Tsp::nCopy(pDst,&xDatum.m_iTsp);
      break;
      
    case DM_VCH:
      nBytes = StrLib::iWordFlip(pDst, pSrc);
      break;
      
    default:
      NEVER_REACH;
  }
  
  pEntWrk->vIncKeyBytes(nBytes);
}










/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
