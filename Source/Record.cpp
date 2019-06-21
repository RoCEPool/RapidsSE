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
  m_pColumnVec     = nullptr;
  
  _checkInitialised_(CN_RECORD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Record::Record(ColumnVec* pColumnVec)
: Object(CN_RECORD)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  m_azAppendCursor = m_sRecStr.aChar();
  m_pColumnVec     = pColumnVec;
  
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
void Record::vAppend(Row* pRow)
{
  vReset();
  
  // get no of columns
  i8 nColumns = m_pColumnVec->nItems();
  
  // for each column
  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
    
    // get the column at index
    Column* pCol = m_pColumnVec->pGet(i_Col);
    
    // comma separate column values
    if (i_Col)
      m_sRecStr.vAppend(&m_azAppendCursor,","_Stz);
    
    // append column value
    vAppend(pCol,pRow);
    
    //vTrace("%s\n"_Stz,pRec->szRecStr());
  }
}

/*----------------------------------------------------------------------------*/
/* append ascii of datum to record                                            */
/*----------------------------------------------------------------------------*/
void Record::vAppend(Column* pCol, Datum* pDatum)
{
  if (pDatum->bIsNull()) {
    m_sRecStr.vAppend(&m_azAppendCursor,"null"_Stz);
    return;
  }

  eDM eDatum = pCol->eDatumType();
  
  switch (eDatum) {
    case DM_BCD: vAppendBcd(pCol,pDatum); break;
    case DM_BOL: vAppendBol(pCol,pDatum); break;
    case DM_DTE: vAppendDte(pCol,pDatum); break;
    case DM_FLT: vAppendFlt(pCol,pDatum); break;
    case DM_INT: vAppendInt(pCol,pDatum); break;
    case DM_IVL: vAppendIvl(pCol,pDatum); break;
    case DM_TME: vAppendTme(pCol,pDatum); break;
    case DM_TSP: vAppendTsp(pCol,pDatum); break;
    case DM_VCH: vAppendVch(pCol,pDatum); break;
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* append ascii of datum to record                                            */
/*----------------------------------------------------------------------------*/
void Record::vAppend(Column* pCol, Row* pRow)
{
  Datum oDatum;
  i8 i_Col = pCol->iColumnNo();
  
  oDatum.m_wDtm.m_iInt = pRow->m_waColVal[i_Col].m_iInt;
  
  if (oDatum.bIsNull()) {
    m_sRecStr.vAppend(&m_azAppendCursor,"null"_Stz);
    return;
  }
  
  eDM eDatum = pCol->eDatumType();
 
  switch (eDatum) {
    case DM_BCD: vAppendBcd(pCol,&oDatum); break;
    case DM_BOL: vAppendBol(pCol,&oDatum); break;
    case DM_DTE: vAppendDte(pCol,&oDatum); break;
    case DM_FLT: vAppendFlt(pCol,&oDatum); break;
    case DM_INT: vAppendInt(pCol,&oDatum); break;
    case DM_IVL: vAppendIvl(pCol,&oDatum); break;
    case DM_TME: vAppendTme(pCol,&oDatum); break;
    case DM_TSP: vAppendTsp(pCol,&oDatum); break;
    case DM_VCH:
      oDatum.m_sVch.vCopy((Stz*)oDatum.m_wDtm.m_iInt);
      vAppendVch(pCol,&oDatum);
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
  
  s32 sAscii;
  bool bNegative = false;

  i8 iDigits = 1;
  for (i8 i_Digit = 0; i_Digit < pCol->iColumnScale(); i_Digit++)
    iDigits *= 10;
  
  i8 iBcd_Whole    = pDatum->m_wDtm.m_iBcd / iDigits;
  i8 iBcd_Fraction = pDatum->m_wDtm.m_iBcd % iDigits;
  
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
    StrLib::vSprint(sAscii.aChar(),"%s%lld"_Stz,
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
  
  m_sRecStr.vAppend(&m_azAppendCursor,spBol.sz(pDatum->m_wDtm.m_iBol));
}

/*----------------------------------------------------------------------------*/
/* append ascii of date datum to record                                       */
/*----------------------------------------------------------------------------*/
void Record::vAppendDte(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  s32 sAscii;
  
  StrLib::vSprint(sAscii.aChar(),
                  "%02lld-%02lld-%02lld"_Stz,
                  (i8)pDatum->xDte.m_uYearDelta + C_YEAR_BASE,
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
  
  s32 sAscii;

  StrLib::vSprint(sAscii.aChar(),"%g"_Stz,pDatum->m_wDtm.m_dFlt);
  
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
  
  m_sRecStr.vAppend(&m_azAppendCursor,spInt.sz(pDatum->m_wDtm.m_iInt));
}

/*----------------------------------------------------------------------------*/
/* append ascii of interval datum to record                                   */
/*----------------------------------------------------------------------------*/
void Record::vAppendIvl(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
 
  spIVL spIvl;

  m_sRecStr.vAppend(&m_azAppendCursor,spIvl.sz(pDatum->m_wDtm.m_iIvl));
}

/*----------------------------------------------------------------------------*/
/* append ascii of time datum to record                                       */
/*----------------------------------------------------------------------------*/
void Record::vAppendTme(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  spTME spTme;

  m_sRecStr.vAppend(&m_azAppendCursor,spTme.sz(pDatum->m_wDtm.m_iTme));
}

/*----------------------------------------------------------------------------*/
/* append ascii of timestamp datum to record                                  */
/*----------------------------------------------------------------------------*/
void Record::vAppendTsp(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  spTSP spTsp;
  
  m_sRecStr.vAppend(&m_azAppendCursor,spTsp.sz(pDatum->m_wDtm.m_iTsp));
}

/*----------------------------------------------------------------------------*/
/* append ascii of varchar datum to record                                    */
/*----------------------------------------------------------------------------*/
void Record::vAppendVch(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  spVCH spVch;
  
  m_sRecStr.vAppend(&m_azAppendCursor,spVch.sz(pDatum->m_sVch.aChar()));
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
