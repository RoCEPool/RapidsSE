/*============================================================================*/
/* ObjectPool.cpp - object pool                                               */
/*============================================================================*/

#include "Resources.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
PoolSnapshot* Resources::_pObjectsInPool(void* aAddr)
{
  PoolSnapshot* pObjectsInPool = reinterpret_cast<PoolSnapshot*>(aAddr);
  
  RASSERT_TYPE(pObjectsInPool,CN_POOLSNAPSHOT);
  
  return pObjectsInPool;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* PoolSnapshot::operator new(size_t)
{
  PoolSnapshot* pObjectsInPool = (PoolSnapshot*)HeapMem::pAllocate(CN_POOLSNAPSHOT);
  
  return pPoolCheck<void*>(pObjectsInPool);
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void PoolSnapshot::operator delete(void* pObjectPoolAddress)
{
  PoolSnapshot* pObjectsInPool = static_cast<PoolSnapshot*>(pObjectPoolAddress);
  
  HeapMem::vFree(pObjectsInPool);
}

/*----------------------------------------------------------------------------*/
/* constructor (new region)                                                   */
/*----------------------------------------------------------------------------*/
PoolSnapshot::PoolSnapshot(void)
: Object(CN_POOLSNAPSHOT)
{
  for (i8 i_enum = 0; i_enum < CN_ENUMS; i_enum++)
    m_nObjects[i_enum] = 0;
  
  _checkInitialised_(CN_POOLSNAPSHOT);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_POOLSNAPSHOT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PoolSnapshot::~PoolSnapshot(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POOLSNAPSHOT,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* take snapshot                                                              */
/*----------------------------------------------------------------------------*/
void PoolSnapshot::vTakeSnapshot(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POOLSNAPSHOT,_FOOTER_);
  
  PoolHelper::vObjectSnapshot(this);
}



/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* PoolSnapshot::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POOLSNAPSHOT);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
