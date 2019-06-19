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
/* append column value to row                                                 */
/*----------------------------------------------------------------------------*/
void RowEncoding::vEncode(Row* pRow)
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
