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
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* decode and sprint row into record                                          */
/*----------------------------------------------------------------------------*/
void Record::vAppend(ColumnVec* pColVec, Row* pRow)
{
  vReset();
  
  // get no of columns
  i8 nColumns = pColVec->nItems();
  
  // for each column
  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
    
    // get the column at index
    Column* pCol = pColVec->pGet(i_Col);
    
    // comma separate column values
    if (i_Col)
      m_sRecStr.vAppend(&m_azAppendCursor,","_Stz);
    
    // append column value
    vAppend_Alt(pCol,pRow);
    
    //vTrace("%s\n"_Stz,pRec->szRecStr());
  }
}

/*----------------------------------------------------------------------------*/
/* append ascii of datum to record                                            */
/*----------------------------------------------------------------------------*/
void Record::vAppend_Alt(Column* pCol, Datum* pDatum)
{
  if (pDatum->bIsNull()) {
    m_sRecStr.vAppend(&m_azAppendCursor,"null"_Stz);
    return;
  }

  eDM eDatum = pCol->eDatumType();
  
  switch (eDatum) {
    case DM_BCD: vAppendBcd_Alt(pCol,pDatum); break;
    case DM_BOL: vAppendBol_Alt(pCol,pDatum); break;
    case DM_DTE: vAppendDte_Alt(pCol,pDatum); break;
    case DM_FLT: vAppendFlt_Alt(pCol,pDatum); break;
    case DM_INT: vAppendInt_Alt(pCol,pDatum); break;
    case DM_IVL: vAppendIvl_Alt(pCol,pDatum); break;
    case DM_TME: vAppendTme_Alt(pCol,pDatum); break;
    case DM_TSP: vAppendTsp_Alt(pCol,pDatum); break;
    case DM_VCH: vAppendVch_Alt(pCol,pDatum); break;
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* append ascii of datum to record                                            */
/*----------------------------------------------------------------------------*/
void Record::vAppend_Alt(Column* pCol, Row* pRow)
{
  Datum oDatum;
  i8 i_Col = pCol->iColumnNo();
  
  oDatum.m_iInt = pRow->m_aiValue[i_Col];
  
  if (oDatum.bIsNull()) {
    m_sRecStr.vAppend(&m_azAppendCursor,"null"_Stz);
    return;
  }
  
  eDM eDatum = pCol->eDatumType();
 
  switch (eDatum) {
    case DM_BCD: vAppendBcd_Alt(pCol,&oDatum); break;
    case DM_BOL: vAppendBol_Alt(pCol,&oDatum); break;
    case DM_DTE: vAppendDte_Alt(pCol,&oDatum); break;
    case DM_FLT: vAppendFlt_Alt(pCol,&oDatum); break;
    case DM_INT: vAppendInt_Alt(pCol,&oDatum); break;
    case DM_IVL: vAppendIvl_Alt(pCol,&oDatum); break;
    case DM_TME: vAppendTme_Alt(pCol,&oDatum); break;
    case DM_TSP: vAppendTsp_Alt(pCol,&oDatum); break;
    case DM_VCH:
      //! TOTAL HACK
      if (oDatum.m_iInt)
        oDatum.m_sVch.vCopy((Stz*)oDatum.m_iInt);
      else
        oDatum.m_sVch.vCopy(""_Stz);
      vAppendVch_Alt(pCol,&oDatum);
      break;
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* append ascii of decimal datum to record                                    */
/*----------------------------------------------------------------------------*/
void Record::vAppendBcd(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);

  spBCD spBcd;
  
  m_sRecStr.vConcat(spBcd.sz(pDatum->m_iBcd,pCol->iColumnScale()));
}
/*----------------------------------------------------------------------------*/
//void Record::vAppendBcd(Column* pCol, Datum* pDatum)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
//
//  Bcd wBcd;
//  wBcd.vAsI8(pDatum->m_iBcd);
//
//  s32 sBcd;
//  wBcd.vSprint(sBcd.aStz(), pCol);
//
//  m_sRecStr.vConcat(sBcd.aStz());
//}
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

  //m_sRecStr.vConcat((Stz*)sAscii.aChar());
  m_sRecStr.vAppend(&m_azAppendCursor,(Stz*)sAscii.aChar());
}

/*----------------------------------------------------------------------------*/
/* append ascii of boolean datum to record                                    */
/*----------------------------------------------------------------------------*/
void Record::vAppendBol(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  spBOL spBol;
  
  m_sRecStr.vConcat(spBol.sz(pDatum->m_iBol));
}
/*----------------------------------------------------------------------------*/
//void Record::vAppendBol(Column* pCol, Datum* pDatum)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
//
//  if (pDatum->bIsNull()) {
//    m_sRecStr.vCatSprint("null"_Stz);
//    return;
//  }
//
//  m_sRecStr.vConcat(pDatum->m_iBol ? "true"_Stz : "false"_Stz);
//}
/*----------------------------------------------------------------------------*/
void Record::vAppendBol_Alt(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  spBOL spBol;
  
  m_sRecStr.vAppend(&m_azAppendCursor,spBol.sz(pDatum->m_iBol));
}
/*----------------------------------------------------------------------------*/
//void Record::vAppendBol_Alt(Column* pCol, Datum* pDatum)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
//
//  s32 sAscii;
//
//  if (pDatum->m_iBol)
//    sAscii = "true"_Stz;
//  else
//    sAscii = "false"_Stz;
//
//  //m_sRecStr.vConcat(pDatum->m_iBol ? "true"_Stz : "false"_Stz);
//  m_sRecStr.vAppend(&m_azAppendCursor,(Stz*)sAscii.aChar());
//}

/*----------------------------------------------------------------------------*/
/* append ascii of date datum to record                                       */
/*----------------------------------------------------------------------------*/
void Record::vAppendDte(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  // here we declare the Dte helper.
  // it has both the default format string in it, as well as a result buffer array
  spDTE spDte;
  
  // we call spDte.sz() with the date to be formatted.
  // it returns a Stz* to the formatted date result
  m_sRecStr.vConcat(spDte.sz(pDatum->m_iDte));
}
/*----------------------------------------------------------------------------*/
//void Record::vAppendDte(Column* pCol, Datum* pDatum)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
//
//    //Dte wDte;
//    //wDte.vAsI8(pDatum->m_iDte);
//
//    s32 sDte;
//    //wDte.vSprint(sDte.aStz());
//    StrLib::vSprint(sDte.aChar(),
//                  "%02lld-%02lld-%02lld"_Stz,
//                  (i8)pDatum->xEdt.m_uYearInEra + C_BASE_YEAR,
//                  (i8)pDatum->xEdt.m_uMonth,
//                  (i8)pDatum->xEdt.m_uDay);
//    m_sRecStr.vConcat(sDte.aStz());
//}
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

  //m_sRecStr.vConcat((Stz*)sAscii.aChar());
  m_sRecStr.vAppend(&m_azAppendCursor,(Stz*)sAscii.aChar());
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
  
  //m_sRecStr.vConcat((Stz*)sAscii.aChar());
  m_sRecStr.vAppend(&m_azAppendCursor,(Stz*)sAscii.aChar());
}

/*----------------------------------------------------------------------------*/
/* append ascii of integer datum to record                                    */
/*----------------------------------------------------------------------------*/
void Record::vAppendInt(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  spINT spInt;
  
  m_sRecStr.vConcat(spInt.sz(pDatum->m_iInt));
}
/*----------------------------------------------------------------------------*/
//void Record::vAppendInt(Column* pCol, Datum* pDatum)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
//
//  if (pDatum->bIsNull()) {
//    m_sRecStr.vCatSprint("null"_Stz);
//    return;
//  }
//
//  m_sRecStr.vCatSprint("%lld"_Stz,pDatum->m_iInt);
//}
/*----------------------------------------------------------------------------*/
void Record::vAppendInt_Alt(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  spINT spInt;
  
  m_sRecStr.vAppend(&m_azAppendCursor,spInt.sz(pDatum->m_iInt));
}
/*----------------------------------------------------------------------------*/
//void Record::vAppendInt_Alt(Column* pCol, Datum* pDatum)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
//
//  s32 sAscii;
//
//  StrLib::vSprint(sAscii.aChar(),"%lld"_Stz,pDatum->m_iInt);
//
//  //m_sRecStr.vConcat((Stz*)sAscii.aChar());
//  m_sRecStr.vAppend(&m_azAppendCursor,(Stz*)sAscii.aChar());
//}

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
  
  //m_sRecStr.vConcat((Stz*)sAscii.aChar());
  m_sRecStr.vAppend(&m_azAppendCursor,(Stz*)sAscii.aChar());
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
  
  //m_sRecStr.vConcat((Stz*)sAscii.aChar());
  m_sRecStr.vAppend(&m_azAppendCursor,(Stz*)sAscii.aChar());
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
  
  //m_sRecStr.vConcat((Stz*)sAscii.aChar());
  m_sRecStr.vAppend(&m_azAppendCursor,(Stz*)sAscii.aChar());
}

