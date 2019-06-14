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
/*-- Public Static -----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add element to entwrk key                                                  */
/*----------------------------------------------------------------------------*/
void Extractor::vAddElement(EntWrk* pEntWrk, Stz* aSrc,
                            Column* pCol, Formatters* pFormatters)
{
  bool b;
  i8 nBytes;
  
  char* pDst = (char*)pEntWrk->ayElementCrs();
  char* pSrc = (char*)aSrc;
  Datum oDatum;
  oDatum.m_pCol = pCol;
  
  if (pCol->eDatumType() != DM_VCH) {
    b = Extractor::bExtractElement(&oDatum,&pSrc,pCol,pFormatters);
    RASSERT(b);
  }
  
  switch(pCol->eDatumType()) {
      
    case DM_BCD:
      nBytes = Bcd::nCopy((char*)pDst,(char*)&oDatum.m_iBcd);
      break;
      
    case DM_BOL:
      nBytes = Bol::nCopy((char*)pDst,(char*)&oDatum.m_iBol);
      break;
      
    case DM_DTE:
      nBytes = Dte::nCopy((char*)pDst,(char*)&oDatum.m_iDte);
      break;
      
    case DM_INT:
      nBytes = Int::nCopy(pDst,(void*)&oDatum.m_iDte);
      break;
      
    case DM_IVL:
      nBytes = Ivl::nCopy(pDst,(void*)&oDatum.m_iIvl);
      break;
      
    case DM_FLT:
      nBytes = Flt::nCopy(pDst,(void*)&oDatum.m_dFlt);
      break;
      
    case DM_TME:
      nBytes = Tme::nCopy(pDst,&oDatum.m_iTme);
      break;
      
    case DM_TSP:
      nBytes = Tsp::nCopy(pDst,&oDatum.m_iTsp);
      break;
      
    case DM_VCH:
      nBytes = StrLib::iWordFlip(pDst, pSrc);
      break;
      
    default:
      NEVER_REACH;
  }
  
  pEntWrk->vIncKeyBytes(nBytes);
}

