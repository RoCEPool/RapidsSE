/*============================================================================*/
/* HeapMem.cpp -- Set of Heaps in Pool                                        */
/*============================================================================*/

#include "Pool.h"

extern SizeHeap  gxaSizeHeap[];

bool gbAddrAllocated = false;

/*----------------------------------------------------------------------------*/
/* initialise the various heaps                                               */
/*----------------------------------------------------------------------------*/
void HeapMem::vInit(void)
{
  // create the transient heaps
  Transient.m_pLarge  = new Heap(CN_LARGETRANSIENT,  C_SIZE_LARGE_OBJECT);
  Transient.m_pMedium = new Heap(CN_MEDIUMTRANSIENT, C_SIZE_MEDIUM_OBJECT);
  Transient.m_pSmall  = new Heap(CN_SMALLTRANSIENT,  C_SIZE_SMALL_OBJECT);
  Transient.m_pTiny   = new Heap(CN_TINYTRANSIENT,   C_SIZE_TINY_OBJECT);

  if (AM::bVerboseProgress()) {
    vInfo("Created large heap %p: %lldB objects\n"_Stz,
          Transient.m_pLarge, C_SIZE_LARGE_OBJECT);
    vInfo("Created medium heap %p: %lldB objects\n"_Stz,
          Transient.m_pMedium, C_SIZE_MEDIUM_OBJECT);
    vInfo("Created small heap %p: %lldB objects\n"_Stz,
          Transient.m_pSmall, C_SIZE_SMALL_OBJECT);
    vInfo("Created tiny heap %p: %lldB objects\n"_Stz,
          Transient.m_pTiny, C_SIZE_TINY_OBJECT);
  }
    
  // create the persistent heaps
  Persistent.m_pLarge  = new Heap(CN_LARGEPERSISTENT,  C_SIZE_LARGE_OBJECT);
  Persistent.m_pMedium = new Heap(CN_MEDIUMPERSISTENT, C_SIZE_MEDIUM_OBJECT);
  Persistent.m_pSmall  = new Heap(CN_SMALLPERSISTENT,  C_SIZE_SMALL_OBJECT);
  Persistent.m_pTiny   = new Heap(CN_TINYPERSISTENT,   C_SIZE_SMALL_OBJECT);
}

/*----------------------------------------------------------------------------*/
/* heap memory portrait                                                       */
/*----------------------------------------------------------------------------*/
void HeapMem::vMonitorPortrait(s48K& sPortrait)
{
  s48K s;
  
  s = ""_Stz; Transient.m_pTiny->vMonitorPortrait(s);   sPortrait.vConcat(s);
  s = ""_Stz; Transient.m_pSmall->vMonitorPortrait(s);  sPortrait.vConcat(s);
  s = ""_Stz; Transient.m_pMedium->vMonitorPortrait(s); sPortrait.vConcat(s);
  s = ""_Stz; Transient.m_pLarge->vMonitorPortrait(s);  sPortrait.vConcat(s);
}

