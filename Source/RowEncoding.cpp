/*============================================================================*/
/* RowEncoding.cpp -- RowEncoding Encoding                                    */
/*============================================================================*/

#include "Transform.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
RowEncoding* Data::_pRowEncoding(void* aAddr)
{
  RowEncoding* pRow = reinterpret_cast<RowEncoding*>(aAddr);
  
  return pRow;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* RowEncoding::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_ROWENCODING);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* perform new of row buffer for given address                                */
/*----------------------------------------------------------------------------*/
void* RowEncoding::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void RowEncoding::operator delete(void* aAddr)
{
  RowEncoding* pRow = reinterpret_cast<RowEncoding*>(aAddr);
  
  HeapMem::vFree(pRow);
}


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowEncoding::RowEncoding(ColumnVec* pColVec)
: Object(CN_ROWENCODING)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
  
  m_pColVec  = pColVec;
  m_nColumns = m_pColVec->nItems();
  
  // - - - - - - - - - - - - - - - - - -

  for (i8 i_Col = 0; i_Col < C_MAX_COLUMNS; i_Col++) {
    m_waColVal[i_Col].m_iInt       = 0;
    xEncode[i_Col].m_eEncoding     = MC_NULL;
    xEncode[i_Col].m_nEncodedBytes = 0;
  }
  
#ifdef WITH_CHECK_INITIALISED
  m_xWsp.vZero(CN_ROWENCODING);
#else
  // m_xWsp
