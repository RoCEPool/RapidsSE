/*============================================================================*/
/* RowOnce.cpp - row once                                                     */
/*============================================================================*/

#include "DBobjects.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
RowOnce* DBobjects::_pRowOnce(void* aAddr)
{
  RowOnce* pRowOnce = reinterpret_cast<RowOnce*>(aAddr);
  
  RASSERT_TYPE(pRowOnce,CN_ROWONCE);
  
  return pRowOnce;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* RowOnce::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_ROWONCE);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void RowOnce::operator delete(void* pRowOnceAddress)
{
  Object* pObject = reinterpret_cast<Object*>(pRowOnceAddress);
  
  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowOnce::RowOnce(void)
: Object(CN_ROWONCE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWONCE,_FOOTER_);
  
  m_oLock.vInitLock(LK_LIST);
  m_pSkipList = new SkipList(C_UNQIUE_KEY,bI8Compare,true);
  
  _checkInitialised_(CN_ROWONCE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWONCE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RowOnce::~RowOnce(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWONCE,_FOOTER_);
  
  delete m_pSkipList;
}

/*----------------------------------------------------------------------------*/
/* has row been seen? (complete implementation)                               */
/*----------------------------------------------------------------------------*/
bool RowOnce::bRowSeen(Id xRowId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWONCE,_FOOTER_);
  
  i8 iRowId = xRowId.iAsI8();
  
  m_oLock.vGrabWrite();
  
  if (m_pSkipList->pFind((void*)iRowId)) {
    m_oLock.vDropWrite();
    return true;
  }
  
  m_pSkipList->bInsert((void*)iRowId);
  
  m_oLock.vDropWrite();
  
  return false;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* RowOnce::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWONCE);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
