/*============================================================================*/
/* InfoSet.cpp -- Information Set                                             */
/*============================================================================*/

#include "InfoData.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
InfoSet* InfoData::_pInfoSet(void* aAddr)
{
  InfoSet* pInfoSet = reinterpret_cast<InfoSet*>(aAddr);
  
  return pInfoSet;
}

/*----------------------------------------------------------------------------*/
/* allocate from heap                                                         */
/*----------------------------------------------------------------------------*/
void* InfoSet::operator new(size_t)
{
  InfoSet* pInfoSet =
  static_cast<InfoSet*>(HeapMem::pAllocate(CN_INFOSET));
  
  return pInfoSet;
}

/*----------------------------------------------------------------------------*/
/* free back to the heap                                                      */
/*----------------------------------------------------------------------------*/
void InfoSet::operator delete(void* aAddress)
{
  Object* pObject = static_cast<Object*>(aAddress);
  
  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
InfoSet::InfoSet(void)
: RowBuffer(CN_INFOSET)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INFOSET);
  
  m_nRowSize  = C_UNKNOWN;
  m_nInfoRows = 0;
  
  _checkInitialised_(CN_INFOSET);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_INFOSET);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
InfoSet::InfoSet(i8 nRowSize)
: RowBuffer(CN_INFOSET)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INFOSET);
  
  m_nRowSize  = nRowSize;
  m_nInfoRows = 0;
  
  _checkInitialised_(CN_INFOSET);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_INFOSET);
  
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
InfoSet::~InfoSet(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INFOSET);
}

/*----------------------------------------------------------------------------*/
/* allot a row                                                                */
/*----------------------------------------------------------------------------*/
Row* InfoSet::aAllotRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INFOSET);

  Row* pRow = nullptr;

  if (nFreeBytes() >= m_nRowSize) {
    pRow = RowBuffer::aAllotRow(m_nRowSize);
    m_nInfoRows++;
  }

  return pRow;
}

/*----------------------------------------------------------------------------*/
/* allot a row copying the rowSingle into it                                  */
/*----------------------------------------------------------------------------*/
Row* InfoSet::aAllotRow(RowSingle* pRowSingle)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INFOSET);
  
  return RowBuffer::aAllotRow(pRowSingle);
}

/*----------------------------------------------------------------------------*/
/* get first row                                                              */
/*----------------------------------------------------------------------------*/
Row* InfoSet::aFirstRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INFOSET);
  
  return RowBuffer::aFirstRow();
}

/*----------------------------------------------------------------------------*/
/* allot a row                                                                */
/*----------------------------------------------------------------------------*/
i8 InfoSet::nInfoRows(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INFOSET);
  
  return m_nInfoRows;
}

/*----------------------------------------------------------------------------*/
/* allot a row                                                                */
/*----------------------------------------------------------------------------*/
Row* InfoSet::aNextRow(Row* aPrevRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INFOSET);
  
  return RowBuffer::aNextRow(aPrevRow, m_nRowSize);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
