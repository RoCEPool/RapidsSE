/*============================================================================*/
/* Heaps.cpp -- Set of Heaps in Pool                                          */
/*============================================================================*/

#include "Pool.h"
#include "StorageEngine.h"

extern AttachMap*  gpAttachMap;

extern eHT geWhichHeap[];
bool gbAddrAllocated = false;
/*----------------------------------------------------------------------------*/
/* initialise the various heaps                                               */
/*----------------------------------------------------------------------------*/
void Heaps::vInit(void)
{
  // create the transient heaps
  Transient.m_pJumbo  = new Heap(CN_JUMBOTRANSIENT,  C_SIZE_JUMBO_OBJECT);
  Transient.m_pLarge  = new Heap(CN_LARGETRANSIENT,  C_SIZE_LARGE_OBJECT);
  Transient.m_pMedium = new Heap(CN_MEDIUMTRANSIENT, C_SIZE_MEDIUM_OBJECT);
  Transient.m_pSmall  = new Heap(CN_SMALLTRANSIENT,  C_SIZE_SMALL_OBJECT);
  
  // set up the jumbo heap
  Transient.m_pJumbo->vFixedHeap(true);
  
  Buffer* pJumbo = nullptr;
  while ( (pJumbo = gpBufferPool->pNextJumbo(pJumbo)) ) {
    
    pJumbo->vSetMagic();
    pJumbo->vSetClass(CN_FREECHUNK);
    
    Transient.m_pJumbo->vAddFree((FreeChunk*)pJumbo);
  }
  
  // create the persistent heaps
  Persistent.m_pJumbo  = new Heap(CN_JUMBOPERSISTENT,  C_SIZE_JUMBO_OBJECT);
  Persistent.m_pLarge  = new Heap(CN_LARGEPERSISTENT,  C_SIZE_LARGE_OBJECT);
  Persistent.m_pMedium = new Heap(CN_MEDIUMPERSISTENT, C_SIZE_MEDIUM_OBJECT);
  Persistent.m_pSmall  = new Heap(CN_SMALLPERSISTENT,  C_SIZE_SMALL_OBJECT);
}


