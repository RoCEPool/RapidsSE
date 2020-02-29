/*============================================================================*/
/* HeapMem.cpp -- Set of Heaps in Pool                                        */
/*============================================================================*/

#include "Pool.h"


extern i8  giClassSize[];
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
  
  PoolBuffer* pJumbo = nullptr;
  while ( (pJumbo = gpBufferPool->pNextJumbo(pJumbo)) ) {
    
    pJumbo->vMagic();
    pJumbo->vClass(CN_FREECHUNK);
    
    Transient.m_pJumbo->vAddFree((FreeChunk*)pJumbo);
  }
  
  // create the persistent heaps
  Persistent.m_pJumbo  = new Heap(CN_JUMBOPERSISTENT,  C_SIZE_JUMBO_OBJECT);
  Persistent.m_pLarge  = new Heap(CN_LARGEPERSISTENT,  C_SIZE_LARGE_OBJECT);
  Persistent.m_pMedium = new Heap(CN_MEDIUMPERSISTENT, C_SIZE_MEDIUM_OBJECT);
  Persistent.m_pSmall  = new Heap(CN_SMALLPERSISTENT,  C_SIZE_SMALL_OBJECT);
}

/*----------------------------------------------------------------------------*/
/* heap memory portrait                                                       */
/*----------------------------------------------------------------------------*/
void HeapMem::vMonitorPortrait(s48K& sPortrait)
{
  s48K s;
  
  s = ""_Stz; Transient.m_pSmall->vMonitorPortrait(s);  sPortrait.vConcat(s);
  s = ""_Stz; Transient.m_pMedium->vMonitorPortrait(s); sPortrait.vConcat(s);
  s = ""_Stz; Transient.m_pLarge->vMonitorPortrait(s);  sPortrait.vConcat(s);
  s = ""_Stz; Transient.m_pJumbo->vMonitorPortrait(s);  sPortrait.vConcat(s);
}

/*----------------------------------------------------------------------------*/
/* allocate a class of object from a heap                                     */
/*----------------------------------------------------------------------------*/
Object* HeapMem::pAllocate(eCN eAllocClass, bool bPersistent)
{
  HeapMem* pHeapMem = AM::pHeapMem();
  
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
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
             "HeapMem::pAllocate: Invalid heap allocation: class %lld %s\n"_Stz,
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
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
             "HeapMem::pAllocate: Invalid heap allocation: eHeap %lld\n"_Stz,
             eHeap);
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* free an object back to heap                                                */
/*----------------------------------------------------------------------------*/
void HeapMem::vFree(Object* pObject, bool bPersistent)
{
  // should not be freeing objects that are part of a word body
  ASSERT(!pObject->bWordBody());
  
  HeapMem* pHeapMem = AM::pHeapMem();
  
  eCN eClassBeingFreed = pObject->eClass();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // dont-free capability
  
  if (gpBufferPool->bDontFree()) {
    
    if (CN_ENTBUFFER != eClassBeingFreed) {
      pObject->vClass(CN_FREEDNOT);
      return;
    }
    
    if ( ! gpBufferPool->bFreeCB() ) {
      pObject->vClass(CN_FREEDNOT);
      return;
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // actually free the object and return its space for reuse
  
  eHT eHeap = geWhichHeap[eClassBeingFreed];
  
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
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
             "HeapMem::vFree: Invalid heap free: class %lld %s\n"_Stz,
             eClassBeingFreed,aCN[eClassBeingFreed]);
      
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
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
             "HeapMem::vFree: Invalid heap free: eHeap %lld\n"_Stz,
             eHeap);
  }
}

