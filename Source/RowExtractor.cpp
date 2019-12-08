/*============================================================================*/
/* RowExtractor.cpp -- RowExtractor                                           */
/*============================================================================*/

#include "Rows.h"
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
void RowExtractor::vAddElement(EntWrk* pEntWrk, Stz* szSrc,
                            Column* pCol, Formatters* pFmt)
{
  i8 nBytes;

  char* aSrc = (char*)szSrc;
  char* aDst = (char*)pEntWrk->ayElementCrs();

  if (DM_VCH == pCol->eDatumType()) {
    nBytes = StrLib::iWordFlip(aDst,aSrc);
    pEntWrk->vIncKeyBytes(nBytes);
    return;
  }

  DtmVal oDatum;
  oDatum.m_pCol = pCol;

  bool b = RowExtractor::bExtractElement(&oDatum,&aSrc,pCol,pFmt);
  ASSERT(b);

  if (oDatum.bIsNull()) {
    nBytes = Null::nCopy((byte*)aDst);
    pEntWrk->vIncKeyBytes(nBytes);
    return;
  }

  nBytes = Dtm::nCopy((byte*)aDst,oDatum.m_wDtm.m_iInt);
  pEntWrk->vIncKeyBytes(nBytes);
}

/*----------------------------------------------------------------------------*/
/* extract row from record (returning first column which has invalid field)   */
/*----------------------------------------------------------------------------*/
bool RowExtractor::bExtractNext(Row* pRow, char** aacCrs,
                             StreamDef* pStreamDef, Formatters* pFmt,
                             bool& bBlankLine_)
{
  char* acRec = *aacCrs;
  
  bBlankLine_ = false;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // leading blanks and blank lines
  
  // skip leading spaces
  if (pFmt->m_cRecordTerminator)
    StrLib::vSkipWhite(aacCrs);
  else
    StrLib::vSkipSpaces(aacCrs);
  
  // ignore blank lines
  if (! bIsThereRecordData(*aacCrs,pFmt) ) {
    bBlankLine_ = true;
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // debug
  
#ifdef WITH_ENCODING_TRACE
  char caRecord[C_16KB];
  StrLib::vCopyTill(caRecord,acRec,'\n');
  vTrace("\n--------------------------------------------------------------------------\n"_Stz);
  vTrace("\nFormatting record: %s\n"_Stz,caRecord);
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // extract row from record
  
  Column* pBadCol = pExtractRow(pRow,aacCrs,pStreamDef,pFmt);
  
  // any invalid field?
  if (pBadCol) {
    if (! AM::bMetadata()) {
      vWarn("Skipping row - invalid field %s in record: "_Stz,
            pBadCol->szColNm());
      for (char* ac = acRec; ( *ac && '\n' != *ac && '\r' != *ac ); ac++)
        vTrace("%c"_Stz,*ac);
      vTrace("\n"_Stz);
    }
    return false;
  }
  
  //SP_Row spRow;
  //vTrace("%s\n"_Stz,spRow.sz(pRow));
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract record into row (returning first column which has invalid field)   */
/*----------------------------------------------------------------------------*/
Column* RowExtractor::pExtractRow(Row* pRow, char** aacCrs,
                               StreamDef* pStreamDef, Formatters* pFmt)
{
  i8 nCols = pStreamDef->nColumns(C_ALL);
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    
    Column* pCol = pStreamDef->pGetColumn(i_Col);
  
    DtmVal oDatum;
    oDatum.m_pCol = pCol;
    
    bool b = false;
    eDM eDatum = pCol->eDatumType();
    
    switch (eDatum) {
      case DM_BCD: b = bExtractBcd(&oDatum,aacCrs,pFmt); break;
      case DM_BOL: b = bExtractBol(&oDatum,aacCrs,pFmt); break;
      case DM_CHR: b = bExtractVch(&oDatum,aacCrs,pFmt); break;
      case DM_DTE: b = bExtractDte(&oDatum,aacCrs,pFmt); break;
      case DM_FLT: b = bExtractFlt(&oDatum,aacCrs,pFmt); break;
      case DM_INT: b = bExtractInt(&oDatum,aacCrs,pFmt); break;
      case DM_IVL: b = bExtractIvl(&oDatum,aacCrs,pFmt); break;
      case DM_TME: b = bExtractTme(&oDatum,aacCrs,pFmt); break;
      case DM_TSP: b = bExtractTsp(&oDatum,aacCrs,pFmt); break;
      case DM_VCH: b = bExtractVch(&oDatum,aacCrs,pFmt); break;
      default:
        vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
               "RowExtractor::bExtract: Invalid eDatum : %lld\n"_Stz,eDatum);
    }
    
    if (! b)
      return pCol;
    
    // move past the field delimiter
    //   (but not past the RowExtractor terminator or RowExtractor ends)
    
    if (pCol->bIsLastColumn()) {
      // do not expect to see a field delimiter
      if ( pFmt->m_cFieldDelimiter &&
          pFmt->m_cRecordTerminator &&
          pFmt->m_cFieldDelimiter != pFmt->m_cRecordTerminator &&
          *(*aacCrs) == pFmt->m_cFieldDelimiter ) {
        // saw a field delimiter after last field (incorrect)
        return pCol;
      }
      /*
       // did not see a field delimiter (correct)
       if ( pFmt->m_cFieldDelimiter &&
       *(*aacCrs) == pFmt->m_cFieldDelimiter )
       (*aacCrs)++;
       */
    }
    else {
      // expect to see a field delimiter
      if ( pFmt->m_cFieldDelimiter &&
          *(*aacCrs) == pFmt->m_cFieldDelimiter )
        // saw a field delimiter after non-last field (correct)
        (*aacCrs)++;
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
bool RowExtractor::bExtractElement(DtmVal* pDtmVal, char** aacCrs,
                                Column* pCol, Formatters* pFmt)
{
  bool b = false;
  eDM eDatum = pCol->eDatumType();
  
  pDtmVal->m_pCol = pCol;
  
  i8 iYear, iMonth, iDay;
  i8 iHour, iMinute, iSecond, iMicro, iMilli;
  
  switch (eDatum) {
    case DM_BCD:
      b = StrLib::bExtractDecimal(aacCrs,pDtmVal->m_wDtm.m_iBcd,
                                  pCol->iColumnLength(),
                                  pCol->iColumnScale());
      break;
      
    case DM_BOL:
      b = StrLib::bExtractBoolean(aacCrs,pDtmVal->m_wDtm.m_iBol);
      break;
      
    case DM_CHR:
      b = bExtractVch(pDtmVal,aacCrs,pFmt);
      break;
      
    case DM_DTE:
      b = StrLib::bExtractDate2(aacCrs, iYear, iMonth, iDay,
                                pFmt->m_eDateComponent,
                                pFmt->m_cDateSeparator);
      if (!b)
        break;
      
      pDtmVal->m_wDtm.xDte.m_uYearDelta  = iYear - C_YEAR_BASE;
      pDtmVal->m_wDtm.xDte.m_uMonth = iMonth;
      pDtmVal->m_wDtm.xDte.m_uDay   = iDay;
      pDtmVal->m_wDtm.xDte.m_uSpare = 0;
      break;
      
    case DM_FLT:
      b = StrLib::bExtractFloat(aacCrs,pDtmVal->m_wDtm.m_dFlt);
      break;
      
    case DM_INT:
      b = StrLib::bExtractInteger(aacCrs,&pDtmVal->m_wDtm.m_iInt, true/*bWithSign*/);
      break;
      
    case DM_IVL:
      b = StrLib::bExtractInterval(aacCrs, iSecond, iMilli);
      pDtmVal->m_wDtm.xIvl.m_uSecond = iSecond;
      pDtmVal->m_wDtm.xIvl.m_uMicro  = iMilli*C_1K;
      break;
      
    case DM_TME:
      b = StrLib::bExtractTime(aacCrs, iHour, iMinute, iSecond, iMicro);
      
      if (!b)
        break;
      
      pDtmVal->m_wDtm.xTme.m_uHour   = iHour;
      pDtmVal->m_wDtm.xTme.m_uMinute = iMinute;
      pDtmVal->m_wDtm.xTme.m_uSecond = iSecond;
      pDtmVal->m_wDtm.xTme.m_uMicro  = iMicro;
      pDtmVal->m_wDtm.xTme.m_uSpare  = 0;
      
      break;
      
    case DM_TSP:
      b = StrLib::bExtractTimestamp2(aacCrs, iYear, iMonth, iDay,
                                     iHour, iMinute, iSecond, iMicro,
                                     pFmt->m_eDateComponent,
                                     pFmt->m_cDateSeparator);
      
      if (!b)
        break;
      
      pDtmVal->m_wDtm.xTsp.m_uYearDelta   = iYear - C_YEAR_BASE;
      pDtmVal->m_wDtm.xTsp.m_uMonth  = iMonth;
      pDtmVal->m_wDtm.xTsp.m_uDay    = iDay;
      pDtmVal->m_wDtm.xTsp.m_uHour   = iHour;
      pDtmVal->m_wDtm.xTsp.m_uMinute = iMinute;
      pDtmVal->m_wDtm.xTsp.m_uSecond = iSecond;
      pDtmVal->m_wDtm.xTsp.m_uMicro  = iMicro;
      pDtmVal->m_wDtm.xTsp.m_uLocal  = C_TSP_LOCAL;
      pDtmVal->m_wDtm.xTsp.m_uSpare  = 0;
      break;
      
    case DM_VCH:
      b = StrLib::bExtractVarcharDefault(aacCrs,
                                         pDtmVal->m_sVch.azChar(),
                                         pCol->iColumnLength(),
                                         pDtmVal->m_nChars);
      break;
      
    default:
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
             "RowExtractor::bExtractElement: Invalid eDatum : %lld\n"_Stz,eDatum);
  }
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* extract a decimal field from csv substr                                    */
/*----------------------------------------------------------------------------*/
bool RowExtractor::bExtractBcd(DtmVal* pDtmVal, char** aacCrs,
                                    Formatters* pFmt)
{
  Column* pColumn = pDtmVal->m_pCol;
  
  pDtmVal->m_eDatum = DM_BCD;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  bool bSawEnclosedLeading =
  StrLib::bSkipLeading(aacCrs,pFmt->m_cEnclosedBy_Leading);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (bTestForNull(pColumn,aacCrs,pFmt,bValidNull)) {
    if ( ! bValidNull )
      return false;
    pDtmVal->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // decimal field processing
  
  SN_Bcd snBcd;
  try {
    pDtmVal->m_wDtm.m_iBcd = snBcd.i(aacCrs,pColumn,pFmt);
  }
  catch (Exception xException) {
    return false;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  bool bSawEnclosedTrailing =
  StrLib::bSkipTrailing(aacCrs,bSawEnclosedLeading,pFmt->m_cEnclosedBy_Trailing);
  if (! bSawEnclosedTrailing)
    return false;
  
  // should see a valid field end
  if ( ! bValidFieldEnd(*aacCrs,pFmt) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a boolean field from csv substr                                    */
/*----------------------------------------------------------------------------*/
bool RowExtractor::bExtractBol(DtmVal* pDtmVal, char** aacCrs,
                                    Formatters* pFmt)
{
  Column* pColumn = pDtmVal->m_pCol;
  
  pDtmVal->m_eDatum = DM_BOL;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  bool bSawEnclosedLeading =
  StrLib::bSkipLeading(aacCrs,pFmt->m_cEnclosedBy_Leading);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (bTestForNull(pColumn,aacCrs,pFmt,bValidNull)) {
    if ( ! bValidNull )
      return false;
    pDtmVal->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // boolean field processing
  
  SN_Bol snBol;
  try {
    pDtmVal->m_wDtm.m_iBol = snBol.i(aacCrs,pColumn,pFmt);
  }
  catch (Exception xException) {
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  bool bSawEnclosedTrailing =
  StrLib::bSkipTrailing(aacCrs,bSawEnclosedLeading,pFmt->m_cEnclosedBy_Trailing);
  if (! bSawEnclosedTrailing)
    return false;
  
  // should see a valid field end
  if ( ! bValidFieldEnd(*aacCrs,pFmt) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a date field from csv substr                           yyyy-mo-ddZ */
/*----------------------------------------------------------------------------*/
bool RowExtractor::bExtractDte(DtmVal* pDtmVal, char** aacCrs,
                                    Formatters* pFmt)
{
  
  Column* pColumn = pDtmVal->m_pCol;
  
  pDtmVal->m_eDatum = DM_DTE;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  bool bSawEnclosedLeading =
  StrLib::bSkipLeading(aacCrs,pFmt->m_cEnclosedBy_Leading);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (bTestForNull(pColumn,aacCrs,pFmt,bValidNull)) {
    if ( ! bValidNull )
      return false;
    pDtmVal->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // date field processing
  
  SN_Dte snDte;
  try {
    pDtmVal->m_wDtm.m_iDte = snDte.i(aacCrs,pColumn,pFmt);
  }
  catch (Exception xException) {
    return false;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  bool bSawEnclosedTrailing =
  StrLib::bSkipTrailing(aacCrs,bSawEnclosedLeading,pFmt->m_cEnclosedBy_Trailing);
  if (! bSawEnclosedTrailing)
    return false;
  
  // should see a valid field end
  if ( ! bValidFieldEnd(*aacCrs,pFmt) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a float field from csv substr                                      */
/*----------------------------------------------------------------------------*/
bool RowExtractor::bExtractFlt(DtmVal* pDtmVal, char** aacCrs,
                                    Formatters* pFmt)
{
  Column* pColumn = pDtmVal->m_pCol;
  
  pDtmVal->m_eDatum = DM_FLT;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  bool bSawEnclosedLeading =
  StrLib::bSkipLeading(aacCrs,pFmt->m_cEnclosedBy_Leading);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (bTestForNull(pColumn,aacCrs,pFmt,bValidNull)) {
    if ( ! bValidNull )
      return false;
    pDtmVal->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // float field processing
  
  SN_Flt snFlt;
  try {
    pDtmVal->m_wDtm.m_dFlt = snFlt.d(aacCrs,pColumn,pFmt);
  }
  catch (Exception xException) {
    return false;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  bool bSawEnclosedTrailing =
  StrLib::bSkipTrailing(aacCrs,bSawEnclosedLeading,pFmt->m_cEnclosedBy_Trailing);
  if (! bSawEnclosedTrailing)
    return false;
  
  // should see a valid field end
  if ( ! bValidFieldEnd(*aacCrs,pFmt) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract an integer field from csv substr                                   */
/*----------------------------------------------------------------------------*/
bool RowExtractor::bExtractInt(DtmVal* pDtmVal, char** aacCrs,
                                    Formatters* pFmt)
{
  Column* pColumn = pDtmVal->m_pCol;
  
  pDtmVal->m_eDatum = DM_INT;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  bool bSawEnclosedLeading =
  StrLib::bSkipLeading(aacCrs,pFmt->m_cEnclosedBy_Leading);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (bTestForNull(pColumn,aacCrs,pFmt,bValidNull)) {
    if ( ! bValidNull )
      return false;
    pDtmVal->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // integer field processing
  
  SN_Int snInt;
  try {
    pDtmVal->m_wDtm.m_iInt = snInt.i(aacCrs,pColumn,pFmt);
  }
  catch (Exception xException) {
    return false;
  }
  
  // ignore any trailing digits (after optional '.')
  if ('.' == **aacCrs) {
    
    // skip the '.' and any digits
    (*aacCrs)++;
    StrLib::vSkipDigits(aacCrs);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  bool bSawEnclosedTrailing =
  StrLib::bSkipTrailing(aacCrs,bSawEnclosedLeading,pFmt->m_cEnclosedBy_Trailing);
  if (! bSawEnclosedTrailing)
    return false;
  
  // should see a valid field end
  if ( ! bValidFieldEnd(*aacCrs,pFmt) )
    return false;
  
  return true;
}


/*----------------------------------------------------------------------------*/
/* extract an interval field from csv substr                                  */
/*----------------------------------------------------------------------------*/
bool RowExtractor::bExtractIvl(DtmVal* pDtmVal, char** aacCrs,
                                    Formatters* pFmt)
{
  Column* pColumn = pDtmVal->m_pCol;
  
  pDtmVal->m_eDatum = DM_IVL;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  bool bSawEnclosedLeading =
  StrLib::bSkipLeading(aacCrs,pFmt->m_cEnclosedBy_Leading);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (bTestForNull(pColumn,aacCrs,pFmt,bValidNull)) {
    if ( ! bValidNull )
      return false;
    pDtmVal->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // interval field processing
  
  SN_Ivl snIvl;
  try {
    pDtmVal->m_wDtm.m_iIvl = snIvl.i(aacCrs,pColumn,pFmt);
  }
  catch (Exception xException) {
    return false;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  bool bSawEnclosedTrailing =
  StrLib::bSkipTrailing(aacCrs,bSawEnclosedLeading,pFmt->m_cEnclosedBy_Trailing);
  if (! bSawEnclosedTrailing)
    return false;
  
  // should see a valid field end
  if ( ! bValidFieldEnd(*aacCrs,pFmt) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a time field from csv substr                         hh:mi:ss.fffZ */
/*----------------------------------------------------------------------------*/
bool RowExtractor::bExtractTme(DtmVal* pDtmVal, char** aacCrs,
                                    Formatters* pFmt)
{
  
  Column* pColumn = pDtmVal->m_pCol;
  
  pDtmVal->m_eDatum = DM_TME;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  bool bSawEnclosedLeading =
  StrLib::bSkipLeading(aacCrs,pFmt->m_cEnclosedBy_Leading);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (bTestForNull(pColumn,aacCrs,pFmt,bValidNull)) {
    if ( ! bValidNull )
      return false;
    pDtmVal->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // time field processing
  
  SN_Tsp snTsp;
  try {
    pDtmVal->m_wDtm.m_iTsp = snTsp.i(aacCrs,pColumn,pFmt);
  }
  catch (Exception xException) {
    return false;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  bool bSawEnclosedTrailing =
  StrLib::bSkipTrailing(aacCrs,bSawEnclosedLeading,pFmt->m_cEnclosedBy_Trailing);
  if (! bSawEnclosedTrailing)
    return false;
  
  // should see a valid field end
  if ( ! bValidFieldEnd(*aacCrs,pFmt) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a timestamp field from csv substr         yyyy-mo-ddThh:mi:ss.fffZ */
/*----------------------------------------------------------------------------*/
bool RowExtractor::bExtractTsp(DtmVal* pDtmVal, char** aacCrs,
                                    Formatters* pFmt)
{
  
  Column* pColumn = pDtmVal->m_pCol;
  
  pDtmVal->m_eDatum = DM_TSP;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  bool bSawEnclosedLeading =
  StrLib::bSkipLeading(aacCrs,pFmt->m_cEnclosedBy_Leading);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (bTestForNull(pColumn,aacCrs,pFmt,bValidNull)) {
    if ( ! bValidNull )
      return false;
    pDtmVal->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // timestamp field processing
  
  SN_Tsp snTsp;
  try {
    pDtmVal->m_wDtm.m_iTsp = snTsp.i(aacCrs,pColumn,pFmt);
  }
  catch (Exception xException) {
    return false;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  bool bSawEnclosedTrailing =
  StrLib::bSkipTrailing(aacCrs,bSawEnclosedLeading,pFmt->m_cEnclosedBy_Trailing);
  if (! bSawEnclosedTrailing)
    return false;
  
  // should see a valid field end
  if ( ! bValidFieldEnd(*aacCrs,pFmt) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a varchar field from csv substr                                    */
/*----------------------------------------------------------------------------*/
bool RowExtractor::bExtractVch(DtmVal* pDtmVal, char** aacCrs,
                                    Formatters* pFmt)
{
  Column* pColumn = pDtmVal->m_pCol;
  
  pDtmVal->m_eDatum = DM_VCH;
  
 // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for null
  
  bool bValidNull = false;
  if (bTestForNull(pColumn,aacCrs,pFmt,bValidNull,false)) {
    if ( ! bValidNull )
      return false;
    pDtmVal->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for enclosed
  bool bSawEnclosedLeading = bIsEnclosedString(aacCrs,pFmt);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // varchar field processing
  
  SN_Vch snVch;
  try {
    pDtmVal->m_sVch = snVch.sz(aacCrs,pColumn,pFmt,
                              bSawEnclosedLeading,pDtmVal->m_nChars);
  }
  catch (Exception xException) {
    return false;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  bool bSawEnclosedTrailing =
  StrLib::bSkipTrailing(aacCrs,bSawEnclosedLeading,
                        pFmt->m_cEnclosedBy_Trailing);
  if (! bSawEnclosedTrailing)
    return false;
  
  // should see a valid field end
  if ( ! bValidFieldEnd(*aacCrs,pFmt) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is the string enclosed?                                                    */
/*----------------------------------------------------------------------------*/
bool RowExtractor::bIsEnclosedString(char** aacCrs, Formatters* pFmt)
{
  if (! pFmt->m_cEnclosedBy_Leading)
    return false;
  
  char *aStart = *aacCrs;
  
  // find first non-space character
  while (' ' == **aacCrs)
    (*aacCrs)++;
  
  // is it an enclosed varchar?
  if (**aacCrs == pFmt->m_cEnclosedBy_Leading) {
    (*aacCrs)++;
    return true;
  }
  else {
    *(aacCrs) = aStart;
    return false;
  }
}

/*----------------------------------------------------------------------------*/
/* is there data?                                                             */
/*----------------------------------------------------------------------------*/
bool RowExtractor::bIsThereFieldData(char* aSrc, Formatters* pFmt)
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
bool RowExtractor::bIsThereRecordData(char* aSrc, Formatters* pFmt)
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
bool RowExtractor::bTestForNull(Column* pColumn, char** aacCrs,
                          Formatters* pFmt,
                          bool& bValidNull,
                          bool bSkipTrailingSpaces)
{
  /*
   * This function scans aacCrs and attempts to identify the text form of a null.
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
  
  if ( ! bIsThereFieldData(*aacCrs, pFmt)) {
    if ( ! pColumn->bIsNullable() ) {
      // a null was detected in a non-nullable column
      bValidNull = false;
      return true;
    }
    // a null was detected in a nullable column
    bValidNull = true;
    return true;
  }
  
  //if ( ! bIsThereFieldData(*aacCrs, pFmt))
  //  return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for null, \N, '\N' or [\N]
  
  char* aStart = *aacCrs;
  
  if ( (**aacCrs == 'N' || **aacCrs == 'n') && (0 == StrLib::iCompare(*aacCrs+1, "ULL", 3, C_NO_CASE)) ) {
    // skip over token null
    *aacCrs = (*aacCrs) + 4;
  }
  else if (**aacCrs     == pFmt->m_cEscapeChar &&
           *(*aacCrs+1) == 'N') {
    // skip over \N
    *aacCrs = (*aacCrs) + 2;
  }
  else if (**aacCrs     == pFmt->m_cEnclosedBy_Leading &&
           *(*aacCrs+1) == pFmt->m_cEscapeChar &&
           *(*aacCrs+2) == 'N'                        &&
           *(*aacCrs+3) == pFmt->m_cEnclosedBy_Trailing) {
    // skip over '\N' or [\N]
    *aacCrs = (*aacCrs) + 4;
  }
  else if (**aacCrs    == pFmt->m_cEnclosedBy_Leading &&
           ((*(*aacCrs+1) == 'N' || *(*aacCrs+1) == 'n') && (0 == StrLib::iCompare(*aacCrs+2, "ULL", 3, C_NO_CASE))) &&
           *(*aacCrs+5) == pFmt->m_cEnclosedBy_Trailing) {
    // skip over 'NULL'
    *aacCrs = (*aacCrs) + 6;
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
    StrLib::vSkipSpaces(aacCrs);
  
  // check it was "null" only
  if (! bValidFieldEnd(*aacCrs,pFmt)) {
    // no well formed null detected
    *(aacCrs) = aStart;
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
bool RowExtractor::bTestForNullDefault(Column* pColumn, char** aacCrs,
                                 Formatters* pFmt, bool& bValidNull,
                                 bool bSkipTrailingSpaces)
{
  /*
   * This function scans aacCrs and attempts to identify the text form of a null.
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
  
  if ( ! bIsThereFieldData(*aacCrs, pFmt)) {
    if ( ! pColumn->bIsNullable() ) {
      // a null was detected in a non-nullable column
      bValidNull = false;
      return true;
    }
    // a null was detected in a nullable column
    bValidNull = true;
    return true;
  }
  
  //if ( ! bIsThereFieldData(*aacCrs, pFmt))
  //  return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for null, \N, '\N' or [\N]
  
  char* aStart = *aacCrs;
  
  if ( (**aacCrs == 'N' || **aacCrs == 'n') && (0 == StrLib::iCompare(*aacCrs+1, "ULL", 3, C_NO_CASE)) ) {
    // skip over token null
    *aacCrs = (*aacCrs) + 4;
  }
  else if (**aacCrs     == pFmt->m_cEscapeChar &&
           *(*aacCrs+1) == 'N') {
    // skip over \N
    *aacCrs = (*aacCrs) + 2;
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
    StrLib::vSkipSpaces(aacCrs);
  // check it was "null" only
  if (! bValidFieldEndDefault(*aacCrs,pFmt)) {
    // no well formed null detected
    *(aacCrs) = aStart;
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
bool RowExtractor::bValidFieldEnd(char* aSrc, Formatters* pFmt)
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
bool RowExtractor::bValidFieldEndDefault(char* aSrc, Formatters* pFmt)
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
bool RowExtractor::bValidRecordEnd(char* aSrc, Formatters* pFmt)
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
