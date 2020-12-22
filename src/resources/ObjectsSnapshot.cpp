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
ObjectSnapshot* Resources::_pObjectsInPool(void* aAddr)
{
  ObjectSnapshot* pObjectsInPool = reinterpret_cast<ObjectSnapshot*>(aAddr);
  
  RASSERT_TYPE(pObjectsInPool,CN_OBJECTSNAPSHOT);
  
  return pObjectsInPool;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* ObjectSnapshot::operator new(size_t)
{
  ObjectSnapshot* pObjectsInPool = (ObjectSnapshot*)HeapMem::pAllocate(CN_OBJECTSNAPSHOT);
  
  return pPoolCheck<void*>(pObjectsInPool);
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void ObjectSnapshot::operator delete(void* pObjectPoolAddress)
{
  ObjectSnapshot* pObjectsInPool = static_cast<ObjectSnapshot*>(pObjectPoolAddress);
  
  HeapMem::vFree(pObjectsInPool);
}

/*----------------------------------------------------------------------------*/
/* constructor (new region)                                                   */
/*----------------------------------------------------------------------------*/
ObjectSnapshot::ObjectSnapshot(void)
: Object(CN_OBJECTSNAPSHOT)
{
  for (i8 i_enum = 0; i_enum < CN_ENUMS; i_enum++)
    m_nObjects[i_enum] = 0;
  
  _checkInitialised_(CN_OBJECTSNAPSHOT);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_OBJECTSNAPSHOT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ObjectSnapshot::~ObjectSnapshot(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OBJECTSNAPSHOT,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* take snapshot                                                              */
/*----------------------------------------------------------------------------*/
void ObjectSnapshot::vTakeSnapshot(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OBJECTSNAPSHOT,_FOOTER_);
  
  PoolHelper::vObjectSnapshot(this);
}



/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* ObjectSnapshot::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OBJECTSNAPSHOT);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
