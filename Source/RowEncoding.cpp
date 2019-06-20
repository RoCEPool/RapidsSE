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
    xEncode[i_Col].m_eMetaCode  = MC_NULL;
    xEncode[i_Col].m_wDataCode  = C_UNKNOWN;
    xEncode[i_Col].m_nDataBytes = C_UNKNOWN;
    xEncode[i_Col].m_nOffset    = 0;
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
    eDM eDatum   = pCol->eDatumType();
    
    switch (eDatum) {
      
      case DM_BCD:
        vEncodeBcd(pRow->m_waColVal[i_Col],
                   xEncode[i_Col].m_eMetaCode,
                   xEncode[i_Col].m_wDataCode,
                   xEncode[i_Col].m_nDataBytes);
        break;
        
      case DM_BOL:
        vEncodeBol(pRow->m_waColVal[i_Col],
                   xEncode[i_Col].m_eMetaCode,
                   xEncode[i_Col].m_wDataCode,
                   xEncode[i_Col].m_nDataBytes);
        break;
        
      case DM_DTE:
        vEncodeDte(pRow->m_waColVal[i_Col],
                   xEncode[i_Col].m_eMetaCode,
                   xEncode[i_Col].m_wDataCode,
                   xEncode[i_Col].m_nDataBytes);
        break;
        
      case DM_FLT:
        vEncodeFlt(pRow->m_waColVal[i_Col],
                   xEncode[i_Col].m_eMetaCode,
                   xEncode[i_Col].m_wDataCode,
                   xEncode[i_Col].m_nDataBytes);
        break;
        
      case DM_INT:
        vEncodeInt(pRow->m_waColVal[i_Col],
                   xEncode[i_Col].m_eMetaCode,
                   xEncode[i_Col].m_wDataCode,
                   xEncode[i_Col].m_nDataBytes);
        break;
        
      case DM_IVL:
        vEncodeIvl(pRow->m_waColVal[i_Col],
                   xEncode[i_Col].m_eMetaCode,
                   xEncode[i_Col].m_wDataCode,
                   xEncode[i_Col].m_nDataBytes);
        break;
        
      case DM_TME:
        vEncodeTme(pRow->m_waColVal[i_Col],
                   xEncode[i_Col].m_eMetaCode,
                   xEncode[i_Col].m_wDataCode,
                   xEncode[i_Col].m_nDataBytes);
        break;
        
      case DM_TSP:
        vEncodeTsp(pRow->m_waColVal[i_Col],
                   xEncode[i_Col].m_eMetaCode,
                   xEncode[i_Col].m_wDataCode,
                   xEncode[i_Col].m_nDataBytes);
        break;
        
      case DM_VCH:
        xEncode[i_Col].m_wDataCode = (i8)m_xWsp.ayLatter();
        
        vEncodeVch(pRow->m_waColVal[i_Col],
                   xEncode[i_Col].m_eMetaCode,
                   xEncode[i_Col].m_wDataCode,
                   xEncode[i_Col].m_nDataBytes);
        
        m_xWsp.vTake(xEncode[i_Col].m_nDataBytes + 1);
        break;
        
      default:
        NEVER_REACH;
        
    } // switch eDatum
    
    if (! i_Col)
      continue;
    
    // compute offset location
    
    xEncode[i_Col].m_nOffset =
      xEncode[i_Col - 1].m_nOffset
        + nDataBytes(xEncode[i_Col - 1].m_eMetaCode);

    if ( bIsFit(xEncode[i_Col - 1].m_eMetaCode) )
      xEncode[i_Col].m_nOffset += xEncode[i_Col - 1].m_nDataBytes;
    
  } // for i_Col

}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* encode Bcd                                                                 */
/*----------------------------------------------------------------------------*/
void RowEncoding::vEncodeBcd(Dtm wBcd,
                           eMC& eMetaCode_, Dtm& wDataCode_, i8& nDataBytes_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
  
  wDataCode_.m_iBcd = wBcd.m_iBcd;

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
/* encode Bol                                                                 */
/*----------------------------------------------------------------------------*/
void RowEncoding::vEncodeBol(Dtm wBol,
                           eMC& eMetaCode_, Dtm& wDataCode_, i8& nDataBytes_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
  
  wDataCode_.m_iBol = wBol.m_iBol;

  if (wBol.bIsNull())
    eMetaCode_ = MC_NULL;
  else if (wBol.m_iBol)
    eMetaCode_ = MC_TRUE;
  else
    eMetaCode_ = MC_FALSE;
  
  nDataBytes_ = C_0B;
}

/*----------------------------------------------------------------------------*/
/* encode Dte                                                                 */
/*----------------------------------------------------------------------------*/
void RowEncoding::vEncodeDte(Dtm wDte,
                           eMC& eMetaCode_, Dtm& wDataCode_, i8& nDataBytes_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
  
  wDataCode_.m_iDte = wDte.m_iDte;

  if (wDte.bIsNull()) {
    eMetaCode_  = MC_NULL;
    nDataBytes_ = C_0B;
    return;
  }
  
  i8 iEra = 0;
  vDteIntoEdt(wDte,wDataCode_,iEra);
  
       if (C_ERA_0 == iEra) eMetaCode_ = MC_DATE_E0_2B;
  else if (C_ERA_1 == iEra) eMetaCode_ = MC_DATE_E1_2B;
  else                      eMetaCode_ = MC_DATE_E2_2B;
  
  nDataBytes_ = C_2B;
}

/*----------------------------------------------------------------------------*/
/* encode Flt                                                                 */
/*----------------------------------------------------------------------------*/
void RowEncoding::vEncodeFlt(Dtm wFlt,
                           eMC& eMetaCode_, Dtm& wDataCode_, i8& nDataBytes_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
  
  wDataCode_.m_dFlt = wFlt.m_dFlt;

  eMetaCode_ = (eMC)IntLib::iByteMagNull(wFlt.m_iInt);
  
  nDataBytes_ = eMetaCode_;
}

/*----------------------------------------------------------------------------*/
/* encode Int                                                                 */
/*----------------------------------------------------------------------------*/
void RowEncoding::vEncodeInt(Dtm wInt,
                           eMC& eMetaCode_, Dtm& wDataCode_, i8& nDataBytes_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
  
  wDataCode_.m_iInt = wInt.m_iInt;
  
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
/* encode Ivl                                                                 */
/*----------------------------------------------------------------------------*/
void RowEncoding::vEncodeIvl(Dtm wIvl,
                           eMC& eMetaCode_, Dtm& wDataCode_, i8& nDataBytes_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
  
  wDataCode_.m_iIvl = wIvl.m_iIvl;

  eMetaCode_  = (eMC)IntLib::iByteMagNull(wIvl.m_iIvl);
  nDataBytes_ = eMetaCode_;
}

/*----------------------------------------------------------------------------*/
/* encode Tme                                                                 */
/*----------------------------------------------------------------------------*/
void RowEncoding::vEncodeTme(Dtm wTme,
                           eMC& eMetaCode_, Dtm& wDataCode_, i8& nDataBytes_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
  
  wDataCode_.m_iTme = wTme.m_iTme;
  
  eMetaCode_  = (eMC)IntLib::iByteMagNull(wTme.m_iTme);
  nDataBytes_ = eMetaCode_;
}

/*----------------------------------------------------------------------------*/
/* encode Tsp                                                                 */
/*----------------------------------------------------------------------------*/
void RowEncoding::vEncodeTsp(Dtm wTsp,
                           eMC& eMetaCode_, Dtm& wDataCode_, i8& nDataBytes_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
  
  wDataCode_.m_iTsp = wTsp.m_iTsp;
  
  eMetaCode_  = (eMC)IntLib::iByteMagNull(wTsp.m_iTsp);
  nDataBytes_ = eMetaCode_;
}

/*----------------------------------------------------------------------------*/
/* encode Vch                                                                 */
/*----------------------------------------------------------------------------*/
void RowEncoding::vEncodeVch(Dtm wVch,
                           eMC& eMetaCode_, Dtm& wDataCode_, i8& nDataBytes_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null case
  
  if (wVch.bIsNull()) {
    eMetaCode_  = MC_NULL;
    nDataBytes_ = C_0B;
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  eMetaCode_  = MC_STR_FIT_1L;
  nDataBytes_ = StrLib::nCopy((char*)wDataCode_.m_szVch,(char*)wVch.m_szVch);
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
    xEncode[i_Col].m_eMetaCode  = MC_NULL;
    xEncode[i_Col].m_wDataCode  = C_UNKNOWN;
    xEncode[i_Col].m_nDataBytes = C_UNKNOWN;
    xEncode[i_Col].m_nOffset    = 0;
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
