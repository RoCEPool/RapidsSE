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
: m_okLock(LK_FOCAL_OBJECTS)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FOCALOBJECTS,_FOOTER_);
    
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
}

/*----------------------------------------------------------------------------*/
/* remove focal object                                                        */
/*----------------------------------------------------------------------------*/
void FocalObjects::vRemove(Focal* pFocal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FOCALOBJECTS);
  
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
  
  i8 nTokens = pTknVec->nItems();
  
  if (1 == nTokens) {
    vMriAll();
    return;
  }
  
  Token* pTkn = pTknVec->pGet(1);
  i8 i_Focal = pTkn->iToI8();
  
  vMriFocal(i_Focal);
}

/*----------------------------------------------------------------------------*/
/* mri                                                                        */
/*----------------------------------------------------------------------------*/
void FocalObjects::vMriAll(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FOCALOBJECTS);
  
  s4K sString;
  
  m_okLock.vGrabWrite();
  
  i8 i_Focal = C_UNKNOWN;
  while ( (i_Focal = iNext(i_Focal)) != C_UNKNOWN) {
    Focal* pFocal = pGet(i_Focal);
    
    AM::vLogEntrySprint(sString,pFocal->m_i_Log);
    
    vTrace("  [%04lld] %p %04lld %16s :: %s\n"_Stz,
           i_Focal,
           pFocal,
           pFocal->m_i_Log,
           aCN[pFocal->eClass()],
           sString.aChar());
  }
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* mri                                                                        */
/*----------------------------------------------------------------------------*/
void FocalObjects::vMriFocal(i8 i_Focal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FOCALOBJECTS);
  
  s4K sString;
  
  m_okLock.vGrabWrite();
  
  Focal* pFocal = pGet(i_Focal);
  
  if (! pFocal) {
    m_okLock.vDropWrite();
    vTrace("Unknown focal object\n"_Stz);
    return;
  }

  i8 i_Log = pFocal->m_i_Log;
  while (C_UNKNOWN != i_Log) {
    
    AM::vLogEntrySprint(sString,i_Log);
    
    vTrace("  [%04lld] %p %04lld %16s :: %s\n"_Stz,
           i_Focal,
           pFocal,
           i_Log,
           aCN[pFocal->eClass()],
           sString.aChar());
  
    i_Log = AM::iLogEntryPrev(i_Log);
  }
  
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
