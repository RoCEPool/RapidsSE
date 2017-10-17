/*============================================================================*/
/* RowOnce.cpp - row once                                                     */
/*============================================================================*/

#include "DBobjects.h"

extern Heap* gpSmallHeap;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
RowOnce* DBobjects::_pRowOnce(void* aAddr)
{
  RowOnce* pRowOnce = reinterpret_cast<RowOnce*>(aAddr);
  
  RASSERT( !pRowOnce || CN_ROWONCE == pRowOnce->eClass() );
  
  return pRowOnce;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* RowOnce::operator new(size_t iSize)
{
  RASSERT(iSize);

  void* pObject = Heap::pAllocate(CN_ROWONCE);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void RowOnce::operator delete(void* pRowOnceAddress)
{
  Object* pObject = reinterpret_cast<Object*>(pRowOnceAddress);
  
  Heap::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowOnce::RowOnce(void)
: Object(CN_ROWONCE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWONCE,_FOOTER_);
  
  m_pSkipList = new SkipList(true,bI8Compare);
  
  _checkInitialised_(CN_ROWONCE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWONCE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RowOnce::~RowOnce(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWONCE,_FOOTER_);
  
  delete m_pSkipList;
}

/*----------------------------------------------------------------------------*/
/* has row been seen?                                                         */
/*----------------------------------------------------------------------------*/
bool RowOnce::bRowSeen(Id xRowId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWONCE,_FOOTER_);

  return false;
}

/*----------------------------------------------------------------------------*/
/* has row been seen?                                                         */
/*----------------------------------------------------------------------------*/
bool RowOnce::bRowSeen2(Id xRowId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWONCE,_FOOTER_);
  
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
