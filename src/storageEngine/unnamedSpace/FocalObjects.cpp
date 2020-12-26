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
: m_okLock(LK_FOCAL_OBJECTS,this)
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
/* process mri command                                                        */
/*----------------------------------------------------------------------------*/
void FocalObjects::vMri(TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FOCALOBJECTS);
  
  i8 nTokens = pTknVec->nItems();
  
  if (1 == nTokens) {
    vMriAll();
    return;
  }
 
  if (2 == nTokens && pTknVec->pGet(1)->bIsInteger()) {
    i8 i_Focal = pTknVec->pGet(1)->iToI8();
    vMriFocal(i_Focal);
    return;
  }

  if (3 == nTokens && pTknVec->pGet(1)->bSame("class"_Stz)
                   && pTknVec->pGet(2)->bIsName()) {
    vMriClass(pTknVec->pGet(2)->szChars());
    return;
  }
  
  vTrace("Unknown Mri\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* list all focal objects                                                     */
/*----------------------------------------------------------------------------*/
void FocalObjects::vMriAll(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FOCALOBJECTS);
  
  s4K sLogEntry;
  s4K sFocalOutline;
  
  m_okLock.vGrabWrite();
  
  i8 i_Focal = C_UNKNOWN;
  while ( (i_Focal = iNext(i_Focal)) != C_UNKNOWN) {
    Focal* pFocal = pGet(i_Focal);
    
    AM::vLogEntrySprint(sLogEntry,pFocal->m_i_Log);
    AM::vFocalOutline(sFocalOutline,pFocal);
    
    vTrace("  [%04lld] %p %16s %s :: "
           "%32s :: "
           "%s\n"_Stz,
           
           i_Focal,
           pFocal,
           aCN[pFocal->eClass()],
           pFocal->bValidMagic() ? "M" : "?",
           
           sFocalOutline.azChar(),
           
           sLogEntry.azChar()
           );
  }
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* list all focal objects of given class name                                 */
/*----------------------------------------------------------------------------*/
void FocalObjects::vMriClass(Stz* szClassNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FOCALOBJECTS);
  
  s4K sLogEntry;
  s4K sFocalOutline;

  eCN eClassNo = ClassNumbers::eFind(szClassNm);
  if (CN_NONE == eClassNo)
    return;
  
  m_okLock.vGrabWrite();
  
  i8 i_Focal = C_UNKNOWN;
  while ( (i_Focal = iNext(i_Focal)) != C_UNKNOWN) {
    Focal* pFocal = pGet(i_Focal);
    
    if (pFocal->eClass() != eClassNo)
      continue;
    
    AM::vLogEntrySprint(sLogEntry,pFocal->m_i_Log);
    AM::vFocalOutline(sFocalOutline,pFocal);

    vTrace("  [%04lld] %p %16s %s :: "
           "%32s :: "
           "%s\n"_Stz,
           
           i_Focal,
           pFocal,
           aCN[pFocal->eClass()],
           pFocal->bValidMagic() ? "M" : "?",

           sFocalOutline.azChar(),

           sLogEntry.azChar()
           );
  }
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* list given focal object                                                    */
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
           sString.azChar());
  
    i_Log = AM::iLogEntryPrev(i_Log);
  }
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(FocalObjects)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