/*----------------------------------------------------------------------------*/
/* free a non-object buffer                                                   */
/*----------------------------------------------------------------------------*/
void HeapMem::vFree_NonObject(PoolBuffer* pBuffer)
{
  pBuffer->vMagic();
  
  AM::pHeapMem()->Transient.m_pLarge->vFreeObject(pBuffer);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* heap memory report                                                         */
/*----------------------------------------------------------------------------*/
void HeapMem::vReport(void)
{
  s256 sReport;

  // small transient heap
  AM::pHeapMem()->Transient.m_pSmall->vReport(sReport.pString());
  vTrace(" %s\n"_Stz,sReport.szStz());
  
  // medium transient heap
  AM::pHeapMem()->Transient.m_pMedium->vReport(sReport.pString());
  vTrace(" %s\n"_Stz,sReport.szStz());

  // large transient heap
  AM::pHeapMem()->Transient.m_pLarge->vReport(sReport.pString());
  vTrace(" %s\n"_Stz,sReport.szStz());

  // jumbo transient heap
  AM::pHeapMem()->Transient.m_pJumbo->vReport(sReport.pString());
  vTrace(" %s\n"_Stz,sReport.szStz());

  
  // small persistent heap
//  AM::pHeapMem()->Persistent.m_pSmall->vReport(sReport.pString());
//  vTrace(" %s\n"_Stz,sReport.szStz());
//
//  // medium persistent heap
//  AM::pHeapMem()->Persistent.m_pMedium->vReport(sReport.pString());
//  vTrace(" %s\n"_Stz,sReport.szStz());
//
//  // large persistent heap
//  AM::pHeapMem()->Persistent.m_pLarge->vReport(sReport.pString());
//  vTrace(" %s\n"_Stz,sReport.szStz());
//
//  // large jumbo heap
//  AM::pHeapMem()->Persistent.m_pJumbo->vReport(sReport.pString());
//  vTrace(" %s\n"_Stz,sReport.szStz());
}

/*----------------------------------------------------------------------------*/
/* search free list                                                           */
/*----------------------------------------------------------------------------*/
void HeapMem::vSearchFree(void)
{
  if (gbAddrAllocated) {
    for (FreeChunk* pTestChunk = AM::pHeapMem()->Transient.m_pMedium->m_pFreeHead;
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
  // note: using i8 for i_Class instead of eCN because you cannot
  //       increment (++) an enum
  
  for (i8 i_Class = CN_NONE; i_Class < CN_ENUMS; i_Class++) {
    
    eCN eClass = (eCN)i_Class;
    
    eHT eHeap = geWhichHeap[eClass];
    i8  iSize = giClassSize[eClass];
    
    switch(eHeap) {
      case HT_INVALID:
        break;
        
      case HT_TRANSIENT_SMALL:
        if (iSize >= C_SIZE_SMALL_OBJECT)
          vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
                 "HeapMem::vValidateEnums: transient small class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
        break;
        
      case HT_TRANSIENT_MEDIUM:
        if (iSize >= C_SIZE_MEDIUM_OBJECT)
          vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
                 "HeapMem::vValidateEnums: transient medium class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
        break;
        
      case HT_TRANSIENT_LARGE:
        if (iSize >= C_SIZE_LARGE_OBJECT)
          vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
                 "HeapMem::vValidateEnums: transient large class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
        break;
        
      case HT_TRANSIENT_JUMBO:
        if (iSize >= C_SIZE_JUMBO_OBJECT)
          vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
                 "HeapMem::vValidateEnums: transient jumbo class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
        break;
        
      case HT_PERSISTENT_SMALL:
        if (iSize >= C_SIZE_SMALL_OBJECT)
          vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
                 "HeapMem::vValidateEnums: persistent small class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
        break;
        
      case HT_PERSISTENT_MEDIUM:
        if (iSize >= C_SIZE_MEDIUM_OBJECT)
          vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
                 "HeapMem::vValidateEnums: persistent medium class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
        break;
        
      case HT_PERSISTENT_LARGE:
        if (iSize >= C_SIZE_LARGE_OBJECT)
          vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
                 "HeapMem::vValidateEnums: persistent large class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
        break;
        
      case HT_PERSISTENT_JUMBO:
        if (iSize >= C_SIZE_JUMBO_OBJECT)
          vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
                 "HeapMem::vValidateEnums: persistent jumbo class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
        break;
        
      default:
        vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
               "HeapMem::vValidateEnums: Invalid which heap: eHeap %lld\n"_Stz,
               eHeap);
    }
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
