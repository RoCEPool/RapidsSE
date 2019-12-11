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
  oDatum.vColumn(pCol);

  bool b = RowExtractor::bExtractDtm(&oDatum,&aSrc,pFmt);
  ASSERT(b);

  if (oDatum.bIsNull()) {
    nBytes = Null::nCopy((byte*)aDst);
    pEntWrk->vIncKeyBytes(nBytes);
    return;
  }

  nBytes = Dtm::nCopy((byte*)aDst,oDatum.u.m_xAnyVal.m_iInt);
  pEntWrk->vIncKeyBytes(nBytes);
}

/*----------------------------------------------------------------------------*/
/* extract row from record (returning first column which has invalid field)   */
/*----------------------------------------------------------------------------*/
bool RowExtractor::bExtractNext(Row* pRow, StreamDef* pStreamDef,
                                char** aazCrs, Format* pFmt,
                                bool& bBlankLine_)
{
  char* acRec = *aazCrs;
  
  bBlankLine_ = false;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // leading blanks and blank lines
  
  // skip leading spaces
  if (pFmt->m_cRecordTerminator)
    StrLib::vSkipWhite(aazCrs);
  else
    StrLib::vSkipSpaces(aazCrs);
  
  // ignore blank lines
  if (! bIsThereRecordData(*aazCrs,pFmt) ) {
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
  
  Column* pBadCol = pExtractRow(pRow,pStreamDef,aazCrs,pFmt);
  
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
Column* RowExtractor::pExtractRow(Row* pRow, StreamDef* pStreamDef,
                                  char** aazCrs, Format* pFmt)
{
  bool bSawEnclosedLeading;
  
  i8 nCols = pStreamDef->nColumns(C_ALL);
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    
    Column* pCol = pStreamDef->pGetColumn(i_Col);
  
    Datum oDatum;
    oDatum.vColumn(pCol);
    
    bool b = false;
    eDM eDatum = pCol->eDatum();
    
    
    // - - - -
    // leading
    
    if (DM_VCH == eDatum)
      bSawEnclosedLeading = false;
    else
      bSawEnclosedLeading =
        StrLib::bSkipLeading(aazCrs,pFmt->m_cEnclosedBy_Leading);
    
    // - - - -
    // null

//    bool bValidNull = false;
//    if (bTestForNull(pCol,aazCrs,pFmt,bValidNull)) {
//      if ( ! bValidNull )
//        return pCol;
//      oDatum.vSetNull();
//    }
   
    if (bIsSomeNull(aazCrs,pFmt)) {
      if ( ! pCol->bIsNullable() ) {
        // a null was detected in a non-nullable column
        return pCol;
      }
      oDatum.vSetNull();
      goto L_TRAILING;
    }

    
    // - - - -
    // value
    
    switch (eDatum) {
      case DM_BCD: b = bExtractBcd(&oDatum,aazCrs,pFmt); break;
      case DM_BOL: b = bExtractBol(&oDatum,aazCrs,pFmt); break;
      case DM_DTE: b = bExtractDte(&oDatum,aazCrs,pFmt); break;
      case DM_FLT: b = bExtractFlt(&oDatum,aazCrs,pFmt); break;
      case DM_INT: b = bExtractInt(&oDatum,aazCrs,pFmt); break;
      case DM_IVL: b = bExtractIvl(&oDatum,aazCrs,pFmt); break;
      case DM_TME: b = bExtractTme(&oDatum,aazCrs,pFmt); break;
      case DM_TSP: b = bExtractTsp(&oDatum,aazCrs,pFmt); break;
      case DM_VCH: b = bExtractVch(&oDatum,aazCrs,pFmt); break;
      default:
        vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
               "RowExtractor::bExtract: Invalid eDatum : %lld\n"_Stz,eDatum);
    }
    
    if (! b)
      return pCol;
    
    // - - - - -
    // trailing
    
  L_TRAILING:
    
    bool bSawEnclosedTrailing =
    StrLib::bSkipTrailing(aazCrs,bSawEnclosedLeading,pFmt->m_cEnclosedBy_Trailing);
    if (! bSawEnclosedTrailing)
      return pCol;
    
    // should see a valid field end
    if ( ! bValidFieldEnd(*aazCrs,pFmt) )
      return pCol;
    
    

    
    // move past the field delimiter
    //   (but not past the RowExtractor terminator or RowExtractor ends)
    
    // - - - -
    
    if (pCol->bIsLastColumn()) {
      // do not expect to see a field delimiter
      if ( pFmt->m_cFieldDelimiter &&
          pFmt->m_cRecordTerminator &&
          pFmt->m_cFieldDelimiter != pFmt->m_cRecordTerminator &&
          *(*aazCrs) == pFmt->m_cFieldDelimiter ) {
        // saw a field delimiter after last field (incorrect)
        return pCol;
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
bool RowExtractor::bExtractDtm(Datum* pDatum, char** aazCrs, Format* pFmt)
{
  bool b = false;
  
  Column* pCol = pDatum->pColumn();
  eDM eDatum   = pCol->eDatum();
  
  i8 iYear, iMonth, iDay;
  i8 iHour, iMinute, iSecond, iMicro, iMilli;
  
  switch (eDatum) {
    case DM_BCD:
      b = StrLib::bExtractDecimal(aazCrs,pDatum->u.m_xAnyVal.m_iBcd,
                                  pCol->iColumnLength(),
                                  pCol->iColumnScale());
      break;
      
    case DM_BOL:
      b = StrLib::bExtractBoolean(aazCrs,pDatum->u.m_xAnyVal.m_iBol);
      break;
      
    case DM_DTE:
      b = StrLib::bExtractDate2(aazCrs, iYear, iMonth, iDay,
                                pFmt->m_eDateComponent,
                                pFmt->m_cDateSeparator);
      if (!b)
        break;
      
      pDatum->u.m_xAnyVal.m_xDte.m_uYearDelta  = iYear - C_YEAR_BASE;
      pDatum->u.m_xAnyVal.m_xDte.m_uMonth = iMonth;
      pDatum->u.m_xAnyVal.m_xDte.m_uDay   = iDay;
      pDatum->u.m_xAnyVal.m_xDte.m_uSpare = 0;
      break;
      
    case DM_FLT:
      b = StrLib::bExtractFloat(aazCrs,pDatum->u.m_xAnyVal.m_dFlt);
      break;
      
    case DM_INT:
      b = StrLib::bExtractInteger(aazCrs,&pDatum->u.m_xAnyVal.m_iInt, true/*bWithSign*/);
      break;
      
    case DM_IVL:
      b = StrLib::bExtractInterval(aazCrs, iSecond, iMilli);
      pDatum->u.m_xAnyVal.m_xIvl.m_uSecond = iSecond;
      pDatum->u.m_xAnyVal.m_xIvl.m_uMicro  = iMilli*C_1K;
      break;
      
    case DM_TME:
      b = StrLib::bExtractTime(aazCrs, iHour, iMinute, iSecond, iMicro);
      
      if (!b)
        break;
      
      pDatum->u.m_xAnyVal.m_xTme.m_uHour   = iHour;
      pDatum->u.m_xAnyVal.m_xTme.m_uMinute = iMinute;
      pDatum->u.m_xAnyVal.m_xTme.m_uSecond = iSecond;
      pDatum->u.m_xAnyVal.m_xTme.m_uMicro  = iMicro;
      pDatum->u.m_xAnyVal.m_xTme.m_uSpare  = 0;
      
      break;
      
    case DM_TSP:
      b = StrLib::bExtractTimestamp2(aazCrs, iYear, iMonth, iDay,
                                     iHour, iMinute, iSecond, iMicro,
                                     pFmt->m_eDateComponent,
                                     pFmt->m_cDateSeparator);
      
      if (!b)
        break;
      
      pDatum->u.m_xAnyVal.m_xTsp.m_uYearDelta   = iYear - C_YEAR_BASE;
      pDatum->u.m_xAnyVal.m_xTsp.m_uMonth  = iMonth;
      pDatum->u.m_xAnyVal.m_xTsp.m_uDay    = iDay;
      pDatum->u.m_xAnyVal.m_xTsp.m_uHour   = iHour;
      pDatum->u.m_xAnyVal.m_xTsp.m_uMinute = iMinute;
      pDatum->u.m_xAnyVal.m_xTsp.m_uSecond = iSecond;
      pDatum->u.m_xAnyVal.m_xTsp.m_uMicro  = iMicro;
      pDatum->u.m_xAnyVal.m_xTsp.m_uLocal  = C_TSP_LOCAL;
      pDatum->u.m_xAnyVal.m_xTsp.m_uSpare  = 0;
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
/* extract a decimal datum from csv field value                               */
/*----------------------------------------------------------------------------*/
bool RowExtractor::bExtractBcd(Datum* pDatum, char** aazCrs, Format* pFmt)
{
  SN_Bcd snBcd;
  
  Column* pColumn = pDatum->pColumn();
  
  try {
    pDatum->u.m_xAnyVal.m_iBcd = snBcd.i(aazCrs,pColumn,pFmt);
  }
  catch (Exception xException) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a boolean datum from csv field value                               */
/*----------------------------------------------------------------------------*/
bool RowExtractor::bExtractBol(Datum* pDatum, char** aazCrs, Format* pFmt)
{
  SN_Bol snBol;
  
  Column* pColumn = pDatum->pColumn();
  
  try {
    pDatum->u.m_xAnyVal.m_iBol = snBol.i(aazCrs,pColumn,pFmt);
  }
  catch (Exception xException) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a date datum from csv field value                      yyyy-mo-ddZ */
/*----------------------------------------------------------------------------*/
bool RowExtractor::bExtractDte(Datum* pDatum, char** aazCrs, Format* pFmt)
{
  SN_Dte snDte;
  
  Column* pColumn = pDatum->pColumn();
  
  try {
    pDatum->u.m_xAnyVal.m_iDte = snDte.i(aazCrs,pColumn,pFmt);
  }
  catch (Exception xException) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a float datum from csv field value                                 */
/*----------------------------------------------------------------------------*/
bool RowExtractor::bExtractFlt(Datum* pDatum, char** aazCrs, Format* pFmt)
{
  SN_Flt snFlt;
  
  Column* pColumn = pDatum->pColumn();
  
  try {
    pDatum->u.m_xAnyVal.m_dFlt = snFlt.d(aazCrs,pColumn,pFmt);
  }
  catch (Exception xException) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract an integer datum from csv field value                              */
/*----------------------------------------------------------------------------*/
bool RowExtractor::bExtractInt(Datum* pDatum, char** aazCrs, Format* pFmt)
{
  SN_Int snInt;
  
  Column* pColumn = pDatum->pColumn();
  
  try {
    pDatum->u.m_xAnyVal.m_iInt = snInt.i(aazCrs,pColumn,pFmt);
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
bool RowExtractor::bExtractIvl(Datum* pDatum, char** aazCrs, Format* pFmt)
{
  SN_Ivl snIvl;
  
  Column* pColumn = pDatum->pColumn();
  
  try {
    pDatum->u.m_xAnyVal.m_iIvl = snIvl.i(aazCrs,pColumn,pFmt);
  }
  catch (Exception xException) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a time datum from csv field value                    hh:mi:ss.fffZ */
/*----------------------------------------------------------------------------*/
bool RowExtractor::bExtractTme(Datum* pDatum, char** aazCrs, Format* pFmt)
{
  SN_Tsp snTsp;
  
  Column* pColumn = pDatum->pColumn();
  
  try {
    pDatum->u.m_xAnyVal.m_iTsp = snTsp.i(aazCrs,pColumn,pFmt);
  }
  catch (Exception xException) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a timestamp datum from csv field value    yyyy-mo-ddThh:mi:ss.fffZ */
/*----------------------------------------------------------------------------*/
bool RowExtractor::bExtractTsp(Datum* pDatum, char** aazCrs, Format* pFmt)
{
  SN_Tsp snTsp;
  
  Column* pColumn = pDatum->pColumn();
  
  try {
    pDatum->u.m_xAnyVal.m_iTsp = snTsp.i(aazCrs,pColumn,pFmt);
  }
  catch (Exception xException) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a varchar datum from csv field value                               */
/*----------------------------------------------------------------------------*/
bool RowExtractor::bExtractVch(Datum* pDatum, char** aazCrs, Format* pFmt)
{
  SN_Vch snVch;
  bool bSawEnclosedLeading = false;
  
  Column* pColumn = pDatum->pColumn();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for enclosed
  bSawEnclosedLeading = bIsEnclosedString(aazCrs,pFmt);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // varchar field value processing
  
  try {
    
    i8 nChars = 0;
    * pDatum = snVch.sz(aazCrs,pColumn,pFmt,bSawEnclosedLeading,
                      nChars);

    //pDatum->vStzDump();

  }
  catch (Exception xException) {
    return false;
  }
  
  return true;
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
