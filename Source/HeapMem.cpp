/*============================================================================*/
/* HeapMem.cpp -- Set of Heaps in Pool                                        */
/*============================================================================*/

#include "Pool.h"
#include "StorageEngine.h"

extern AttachMap*  gpAttachMap;

extern eHT geWhichHeap[];
bool gbAddrAllocated = false;

/*----------------------------------------------------------------------------*/
/* initialise the various heaps                                               */
/*----------------------------------------------------------------------------*/
void HeapMem::vInit(void)
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
  Persistent.m_pMedium  = new Heap(CN_SMALLPERSISTENT,  C_SIZE_SMALL_OBJECT);
}

/*----------------------------------------------------------------------------*/
/* heap memory portrait                                                       */
/*----------------------------------------------------------------------------*/
void HeapMem::vMonitorPortrait(s48K& sPortrait)
{
  s48K s;
  
  s = ""_Stz; Persistent.m_pSmall->vMonitorPortrait(s);  sPortrait.vConcat(s);
  s = ""_Stz; Persistent.m_pMedium->vMonitorPortrait(s); sPortrait.vConcat(s);
  s = ""_Stz; Persistent.m_pLarge->vMonitorPortrait(s);  sPortrait.vConcat(s);
  s = ""_Stz; Persistent.m_pJumbo->vMonitorPortrait(s);  sPortrait.vConcat(s);
}

/*----------------------------------------------------------------------------*/
/* allocate a class of object from a heap                                     */
/*----------------------------------------------------------------------------*/
Object* HeapMem::pAllocate(eCN eAllocClass, bool bPersistent)
{
  HeapMem* pHeapMem = &gpAttachMap->m_xHeapMem;
  
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
      vFatal("HeapMem::pAllocate: Invalid heap allocation: class %lld %s\n"_Stz,
             eAllocClass,aCN[eAllocClass]);
      
    case HT_PERSISTENT_SMALL:
      pObject = pHeapMem->Persistent.m_pSmall->pAllocObject(eAllocClass);
      return pObject;
      
    case HT_PERSISTENT_MEDIUM:
      pObject = pHeapMem->Persistent.m_pMedium->pAllocObject(eAllocClass);
      return pObject;
      
    case HT_PERSISTENT_LARGE:
      pObject = pHeapMem->Persistent.m_pLarge->pAllocObject(eAllocClass);
      return pObject;
      
    case HT_PERSISTENT_JUMBO:
      pObject = pHeapMem->Persistent.m_pJumbo->pAllocObject(eAllocClass);
      return pObject;
      
    case HT_TRANSIENT_SMALL:
      pObject = pHeapMem->Transient.m_pSmall->pAllocObject(eAllocClass);
      return pObject;
      
    case HT_TRANSIENT_MEDIUM:
      pObject = pHeapMem->Transient.m_pMedium->pAllocObject(eAllocClass);
      return pObject;
      
    case HT_TRANSIENT_LARGE:
      pObject = pHeapMem->Transient.m_pLarge->pAllocObject(eAllocClass);
      return pObject;
      
    case HT_TRANSIENT_JUMBO:
      pObject = pHeapMem->Transient.m_pJumbo->pAllocObject(eAllocClass);
      return pObject;
      
    default:
      vFatal("HeapMem::pAllocate: Invalid heap allocation: eHeap %lld\n"_Stz,
             eHeap);
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* free an object back to heap                                                */
/*----------------------------------------------------------------------------*/
void HeapMem::vFree(Object* pObject, bool bPersistent)
{
  HeapMem* pHeapMem = &gpAttachMap->m_xHeapMem;
  
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
      vFatal("HeapMem::vFree: Invalid heap free: class %lld %s\n"_Stz,
             eAllocClass,aCN[eAllocClass]);
      
    case HT_TRANSIENT_SMALL:
      pHeapMem->Transient.m_pSmall->vFreeObject(pObject);
      return;
      
    case HT_TRANSIENT_MEDIUM:
      pHeapMem->Transient.m_pMedium->vFreeObject(pObject);
      return;
      
    case HT_TRANSIENT_LARGE:
      pHeapMem->Transient.m_pLarge->vFreeObject(pObject);
      return;
      
    case HT_TRANSIENT_JUMBO:
      pHeapMem->Transient.m_pJumbo->vFreeObject(pObject);
      return;
      
    case HT_PERSISTENT_SMALL:
      pHeapMem->Transient.m_pSmall->vFreeObject(pObject);
      return;
      
    case HT_PERSISTENT_MEDIUM:
      pHeapMem->Transient.m_pMedium->vFreeObject(pObject);
      return;
      
    case HT_PERSISTENT_LARGE:
      pHeapMem->Transient.m_pLarge->vFreeObject(pObject);
      return;
      
    case HT_PERSISTENT_JUMBO:
      pHeapMem->Transient.m_pJumbo->vFreeObject(pObject);
      return;
      
    default:
      vFatal("HeapMem::vFree: Invalid heap free: eHeap %lld\n"_Stz,
             eHeap);
  }
}