/*----------------------------------------------------------------------------*/
/* allocate an object from a heap                                             */
/*----------------------------------------------------------------------------*/
Object* Heaps::pAllocate(eCN eAllocClass, bool bPersistent)
{
  eHT eHeap = geWhichHeap[eAllocClass];
  
  Object* pObject;
  
  if (bPersistent) {
    switch(eHeap) {
      case HT_TRANSIENT_SMALL:  eHeap = HT_PERSISTENT_SMALL;  break;
      case HT_TRANSIENT_MEDIUM: eHeap = HT_PERSISTENT_MEDIUM; break;
      case HT_TRANSIENT_LARGE:  eHeap = HT_PERSISTENT_LARGE;  break;
      case HT_TRANSIENT_JUMBO:  eHeap = HT_PERSISTENT_JUMBO;  break;
      default: break;
    }
  }
  
  switch(eHeap) {
    case HT_INVALID:
      vFatal("Heap::pAllocate: Invalid heap allocation: class %lld %s\n"_Stz,
             eAllocClass,aCN[eAllocClass]);
      
    case HT_PERSISTENT_SMALL:
      pObject = gpAttachMap->m_xHeaps.Persistent.m_pSmall->pAllocObject(eAllocClass);
      return pObject;
      
    case HT_PERSISTENT_MEDIUM:
      pObject = gpAttachMap->m_xHeaps.Persistent.m_pMedium->pAllocObject(eAllocClass);
      return pObject;
      
    case HT_PERSISTENT_LARGE:
      pObject = gpAttachMap->m_xHeaps.Persistent.m_pLarge->pAllocObject(eAllocClass);
      return pObject;
      
    case HT_PERSISTENT_JUMBO:
      pObject = gpAttachMap->m_xHeaps.Persistent.m_pJumbo->pAllocObject(eAllocClass);
      return pObject;
      
    case HT_TRANSIENT_SMALL:
      pObject = gpAttachMap->m_xHeaps.Transient.m_pSmall->pAllocObject(eAllocClass);
      return pObject;
      
    case HT_TRANSIENT_MEDIUM:
      pObject = gpAttachMap->m_xHeaps.Transient.m_pMedium->pAllocObject(eAllocClass);
      return pObject;
      
    case HT_TRANSIENT_LARGE:
      pObject = gpAttachMap->m_xHeaps.Transient.m_pLarge->pAllocObject(eAllocClass);
      return pObject;
      
    case HT_TRANSIENT_JUMBO:
      pObject = gpAttachMap->m_xHeaps.Transient.m_pJumbo->pAllocObject(eAllocClass);
      return pObject;
      
    default:
      vFatal("Heap::pAllocate: Invalid heap allocation: eHeap %lld\n"_Stz,
             eHeap);
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* free an object back to heap                                                */
/*----------------------------------------------------------------------------*/
void Heaps::vFree(Object* pObject, bool bPersistent)
{
  eCN eAllocClass = pObject->eClass();
  
  if (gpBufferPool->bDontFree()) {
    
    if (CN_ENTRYBUFFER != eAllocClass) {
      pObject->vSetClass(CN_FREEDNOT);
      return;
    }
    
    if ( ! gpBufferPool->bFreeRB() ) {
      pObject->vSetClass(CN_FREEDNOT);
      return;
    }
  }
  
  // should not be freeing objects that are part of a word body
  ASSERT(!pObject->bWordBody());
  
  eHT eHeap = geWhichHeap[eAllocClass];
  
  if (bPersistent) {
    switch(eHeap) {
      case HT_TRANSIENT_SMALL:  eHeap = HT_PERSISTENT_SMALL;  break;
      case HT_TRANSIENT_MEDIUM: eHeap = HT_PERSISTENT_MEDIUM; break;
      case HT_TRANSIENT_LARGE:  eHeap = HT_PERSISTENT_LARGE;  break;
      case HT_TRANSIENT_JUMBO:  eHeap = HT_PERSISTENT_JUMBO;  break;
      default: break;
    }
  }
  
  switch(eHeap) {
    case HT_INVALID:
      MemLib::vWordDump(pObject,256,0);
      vFatal("Heap::vFree: Invalid heap free: class %lld %s\n"_Stz,
             eAllocClass,aCN[eAllocClass]);
      
    case HT_TRANSIENT_SMALL:
      gpAttachMap->m_xHeaps.Transient.m_pSmall->vFreeObject(pObject);
      return;
      
    case HT_TRANSIENT_MEDIUM:
      gpAttachMap->m_xHeaps.Transient.m_pMedium->vFreeObject(pObject);
      return;
      
    case HT_TRANSIENT_LARGE:
      gpAttachMap->m_xHeaps.Transient.m_pLarge->vFreeObject(pObject);
      return;
      
    case HT_TRANSIENT_JUMBO:
      gpAttachMap->m_xHeaps.Transient.m_pJumbo->vFreeObject(pObject);
      return;
      
    case HT_PERSISTENT_SMALL:
      gpAttachMap->m_xHeaps.Transient.m_pSmall->vFreeObject(pObject);
      return;
      
    case HT_PERSISTENT_MEDIUM:
      gpAttachMap->m_xHeaps.Transient.m_pMedium->vFreeObject(pObject);
      return;
      
    case HT_PERSISTENT_LARGE:
      gpAttachMap->m_xHeaps.Transient.m_pLarge->vFreeObject(pObject);
      return;
      
    case HT_PERSISTENT_JUMBO:
      gpAttachMap->m_xHeaps.Transient.m_pJumbo->vFreeObject(pObject);
      return;
      
    default:
      vFatal("Heap::vFree: Invalid heap free: eHeap %lld\n"_Stz,
             eHeap);
      
  }
}

/*----------------------------------------------------------------------------*/
/* free a non-object buffer                                                   */
/*----------------------------------------------------------------------------*/
void Heaps::vFree_NonObject(Buffer* pBuffer)
{
  pBuffer->vSetMagic();
  
  gpAttachMap->m_xHeaps.Transient.m_pLarge->vFreeObject(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* search free list                                                           */
/*----------------------------------------------------------------------------*/
void Heaps::vSearchFree(void)
{
  if (gbAddrAllocated) {
    for (FreeChunk* pTestChunk = gpAttachMap->m_xHeaps.Transient.m_pMedium->m_pFreeHead;
         pTestChunk;
         pTestChunk = pTestChunk->pNextFree())
      if ((void*)pTestChunk >= (void*)0x204828fc0038 &&
          (void*)pTestChunk < (void*)0x204828fcfd30)
        int iDebug = true;
  }
  
}

/*----------------------------------------------------------------------------*/
/* validate tables                                                            */
/*----------------------------------------------------------------------------*/
void Heaps::vValidateTables(void)
{
  // note: using i8 for iClassNo instead of eCN because you cannot
  //       increment (++) an enum
  
  for (i8 iClassNo = CN_NONE; iClassNo < CN_ENUMS; iClassNo++) {
    
    eCN eClassNo = (eCN)iClassNo;
    
    eHT eHeap = geWhichHeap[eClassNo];
    i8  iSize = giClassSize[eHeap];
    
    switch(eHeap) {
      case HT_INVALID:
        break;
        
      case HT_TRANSIENT_SMALL:
        if (iSize >= C_SIZE_SMALL_OBJECT)
          vFatal("Heap::vValidateTables: transient small class %lld %s is too big\n"_Stz,
                 eClassNo,aCN[eClassNo]);
        //if (!iSize)
        //  vWarn("Heap::vValidateTables: small class %lld %lls has 0 size\n"_Stz,
        //         eClassNo,aCN[eClassNo]);
        break;
        
      case HT_TRANSIENT_MEDIUM:
        if (iSize >= C_SIZE_MEDIUM_OBJECT)
          vFatal("Heap::vValidateTables: transient medium class %lld %s is too big\n"_Stz,
                 eClassNo,aCN[eClassNo]);
        //if (!iSize)
        // vWarn("Heap::vValidateTables: medium class %lld %lls has 0 size\n"_Stz,
        //        eClassNo,aCN[eClassNo]);
        break;
        
      case HT_TRANSIENT_LARGE:
        if (iSize >= C_SIZE_LARGE_OBJECT)
          vFatal("Heap::vValidateTables: transient large class %lld %s is too big\n"_Stz,
                 eClassNo,aCN[eClassNo]);
        //if (!iSize)
        // vWarn("Heap::vValidateTables: large class %lld %lls has 0 size\n"_Stz,
        //        eClassNo,aCN[eClassNo]);
        break;
        
      case HT_TRANSIENT_JUMBO:
        if (iSize >= C_SIZE_JUMBO_OBJECT)
          vFatal("Heap::vValidateTables: transient jumbo class %lld %s is too big\n"_Stz,
                 eClassNo,aCN[eClassNo]);
        //if (!iSize)
        //  vWarn("Heap::vValidateTables: jumbo class %lld %lls has 0 size\n"_Stz,
        //        eClassNo,aCN[eClassNo]);
        break;
        
      case HT_PERSISTENT_SMALL:
        if (iSize >= C_SIZE_SMALL_OBJECT)
          vFatal("Heap::vValidateTables: persistent small class %lld %s is too big\n"_Stz,
                 eClassNo,aCN[eClassNo]);
        //if (!iSize)
        //  vWarn("Heap::vValidateTables: small class %lld %lls has 0 size\n"_Stz,
        //         eClassNo,aCN[eClassNo]);
        break;
        
      case HT_PERSISTENT_MEDIUM:
        if (iSize >= C_SIZE_MEDIUM_OBJECT)
          vFatal("Heap::vValidateTables: persistent medium class %lld %s is too big\n"_Stz,
                 eClassNo,aCN[eClassNo]);
        //if (!iSize)
        // vWarn("Heap::vValidateTables: medium class %lld %lls has 0 size\n"_Stz,
        //        eClassNo,aCN[eClassNo]);
        break;
        
      case HT_PERSISTENT_LARGE:
        if (iSize >= C_SIZE_LARGE_OBJECT)
          vFatal("Heap::vValidateTables: persistent large class %lld %s is too big\n"_Stz,
                 eClassNo,aCN[eClassNo]);
        //if (!iSize)
        // vWarn("Heap::vValidateTables: large class %lld %lls has 0 size\n"_Stz,
        //        eClassNo,aCN[eClassNo]);
        break;
        
      case HT_PERSISTENT_JUMBO:
        if (iSize >= C_SIZE_JUMBO_OBJECT)
          vFatal("Heap::vValidateTables: persistent jumbo class %lld %s is too big\n"_Stz,
                 eClassNo,aCN[eClassNo]);
        //if (!iSize)
        //  vWarn("Heap::vValidateTables: jumbo class %lld %lls has 0 size\n"_Stz,
        //        eClassNo,aCN[eClassNo]);
        break;
        
      default:
        vFatal("Heap::vValidateTables: Invalid which heap: eHeap %lld\n"_Stz,
               eHeap);
    }
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
