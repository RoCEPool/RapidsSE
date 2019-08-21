/*============================================================================*/
/* FocalObjects.cpp -- Lattice of Focal Objects                               */
/*============================================================================*/

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
FocalObjects* StorageEngine::_pFocalObjects(void* aAddr)
{
  FocalObjects* pFocalObjects = reinterpret_cast<FocalObjects*>(aAddr);
  
  RASSERT_TYPE(pFocalObjects,CN_FOCALOBJECTS);
  
  return pFocalObjects;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* FocalObjects::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_FOCALOBJECTS);
  
  return pObject;
}

//*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void FocalObjects::operator delete(void* pTaskQueueAddress)
{
  FocalObjects* pTaskQueue = reinterpret_cast<FocalObjects*>(pTaskQueueAddress);
  
  HeapMem::vFree(pTaskQueue);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
FocalObjects::FocalObjects(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FOCALOBJECTS,_FOOTER_);
  
  m_okLock.vInitLock(LK_TASK_QUEUE);
  
  _checkInitialised_(CN_FOCALOBJECTS);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_FOCALOBJECTS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
FocalObjects::~FocalObjects(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FOCALOBJECTS,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* FocalObjects::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FOCALOBJECTS);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
