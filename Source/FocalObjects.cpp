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
  
  m_okLock.vInitLock(LK_FOCAL_OBJECTS);
  
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
/* add focal object                                                           */
/*----------------------------------------------------------------------------*/
void FocalObjects::vAdd(Focal* pFocal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FOCALOBJECTS);
  
  m_okLock.vGrabWrite();
  pFocal->m_i_Focal = iAdd(pFocal);
  m_okLock.vDropWrite();

  vTrace("vFocalAdd: %lld %p\n"_Stz,pFocal->m_i_Focal,pFocal);
}

/*----------------------------------------------------------------------------*/
/* remove focal object                                                        */
/*----------------------------------------------------------------------------*/
void FocalObjects::vRemove(Focal* pFocal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FOCALOBJECTS);
  
  vTrace("vFocalRemove: %lld %p\n"_Stz,pFocal->m_i_Focal,pFocal);
  
  m_okLock.vGrabWrite();
  ASSERT(pGet(pFocal->m_i_Focal) == pFocal);
  vRemoveItem(pFocal->m_i_Focal);
  pFocal->m_i_Focal = C_UNKNOWN;
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* mri                                                                        */
/*----------------------------------------------------------------------------*/
void FocalObjects::vMri(TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FOCALOBJECTS);
  
  i8 iNumTokens = pTknVec->nItems();

  m_okLock.vGrabWrite();

  m_okLock.vDropWrite();
}

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
