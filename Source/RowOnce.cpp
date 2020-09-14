/*============================================================================*/
/* RowOnce.cpp - row once                                                     */
/*============================================================================*/

#include "Jobs.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
RowOnce* Jobs::_pRowOnce(void* aAddr)
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
, m_okLock(LK_LIST,this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWONCE,_FOOTER_);
  
  m_pSkipList = new SkipListNL(C_UNQIUE_KEY,bI8Compare,true);
  
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
bool RowOnce::bRowSeen(Rrf xRowRef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWONCE,_FOOTER_);
  
  i8 iRowId = xRowRef.iAsI8();
  
  m_okLock.vGrabWrite();
  
  if (m_pSkipList->pFind((void*)iRowId)) {
    m_okLock.vDropWrite();
    return true;
  }
  
  m_pSkipList->bInsert((void*)iRowId);
  
  m_okLock.vDropWrite();
  
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