/*----------------------------------------------------------------------------*/
/* append ascii of varchar datum to record                                    */
/*----------------------------------------------------------------------------*/
void Record::vAppendVch(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  spVCH spVch;
  
  m_sRecStr.vConcat(spVch.sz(pDatum->m_sVch.aChar()));
}
/*----------------------------------------------------------------------------*/
//void Record::vAppendVch(Column* pCol, Datum* pDatum)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
//
//  if (pDatum->bIsNull()) {
//    m_sRecStr.vCatSprint("null"_Stz);
//    return;
//  }
//
//  s256 sBuf;
//
//  Vch::vSprintQuotes(sBuf,pDatum->m_sVch.aStz());
//  m_sRecStr.vConcat(sBuf.aStz());
//}
/*----------------------------------------------------------------------------*/
void Record::vAppendVch_Alt(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  spVCH spVch;
  
  m_sRecStr.vAppend(&m_azAppendCursor,spVch.sz(pDatum->m_sVch.aChar()));
}
/*----------------------------------------------------------------------------*/
//void Record::vAppendVch_Alt(Column* pCol, Datum* pDatum)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
//
//  s256 sAscii;
//
//  char* aDstChar = (char*)sAscii.aChar();
//  char* aSrcChar = (char*)pDatum->m_sVch.aChar();
//
//  // put leading enclosing quote/char
//  *aDstChar++ = '\'';
//
//  // copy from src to dst
//  while (*aSrcChar) {
//
//    if ('\'' == *aSrcChar) {
//      // escape an enclosing quote/char
//      *aDstChar++ = '\'';
//      *aDstChar++ = *aSrcChar++;
//    }
//    else {
//      // copy an ordinary char
//      *aDstChar++ = *aSrcChar++;
//    }
//  }
//
//  // put trailing enclosing quote/char and \0 terminate
//  *aDstChar++ = '\'';
//  *aDstChar = '\0';
//
//  //m_sRecStr.vConcat(sAscii.aStz());
//  m_sRecStr.vAppend(&m_azAppendCursor,(Stz*)sAscii.aChar());
//}

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
