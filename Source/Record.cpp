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
bool Record::bExtract(Column* pCol, Datum* pDatum, char** aaSrc,
                      bool bLastColumn, Formatters* pFmt)
{
  bool b = false;
  eDM eDatum = pCol->eDatumType();
  
  Stz* szDst = pDatum->m_xWsp.aStz();
  
  if (pFmt->m_bDefault) {
    switch (eDatum) {
      case DM_BCD: b = bExtractBcdDefault(pDatum,aaSrc,szDst,pCol,pFmt); break;
      case DM_BOL: b = bExtractBolDefault(pDatum,aaSrc,szDst,pCol,pFmt); break;
      case DM_CHR: b = bExtractVchDefault(pDatum,aaSrc,szDst,pCol,pFmt); break;
      case DM_DTE: b = bExtractDteDefault(pDatum,aaSrc,szDst,pCol,pFmt); break;
      case DM_FLT: b = bExtractFltDefault(pDatum,aaSrc,szDst,pCol,pFmt); break;
      case DM_INT: b = bExtractIntDefault(pDatum,aaSrc,szDst,pCol,pFmt); break;
      case DM_IVL: b = bExtractIvlDefault(pDatum,aaSrc,szDst,pCol,pFmt); break;
      case DM_TME: b = bExtractTmeDefault(pDatum,aaSrc,szDst,pCol,pFmt); break;
      case DM_TSP: b = bExtractTspDefault(pDatum,aaSrc,szDst,pCol,pFmt); break;
      case DM_VCH: b = bExtractVchDefault(pDatum,aaSrc,szDst,pCol,pFmt); break;
      default:
        vFatal("Record::bExtract: Invalid eDatum : %lld\n"_Stz,eDatum);
    }
  }
  else {
    switch (eDatum) {
      case DM_BCD: b = bExtractBcdSupplied(pDatum,aaSrc,szDst,pCol,pFmt); break;
      case DM_BOL: b = bExtractBolSupplied(pDatum,aaSrc,szDst,pCol,pFmt); break;
      case DM_CHR: b = bExtractVchSupplied(pDatum,aaSrc,szDst,pCol,pFmt); break;
      case DM_DTE: b = bExtractDteSupplied(pDatum,aaSrc,szDst,pCol,pFmt); break;
      case DM_FLT: b = bExtractFltSupplied(pDatum,aaSrc,szDst,pCol,pFmt); break;
      case DM_INT: b = bExtractIntSupplied(pDatum,aaSrc,szDst,pCol,pFmt); break;
      case DM_IVL: b = bExtractIvlSupplied(pDatum,aaSrc,szDst,pCol,pFmt); break;
      case DM_TME: b = bExtractTmeSupplied(pDatum,aaSrc,szDst,pCol,pFmt); break;
      case DM_TSP: b = bExtractTspSupplied(pDatum,aaSrc,szDst,pCol,pFmt); break;
      case DM_VCH: b = bExtractVchSupplied(pDatum,aaSrc,szDst,pCol,pFmt); break;
      default:
        vFatal("Record::bExtract: Invalid eDatum : %lld\n"_Stz,eDatum);
    }
  }
  
  pDatum->m_ayDatum = (byte*)szDst->aChar();
  
  // move past the field delimiter
  //   (but not past the record terminator or record ends)
  
  if (bLastColumn) {
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
/* extract a decimal field from csv substr                                    */
/*----------------------------------------------------------------------------*/
bool Record::bExtractBcdSupplied(Datum* pDatum, char** aaSrc, Stz* aDst,
                                 Column* pColumn, Formatters* pFormatters)
{
  Bcd* aBcd = (Bcd*)aDst;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  bool bSawEnclosedLeading =
  StrLib::bSkipLeading(aaSrc,pFormatters->m_cEnclosedBy_Leading);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (DtmLib::bTestForNull(pColumn,aaSrc,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    aBcd->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // decimal field processing
  
  if ( ! StrLib::bExtractDecimal(aaSrc,aBcd->m_iBcd,
                                 pColumn->iColumnLength(),pColumn->iColumnScale()) )
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  bool bSawEnclosedTrailing =
  StrLib::bSkipTrailing(aaSrc,bSawEnclosedLeading,pFormatters->m_cEnclosedBy_Trailing);
  if (! bSawEnclosedTrailing)
    return false;
  
  // should see a valid field end
  if ( ! DtmLib::bValidFieldEnd(*aaSrc,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a decimal field from csv substr                                    */
/*----------------------------------------------------------------------------*/
bool Record::bExtractBcdDefault(Datum* pDatum, char** aaSrc, Stz* aDst,
                                Column* pColumn, Formatters* pFormatters)
{
  Bcd* aBcd = (Bcd*)aDst;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  StrLib::vSkipSpaces(aaSrc);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (DtmLib::bTestForNullDefault(pColumn,aaSrc,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    aBcd->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // decimal field processing
  
  if ( ! StrLib::bExtractDecimal(aaSrc,aBcd->m_iBcd,
                                 pColumn->iColumnLength(),pColumn->iColumnScale()) )
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  StrLib::vSkipSpaces(aaSrc);
  
  // should see a valid field end
  if ( ! DtmLib::bValidFieldEndDefault(*aaSrc,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a boolean field from csv substr                                    */
/*----------------------------------------------------------------------------*/
bool Record::bExtractBolSupplied(Datum* pDatum, char** aaSrc, Stz* aDst,
                                 Column* pColumn, Formatters* pFormatters)
{
  Bol* aBol = (Bol*)aDst;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  bool bSawEnclosedLeading =
  StrLib::bSkipLeading(aaSrc,pFormatters->m_cEnclosedBy_Leading);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (DtmLib::bTestForNull(pColumn,aaSrc,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    aBol->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // boolean field processing
  
  if ( ! StrLib::bExtractBoolean(aaSrc,aBol->m_bBol) )
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  bool bSawEnclosedTrailing =
  StrLib::bSkipTrailing(aaSrc,bSawEnclosedLeading,pFormatters->m_cEnclosedBy_Trailing);
  if (! bSawEnclosedTrailing)
    return false;
  
  // should see a valid field end
  if ( ! DtmLib::bValidFieldEnd(*aaSrc,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a boolean field from csv substr                                    */
/*----------------------------------------------------------------------------*/
bool Record::bExtractBolDefault(Datum* pDatum, char** aaSrc, Stz* aDst,
                                Column* pColumn, Formatters* pFormatters)
{
  Bol* aBol = (Bol*)aDst;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  StrLib::vSkipSpaces(aaSrc);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (DtmLib::bTestForNullDefault(pColumn,aaSrc,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    aBol->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // boolean field processing
  
  if ( ! StrLib::bExtractBoolean(aaSrc,aBol->m_bBol) )
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  StrLib::vSkipSpaces(aaSrc);
  
  // should see a valid field end
  if ( ! DtmLib::bValidFieldEndDefault(*aaSrc,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a date field from csv substr                           yyyy-mo-ddZ */
/*----------------------------------------------------------------------------*/
bool Record::bExtractDteSupplied(Datum* pDatum, char** aaSrc, Stz* aDst,
                                 Column* pColumn, Formatters* pFormatters)
{
  // refer: http://en.wikipedia.org/wiki/ISO_8601
  
  Dte* aDte = (Dte*)aDst;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  bool bSawEnclosedLeading =
  StrLib::bSkipLeading(aaSrc,pFormatters->m_cEnclosedBy_Leading);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (DtmLib::bTestForNull(pColumn,aaSrc,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    aDte->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // date field processing
  
  i8 iYear, iMonth, iDay;
  
  if ( ! StrLib::bExtractDate2(aaSrc, iYear, iMonth, iDay,
                               pFormatters->m_eDateComponent,
                               pFormatters->m_cDateSeparator) )
    return false;
  
  aDte->x.m_uYear   = iYear;
  aDte->x.m_uMonth  = iMonth;
  aDte->x.m_uDay    = iDay;
  aDte->x.m_uSpare  = 0;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  bool bSawEnclosedTrailing =
  StrLib::bSkipTrailing(aaSrc,bSawEnclosedLeading,pFormatters->m_cEnclosedBy_Trailing);
  if (! bSawEnclosedTrailing)
    return false;
  
  // should see a valid field end
  if ( ! DtmLib::bValidFieldEnd(*aaSrc,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a date field from csv substr                           yyyy-mo-ddZ */
/*----------------------------------------------------------------------------*/
bool Record::bExtractDteDefault(Datum* pDatum, char** aaSrc, Stz* aDst,
                                Column* pColumn, Formatters* pFormatters)
{
  // refer: http://en.wikipedia.org/wiki/ISO_8601
  
  Dte* aDte = (Dte*)aDst;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  StrLib::vSkipSpaces(aaSrc);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (DtmLib::bTestForNullDefault(pColumn,aaSrc,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    aDte->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // date field processing
  
  i8 iYear, iMonth, iDay;
  
  if ( ! StrLib::bExtractDate2(aaSrc, iYear, iMonth, iDay,
                               pFormatters->m_eDateComponent,
                               pFormatters->m_cDateSeparator) )
    return false;
  
  aDte->x.m_uYear   = iYear;
  aDte->x.m_uMonth  = iMonth;
  aDte->x.m_uDay    = iDay;
  aDte->x.m_uSpare  = 0;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  StrLib::vSkipSpaces(aaSrc);
  
  // should see a valid field end
  if ( ! DtmLib::bValidFieldEndDefault(*aaSrc,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a float field from csv substr                                      */
/*----------------------------------------------------------------------------*/
bool Record::bExtractFltSupplied(Datum* pDatum, char** aaSrc, Stz* aDst,
                                 Column* pColumn, Formatters* pFormatters)
{
  Flt* aFlt = (Flt*)aDst;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  bool bSawEnclosedLeading =
  StrLib::bSkipLeading(aaSrc,pFormatters->m_cEnclosedBy_Leading);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (DtmLib::bTestForNull(pColumn,aaSrc,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    aFlt->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // float field processing
  
  if ( ! StrLib::bExtractFloat(aaSrc,aFlt->m_dFlt) )
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  bool bSawEnclosedTrailing =
  StrLib::bSkipTrailing(aaSrc,bSawEnclosedLeading,pFormatters->m_cEnclosedBy_Trailing);
  if (! bSawEnclosedTrailing)
    return false;
  
  // should see a valid field end
  if ( ! DtmLib::bValidFieldEnd(*aaSrc,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a float field from csv substr                                      */
/*----------------------------------------------------------------------------*/
bool Record::bExtractFltDefault(Datum* pDatum, char** aaSrc, Stz* aDst,
                                Column* pColumn, Formatters* pFormatters)
{
  Flt* aFlt = (Flt*)aDst;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  StrLib::vSkipSpaces(aaSrc);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (DtmLib::bTestForNullDefault(pColumn,aaSrc,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    aFlt->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // float field processing
  
  if ( ! StrLib::bExtractFloat(aaSrc,aFlt->m_dFlt) )
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  StrLib::vSkipSpaces(aaSrc);
  
  // should see a valid field end
  if ( ! DtmLib::bValidFieldEndDefault(*aaSrc,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract an integer field from csv substr                                   */
/*----------------------------------------------------------------------------*/
bool Record::bExtractIntSupplied(Datum* pDatum, char** aaSrc, Stz* aDst,
                                 Column* pColumn, Formatters* pFormatters)
{
  Int* aInt = (Int*)aDst;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  bool bSawEnclosedLeading =
  StrLib::bSkipLeading(aaSrc,pFormatters->m_cEnclosedBy_Leading);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (DtmLib::bTestForNull(pColumn,aaSrc,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    aInt->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // integer field processing
  
  if ( ! StrLib::bExtractInteger(aaSrc,&aInt->m_iInt,
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
  if ( ! DtmLib::bValidFieldEnd(*aaSrc,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract an integer field from csv substr                                   */
/*----------------------------------------------------------------------------*/
bool Record::bExtractIntDefault(Datum* pDatum, char** aaSrc, Stz* aDst,
                                Column* pColumn, Formatters* pFormatters)
{
  Int* aInt = (Int*)aDst;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  StrLib::vSkipSpaces(aaSrc);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (DtmLib::bTestForNullDefault(pColumn,aaSrc,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    aInt->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // integer field processing
  
  if ( ! StrLib::bExtractInteger(aaSrc,&aInt->m_iInt,
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
  if ( ! DtmLib::bValidFieldEndDefault(*aaSrc,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract an interval field from csv substr                                  */
/*----------------------------------------------------------------------------*/
bool Record::bExtractIvlSupplied(Datum* pDatum, char** aaSrc, Stz* aDst,
                                 Column* pColumn, Formatters* pFormatters)
{
  Ivl* aIvl = (Ivl*)aDst;
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  bool bSawEnclosedLeading =
  StrLib::bSkipLeading(aaSrc,pFormatters->m_cEnclosedBy_Leading);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (DtmLib::bTestForNull(pColumn,aaSrc,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    aIvl->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // interval field processing
  
  i8 iSecond, iMilli;
  
  if ( ! StrLib::bExtractInterval(aaSrc, iSecond, iMilli) )
    return false;
  
  aIvl->x.m_uSecond = iSecond;
  aIvl->x.m_uMilli  = iMilli;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  bool bSawEnclosedTrailing =
  StrLib::bSkipTrailing(aaSrc,bSawEnclosedLeading,pFormatters->m_cEnclosedBy_Trailing);
  if (! bSawEnclosedTrailing)
    return false;
  
  // should see a valid field end
  if ( ! DtmLib::bValidFieldEnd(*aaSrc,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract an interval field from csv substr                                  */
/*----------------------------------------------------------------------------*/
bool Record::bExtractIvlDefault(Datum* pDatum, char** aaSrc, Stz* aDst,
                                Column* pColumn, Formatters* pFormatters)
{
  Ivl* aIvl = (Ivl*)aDst;
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  StrLib::vSkipSpaces(aaSrc);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (DtmLib::bTestForNullDefault(pColumn,aaSrc,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    aIvl->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // interval field processing
  
  i8 iSecond, iMilli;
  
  if ( ! StrLib::bExtractInterval(aaSrc, iSecond, iMilli) )
    return false;
  
  aIvl->x.m_uSecond = iSecond;
  aIvl->x.m_uMilli  = iMilli;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  StrLib::vSkipSpaces(aaSrc);
  
  // should see a valid field end
  if ( ! DtmLib::bValidFieldEndDefault(*aaSrc,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a time field from csv substr                         hh:mi:ss.fffZ */
/*----------------------------------------------------------------------------*/
bool Record::bExtractTmeSupplied(Datum* pDatum, char** aaSrc, Stz* aDst,
                                 Column* pColumn, Formatters* pFormatters)
{
  // refer: http://en.wikipedia.org/wiki/ISO_8601
  
  Tme* aTme = (Tme*)aDst;
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  bool bSawEnclosedLeading =
  StrLib::bSkipLeading(aaSrc,pFormatters->m_cEnclosedBy_Leading);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (DtmLib::bTestForNull(pColumn,aaSrc,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    aTme->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // time field processing
  
  i8 iHour, iMinute, iSecond, iMicro;
  
  if ( ! StrLib::bExtractTime(aaSrc, iHour, iMinute, iSecond, iMicro) )
    return false;
  
  aTme->x.m_uHour   = iHour;
  aTme->x.m_uMinute = iMinute;
  aTme->x.m_uSecond = iSecond;
  aTme->x.m_uMicro  = iMicro;
  aTme->x.m_uSpare  = 0;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  bool bSawEnclosedTrailing =
  StrLib::bSkipTrailing(aaSrc,bSawEnclosedLeading,pFormatters->m_cEnclosedBy_Trailing);
  if (! bSawEnclosedTrailing)
    return false;
  
  // should see a valid field end
  if ( ! DtmLib::bValidFieldEnd(*aaSrc,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a time field from csv substr                         hh:mi:ss.fffZ */
/*----------------------------------------------------------------------------*/
bool Record::bExtractTmeDefault(Datum* pDatum, char** aaSrc, Stz* aDst,
                                Column* pColumn, Formatters* pFormatters)
{
  // refer: http://en.wikipedia.org/wiki/ISO_8601
  
  Tme* aTme = (Tme*)aDst;
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  StrLib::vSkipSpaces(aaSrc);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (DtmLib::bTestForNullDefault(pColumn,aaSrc,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    aTme->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // time field processing
  
  i8 iHour, iMinute, iSecond, iMicro;
  
  if ( ! StrLib::bExtractTime(aaSrc, iHour, iMinute, iSecond, iMicro) )
    return false;
  
  aTme->x.m_uHour   = iHour;
  aTme->x.m_uMinute = iMinute;
  aTme->x.m_uSecond = iSecond;
  aTme->x.m_uMicro  = iMicro;
  aTme->x.m_uSpare  = 0;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  StrLib::vSkipSpaces(aaSrc);
  
  // should see a valid field end
  if ( ! DtmLib::bValidFieldEndDefault(*aaSrc,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a timestamp field from csv substr         yyyy-mo-ddThh:mi:ss.fffZ */
/*----------------------------------------------------------------------------*/
bool Record::bExtractTspSupplied(Datum* pDatum, char** aaSrc, Stz* aDst,
                                 Column* pColumn, Formatters* pFormatters)
{
  // refer: http://en.wikipedia.org/wiki/ISO_8601
  
  Tsp* aTsp = (Tsp*)aDst;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  bool bSawEnclosedLeading =
  StrLib::bSkipLeading(aaSrc,pFormatters->m_cEnclosedBy_Leading);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (DtmLib::bTestForNull(pColumn,aaSrc,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    aTsp->vSetNull();
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
  
  aTsp->x.m_uYear   = iYear;
  aTsp->x.m_uMonth  = iMonth;
  aTsp->x.m_uDay    = iDay;
  aTsp->x.m_uHour   = iHour;
  aTsp->x.m_uMinute = iMinute;
  aTsp->x.m_uSecond = iSecond;
  aTsp->x.m_uMicro  = iMicro;
  aTsp->x.m_uSpare  = 0;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  bool bSawEnclosedTrailing =
  StrLib::bSkipTrailing(aaSrc,bSawEnclosedLeading,pFormatters->m_cEnclosedBy_Trailing);
  if (! bSawEnclosedTrailing)
    return false;
  
  // should see a valid field end
  if ( ! DtmLib::bValidFieldEnd(*aaSrc,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a timestamp field from csv substr         yyyy-mo-ddThh:mi:ss.fffZ */
/*----------------------------------------------------------------------------*/
bool Record::bExtractTspDefault(Datum* pDatum, char** aaSrc, Stz* aDst,
                                Column* pColumn, Formatters* pFormatters)
{
  // refer: http://en.wikipedia.org/wiki/ISO_8601
  
  Tsp* aTsp = (Tsp*)aDst;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  StrLib::vSkipSpaces(aaSrc);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (DtmLib::bTestForNullDefault(pColumn,aaSrc,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    aTsp->vSetNull();
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
  
  aTsp->x.m_uYear   = iYear;
  aTsp->x.m_uMonth  = iMonth;
  aTsp->x.m_uDay    = iDay;
  aTsp->x.m_uHour   = iHour;
  aTsp->x.m_uMinute = iMinute;
  aTsp->x.m_uSecond = iSecond;
  aTsp->x.m_uMicro  = iMicro;
  aTsp->x.m_uSpare  = 0;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  StrLib::vSkipSpaces(aaSrc);
  
  // should see a valid field end
  if ( ! DtmLib::bValidFieldEndDefault(*aaSrc,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a varchar field from csv substr                                    */
/*----------------------------------------------------------------------------*/
bool Record::bExtractVchSupplied(Datum* pDatum, char** aaSrc, Stz* aDst,
                                 Column* pColumn, Formatters* pFormatters)
{
  Vch* aVch = (Vch*)aDst;
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for null
  
  bool bValidNull = false;
  if (DtmLib::bTestForNull(pColumn,aaSrc,pFormatters,bValidNull,false)) {
    if ( ! bValidNull )
      return false;
    aVch->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for enclosed
  bool bSawEnclosedLeading = DtmLib::bIsEnclosed(aaSrc,pFormatters);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // varchar field processing
  
  i8 iColumnLength = pColumn->iColumnLength();
  if ( ! StrLib::bExtractVarchar(aaSrc,(char*)aVch,
                                 iColumnLength,
                                 bSawEnclosedLeading,
                                 pFormatters->m_cEnclosedBy_Leading,
                                 pFormatters->m_cEnclosedBy_Trailing,
                                 pFormatters->m_cEscapeChar,
                                 pFormatters->m_cFieldDelimiter,
                                 pFormatters->m_cRecordTerminator,
                                 pFormatters->m_bAllowNonPrintable
                                 ) )
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
  StrLib::bSkipTrailing(aaSrc,bSawEnclosedLeading,pFormatters->m_cEnclosedBy_Trailing);
  if (! bSawEnclosedTrailing)
    return false;
  
  // should see a valid field end
  if ( ! DtmLib::bValidFieldEnd(*aaSrc,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a varchar field from csv substr                                    */
/*----------------------------------------------------------------------------*/
bool Record::bExtractVchDefault(Datum* pDatum, char** aaSrc, Stz* aDst,
                                Column* pColumn, Formatters* pFormatters)
{
  Vch* aVch = (Vch*)aDst;
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for null
  
  bool bValidNull = false;
  if (DtmLib::bTestForNullDefault(pColumn,aaSrc,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    aVch->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for enclosed
  //bool bSawEnclosedLeading = DtmLib::bIsEnclosed(aaSrc,pFormatters);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // varchar field processing
  
  i8 iColumnLength = pColumn->iColumnLength();
  if ( ! StrLib::bExtractVarcharDefault(aaSrc,(char*)aVch,
                                        iColumnLength) )
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  StrLib::vSkipSpaces(aaSrc);
  
  //bool bSawEnclosedTrailing =
  //StrLib::bSkipTrailing(aaSrc,bSawEnclosedLeading,pFormatters->m_cEnclosedBy_Trailing);
  //if (! bSawEnclosedTrailing)
  //  return false;
  
  // should see a valid field end
  if ( ! DtmLib::bValidFieldEndDefault(*aaSrc,pFormatters) )
    return false;
  
  return true;
}



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append decimal datum to string                                             */
/*----------------------------------------------------------------------------*/
void Record::vAppendBcd(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  m_sRecStr.vCatSprint("%lld"_Stz,pDatum->m_iBcd);
}

/*----------------------------------------------------------------------------*/
/* append boolean datum to string                                             */
/*----------------------------------------------------------------------------*/
void Record::vAppendBol(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  m_sRecStr.vConcat(pDatum->m_iBol ? "true"_Stz : "false"_Stz);
}

/*----------------------------------------------------------------------------*/
/* append date datum to string                                                */
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
/* append float datum to string                                               */
/*----------------------------------------------------------------------------*/
void Record::vAppendFlt(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  m_sRecStr.vCatSprint("%g"_Stz,pDatum->m_dFlt);
}

/*----------------------------------------------------------------------------*/
/* append integer datum to string                                             */
/*----------------------------------------------------------------------------*/
void Record::vAppendInt(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  m_sRecStr.vCatSprint("%lld"_Stz,pDatum->m_iInt);
}

/*----------------------------------------------------------------------------*/
/* append interval datum to string                                            */
/*----------------------------------------------------------------------------*/
void Record::vAppendIvl(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  m_sRecStr.vCatSprint("%lld"_Stz,pDatum->m_iIvl);
}

/*----------------------------------------------------------------------------*/
/* append time datum to string                                                */
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
/* append timestamp datum to string                                           */
/*----------------------------------------------------------------------------*/
void Record::vAppendTsp(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  Tsp wTsp;
  wTsp.vAsI8(pDatum->m_iTsp);
  
  s32 sTsp;
  wTsp.vSprint(sTsp.aStz());
  
  m_sRecStr.vConcat(sTsp.aStz());
}

/*----------------------------------------------------------------------------*/
/* append varchar datum to string                                             */
/*----------------------------------------------------------------------------*/
void Record::vAppendVch(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  s256 sBuf;
  
  Vch::vSprintQuotes(sBuf,(Stz*)pDatum->m_ayDatum);
  m_sRecStr.vConcat(sBuf.aStz());
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
