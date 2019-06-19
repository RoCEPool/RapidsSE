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
/* encode Bcd                                                                 */
/*----------------------------------------------------------------------------*/
void RowEncoding::vCodeBcd(Dtm wBcd, Dtm wEncBcd_, eMC& eMetaCode_, i8& nBytes_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/* encode Bol                                                                 */
/*----------------------------------------------------------------------------*/
void RowEncoding::vCodeBol(Dtm wBol, Dtm wEncBol_, eMC& eMetaCode_, i8& nBytes_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* encode Dte                                                                 */
/*----------------------------------------------------------------------------*/
void RowEncoding::vCodeDte(Dtm wDte, Dtm wEncDte_, eMC& eMetaCode_, i8& nBytes_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* encode Flt                                                                 */
/*----------------------------------------------------------------------------*/
void RowEncoding::vCodeFlt(Dtm wFlt, Dtm wEncFlt_, eMC& eMetaCode_, i8& nBytes_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* encode Int                                                                 */
/*----------------------------------------------------------------------------*/
void RowEncoding::vCodeInt(Dtm wInt, Dtm wEncInt_, eMC& eMetaCode_, i8& nBytes_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* encode Ivl                                                                 */
/*----------------------------------------------------------------------------*/
void RowEncoding::vCodeIvl(Dtm wIvl, Dtm wEncIvl_, eMC& eMetaCode_, i8& nBytes_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* encode Tme                                                                 */
/*----------------------------------------------------------------------------*/
void RowEncoding::vCodeTme(Dtm wTme, Dtm wEncTme_, eMC& eMetaCode_, i8& nBytes_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* encode Tsp                                                                 */
/*----------------------------------------------------------------------------*/
void RowEncoding::vCodeTsp(Dtm wTsp, Dtm wEncTsp_, eMC& eMetaCode_, i8& nBytes_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* encode Vch                                                                 */
/*----------------------------------------------------------------------------*/
void RowEncoding::vCodeVch(Dtm wVch, Dtm wEncVch_, eMC& eMetaCode_, i8& nBytes_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODING,_FOOTER_);
}



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
