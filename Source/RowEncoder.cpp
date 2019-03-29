/*============================================================================*/
/* RowEncoder.hpp -- Row Encoder                                              */
/*============================================================================*/

#include "Data.h"

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* RowEncoder::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_ROWENCODER);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void RowEncoder::operator delete(void* pAddr)
{
  RowEncoder* pRowEncoder = reinterpret_cast<RowEncoder*>(pAddr);
  
  HeapMem::vFree(pRowEncoder);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowEncoder::RowEncoder(RowParts* pRowParts)
: Object(CN_ROWENCODER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODER,_FOOTER_);
  
  m_pRowParts = pRowParts;
  m_i_Offset = 8;
  
  _checkInitialised_(CN_ROWENCODER);
  
  m_pRowParts->vReset();
  i8 i_Head = m_pRowParts->iAllotHead();
  RASSERT(0 == i_Head);
  
  CacheLine* aCacheLine = m_pRowParts->aGet(i_Head);
  
  RowCoder::vCacheLineInit(aCacheLine);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RowEncoder::~RowEncoder(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODER,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* return the row parts tray                                                  */
/*----------------------------------------------------------------------------*/
RowParts* RowEncoder::pRowParts(void)
{
  return m_pRowParts;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* RowEncoder::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
