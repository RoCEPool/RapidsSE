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

  eMC eEncoding = (eMC)IntLib::iByteMagNull(wBcd.m_iBcd);
  
  if (MC_NULL == eEncoding) {
    eMetaCode_  = MC_NULL;
    nDataBytes_ = C_0B;
    return;
  }
  
#ifdef WITH_ENCODING_BCD_DIGIT
  if (MC_1B == eEncoding) {
    
    i8 iBcd1B = wBcd.m_iBcd;
    if (iBcd1B >= 0 && iBcd1B <= 10) {
      eMetaCode_  = (eMC)(MC_ZERO + iBcd1B);
      nDataBytes_ = C_0B;
      return;
    }
    
  }
#endif
  
  eMetaCode_  = eEncoding;
  nDataBytes_ = eEncoding;
}

/*----------------------------------------------------------------------------*/
/* code Bol                                                                   */
/*----------------------------------------------------------------------------*/
void RowEncoding::vCodeBol(Dtm wBol,
                           Dtm& wCoded_, eMC& eMetaCode_, i8& nDataBytes_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* code Dte                                                                   */
/*----------------------------------------------------------------------------*/
void RowEncoding::vCodeDte(Dtm wDte,
                           Dtm& wCoded_, eMC& eMetaCode_, i8& nDataBytes_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* code Flt                                                                   */
/*----------------------------------------------------------------------------*/
void RowEncoding::vCodeFlt(Dtm wFlt,
                           Dtm& wCoded_, eMC& eMetaCode_, i8& nDataBytes_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* code Int                                                                   */
/*----------------------------------------------------------------------------*/
void RowEncoding::vCodeInt(Dtm wInt,
                           Dtm& wCoded_, eMC& eMetaCode_, i8& nDataBytes_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
  
  wCoded_.m_iInt = wInt.m_iInt;
  
  eMC eEncoding = (eMC)IntLib::iByteMagNull(wInt.m_iInt);
  
  if (MC_NULL == eEncoding) {
    eMetaCode_  = MC_NULL;
    nDataBytes_ = C_0B;
    return;
  }
  
#ifdef WITH_ENCODING_INT_DIGIT
  
  if (-1 == wInt.m_iInt) {
    eMetaCode_  = MC_MINUS_ONE;
    nDataBytes_ = C_0B;
    return;
  }
  
  if (MC_1B == eEncoding) {
    
    i8 i1BInt = wInt.m_iInt;
    if (i1BInt >= 0 && i1BInt <= 10) {
      eMetaCode_  = (eMC)(MC_ZERO + i1BInt);
      nDataBytes_ = C_0B;
      return;
    }
    
  }
#endif
  
  eMetaCode_  = eEncoding;
  nDataBytes_ = eEncoding;
}

/*----------------------------------------------------------------------------*/
/* code Ivl                                                                   */
/*----------------------------------------------------------------------------*/
void RowEncoding::vCodeIvl(Dtm wIvl,
                           Dtm& wCoded_, eMC& eMetaCode_, i8& nDataBytes_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* code Tme                                                                   */
/*----------------------------------------------------------------------------*/
void RowEncoding::vCodeTme(Dtm wTme,
                           Dtm& wCoded_, eMC& eMetaCode_, i8& nDataBytes_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* code Tsp                                                                   */
/*----------------------------------------------------------------------------*/
void RowEncoding::vCodeTsp(Dtm wTsp,
                           Dtm& wCoded_, eMC& eMetaCode_, i8& nDataBytes_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
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