/*----------------------------------------------------------------------------*/
/* free a non-object buffer                                                   */
/*----------------------------------------------------------------------------*/
void HeapMem::vFree_NonObject(Buffer* pBuffer)
{
  pBuffer->vSetMagic();
  
  gpAttachMap->m_xHeapMem.Transient.m_pLarge->vFreeObject(pBuffer);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* heap memory report                                                         */
/*----------------------------------------------------------------------------*/
void HeapMem::vReport(void)
{
  // get magnitudes of the various heaps
  s32 sMagSmall;  Magnitudes::vByteMag(C_SIZE_SMALL_OBJECT, sMagSmall);
  s32 sMagMedium; Magnitudes::vByteMag(C_SIZE_MEDIUM_OBJECT,sMagMedium);
  s32 sMagLarge;  Magnitudes::vByteMag(C_SIZE_LARGE_OBJECT, sMagLarge);
  s32 sMagJumbo;  Magnitudes::vByteMag(C_SIZE_JUMBO_OBJECT, sMagJumbo);
  
  vTrace("  Transient  Small  (%6s) buffers %-6lld alloc %-6lld free %-6lld (sum=%lld)\n"_Stz,
         sMagSmall.aChar(),
         gpAttachMap->m_xHeapMem.Transient.m_pSmall->iBufferCount(),
         gpAttachMap->m_xHeapMem.Transient.m_pSmall->iAllocCount(),
         gpAttachMap->m_xHeapMem.Transient.m_pSmall->iFreeCount(),
         
         gpAttachMap->m_xHeapMem.Transient.m_pSmall->iAllocCount() *
         C_SIZE_SMALL_OBJECT
         +
         gpAttachMap->m_xHeapMem.Transient.m_pSmall->iFreeCount()  *
         C_SIZE_SMALL_OBJECT
         );
  
  vTrace("             Medium (%6s) buffers %-6lld alloc %-6lld free %-6lld (sum=%lld)\n"_Stz,
         sMagMedium.aChar(),
         gpAttachMap->m_xHeapMem.Transient.m_pMedium->iBufferCount(),
         gpAttachMap->m_xHeapMem.Transient.m_pMedium->iAllocCount(),
         gpAttachMap->m_xHeapMem.Transient.m_pMedium->iFreeCount(),
         
         gpAttachMap->m_xHeapMem.Transient.m_pMedium->iAllocCount() *
         C_SIZE_MEDIUM_OBJECT
         +
         gpAttachMap->m_xHeapMem.Transient.m_pMedium->iFreeCount()  *
         C_SIZE_MEDIUM_OBJECT
         );
  
  vTrace("             Large  (%6s) buffers %-6lld alloc %-6lld free %-6lld (sum=%lld)\n"_Stz,
         sMagLarge.aChar(),
         gpAttachMap->m_xHeapMem.Transient.m_pLarge->iBufferCount(),
         gpAttachMap->m_xHeapMem.Transient.m_pLarge->iAllocCount(),
         gpAttachMap->m_xHeapMem.Transient.m_pLarge->iFreeCount(),
         
         gpAttachMap->m_xHeapMem.Transient.m_pLarge->iAllocCount() *
         C_SIZE_LARGE_OBJECT
         +
         gpAttachMap->m_xHeapMem.Transient.m_pLarge->iFreeCount()  *
         C_SIZE_LARGE_OBJECT
         );
  
  vTrace("             Jumbo  (%6s) buffers %-6lld alloc %-6lld free %-6lld (sum=%lld)\n"_Stz,
         sMagJumbo.aChar(),
         gpAttachMap->m_xHeapMem.Transient.m_pJumbo->iBufferCount(),
         gpAttachMap->m_xHeapMem.Transient.m_pJumbo->iAllocCount(),
         gpAttachMap->m_xHeapMem.Transient.m_pJumbo->iFreeCount(),
         
         gpAttachMap->m_xHeapMem.Transient.m_pJumbo->iAllocCount() *
         C_SIZE_JUMBO_OBJECT
         +
         gpAttachMap->m_xHeapMem.Transient.m_pJumbo->iFreeCount()  *
         C_SIZE_JUMBO_OBJECT
         );
  
  vTrace("  Persistent Small  (%6s) buffers %-6lld alloc %-6lld free %-6lld (sum=%lld)\n"_Stz,
         sMagSmall.aChar(),
         gpAttachMap->m_xHeapMem.Persistent.m_pSmall->iBufferCount(),
         gpAttachMap->m_xHeapMem.Persistent.m_pSmall->iAllocCount(),
         gpAttachMap->m_xHeapMem.Persistent.m_pSmall->iFreeCount(),

         gpAttachMap->m_xHeapMem.Persistent.m_pSmall->iAllocCount() *
         C_SIZE_SMALL_OBJECT
         +
         gpAttachMap->m_xHeapMem.Persistent.m_pSmall->iFreeCount()  *
         C_SIZE_SMALL_OBJECT
         );
  
  vTrace("             Medium (%6s) buffers %-6lld alloc %-6lld free %-6lld (sum=%lld)\n"_Stz,
         sMagMedium.aChar(),
         gpAttachMap->m_xHeapMem.Persistent.m_pMedium->iBufferCount(),
         gpAttachMap->m_xHeapMem.Persistent.m_pMedium->iAllocCount(),
         gpAttachMap->m_xHeapMem.Persistent.m_pMedium->iFreeCount(),
         
         gpAttachMap->m_xHeapMem.Persistent.m_pMedium->iAllocCount() *
         C_SIZE_MEDIUM_OBJECT
         +
         gpAttachMap->m_xHeapMem.Persistent.m_pMedium->iFreeCount()  *
         C_SIZE_MEDIUM_OBJECT
         );
  
  vTrace("             Large  (%6s) buffers %-6lld alloc %-6lld free %-6lld (sum=%lld)\n"_Stz,
         sMagLarge.aChar(),
         gpAttachMap->m_xHeapMem.Persistent.m_pLarge->iBufferCount(),
         gpAttachMap->m_xHeapMem.Persistent.m_pLarge->iAllocCount(),
         gpAttachMap->m_xHeapMem.Persistent.m_pLarge->iFreeCount(),
         
         gpAttachMap->m_xHeapMem.Persistent.m_pLarge->iAllocCount() *
         C_SIZE_LARGE_OBJECT
         +
         gpAttachMap->m_xHeapMem.Persistent.m_pLarge->iFreeCount()  *
         C_SIZE_LARGE_OBJECT
         );
  
  vTrace("             Jumbo  (%6s) buffers %-6lld alloc %-6lld free %-6lld (sum=%lld)\n"_Stz,
         sMagJumbo.aChar(),
         gpAttachMap->m_xHeapMem.Persistent.m_pJumbo->iBufferCount(),
         gpAttachMap->m_xHeapMem.Persistent.m_pJumbo->iAllocCount(),
         gpAttachMap->m_xHeapMem.Persistent.m_pJumbo->iFreeCount(),
         
         gpAttachMap->m_xHeapMem.Persistent.m_pJumbo->iAllocCount() *
         C_SIZE_JUMBO_OBJECT
         +
         gpAttachMap->m_xHeapMem.Persistent.m_pJumbo->iFreeCount()  *
         C_SIZE_JUMBO_OBJECT
         );
}

/*----------------------------------------------------------------------------*/
/* search free list                                                           */
/*----------------------------------------------------------------------------*/
void HeapMem::vSearchFree(void)
{
  if (gbAddrAllocated) {
    for (FreeChunk* pTestChunk = gpAttachMap->m_xHeapMem.Transient.m_pMedium->m_pFreeHead;
         pTestChunk;
         pTestChunk = pTestChunk->pNextFree())
      if ((void*)pTestChunk >= (void*)0x204828fc0038 &&
          (void*)pTestChunk < (void*)0x204828fcfd30)
        int iDebug = true;
  }
  
}

/*----------------------------------------------------------------------------*/
/* validate enum tables                                                       */
/*----------------------------------------------------------------------------*/
void HeapMem::vValidateEnums(void)
{
  // note: using i8 for iClassNo instead of eCN because you cannot
  //       increment (++) an enum
  
  for (i8 i_Class = CN_NONE; i_Class < CN_ENUMS; i_Class++) {
    
    eCN eClass = (eCN)i_Class;
    
    eHT eHeap = geWhichHeap[eClass];
    i8  iSize = giClassSize[eHeap];
    
    switch(eHeap) {
      case HT_INVALID:
        break;
        
      case HT_TRANSIENT_SMALL:
        if (iSize >= C_SIZE_SMALL_OBJECT)
          vFatal("HeapMem::vValidateEnums: transient small class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
        //if (!iSize)
        //  vWarn("HeapMem::vValidateEnums: small class %lld %lls has 0 size\n"_Stz,
        //         eClassNo,aCN[eClassNo]);
        break;
        
      case HT_TRANSIENT_MEDIUM:
        if (iSize >= C_SIZE_MEDIUM_OBJECT)
          vFatal("HeapMem::vValidateEnums: transient medium class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
        //if (!iSize)
        // vWarn("HeapMem::vValidateEnums: medium class %lld %lls has 0 size\n"_Stz,
        //        eClassNo,aCN[eClassNo]);
        break;
        
      case HT_TRANSIENT_LARGE:
        if (iSize >= C_SIZE_LARGE_OBJECT)
          vFatal("HeapMem::vValidateEnums: transient large class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
        //if (!iSize)
        // vWarn("HeapMem::vValidateEnums: large class %lld %lls has 0 size\n"_Stz,
        //        eClassNo,aCN[eClassNo]);
        break;
        
      case HT_TRANSIENT_JUMBO:
        if (iSize >= C_SIZE_JUMBO_OBJECT)
          vFatal("HeapMem::vValidateEnums: transient jumbo class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
        //if (!iSize)
        //  vWarn("HeapMem::vValidateEnums: jumbo class %lld %lls has 0 size\n"_Stz,
        //        eClassNo,aCN[eClassNo]);
        break;
        
      case HT_PERSISTENT_SMALL:
        if (iSize >= C_SIZE_SMALL_OBJECT)
          vFatal("HeapMem::vValidateEnums: persistent small class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
        //if (!iSize)
        //  vWarn("HeapMem::vValidateEnums: small class %lld %lls has 0 size\n"_Stz,
        //         eClassNo,aCN[eClassNo]);
        break;
        
      case HT_PERSISTENT_MEDIUM:
        if (iSize >= C_SIZE_MEDIUM_OBJECT)
          vFatal("HeapMem::vValidateEnums: persistent medium class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
        //if (!iSize)
        // vWarn("HeapMem::vValidateEnums: medium class %lld %lls has 0 size\n"_Stz,
        //        eClassNo,aCN[eClassNo]);
        break;
        
      case HT_PERSISTENT_LARGE:
        if (iSize >= C_SIZE_LARGE_OBJECT)
          vFatal("HeapMem::vValidateEnums: persistent large class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
        //if (!iSize)
        // vWarn("HeapMem::vValidateEnums: large class %lld %lls has 0 size\n"_Stz,
        //        eClassNo,aCN[eClassNo]);
        break;
        
      case HT_PERSISTENT_JUMBO:
        if (iSize >= C_SIZE_JUMBO_OBJECT)
          vFatal("HeapMem::vValidateEnums: persistent jumbo class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
        //if (!iSize)
        //  vWarn("HeapMem::vValidateEnums: jumbo class %lld %lls has 0 size\n"_Stz,
        //        eClassNo,aCN[eClassNo]);
        break;
        
      default:
        vFatal("HeapMem::vValidateEnums: Invalid which heap: eHeap %lld\n"_Stz,
               eHeap);
    }
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
