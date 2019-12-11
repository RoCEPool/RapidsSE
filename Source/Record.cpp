/*============================================================================*/
/* Record.cpp -- Record                                                       */
/*============================================================================*/

#include "Transform.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Record* Transform::_pRecord(void* aAddr)
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
  
  m_azAppendCursor = m_sRecStr.azChar();
  m_pStreamDef     = nullptr;
  m_iSubSet        = C_UNKNOWN;
  
  _checkInitialised_(CN_RECORD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Record::Record(StreamDef* pStreamDef, i8 iSubSet)
: Object(CN_RECORD)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  m_azAppendCursor = m_sRecStr.azChar();
  m_pStreamDef     = pStreamDef;
  m_iSubSet        = iSubSet;
  
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
  
  if (m_iSubSet == C_ALL) {
    
    for (i8 i_DefSet = 0; i_DefSet < m_pStreamDef->m_nDefSets; i_DefSet++) {
      
      // get no of columns in this stream def
      i8 nColumns = m_pStreamDef->m_nDefColumns[i_DefSet];
    
      // get start of stream def columns
      i8 iDefStart = m_pStreamDef->m_i_DefStart[i_DefSet];
    
      // for each column
      for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
        
        // get the column at index
        Column xCol = m_pStreamDef->m_xaColumn[i_Col + iDefStart];
        
        // comma separate column values
        if (i_Col)
          m_sRecStr.vAppend(&m_azAppendCursor,","_Stz);
        
        //! ALKA: Revert once Column not newable
        // append column value
        //vAppend(&xCol,pRow);
        vAppend(&m_pStreamDef->m_xaColumn[i_Col + iDefStart],pRow);
        //vTrace("%s\n"_Stz,pRec->szRecStr());
        
      }
    }
  }
  else {
    // get no of columns in this stream def
     i8 nColumns = m_pStreamDef->m_nDefColumns[m_iSubSet];

     // get start of stream def columns
     i8 iDefStart = m_pStreamDef->m_i_DefStart[m_iSubSet];

     // for each column
     for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
       
       // get the column at index
       Column xCol = m_pStreamDef->m_xaColumn[i_Col + iDefStart];
       
       // comma separate column values
       if (i_Col)
         m_sRecStr.vAppend(&m_azAppendCursor,","_Stz);
       
       //! ALKA: Revert once Column not newable
       // append column value
       //vAppend(&xCol,pRow);
       vAppend(&m_pStreamDef->m_xaColumn[i_Col + iDefStart],pRow);
     }
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

  eDM eDatum = pCol->eDatum();
  
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
  oDatum = pRow->m_waColVal[i_Col].m_iInt;
  
  if (oDatum.bIsNull()) {
    m_sRecStr.vAppend(&m_azAppendCursor,"null"_Stz);
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  eDM eDatum = pCol->eDatum();
 
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
      oDatum.vStz(pRow->m_waColVal[i_Col].m_szVch);
      //oDatum.vStzDump();
      
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
  
  i8 iBcd_Whole    = pDatum->iBcd() / iDigits;
  i8 iBcd_Fraction = pDatum->iBcd() % iDigits;
  
  if (iBcd_Fraction < 0 || iBcd_Whole < 0) {
    bNegative     = true;
    iBcd_Fraction = iBcd_Fraction * -1;
    iBcd_Whole    = iBcd_Whole * -1;
  }
  
  if (pCol->iColumnScale())
    StrLib::vSprint(sAscii.azChar(),"%s%lld.%.*lld"_Stz,
                    bNegative ? "-" : "",
                    iBcd_Whole,
                    pCol->iColumnScale(),
                    iBcd_Fraction);
  else
    StrLib::vSprint(sAscii.azChar(),"%s%lld"_Stz,
                    bNegative ? "-" : "",
                    iBcd_Whole);

  //m_sRecStr.vConcat((Stz*)sAscii.azChar());
  m_sRecStr.vAppend(&m_azAppendCursor,(Stz*)sAscii.azChar());
}