#endif
  
  _checkInitialised_(CN_ROWENCODING);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RowEncoding::~RowEncoding(void) noexcept(false)
{
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert a Dte into an Edt                                                  */
/*----------------------------------------------------------------------------*/
void RowEncoding::vDteIntoEdt(Dtm wDte, Dtm& wEdt_, i8& iEra_)
{
  i8 iYearDelta;
  i8 iYearInEra;
  i8 iMonth;
  i8 iDay;
  
  iYearDelta = wDte.xDte.m_uYearDelta;
  iMonth     = wDte.xDte.m_uMonth;
  iDay       = wDte.xDte.m_uDay;
  
  if (iYearDelta < C_ERA_1) {
    iEra_ = C_ERA_0;
    iYearInEra = iYearDelta - C_ERA_0;
  }
  else if (iYearDelta < C_ERA_2) {
    iEra_ = C_ERA_1;
    iYearInEra = iYearDelta - C_ERA_1;
  }
  else {
    iEra_ = C_ERA_2;
    iYearInEra = iYearDelta - C_ERA_2;
  }
  
  wEdt_.m_iEdt = 0;
  wEdt_.xEdt.m_uYearInEra = iYearInEra;
  wEdt_.xEdt.m_uMonth     = iMonth;
  wEdt_.xEdt.m_uDay       = iDay;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* encode row                                                                 */
/*----------------------------------------------------------------------------*/
void RowEncoding::vEncode(Row* pRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
  
  vReset();
  
  for (i8 i_Col = 0; i_Col < m_nColumns; i_Col++) {
    
    Column* pCol = m_pColVec->pGet(i_Col);
    eDM eDatum  = pCol->eDatumType();
    
    switch (eDatum) {
      
      case DM_BCD: vEncodeBcd(pRow,pCol); break;
      case DM_BOL: vEncodeBol(pRow,pCol); break;
      case DM_DTE: vEncodeDte(pRow,pCol); break;
      case DM_FLT: vEncodeFlt(pRow,pCol); break;
      case DM_INT: vEncodeInt(pRow,pCol); break;
      case DM_IVL: vEncodeIvl(pRow,pCol); break;
      case DM_TME: vEncodeTme(pRow,pCol); break;
      case DM_TSP: vEncodeTsp(pRow,pCol); break;
      case DM_VCH: vEncodeVch(pRow,pCol); break;
        
      default:
        NEVER_REACH;
    }
    
  }

}

/*----------------------------------------------------------------------------*/
/* encode Bcd                                                                 */
/*----------------------------------------------------------------------------*/
void RowEncoding::vEncodeBcd(Row* pRow, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/* encode Bol                                                                 */
/*----------------------------------------------------------------------------*/
void RowEncoding::vEncodeBol(Row* pRow, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* encode Dte                                                                 */
/*----------------------------------------------------------------------------*/
void RowEncoding::vEncodeDte(Row* pRow, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* encode Flt                                                                 */
/*----------------------------------------------------------------------------*/
void RowEncoding::vEncodeFlt(Row* pRow, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* encode Int                                                                 */
/*----------------------------------------------------------------------------*/
void RowEncoding::vEncodeInt(Row* pRow, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* encode Ivl                                                                 */
/*----------------------------------------------------------------------------*/
void RowEncoding::vEncodeIvl(Row* pRow, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* encode Tme                                                                 */
/*----------------------------------------------------------------------------*/
void RowEncoding::vEncodeTme(Row* pRow, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* encode Tsp                                                                 */
/*----------------------------------------------------------------------------*/
void RowEncoding::vEncodeTsp(Row* pRow, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* encode Vch                                                                 */
/*----------------------------------------------------------------------------*/
void RowEncoding::vEncodeVch(Row* pRow, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* code Bcd                                                                   */
/*----------------------------------------------------------------------------*/
void RowEncoding::vCodeBcd(Dtm wBcd,
                           Dtm& wCoded_, eMC& eMetaCode_, i8& nDataBytes_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
  
  wCoded_.m_iBcd = wBcd.m_iBcd;

  eMetaCode_ = (eMC)IntLib::iByteMagNull(wBcd.m_iBcd);
  
  if (MC_NULL == eMetaCode_) {
     nDataBytes_ = C_0B;
    return;
  }
  
#ifdef WITH_ENCODING_BCD_DIGIT
  if (MC_1B == eMetaCode_) {
    
    i8 iBcd1B = wBcd.m_iBcd;
    if (iBcd1B >= 0 && iBcd1B <= 10) {
      eMetaCode_  = (eMC)(MC_ZERO + iBcd1B);
      nDataBytes_ = C_0B;
      return;
    }
    
  }
#endif
  
  nDataBytes_ = eMetaCode_;
}

/*----------------------------------------------------------------------------*/
/* code Bol                                                                   */
/*----------------------------------------------------------------------------*/
void RowEncoding::vCodeBol(Dtm wBol,
                           Dtm& wCoded_, eMC& eMetaCode_, i8& nDataBytes_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
  
  wCoded_.m_iBol = wBol.m_iBol;

  if (wBol.bIsNull())
    eMetaCode_ = MC_NULL;
  else if (wBol.m_iBol)
    eMetaCode_ = MC_TRUE;
  else
    eMetaCode_ = MC_FALSE;
  
  nDataBytes_ = C_0B;
}

/*----------------------------------------------------------------------------*/
/* code Dte                                                                   */
/*----------------------------------------------------------------------------*/
void RowEncoding::vCodeDte(Dtm wDte,
                           Dtm& wCoded_, eMC& eMetaCode_, i8& nDataBytes_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
  
  wCoded_.m_iDte = wDte.m_iDte;

  if (wDte.bIsNull()) {
    eMetaCode_  = MC_NULL;
    nDataBytes_ = C_0B;
    return;
  }
  
  i8 iEra = 0;
  vDteIntoEdt(wDte,wCoded_,iEra);
  
       if (C_ERA_0 == iEra) eMetaCode_ = MC_DATE_E0_2B;
  else if (C_ERA_1 == iEra) eMetaCode_ = MC_DATE_E1_2B;
  else                      eMetaCode_ = MC_DATE_E2_2B;
  
  nDataBytes_ = C_2B;
}

/*----------------------------------------------------------------------------*/
/* code Flt                                                                   */
/*----------------------------------------------------------------------------*/
void RowEncoding::vCodeFlt(Dtm wFlt,
                           Dtm& wCoded_, eMC& eMetaCode_, i8& nDataBytes_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
  
  wCoded_.m_dFlt = wFlt.m_dFlt;

  eMetaCode_ = (eMC)IntLib::iByteMagNull(wFlt.m_iInt);
  
  nDataBytes_ = eMetaCode_;
}

/*----------------------------------------------------------------------------*/
/* code Int                                                                   */
/*----------------------------------------------------------------------------*/
void RowEncoding::vCodeInt(Dtm wInt,
                           Dtm& wCoded_, eMC& eMetaCode_, i8& nDataBytes_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
  
  wCoded_.m_iInt = wInt.m_iInt;
  
  eMetaCode_ = (eMC)IntLib::iByteMagNull(wInt.m_iInt);
  
  if (MC_NULL == eMetaCode_) {
    nDataBytes_ = C_0B;
    return;
  }
  
#ifdef WITH_ENCODING_INT_DIGIT
  
  if (-1 == wInt.m_iInt) {
    eMetaCode_  = MC_MINUS_ONE;
    nDataBytes_ = C_0B;
    return;
  }
  
  if (MC_1B == eMetaCode_) {
    
    i8 i1BInt = wInt.m_iInt;
    if (i1BInt >= 0 && i1BInt <= 10) {
      eMetaCode_  = (eMC)(MC_ZERO + i1BInt);
      nDataBytes_ = C_0B;
      return;
    }
    
  }
#endif
  
  nDataBytes_ = eMetaCode_;
}

/*----------------------------------------------------------------------------*/
/* code Ivl                                                                   */
/*----------------------------------------------------------------------------*/
void RowEncoding::vCodeIvl(Dtm wIvl,
                           Dtm& wCoded_, eMC& eMetaCode_, i8& nDataBytes_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
  
  wCoded_.m_iIvl = wIvl.m_iIvl;

  eMetaCode_  = (eMC)IntLib::iByteMagNull(wIvl.m_iIvl);
  nDataBytes_ = eMetaCode_;
}

/*----------------------------------------------------------------------------*/
/* code Tme                                                                   */
/*----------------------------------------------------------------------------*/
void RowEncoding::vCodeTme(Dtm wTme,
                           Dtm& wCoded_, eMC& eMetaCode_, i8& nDataBytes_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
  
  wCoded_.m_iTme = wTme.m_iTme;
  
  eMetaCode_  = (eMC)IntLib::iByteMagNull(wTme.m_iTme);
  nDataBytes_ = eMetaCode_;
}

/*----------------------------------------------------------------------------*/
/* code Tsp                                                                   */
/*----------------------------------------------------------------------------*/
void RowEncoding::vCodeTsp(Dtm wTsp,
                           Dtm& wCoded_, eMC& eMetaCode_, i8& nDataBytes_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
  
  wCoded_.m_iTsp = wTsp.m_iTsp;
  
  eMetaCode_  = (eMC)IntLib::iByteMagNull(wTsp.m_iTsp);
  nDataBytes_ = eMetaCode_;
}

/*----------------------------------------------------------------------------*/
/* code Vch                                                                   */
/*----------------------------------------------------------------------------*/
void RowEncoding::vCodeVch(Dtm wVch,
                           Dtm& wCoded_, eMC& eMetaCode_, i8& nDataBytes_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* reset all encodings                                                        */
/*----------------------------------------------------------------------------*/
void RowEncoding::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
  
  for (i8 i_Col = 0; i_Col < C_MAX_COLUMNS; i_Col++) {
    m_waColVal[i_Col].m_iInt       = 0;
    xEncode[i_Col].m_eEncoding     = MC_NULL;
    xEncode[i_Col].m_nEncodedBytes = 0;
  }
  
#ifdef WITH_CHECK_INITIALISED
  m_xWsp.vZero(CN_ROWENCODING);
#else
  m_xWsp.vInit();
#endif
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* RowEncoding::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