/*----------------------------------------------------------------------------*/
/* allocate a class of object from a heap                                     */
/*----------------------------------------------------------------------------*/
Object* HeapMem::pAllocate(eCN eAllocClass, bool bPersistent)
{
  HeapMem* pHeapMem = AM::pHeapMem();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  eHT eHeap = gxaSizeHeap[eAllocClass].m_eWhichHeap;
  
  if (bPersistent) {
    switch(eHeap) {
      case HT_TRANSIENT_TINY:   eHeap = HT_PERSISTENT_TINY;   break;
      case HT_TRANSIENT_SMALL:  eHeap = HT_PERSISTENT_SMALL;  break;
      case HT_TRANSIENT_MEDIUM: eHeap = HT_PERSISTENT_MEDIUM; break;
      case HT_TRANSIENT_LARGE:  eHeap = HT_PERSISTENT_LARGE;  break;
      default: break;
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Object* pObject = nullptr;
  
  switch(eHeap) {
    case HT_INVALID:
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
             "HeapMem::pAllocate: Invalid heap allocation: class %lld %s\n"_Stz,
             eAllocClass,aCN[eAllocClass]);
      
      // transient -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

      case HT_TRANSIENT_TINY:
        pObject = pHeapMem->Transient.m_pTiny->pAllocObject(eAllocClass);
        break;

      case HT_TRANSIENT_SMALL:
        pObject = pHeapMem->Transient.m_pSmall->pAllocObject(eAllocClass);
        break;

      case HT_TRANSIENT_MEDIUM:
        pObject = pHeapMem->Transient.m_pMedium->pAllocObject(eAllocClass);
        break;

      case HT_TRANSIENT_LARGE:
        pObject = pHeapMem->Transient.m_pLarge->pAllocObject(eAllocClass);
        break;

      // persistent -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      
      case HT_PERSISTENT_TINY:
        pObject = pHeapMem->Persistent.m_pTiny->pAllocObject(eAllocClass);
        break;
        
      case HT_PERSISTENT_SMALL:
        pObject = pHeapMem->Persistent.m_pSmall->pAllocObject(eAllocClass);
        break;
        
      case HT_PERSISTENT_MEDIUM:
        pObject = pHeapMem->Persistent.m_pMedium->pAllocObject(eAllocClass);
        break;

      case HT_PERSISTENT_LARGE:
        pObject = pHeapMem->Persistent.m_pLarge->pAllocObject(eAllocClass);
        break;
      
    default:
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
             "HeapMem::pAllocate: Invalid heap allocation: eHeap %lld\n"_Stz,
             eHeap);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  RU_Memory* pRU_Memory = AM::pRU_Memory();
  
  if (pRU_Memory)
    AM::vAllocInc(pRU_Memory, eAllocClass);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* free an object back to heap                                                */
/*----------------------------------------------------------------------------*/
void HeapMem::vFree(Object* pObject, bool bPersistent)
{  
  HeapMem* pHeapMem = AM::pHeapMem();
  
  eCN eFreeingClass = pObject->eClass();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  RU_Memory* pRU_Memory = AM::pRU_Memory();
  
  if (pRU_Memory)
    AM::vFreeInc(pRU_Memory, eFreeingClass);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // dont-free capability
  
  if (gpBufferPool->bDontFree()) {
    
    if (CN_ENTBUFFER != eFreeingClass) {
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
  
  eHT eHeap = gxaSizeHeap[eFreeingClass].m_eWhichHeap;
  
  if (bPersistent) {
    switch(eHeap) {
      case HT_TRANSIENT_TINY:   eHeap = HT_PERSISTENT_TINY;   break;
      case HT_TRANSIENT_SMALL:  eHeap = HT_PERSISTENT_SMALL;  break;
      case HT_TRANSIENT_MEDIUM: eHeap = HT_PERSISTENT_MEDIUM; break;
      case HT_TRANSIENT_LARGE:  eHeap = HT_PERSISTENT_LARGE;  break;
      default: break;
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  switch(eHeap) {
    case HT_INVALID:
      MemLib::vWordDump(pObject,256,0);
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
             "HeapMem::vFree: Invalid heap free: class %lld %s\n"_Stz,
             eFreeingClass,aCN[eFreeingClass]);
      
    // transient -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      
    case HT_TRANSIENT_TINY:
      pHeapMem->Transient.m_pTiny->vFreeObject(pObject);
      return;
      
    case HT_TRANSIENT_SMALL:
      pHeapMem->Transient.m_pSmall->vFreeObject(pObject);
      return;
      
    case HT_TRANSIENT_MEDIUM:
      pHeapMem->Transient.m_pMedium->vFreeObject(pObject);
      return;
      
    case HT_TRANSIENT_LARGE:
      pHeapMem->Transient.m_pLarge->vFreeObject(pObject);
      return;
            
    // persistent -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      
    case HT_PERSISTENT_TINY:
      pHeapMem->Transient.m_pTiny->vFreeObject(pObject);
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

  // tiny transient heap
  AM::pHeapMem()->Transient.m_pTiny->vReport(sReport.pString());
  vTrace(" %s\n"_Stz,sReport.szStz());
  
  // small transient heap
  AM::pHeapMem()->Transient.m_pSmall->vReport(sReport.pString());
  vTrace(" %s\n"_Stz,sReport.szStz());
  
  // medium transient heap
  AM::pHeapMem()->Transient.m_pMedium->vReport(sReport.pString());
  vTrace(" %s\n"_Stz,sReport.szStz());

  // large transient heap
  AM::pHeapMem()->Transient.m_pLarge->vReport(sReport.pString());
  vTrace(" %s\n"_Stz,sReport.szStz());

  
//  // tiny persistent heap
//  AM::pHeapMem()->Persistent.m_pTiny->vReport(sReport.pString());
//  vTrace(" %s\n"_Stz,sReport.szStz());
//
//  // small persistent heap
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
    
    eHT eHeap = gxaSizeHeap[eClass].m_eWhichHeap;
    i8  iSize = gxaSizeHeap[eClass].m_nClassSize;
    
    switch(eHeap) {
      case HT_INVALID:
        break;
        
      // transient -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
        
      case HT_TRANSIENT_TINY:
        if (iSize >= C_SIZE_TINY_OBJECT)
          vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
                 "HeapMem::vValidateEnums: transient tiny class %lld %s is too big\n"_Stz,
                 eClass,aCN[eClass]);
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
         
      // persistent -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

      case HT_PERSISTENT_TINY:
        if (iSize >= C_SIZE_SMALL_OBJECT)
          vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
                 "HeapMem::vValidateEnums: persistent tiny class %lld %s is too big\n"_Stz,
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