/*----------------------------------------------------------------------------*/
/* append ascii of boolean datum to record                                    */
/*----------------------------------------------------------------------------*/
void Record::vAppendBol(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  SP_Bol SP_Bol;
  
  m_sRecStr.vAppend(&m_azAppendCursor,SP_Bol.sz(pDatum->iBol()));
}

/*----------------------------------------------------------------------------*/
/* append ascii of date datum to record                                       */
/*----------------------------------------------------------------------------*/
void Record::vAppendDte(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  s32 sAscii;
  
  StrLib::vSprint(sAscii.azChar(),
                  "%02lld-%02lld-%02lld"_Stz,
                  (i8)pDatum->iDte_year(),
                  (i8)pDatum->iDte_month(),
                  (i8)pDatum->iDte_day());

  //m_sRecStr.vConcat((Stz*)sAscii.azChar());
  m_sRecStr.vAppend(&m_azAppendCursor,(Stz*)sAscii.azChar());
}

/*----------------------------------------------------------------------------*/
/* append ascii of float datum to record                                      */
/*----------------------------------------------------------------------------*/
void Record::vAppendFlt(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  s32 sAscii;

  StrLib::vSprint(sAscii.azChar(),"%g"_Stz,pDatum->dFlt());
  
  //m_sRecStr.vConcat((Stz*)sAscii.azChar());
  m_sRecStr.vAppend(&m_azAppendCursor,(Stz*)sAscii.azChar());
}

/*----------------------------------------------------------------------------*/
/* append ascii of integer datum to record                                    */
/*----------------------------------------------------------------------------*/
void Record::vAppendInt(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  SP_Int spInt;
  
  m_sRecStr.vAppend(&m_azAppendCursor,spInt.sz(pDatum->iInt()));
}

/*----------------------------------------------------------------------------*/
/* append ascii of interval datum to record                                   */
/*----------------------------------------------------------------------------*/
void Record::vAppendIvl(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
 
  SP_Ivl spIvl;

  m_sRecStr.vAppend(&m_azAppendCursor,spIvl.sz(pDatum->iIvl()));
}

/*----------------------------------------------------------------------------*/
/* append ascii of time datum to record                                       */
/*----------------------------------------------------------------------------*/
void Record::vAppendTme(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  SP_Tme SP_Tme;

  m_sRecStr.vAppend(&m_azAppendCursor,SP_Tme.sz(pDatum->iTme()));
}

/*----------------------------------------------------------------------------*/
/* append ascii of timestamp datum to record                                  */
/*----------------------------------------------------------------------------*/
void Record::vAppendTsp(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  SP_Tsp SP_Tsp;
  
  m_sRecStr.vAppend(&m_azAppendCursor,SP_Tsp.sz(pDatum->iTsp()));
}

/*----------------------------------------------------------------------------*/
/* append ascii of varchar datum to record                                    */
/*----------------------------------------------------------------------------*/
void Record::vAppendVch(Column* pCol, Datum* pDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  SP_Vch SP_Vch;
  
  m_sRecStr.vAppend(&m_azAppendCursor,SP_Vch.sz(pDatum->azChar()));
}

/*----------------------------------------------------------------------------*/
/* print record string as row                                                 */
/*----------------------------------------------------------------------------*/
void Record::vPrintRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD);
  
  vTrace("row: (%s)\n"_Stz,m_sRecStr.szStz());
}

/*----------------------------------------------------------------------------*/
/* print record string as row                                                 */
/*----------------------------------------------------------------------------*/
void Record::vPrintPartialRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD);
  
  vTrace("%s"_Stz,m_sRecStr.szStz());
}

/*----------------------------------------------------------------------------*/
/* get record string                                                          */
/*----------------------------------------------------------------------------*/
Stz* Record::szRecStr(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD);
  
  return m_sRecStr.szStz();
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
  
  m_azAppendCursor = m_sRecStr.azChar();
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
