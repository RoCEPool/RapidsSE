/*============================================================================*/
/* Record.cpp -- Record                                                       */
/*============================================================================*/

#include "Data.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Record* Data::_pRecord(void* aAddr)
{
  Record* pRecord = reinterpret_cast<Record*>(aAddr);
  
  return pRecord;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Record::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_RECORD);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Record::operator delete(void* aAddr)
{
  Record* pRecord = reinterpret_cast<Record*>(aAddr);
  
  HeapMem::vFree(pRecord);
}


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Record::Record(void)
: Object(CN_RECORD)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  m_azAppendCursor = m_sRecStr.aChar();
  
  _checkInitialised_(CN_RECORD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Record::~Record(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* extract                                                                    */
/*----------------------------------------------------------------------------*/
bool Record::bExtract(Datum* pDatum, char** aaSrc, Column* pCol, Formatters* pFmt)
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
        vFatal("Record::bExtract: Invalid eDatum : %lld\n"_Stz,eDatum);
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
        vFatal("Record::bExtract: Invalid eDatum : %lld\n"_Stz,eDatum);
    }
  }
  
  //pDatum->m_ayDatum = (byte*)szDst->aChar();
  
  // move past the field delimiter
  //   (but not past the record terminator or record ends)
  
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
bool Record::bExtractDatum(Datum* pDatum, char** aaSrc, Column* pCol, Formatters* pFmt)
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
        vFatal("Record::bExtractDatum: Invalid eDatum : %lld\n"_Stz,eDatum);
  }
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* extract a decimal field from csv substr                                    */
/*----------------------------------------------------------------------------*/
bool Record::bExtractBcdSupplied(Datum* pDatum, char** aaSrc,
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
bool Record::bExtractBcdDefault(Datum* pDatum, char** aaSrc,
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
bool Record::bExtractBolSupplied(Datum* pDatum, char** aaSrc,
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
bool Record::bExtractBolDefault(Datum* pDatum, char** aaSrc,
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
bool Record::bExtractDteSupplied(Datum* pDatum, char** aaSrc,
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
bool Record::bExtractDteDefault(Datum* pDatum, char** aaSrc,
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
bool Record::bExtractFltSupplied(Datum* pDatum, char** aaSrc,
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
bool Record::bExtractFltDefault(Datum* pDatum, char** aaSrc,
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
bool Record::bExtractIntSupplied(Datum* pDatum, char** aaSrc,
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
bool Record::bExtractIntDefault(Datum* pDatum, char** aaSrc,
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
bool Record::bExtractIvlSupplied(Datum* pDatum, char** aaSrc,
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
bool Record::bExtractIvlDefault(Datum* pDatum, char** aaSrc,
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
bool Record::bExtractTmeSupplied(Datum* pDatum, char** aaSrc,
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
bool Record::bExtractTmeDefault(Datum* pDatum, char** aaSrc,
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
bool Record::bExtractTspSupplied(Datum* pDatum, char** aaSrc,
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
bool Record::bExtractTspDefault(Datum* pDatum, char** aaSrc,
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
bool Record::bExtractVchSupplied(Datum* pDatum, char** aaSrc,
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
bool Record::bExtractVchDefault(Datum* pDatum, char** aaSrc,
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
/* append ascii of decimal datum to record                                    */
/*----------------------------------------------------------------------------*/
void Record::vAppendBcd(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);

  Bcd wBcd;
  wBcd.vAsI8(pDatum->m_iBcd);
  
  s32 sBcd;
  wBcd.vSprint(sBcd.aStz(), pCol);
  
  m_sRecStr.vConcat(sBcd.aStz());
}
/*----------------------------------------------------------------------------*/
void Record::vAppendBcd_Alt(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  s32 sAscii;
  bool bNegative = false;

  i8 iDigits = 1;
  for (i8 i_Digit = 0; i_Digit < pCol->iColumnScale(); i_Digit++)
    iDigits *= 10;
  
  i8 iBcd_Whole    = pDatum->m_iBcd / iDigits;
  i8 iBcd_Fraction = pDatum->m_iBcd % iDigits;
  
  if (iBcd_Fraction < 0 || iBcd_Whole < 0) {
    bNegative     = true;
    iBcd_Fraction = iBcd_Fraction * -1;
    iBcd_Whole    = iBcd_Whole * -1;
  }
  
  if (pCol->iColumnScale())
    StrLib::vSprint(sAscii.aChar(),"%s%lld.%.*lld"_Stz,
                    bNegative ? "-" : "",
                    iBcd_Whole,
                    pCol->iColumnScale(),
                    iBcd_Fraction);
  else
    StrLib::vSprint(sAscii.aChar(),"%s%lld%c"_Stz,
                    bNegative ? "-" : "",
                    iBcd_Whole);

  m_sRecStr.vConcat((Stz*)sAscii.aChar());
}

/*----------------------------------------------------------------------------*/
/* append ascii of boolean datum to record                                    */
/*----------------------------------------------------------------------------*/
void Record::vAppendBol(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  if (pDatum->bIsNull()) {
    m_sRecStr.vCatSprint("null"_Stz);
    return;
  }
  
  m_sRecStr.vConcat(pDatum->m_iBol ? "true"_Stz : "false"_Stz);
}
/*----------------------------------------------------------------------------*/
void Record::vAppendBol_Alt(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  if (pDatum->bIsNull()) {
    m_sRecStr.vCatSprint("null"_Stz);
    return;
  }
  
  m_sRecStr.vConcat(pDatum->m_iBol ? "true"_Stz : "false"_Stz);
}

/*----------------------------------------------------------------------------*/
/* append ascii of date datum to record                                       */
/*----------------------------------------------------------------------------*/
void Record::vAppendDte(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
    Dte wDte;
    wDte.vAsI8(pDatum->m_iDte);
    
    s32 sDte;
    wDte.vSprint(sDte.aStz());
    
    m_sRecStr.vConcat(sDte.aStz());
}
/*----------------------------------------------------------------------------*/
void Record::vAppendDte_Alt(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  s32 sAscii;
  
  StrLib::vSprint(sAscii.aChar(),
                  "%02lld-%02lld-%02lld"_Stz,
                  (i8)pDatum->xDte.m_uYearDelta + C_BASE_YEAR,
                  (i8)pDatum->xDte.m_uMonth,
                  (i8)pDatum->xDte.m_uDay);

  m_sRecStr.vConcat((Stz*)sAscii.aChar());
}

/*----------------------------------------------------------------------------*/
/* append ascii of float datum to record                                      */
/*----------------------------------------------------------------------------*/
void Record::vAppendFlt(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  if (pDatum->bIsNull()) {
    m_sRecStr.vCatSprint("null"_Stz);
    return;
  }
  
  m_sRecStr.vCatSprint("%g"_Stz,pDatum->m_dFlt);
}
/*----------------------------------------------------------------------------*/
void Record::vAppendFlt_Alt(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  s32 sAscii;

  StrLib::vSprint(sAscii.aChar(),"%g"_Stz,pDatum->m_dFlt);
  
  m_sRecStr.vConcat((Stz*)sAscii.aChar());
}

/*----------------------------------------------------------------------------*/
/* append ascii of integer datum to record                                    */
/*----------------------------------------------------------------------------*/
void Record::vAppendInt(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  if (pDatum->bIsNull()) {
    m_sRecStr.vCatSprint("null"_Stz);
    return;
  }
  
  m_sRecStr.vCatSprint("%lld"_Stz,pDatum->m_iInt);
}
/*----------------------------------------------------------------------------*/
void Record::vAppendInt_Alt(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  s32 sAscii;

  StrLib::vSprint(sAscii.aChar(),"%lld"_Stz,pDatum->m_iInt);
  
  m_sRecStr.vConcat((Stz*)sAscii.aChar());
}

/*----------------------------------------------------------------------------*/
/* append ascii of interval datum to record                                   */
/*----------------------------------------------------------------------------*/
void Record::vAppendIvl(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  if (pDatum->bIsNull()) {
    m_sRecStr.vCatSprint("null"_Stz);
    return;
  }
  
  m_sRecStr.vCatSprint("%lld"_Stz,pDatum->m_iIvl);
}
/*----------------------------------------------------------------------------*/
void Record::vAppendIvl_Alt(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
 
  s32 sAscii;
  
  StrLib::vSprint(sAscii.aChar(),"%lld"_Stz,pDatum->m_iIvl);
  
  m_sRecStr.vConcat((Stz*)sAscii.aChar());
}

/*----------------------------------------------------------------------------*/
/* append ascii of time datum to record                                       */
/*----------------------------------------------------------------------------*/
void Record::vAppendTme(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  Tme wTme;
  wTme.vAsI8(pDatum->m_iTme);
  
  s32 sTme;
  wTme.vSprint(sTme.aStz());
  
  m_sRecStr.vConcat(sTme.aStz());
}
/*----------------------------------------------------------------------------*/
void Record::vAppendTme_Alt(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  s32 sAscii;
  
  u8 uMicro = pDatum->xTme.m_uMicro;
  
  if (uMicro) {
    char caFraction[16];
    StrLib::vFormatFraction(caFraction,uMicro,6);
    
    StrLib::vSprint(sAscii.aChar(),"%02lld:%02lld:%02lld.%s"_Stz,
                    (i8)pDatum->xTme.m_uHour,
                    (i8)pDatum->xTme.m_uMinute,
                    (i8)pDatum->xTme.m_uSecond,
                    caFraction);
  }
  else {
    StrLib::vSprint(sAscii.aChar(),"%02lld:%02lld:%02lld"_Stz,
                    (i8)pDatum->xTme.m_uHour,
                    (i8)pDatum->xTme.m_uMinute,
                    (i8)pDatum->xTme.m_uSecond);
  }
  
  m_sRecStr.vConcat((Stz*)sAscii.aChar());
}

/*----------------------------------------------------------------------------*/
/* append ascii of timestamp datum to record                                  */
/*----------------------------------------------------------------------------*/
void Record::vAppendTsp(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  Tsp wTsp;
  wTsp.vAsI8(pDatum->m_iTsp);
  
  s256 sTsp;
  wTsp.vSprint(sTsp.aStz());
  
  m_sRecStr.vConcat(sTsp.aStz());
}
/*----------------------------------------------------------------------------*/
void Record::vAppendTsp_Alt(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  s32 sAscii;

  if (pDatum->xTsp.m_uMicro) {
    char caFraction[16];
    StrLib::vFormatFraction(caFraction,pDatum->xTsp.m_uMicro,6);
    
    StrLib::vSprint(sAscii.aChar(),
                    "TIMESTAMP '%02lld-%02lld-%02lld %02lld:%02lld:%02lld.%s'"_Stz,
                    (i8)pDatum->xTsp.m_uYearDelta + C_BASE_YEAR,
                    (i8)pDatum->xTsp.m_uMonth,
                    (i8)pDatum->xTsp.m_uDay,
                    (i8)pDatum->xTsp.m_uHour,
                    (i8)pDatum->xTsp.m_uMinute,
                    (i8)pDatum->xTsp.m_uSecond,
                    caFraction);
  }
  else {
  
    StrLib::vSprint(sAscii.aChar(),
                    "TIMESTAMP '%02lld-%02lld-%02lld %02lld:%02lld:%02lld"_Stz,
                    (i8)pDatum->xTsp.m_uYearDelta + C_BASE_YEAR,
                    (i8)pDatum->xTsp.m_uMonth,
                    (i8)pDatum->xTsp.m_uDay,
                    (i8)pDatum->xTsp.m_uHour,
                    (i8)pDatum->xTsp.m_uMinute,
                    (i8)pDatum->xTsp.m_uSecond);
  }
  
  m_sRecStr.vConcat((Stz*)sAscii.aChar());
}

/*----------------------------------------------------------------------------*/
/* append ascii of varchar datum to record                                    */
/*----------------------------------------------------------------------------*/
void Record::vAppendVch(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  if (pDatum->bIsNull()) {
    m_sRecStr.vCatSprint("null"_Stz);
    return;
  }
  
  s256 sBuf;
  
  Vch::vSprintQuotes(sBuf,pDatum->m_sVch.aStz());
  m_sRecStr.vConcat(sBuf.aStz());
}
/*----------------------------------------------------------------------------*/
void Record::vAppendVch_Alt(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  s256 sBuf;
  
  char* aDstChar = (char*)sBuf.aChar();
  char* aSrcChar = (char*)pDatum->m_sVch.aChar();
  
  // put leading enclosing quote/char
  *aDstChar++ = '\'';
  
  // copy from src to dst
  while (*aSrcChar) {
    
    if ('\'' == *aSrcChar) {
      // escape an enclosing quote/char
      *aDstChar++ = '\'';
      *aDstChar++ = *aSrcChar++;
    }
    else {
      // copy an ordinary char
      *aDstChar++ = *aSrcChar++;
    }
  }
  
  // put trailing enclosing quote/char and \0 terminate
  *aDstChar++ = '\'';
  *aDstChar = '\0';

  m_sRecStr.vConcat(sBuf.aStz());
}

/*----------------------------------------------------------------------------*/
/* print record string as row                                                 */
/*----------------------------------------------------------------------------*/
void Record::vPrintRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD);
  
  vTrace("row: (%s)\n"_Stz,m_sRecStr.aStz());
}

/*----------------------------------------------------------------------------*/
/* get record string                                                          */
/*----------------------------------------------------------------------------*/
Stz* Record::szRecStr(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD);
  
  return m_sRecStr.aStz();
}

/*----------------------------------------------------------------------------*/
/* set record string                                                          */
/*----------------------------------------------------------------------------*/
void Record::vRecStr(Stz* szStr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD);
  
  m_sRecStr = szStr;
}

/*----------------------------------------------------------------------------*/
/* reset                                                                      */
/*----------------------------------------------------------------------------*/
void Record::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD);
  
  m_sRecStr.vReset();
  
  m_azAppendCursor = m_sRecStr.aChar();
}

/*----------------------------------------------------------------------------*/
/* is the string enclosed?                                                    */
/*----------------------------------------------------------------------------*/
bool Record::bIsEnclosed(char** aaSrc, Formatters* pFormatters)
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
bool Record::bIsThereFieldData(char* aSrc, Formatters* pFormatters)
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
bool Record::bIsThereRecordData(char* aSrc, Formatters* pFormatters)
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
bool Record::bTestForNull(Column* pColumn, char** aaSrc,
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
bool Record::bTestForNullDefault(Column* pColumn, char** aaSrc,
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
bool Record::bValidFieldEnd(char* aSrc, Formatters* pFormatters)
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
  // test for explicit record terminator
  
  if (pFormatters->m_cRecordTerminator) {
    
    // not a valid field end if expected record terminator is not seen
    if (*aSrc != pFormatters->m_cRecordTerminator)
      return false;
    
    // test we see the record end after the record termintor
    bool bValidEnd = bValidRecordEnd(aSrc+1,pFormatters);
    
    return bValidEnd;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for explicit record ends
  
  bool bValidEnd = bValidRecordEnd(aSrc,pFormatters);
  
  return bValidEnd;
}

/*----------------------------------------------------------------------------*/
/* test for valid field end                                                   */
/*----------------------------------------------------------------------------*/
bool Record::bValidFieldEndDefault(char* aSrc, Formatters* pFormatters)
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
  // test for explicit record ends
  
  bool bValidEnd = bValidRecordEnd(aSrc,pFormatters);
  
  return bValidEnd;
}

/*----------------------------------------------------------------------------*/
/* test for valid record end                                                  */
/*----------------------------------------------------------------------------*/
bool Record::bValidRecordEnd(char* aSrc, Formatters* pFormatters)
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


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
//!                     REVIEW WHICH FUNCTIONS USE THESE                     !//
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//

/*----------------------------------------------------------------------------*/
/*-- Sprint ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* s-print                                                                    */
/*----------------------------------------------------------------------------*/
void Record::vSprint(Column* pColumn, Stz* aDst, Stz* aSrc)
{
  eDM eDatum = pColumn->eDatumType();
  
  switch (eDatum) {
    case DM_BCD: Bcd::vSprint(aDst,aSrc,pColumn); break;
    case DM_BOL: Bol::vSprint(aDst,aSrc); break;
    case DM_CHR: Vch::vSprint(aDst,aSrc); break;
    case DM_DTE: Dte::vSprint(aDst,aSrc); break;
    case DM_FLT: Flt::vSprint(aDst,aSrc); break;
    case DM_INT: Int::vSprint(aDst,aSrc); break;
    case DM_IVL: Ivl::vSprint(aDst,aSrc); break;
    case DM_TME: Tme::vSprint(aDst,aSrc); break;
    case DM_TSP: Tsp::vSprint(aDst,aSrc); break;
    case DM_VCH: Vch::vSprint(aDst,aSrc); break;
    default:
      vFatal("DtmLib::vSprint: Invalid aDatum argument: %lld\n"_Stz,
             eDatum);
  }
}

/*----------------------------------------------------------------------------*/
/* s-print                                                                    */
/*----------------------------------------------------------------------------*/
void Record::vSprintQuotes(Column* pColumn, Stz* aDst, Stz* aSrc)
{
  eDM eDatum = pColumn->eDatumType();
  
  switch (eDatum) {
    case DM_BCD: Bcd::vSprint(aDst,aSrc,pColumn); break;
    case DM_BOL: Bol::vSprint(aDst,aSrc); break;
    case DM_CHR: Vch::vSprint(aDst,aSrc); break;
    case DM_DTE: Dte::vSprint(aDst,aSrc); break;
    case DM_FLT: Flt::vSprint(aDst,aSrc); break;
    case DM_INT: Int::vSprint(aDst,aSrc); break;
    case DM_IVL: Ivl::vSprint(aDst,aSrc); break;
    case DM_TME: Tme::vSprint(aDst,aSrc); break;
    case DM_TSP: Tsp::vSprint(aDst,aSrc); break;
    case DM_VCH: Vch::vSprintQuotes(aDst,aSrc); break;
    default:
      vFatal("DtmLib::vSprint: Invalid aDatum argument: %lld\n"_Stz,
             eDatum);
  }
}

/*----------------------------------------------------------------------------*/
/* s-print escaped                                                            */
/*----------------------------------------------------------------------------*/
void Record::vSprintEscaped(Column* pColumn, Stz* aDst, Stz* aSrc)
{
  eDM eDatum = pColumn->eDatumType();
  
  switch (eDatum) {
    case DM_BCD: Bcd::vSprint(aDst,aSrc,pColumn); break;
    case DM_BOL: Bol::vSprint(aDst,aSrc);         break;
    case DM_CHR: Vch::vSprint(aDst,aSrc);         break;
    case DM_DTE: Dte::vSprintEscaped(aDst,aSrc);  break;
    case DM_FLT: Flt::vSprint(aDst,aSrc);         break;
    case DM_INT: Int::vSprint(aDst,aSrc);         break;
    case DM_IVL: Ivl::vSprint(aDst,aSrc);         break;
    case DM_TME: Tme::vSprintEscaped(aDst,aSrc);  break;
    case DM_TSP: Tsp::vSprintEscaped(aDst,aSrc);  break;
    case DM_VCH: Vch::vSprintEscaped(aDst,aSrc);  break;
    default:
      vFatal("DtmLib::vSprint: Invalid aDatum argument: %lld\n"_Stz,
             eDatum);
  }
}



/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Record::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