/*----------------------------------------------------------------------------*/
/* extract record into row (returning first column which has invalid field)   */
/*----------------------------------------------------------------------------*/
bool Extractor::bExtractNext(Row* pRow, char** aacRecCrs,
                             ColumnVec* pColVec, Formatters* pRecFmt,
                             bool& bBlankLine_)
{
  char* acRec = *aacRecCrs;
  
  bBlankLine_ = false;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // leading blanks and blank lines
  
  // skip leading spaces
  if (pRecFmt->m_cRecordTerminator)
    StrLib::vSkipWhite(aacRecCrs);
  else
    StrLib::vSkipSpaces(aacRecCrs);
  
  // ignore blank lines
  if (! bIsThereRecordData(*aacRecCrs,pRecFmt) ) {
    bBlankLine_ = true;
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // debug
  
#ifdef WITH_TRACE_ENCODING
  char caRecord[C_16KB];
  StrLib::vCopyTill(caRecord,acRec,'\n');
  vTrace("\n--------------------------------------------------------------------------\n"_Stz);
  vTrace("\nFormatting record: %s\n"_Stz,caRecord);
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // extract row from record
  
  Column* pBadCol = pExtractRow(pRow,aacRecCrs,pColVec,pRecFmt);
  
  // any invalid field?
  if (pBadCol) {
    if (! gpAttachMap->m_bMetadata) {
      vWarn("Skipping row - invalid field %s in record: "_Stz,
            pBadCol->szColNm());
      for (char* ac = acRec; ( *ac && '\n' != *ac && '\r' != *ac ); ac++)
        vTrace("%c"_Stz,*ac);
      vTrace("\n"_Stz);
    }
    return false;
  }
  
  //hpRow hp_Row;
  //vTrace("%s\n"_Stz,hp_Row.sz(pRow));
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract record into row (returning first column which has invalid field)   */
/*----------------------------------------------------------------------------*/
Column* Extractor::pExtractRow(Row* pRow, char** aacRecCrs,
                               ColumnVec* pColVec, Formatters* pRecFmt)
{
  i8 nCols = pColVec->nItems();
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    
    Column* pCol = pColVec->pGet(i_Col);
  
    Datum oDatum;
    oDatum.m_pCol = pCol;
    
    bool b = false;
    eDM eDatum = pCol->eDatumType();
    
    if (pRecFmt->m_bDefault) {
      switch (eDatum) {
        case DM_BCD: b = bExtractBcdDefault(&oDatum,aacRecCrs,pRecFmt); break;
        case DM_BOL: b = bExtractBolDefault(&oDatum,aacRecCrs,pRecFmt); break;
        case DM_CHR: b = bExtractVchDefault(&oDatum,aacRecCrs,pRecFmt); break;
        case DM_DTE: b = bExtractDteDefault(&oDatum,aacRecCrs,pRecFmt); break;
        case DM_FLT: b = bExtractFltDefault(&oDatum,aacRecCrs,pRecFmt); break;
        case DM_INT: b = bExtractIntDefault(&oDatum,aacRecCrs,pRecFmt); break;
        case DM_IVL: b = bExtractIvlDefault(&oDatum,aacRecCrs,pRecFmt); break;
        case DM_TME: b = bExtractTmeDefault(&oDatum,aacRecCrs,pRecFmt); break;
        case DM_TSP: b = bExtractTspDefault(&oDatum,aacRecCrs,pRecFmt); break;
        case DM_VCH: b = bExtractVchDefault(&oDatum,aacRecCrs,pRecFmt); break;
        default:
          vFatal("Extractor::bExtract: Invalid eDatum : %lld\n"_Stz,eDatum);
      }
    }
    else {
      switch (eDatum) {
        case DM_BCD: b = bExtractBcdSupplied(&oDatum,aacRecCrs,pRecFmt); break;
        case DM_BOL: b = bExtractBolSupplied(&oDatum,aacRecCrs,pRecFmt); break;
        case DM_CHR: b = bExtractVchSupplied(&oDatum,aacRecCrs,pRecFmt); break;
        case DM_DTE: b = bExtractDteSupplied(&oDatum,aacRecCrs,pRecFmt); break;
        case DM_FLT: b = bExtractFltSupplied(&oDatum,aacRecCrs,pRecFmt); break;
        case DM_INT: b = bExtractIntSupplied(&oDatum,aacRecCrs,pRecFmt); break;
        case DM_IVL: b = bExtractIvlSupplied(&oDatum,aacRecCrs,pRecFmt); break;
        case DM_TME: b = bExtractTmeSupplied(&oDatum,aacRecCrs,pRecFmt); break;
        case DM_TSP: b = bExtractTspSupplied(&oDatum,aacRecCrs,pRecFmt); break;
        case DM_VCH: b = bExtractVchSupplied(&oDatum,aacRecCrs,pRecFmt); break;
        default:
          vFatal("Extractor::bExtract: Invalid eDatum : %lld\n"_Stz,eDatum);
      }
    }
    
    if (! b)
      return pCol;
    
    // move past the field delimiter
    //   (but not past the Extractor terminator or Extractor ends)
    
    if (pCol->bIsLastColumn()) {
      // do not expect to see a field delimiter
      if ( pRecFmt->m_cFieldDelimiter &&
          pRecFmt->m_cRecordTerminator &&
          pRecFmt->m_cFieldDelimiter != pRecFmt->m_cRecordTerminator &&
          *(*aacRecCrs) == pRecFmt->m_cFieldDelimiter ) {
        // saw a field delimiter after last field (incorrect)
        return pCol;
      }
      /*
       // did not see a field delimiter (correct)
       if ( pFormatters->m_cFieldDelimiter &&
       *(*aacRecCrs) == pFormatters->m_cFieldDelimiter )
       (*aacRecCrs)++;
       */
    }
    else {
      // expect to see a field delimiter
      if ( pRecFmt->m_cFieldDelimiter &&
          *(*aacRecCrs) == pRecFmt->m_cFieldDelimiter )
        // saw a field delimiter after non-last field (correct)
        (*aacRecCrs)++;
      else
        // did not see a field delimiter after non-last field (incorrect)
        return pCol;
    }
    
    pRow->vAppendValue(&oDatum,pCol);
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*-- Private Static ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* extract key element                                                        */
/*----------------------------------------------------------------------------*/
bool Extractor::bExtractElement(Datum* pDatum, char** aacRecCrs,
                                Column* pCol, Formatters* pRecFmt)
{
  bool b = false;
  eDM eDatum = pCol->eDatumType();
  
  pDatum->m_pCol = pCol;
  
  i8 iYear, iMonth, iDay;
  i8 iHour, iMinute, iSecond, iMicro, iMilli;
  
  switch (eDatum) {
    case DM_BCD:
      b = StrLib::bExtractDecimal(aacRecCrs,pDatum->m_iBcd,
                                  pCol->iColumnLength(),
                                  pCol->iColumnScale());
      break;
      
    case DM_BOL:
      b = StrLib::bExtractBoolean(aacRecCrs,pDatum->m_iBol);
      break;
      
    case DM_CHR:
      b = bExtractVchDefault(pDatum,aacRecCrs,pRecFmt);
      break;
      
    case DM_DTE:
      b = StrLib::bExtractDate2(aacRecCrs, iYear, iMonth, iDay,
                                pRecFmt->m_eDateComponent,
                                pRecFmt->m_cDateSeparator);
      if (!b)
        break;
      
      pDatum->xDte.m_uYearDelta  = iYear - C_BASE_YEAR;
      pDatum->xDte.m_uMonth = iMonth;
      pDatum->xDte.m_uDay   = iDay;
      pDatum->xDte.m_uSpare = 0;
      break;
      
    case DM_FLT:
      b = StrLib::bExtractFloat(aacRecCrs,pDatum->m_dFlt);
      break;
      
    case DM_INT:
      b = StrLib::bExtractInteger(aacRecCrs,&pDatum->m_iInt, true/*bWithSign*/);
      break;
      
    case DM_IVL:
      b = StrLib::bExtractInterval(aacRecCrs, iSecond, iMilli);
      break;
      
    case DM_TME:
      b = StrLib::bExtractTime(aacRecCrs, iHour, iMinute, iSecond, iMicro);
      
      if (!b)
        break;
      
      pDatum->xTme.m_uHour   = iHour;
      pDatum->xTme.m_uMinute = iMinute;
      pDatum->xTme.m_uSecond = iSecond;
      pDatum->xTme.m_uMicro  = iMicro;
      pDatum->xTme.m_uSpare  = 0;
      
      break;
      
    case DM_TSP:
      b = StrLib::bExtractTimestamp2(aacRecCrs, iYear, iMonth, iDay,
                                     iHour, iMinute, iSecond, iMicro,
                                     pRecFmt->m_eDateComponent,
                                     pRecFmt->m_cDateSeparator);
      
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
      b = StrLib::bExtractVarcharDefault(aacRecCrs,
                                         pDatum->m_sVch.aChar(),
                                         pCol->iColumnLength(),
                                         pDatum->m_nChars);
      break;
      
    default:
      vFatal("Extractor::bExtractElement: Invalid eDatum : %lld\n"_Stz,eDatum);
  }
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* extract a decimal field from csv substr                                    */
/*----------------------------------------------------------------------------*/
bool Extractor::bExtractBcdSupplied(Datum* pDatum, char** aacRecCrs,
                                    Formatters* pFormatters)
{
  Column* pColumn = pDatum->m_pCol;
  
  pDatum->m_eDatum = DM_BCD;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  bool bSawEnclosedLeading =
  StrLib::bSkipLeading(aacRecCrs,pFormatters->m_cEnclosedBy_Leading);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (bTestForNull(pColumn,aacRecCrs,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    pDatum->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // decimal field processing
  
  if ( ! StrLib::bExtractDecimal(aacRecCrs,pDatum->m_iBcd,
                                 pColumn->iColumnLength(),pColumn->iColumnScale()) )
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  bool bSawEnclosedTrailing =
  StrLib::bSkipTrailing(aacRecCrs,bSawEnclosedLeading,pFormatters->m_cEnclosedBy_Trailing);
  if (! bSawEnclosedTrailing)
    return false;
  
  // should see a valid field end
  if ( ! bValidFieldEnd(*aacRecCrs,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a decimal field from csv substr                                    */
/*----------------------------------------------------------------------------*/
bool Extractor::bExtractBcdDefault(Datum* pDatum, char** aacRecCrs,
                                   Formatters* pFormatters)
{
  Column* pColumn = pDatum->m_pCol;
  
  pDatum->m_eDatum = DM_BCD;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  StrLib::vSkipSpaces(aacRecCrs);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (bTestForNullDefault(pColumn,aacRecCrs,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    pDatum->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // decimal field processing
  
  if ( ! StrLib::bExtractDecimal(aacRecCrs,pDatum->m_iBcd,
                                 pColumn->iColumnLength(),pColumn->iColumnScale()) )
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  StrLib::vSkipSpaces(aacRecCrs);
  
  // should see a valid field end
  if ( ! bValidFieldEndDefault(*aacRecCrs,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a boolean field from csv substr                                    */
/*----------------------------------------------------------------------------*/
bool Extractor::bExtractBolSupplied(Datum* pDatum, char** aacRecCrs,
                                    Formatters* pFormatters)
{
  Column* pColumn = pDatum->m_pCol;
  
  pDatum->m_eDatum = DM_BOL;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  bool bSawEnclosedLeading =
  StrLib::bSkipLeading(aacRecCrs,pFormatters->m_cEnclosedBy_Leading);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (bTestForNull(pColumn,aacRecCrs,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    pDatum->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // boolean field processing
  
  if ( ! StrLib::bExtractBoolean(aacRecCrs,pDatum->m_iBol) )
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  bool bSawEnclosedTrailing =
  StrLib::bSkipTrailing(aacRecCrs,bSawEnclosedLeading,pFormatters->m_cEnclosedBy_Trailing);
  if (! bSawEnclosedTrailing)
    return false;
  
  // should see a valid field end
  if ( ! bValidFieldEnd(*aacRecCrs,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a boolean field from csv substr                                    */
/*----------------------------------------------------------------------------*/
bool Extractor::bExtractBolDefault(Datum* pDatum, char** aacRecCrs,
                                   Formatters* pFormatters)
{
  Column* pColumn = pDatum->m_pCol;
  
  pDatum->m_eDatum = DM_BOL;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  StrLib::vSkipSpaces(aacRecCrs);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (bTestForNullDefault(pColumn,aacRecCrs,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    pDatum->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // boolean field processing
  
  if ( ! StrLib::bExtractBoolean(aacRecCrs,pDatum->m_iBol) )
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  StrLib::vSkipSpaces(aacRecCrs);
  
  // should see a valid field end
  if ( ! bValidFieldEndDefault(*aacRecCrs,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a date field from csv substr                           yyyy-mo-ddZ */
/*----------------------------------------------------------------------------*/
bool Extractor::bExtractDteSupplied(Datum* pDatum, char** aacRecCrs,
                                    Formatters* pFormatters)
{
  // refer: http://en.wikipedia.org/wiki/ISO_8601
  
  Column* pColumn = pDatum->m_pCol;
  
  pDatum->m_eDatum = DM_DTE;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  bool bSawEnclosedLeading =
  StrLib::bSkipLeading(aacRecCrs,pFormatters->m_cEnclosedBy_Leading);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (bTestForNull(pColumn,aacRecCrs,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    pDatum->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // date field processing
  
  i8 iYear, iMonth, iDay;
  
  if ( ! StrLib::bExtractDate2(aacRecCrs, iYear, iMonth, iDay,
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
  StrLib::bSkipTrailing(aacRecCrs,bSawEnclosedLeading,pFormatters->m_cEnclosedBy_Trailing);
  if (! bSawEnclosedTrailing)
    return false;
  
  // should see a valid field end
  if ( ! bValidFieldEnd(*aacRecCrs,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a date field from csv substr                           yyyy-mo-ddZ */
/*----------------------------------------------------------------------------*/
bool Extractor::bExtractDteDefault(Datum* pDatum, char** aacRecCrs,
                                   Formatters* pFormatters)
{
  // refer: http://en.wikipedia.org/wiki/ISO_8601
  
  Column* pColumn = pDatum->m_pCol;
  
  pDatum->m_eDatum = DM_DTE;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  StrLib::vSkipSpaces(aacRecCrs);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (bTestForNullDefault(pColumn,aacRecCrs,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    pDatum->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // date field processing
  
  i8 iYear, iMonth, iDay;
  
  if ( ! StrLib::bExtractDate2(aacRecCrs, iYear, iMonth, iDay,
                               pFormatters->m_eDateComponent,
                               pFormatters->m_cDateSeparator) )
    return false;
  
  pDatum->xDte.m_uYearDelta  = iYear - C_BASE_YEAR;
  pDatum->xDte.m_uMonth = iMonth;
  pDatum->xDte.m_uDay   = iDay;
  pDatum->xDte.m_uSpare = 0;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  StrLib::vSkipSpaces(aacRecCrs);
  
  // should see a valid field end
  if ( ! bValidFieldEndDefault(*aacRecCrs,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a float field from csv substr                                      */
/*----------------------------------------------------------------------------*/
bool Extractor::bExtractFltSupplied(Datum* pDatum, char** aacRecCrs,
                                    Formatters* pFormatters)
{
  Column* pColumn = pDatum->m_pCol;
  
  pDatum->m_eDatum = DM_FLT;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  bool bSawEnclosedLeading =
  StrLib::bSkipLeading(aacRecCrs,pFormatters->m_cEnclosedBy_Leading);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (bTestForNull(pColumn,aacRecCrs,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    pDatum->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // float field processing
  
  if ( ! StrLib::bExtractFloat(aacRecCrs,pDatum->m_dFlt) )
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  bool bSawEnclosedTrailing =
  StrLib::bSkipTrailing(aacRecCrs,bSawEnclosedLeading,pFormatters->m_cEnclosedBy_Trailing);
  if (! bSawEnclosedTrailing)
    return false;
  
  // should see a valid field end
  if ( ! bValidFieldEnd(*aacRecCrs,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a float field from csv substr                                      */
/*----------------------------------------------------------------------------*/
bool Extractor::bExtractFltDefault(Datum* pDatum, char** aacRecCrs,
                                   Formatters* pFormatters)
{
  Column* pColumn = pDatum->m_pCol;
  
  pDatum->m_eDatum = DM_FLT;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  StrLib::vSkipSpaces(aacRecCrs);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (bTestForNullDefault(pColumn,aacRecCrs,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    pDatum->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // float field processing
  
  if ( ! StrLib::bExtractFloat(aacRecCrs,pDatum->m_dFlt) )
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  StrLib::vSkipSpaces(aacRecCrs);
  
  // should see a valid field end
  if ( ! bValidFieldEndDefault(*aacRecCrs,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract an integer field from csv substr                                   */
/*----------------------------------------------------------------------------*/
bool Extractor::bExtractIntSupplied(Datum* pDatum, char** aacRecCrs,
                                    Formatters* pFormatters)
{
  Column* pColumn = pDatum->m_pCol;
  
  pDatum->m_eDatum = DM_INT;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  bool bSawEnclosedLeading =
  StrLib::bSkipLeading(aacRecCrs,pFormatters->m_cEnclosedBy_Leading);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (bTestForNull(pColumn,aacRecCrs,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    pDatum->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // integer field processing
  
  if ( ! StrLib::bExtractInteger(aacRecCrs,&pDatum->m_iInt,
                                 true/*bWithSign*/) )
    return false;
  
  // ignore any trailing digits (after optional '.')
  if ('.' == **aacRecCrs) {
    
    // skip the '.' and any digits
    (*aacRecCrs)++;
    StrLib::vSkipDigits(aacRecCrs);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  bool bSawEnclosedTrailing =
  StrLib::bSkipTrailing(aacRecCrs,bSawEnclosedLeading,pFormatters->m_cEnclosedBy_Trailing);
  if (! bSawEnclosedTrailing)
    return false;
  
  // should see a valid field end
  if ( ! bValidFieldEnd(*aacRecCrs,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract an integer field from csv substr                                   */
/*----------------------------------------------------------------------------*/
bool Extractor::bExtractIntDefault(Datum* pDatum, char** aacRecCrs,
                                   Formatters* pFormatters)
{
  Column* pColumn = pDatum->m_pCol;
  
  pDatum->m_eDatum = DM_INT;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  StrLib::vSkipSpaces(aacRecCrs);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (bTestForNullDefault(pColumn,aacRecCrs,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    pDatum->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // integer field processing
  
  if ( ! StrLib::bExtractInteger(aacRecCrs,&pDatum->m_iInt,
                                 true/*bWithSign*/) )
    return false;
  
  // ignore any trailing digits (after optional '.')
  if ('.' == **aacRecCrs) {
    
    // skip the '.' and any digits
    (*aacRecCrs)++;
    StrLib::vSkipDigits(aacRecCrs);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  StrLib::vSkipSpaces(aacRecCrs);
  
  // should see a valid field end
  if ( ! bValidFieldEndDefault(*aacRecCrs,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract an interval field from csv substr                                  */
/*----------------------------------------------------------------------------*/
bool Extractor::bExtractIvlSupplied(Datum* pDatum, char** aacRecCrs,
                                    Formatters* pFormatters)
{
  Column* pColumn = pDatum->m_pCol;
  
  pDatum->m_eDatum = DM_IVL;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  bool bSawEnclosedLeading =
  StrLib::bSkipLeading(aacRecCrs,pFormatters->m_cEnclosedBy_Leading);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (bTestForNull(pColumn,aacRecCrs,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    pDatum->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // interval field processing
  
  i8 iSecond, iMilli;
  
  if ( ! StrLib::bExtractInterval(aacRecCrs, iSecond, iMilli) )
    return false;
  
  pDatum->xIvl.m_uSecond = iSecond;
  pDatum->xIvl.m_uMilli  = iMilli;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  bool bSawEnclosedTrailing =
  StrLib::bSkipTrailing(aacRecCrs,bSawEnclosedLeading,pFormatters->m_cEnclosedBy_Trailing);
  if (! bSawEnclosedTrailing)
    return false;
  
  // should see a valid field end
  if ( ! bValidFieldEnd(*aacRecCrs,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract an interval field from csv substr                                  */
/*----------------------------------------------------------------------------*/
bool Extractor::bExtractIvlDefault(Datum* pDatum, char** aacRecCrs,
                                   Formatters* pFormatters)
{
  Column* pColumn = pDatum->m_pCol;
  
  pDatum->m_eDatum = DM_IVL;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  StrLib::vSkipSpaces(aacRecCrs);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (bTestForNullDefault(pColumn,aacRecCrs,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    pDatum->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // interval field processing
  
  i8 iSecond, iMilli;
  
  if ( ! StrLib::bExtractInterval(aacRecCrs, iSecond, iMilli) )
    return false;
  
  pDatum->xIvl.m_uSecond = iSecond;
  pDatum->xIvl.m_uMilli  = iMilli;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  StrLib::vSkipSpaces(aacRecCrs);
  
  // should see a valid field end
  if ( ! bValidFieldEndDefault(*aacRecCrs,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a time field from csv substr                         hh:mi:ss.fffZ */
/*----------------------------------------------------------------------------*/
bool Extractor::bExtractTmeSupplied(Datum* pDatum, char** aacRecCrs,
                                    Formatters* pFormatters)
{
  // refer: http://en.wikipedia.org/wiki/ISO_8601
  
  Column* pColumn = pDatum->m_pCol;
  
  pDatum->m_eDatum = DM_TME;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  bool bSawEnclosedLeading =
  StrLib::bSkipLeading(aacRecCrs,pFormatters->m_cEnclosedBy_Leading);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (bTestForNull(pColumn,aacRecCrs,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    pDatum->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // time field processing
  
  i8 iHour, iMinute, iSecond, iMicro;
  
  if ( ! StrLib::bExtractTime(aacRecCrs, iHour, iMinute, iSecond, iMicro) )
    return false;
  
  pDatum->xTme.m_uHour   = iHour;
  pDatum->xTme.m_uMinute = iMinute;
  pDatum->xTme.m_uSecond = iSecond;
  pDatum->xTme.m_uMicro  = iMicro;
  pDatum->xTme.m_uSpare  = 0;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  bool bSawEnclosedTrailing =
  StrLib::bSkipTrailing(aacRecCrs,bSawEnclosedLeading,pFormatters->m_cEnclosedBy_Trailing);
  if (! bSawEnclosedTrailing)
    return false;
  
  // should see a valid field end
  if ( ! bValidFieldEnd(*aacRecCrs,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a time field from csv substr                         hh:mi:ss.fffZ */
/*----------------------------------------------------------------------------*/
bool Extractor::bExtractTmeDefault(Datum* pDatum, char** aacRecCrs,
                                   Formatters* pFormatters)
{
  // refer: http://en.wikipedia.org/wiki/ISO_8601
  
  Column* pColumn = pDatum->m_pCol;
  
  pDatum->m_eDatum = DM_TME;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  StrLib::vSkipSpaces(aacRecCrs);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (bTestForNullDefault(pColumn,aacRecCrs,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    pDatum->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // time field processing
  
  i8 iHour, iMinute, iSecond, iMicro;
  
  if ( ! StrLib::bExtractTime(aacRecCrs, iHour, iMinute, iSecond, iMicro) )
    return false;
  
  pDatum->xTme.m_uHour   = iHour;
  pDatum->xTme.m_uMinute = iMinute;
  pDatum->xTme.m_uSecond = iSecond;
  pDatum->xTme.m_uMicro  = iMicro;
  pDatum->xTme.m_uSpare  = 0;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  StrLib::vSkipSpaces(aacRecCrs);
  
  // should see a valid field end
  if ( ! bValidFieldEndDefault(*aacRecCrs,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a timestamp field from csv substr         yyyy-mo-ddThh:mi:ss.fffZ */
/*----------------------------------------------------------------------------*/
bool Extractor::bExtractTspSupplied(Datum* pDatum, char** aacRecCrs,
                                    Formatters* pFormatters)
{
  // refer: http://en.wikipedia.org/wiki/ISO_8601
  
  Column* pColumn = pDatum->m_pCol;
  
  pDatum->m_eDatum = DM_TSP;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  bool bSawEnclosedLeading =
  StrLib::bSkipLeading(aacRecCrs,pFormatters->m_cEnclosedBy_Leading);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (bTestForNull(pColumn,aacRecCrs,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    pDatum->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // timestamp field processing
  
  i8 iYear, iMonth, iDay;
  i8 iHour, iMinute, iSecond, iMicro;
  
  if ( ! StrLib::bExtractTimestamp2(aacRecCrs,
                                    iYear, iMonth, iDay,
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
  StrLib::bSkipTrailing(aacRecCrs,bSawEnclosedLeading,pFormatters->m_cEnclosedBy_Trailing);
  if (! bSawEnclosedTrailing)
    return false;
  
  // should see a valid field end
  if ( ! bValidFieldEnd(*aacRecCrs,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a timestamp field from csv substr         yyyy-mo-ddThh:mi:ss.fffZ */
/*----------------------------------------------------------------------------*/
bool Extractor::bExtractTspDefault(Datum* pDatum, char** aacRecCrs,
                                   Formatters* pFormatters)
{
  // refer: http://en.wikipedia.org/wiki/ISO_8601
  
  Column* pColumn = pDatum->m_pCol;
  
  pDatum->m_eDatum = DM_TSP;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  StrLib::vSkipSpaces(aacRecCrs);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (bTestForNullDefault(pColumn,aacRecCrs,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    pDatum->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // timestamp field processing
  
  i8 iYear, iMonth, iDay;
  i8 iHour, iMinute, iSecond, iMicro;
  
  if ( ! StrLib::bExtractTimestamp2(aacRecCrs,
                                    iYear, iMonth, iDay,
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
  
  StrLib::vSkipSpaces(aacRecCrs);
  
  // should see a valid field end
  if ( ! bValidFieldEndDefault(*aacRecCrs,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a varchar field from csv substr                                    */
/*----------------------------------------------------------------------------*/
bool Extractor::bExtractVchSupplied(Datum* pDatum, char** aacRecCrs,
                                    Formatters* pFormatters)
{
  Column* pColumn = pDatum->m_pCol;
  
  pDatum->m_eDatum = DM_VCH;
  
 // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for null
  
  bool bValidNull = false;
  if (bTestForNull(pColumn,aacRecCrs,pFormatters,bValidNull,false)) {
    if ( ! bValidNull )
      return false;
    pDatum->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for enclosed
  bool bSawEnclosedLeading = bIsEnclosedString(aacRecCrs,pFormatters);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // varchar field processing
  
  i8 iColumnLength = pColumn->iColumnLength();
  i8 nChars;
  if ( ! StrLib::bExtractVarchar(aacRecCrs,
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
  StrLib::bSkipTrailing(aacRecCrs,bSawEnclosedLeading,
                        pFormatters->m_cEnclosedBy_Trailing);
  if (! bSawEnclosedTrailing)
    return false;
  
  // should see a valid field end
  if ( ! bValidFieldEnd(*aacRecCrs,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a varchar field from csv substr                                    */
/*----------------------------------------------------------------------------*/
bool Extractor::bExtractVchDefault(Datum* pDatum, char** aacRecCrs,
                                   Formatters* pFormatters)
{
  Column* pColumn = pDatum->m_pCol;
  
  pDatum->m_eDatum = DM_VCH;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for null
  
  bool bValidNull = false;
  if (bTestForNullDefault(pColumn,aacRecCrs,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    pDatum->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for enclosed
  //bool bSawEnclosedLeading = bIsEnclosedString(aacRecCrs,pFormatters);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // varchar field processing
  
  i8 iColumnLength = pColumn->iColumnLength();
  if ( ! StrLib::bExtractVarcharDefault(aacRecCrs,
                                        pDatum->m_sVch.aChar(),
                                        iColumnLength,
                                        pDatum->m_nChars) )
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  StrLib::vSkipSpaces(aacRecCrs);
  
  //bool bSawEnclosedTrailing =
  //StrLib::bSkipTrailing(aacRecCrs,bSawEnclosedLeading,pFormatters->m_cEnclosedBy_Trailing);
  //if (! bSawEnclosedTrailing)
  //  return false;
  
  // should see a valid field end
  if ( ! bValidFieldEndDefault(*aacRecCrs,pFormatters) )
    return false;
  
  return true;
}



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is the string enclosed?                                                    */
/*----------------------------------------------------------------------------*/
bool Extractor::bIsEnclosedString(char** aacRecCrs, Formatters* pFormatters)
{
  if (! pFormatters->m_cEnclosedBy_Leading)
    return false;
  
  char *aStart = *aacRecCrs;
  
  // find first non-space character
  while (' ' == **aacRecCrs)
    (*aacRecCrs)++;
  
  // is it an enclosed varchar?
  if (**aacRecCrs == pFormatters->m_cEnclosedBy_Leading) {
    (*aacRecCrs)++;
    return true;
  }
  else {
    *(aacRecCrs) = aStart;
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
bool Extractor::bTestForNull(Column* pColumn, char** aacRecCrs,
                          Formatters* pFormatters,
                          bool& bValidNull,
                          bool bSkipTrailingSpaces)
{
  /*
   * This function scans aacRecCrs and attempts to identify the text form of a null.
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
  
  if ( ! bIsThereFieldData(*aacRecCrs, pFormatters)) {
    if ( ! pColumn->bIsNullable() ) {
      // a null was detected in a non-nullable column
      bValidNull = false;
      return true;
    }
    // a null was detected in a nullable column
    bValidNull = true;
    return true;
  }
  
  //if ( ! bIsThereFieldData(*aacRecCrs, pFormatters))
  //  return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for null, \N, '\N' or [\N]
  
  char* aStart = *aacRecCrs;
  
  if ( (**aacRecCrs == 'N' || **aacRecCrs == 'n') && (0 == StrLib::iCompare(*aacRecCrs+1, "ULL", 3, C_NO_CASE)) ) {
    // skip over token null
    *aacRecCrs = (*aacRecCrs) + 4;
  }
  else if (**aacRecCrs     == pFormatters->m_cEscapeChar &&
           *(*aacRecCrs+1) == 'N') {
    // skip over \N
    *aacRecCrs = (*aacRecCrs) + 2;
  }
  else if (**aacRecCrs     == pFormatters->m_cEnclosedBy_Leading &&
           *(*aacRecCrs+1) == pFormatters->m_cEscapeChar &&
           *(*aacRecCrs+2) == 'N'                        &&
           *(*aacRecCrs+3) == pFormatters->m_cEnclosedBy_Trailing) {
    // skip over '\N' or [\N]
    *aacRecCrs = (*aacRecCrs) + 4;
  }
  else if (**aacRecCrs    == pFormatters->m_cEnclosedBy_Leading &&
           ((*(*aacRecCrs+1) == 'N' || *(*aacRecCrs+1) == 'n') && (0 == StrLib::iCompare(*aacRecCrs+2, "ULL", 3, C_NO_CASE))) &&
           *(*aacRecCrs+5) == pFormatters->m_cEnclosedBy_Trailing) {
    // skip over 'NULL'
    *aacRecCrs = (*aacRecCrs) + 6;
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
    StrLib::vSkipSpaces(aacRecCrs);
  
  // check it was "null" only
  if (! bValidFieldEnd(*aacRecCrs,pFormatters)) {
    // no well formed null detected
    *(aacRecCrs) = aStart;
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
bool Extractor::bTestForNullDefault(Column* pColumn, char** aacRecCrs,
                                 Formatters* pFormatters, bool& bValidNull,
                                 bool bSkipTrailingSpaces)
{
  /*
   * This function scans aacRecCrs and attempts to identify the text form of a null.
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
  
  if ( ! bIsThereFieldData(*aacRecCrs, pFormatters)) {
    if ( ! pColumn->bIsNullable() ) {
      // a null was detected in a non-nullable column
      bValidNull = false;
      return true;
    }
    // a null was detected in a nullable column
    bValidNull = true;
    return true;
  }
  
  //if ( ! bIsThereFieldData(*aacRecCrs, pFormatters))
  //  return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for null, \N, '\N' or [\N]
  
  char* aStart = *aacRecCrs;
  
  if ( (**aacRecCrs == 'N' || **aacRecCrs == 'n') && (0 == StrLib::iCompare(*aacRecCrs+1, "ULL", 3, C_NO_CASE)) ) {
    // skip over token null
    *aacRecCrs = (*aacRecCrs) + 4;
  }
  else if (**aacRecCrs     == pFormatters->m_cEscapeChar &&
           *(*aacRecCrs+1) == 'N') {
    // skip over \N
    *aacRecCrs = (*aacRecCrs) + 2;
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
    StrLib::vSkipSpaces(aacRecCrs);
  // check it was "null" only
  if (! bValidFieldEndDefault(*aacRecCrs,pFormatters)) {
    // no well formed null detected
    *(aacRecCrs) = aStart;
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




/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
