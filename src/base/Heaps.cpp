/*============================================================================*/
/* Heaps.cpp -- Set of Heaps in Pool                                        */
/*============================================================================*/

#include "BASE.h"
#include "TagConstants.h"

extern SizeHeap  gxaSizeHeap[];

bool gbAddrAllocated = false;

SubSysFn __Heaps("Heaps"_Stz,(fpSELFFN*)Heaps::vSubSystem,
                   ""_Stz,nullptr);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Heaps::Heaps(void)
: Object(CN_HEAPMEM)
{
  _precondition_(_MAGIC_,CN_HEAPMEM);
  
  Transient.m_pTiny   = nullptr;
  Transient.m_pSmall  = nullptr;
  Transient.m_pMedium = nullptr;
  Transient.m_pLarge  = nullptr;
  
  Virtual.m_pTiny   = nullptr;
  Virtual.m_pSmall  = nullptr;
  Virtual.m_pMedium = nullptr;
  Virtual.m_pLarge  = nullptr;
  
  Persistent.m_pTiny   = nullptr;
  Persistent.m_pSmall  = nullptr;
  Persistent.m_pMedium = nullptr;
  Persistent.m_pLarge  = nullptr;
    
  _postcondition_(_MAGIC_,CN_HEAPMEM);
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* the subsystem function                                                     */
/*----------------------------------------------------------------------------*/
void Heaps::vSubSystem(TokenVec* pTokenVec)
{
  _static_
  UNUSED(pTokenVec);

  vTrace("Heaps::vSubSystem\n"_Stz);
  
  Heaps::vSummaryReport();
}

/*----------------------------------------------------------------------------*/
/* allocate a class of object from a heap                                     */
/*----------------------------------------------------------------------------*/
Object* Heaps::pAllocate(eCN eAllocClass, bool bPersistent)
{
  _static_
  UNUSED(bPersistent);

  Heaps* pHeaps = AM::pHeaps();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  eHT eHeap = gxaSizeHeap[eAllocClass].m_eWhichHeap;
  
  Heap* pHeap = pHeaps->pGetHeap(eHeap);
  
//  if (bPersistent) {
//    switch(eHeap) {
//      case HT_T_TINY:   eHeap = HT_P_TINY;   break;
//      case HT_T_SMALL:  eHeap = HT_P_SMALL;  break;
//      case HT_T_MEDIUM: eHeap = HT_P_MEDIUM; break;
//      case HT_T_LARGE:  eHeap = HT_P_LARGE;  break;
//      default: break;
//    }
//  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Object* pObject = nullptr;
  
  pObject = pHeap->pAllocObject(eAllocClass);
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  RU_Memory* pRU_Memory = AM::pRU_Memory();
  
  if (pRU_Memory)
    AM::vAllocInc(pRU_Memory, eAllocClass);
  
  return pObject;
}


/*----------------------------------------------------------------------------*/
/* free an object back to heap                                                */
/*----------------------------------------------------------------------------*/
void Heaps::vFree(Object* pObject, bool bPersistent)
{
  UNUSED(bPersistent);

  Heaps* pHeaps = AM::pHeaps();
  
  eCN eWasClass = pObject->eGetClassNo();
  
  if (! (eWasClass > CN_NONE && eWasClass < CN_ENUMS) ) {
    vTrace("Heaps::vFree:: invalid class %lld\n"_Stz
           ,(i8)eWasClass
           );
    RASSERT(eWasClass > CN_NONE && eWasClass < CN_ENUMS);
  }
    
  RU_Memory* pRU_Memory = AM::pRU_Memory();
  
  if (pRU_Memory)
    AM::vFreeInc(pRU_Memory, eWasClass);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // dont-free capability
  
  if (gpBufferPool->bDontFree()) {
    
//    if (CN_ENTBUFFER != eWasClass) {
//      pObject->vSetClassNo(CN_FREEDNOT);
//      return;
//    }
//
//    if ( ! gpBufferPool->bFreeSpecific() ) {
//      pObject->vSetClassNo(CN_FREEDNOT);
//      return;
//    }
    
    
    if (gpBufferPool->bFreeSpecific() ) {
      
//      if ( CN_ENTBUFFER == eWasClass ||
//           CN_CRWBUFFER == eWasClass ||
//           CN_ROWBUFFER == eWasClass ||
//           CN_CSVBUFFER == eWasClass ) {
//        // free data/index blocks
//      }
//      else {
//        // dont free everything else
//        pObject->vSetClassNo(CN_FREEDNOT);
//        return;
//      }
      
      if ( CN_JOB       == eWasClass ||
           CN_LOGICUNIT == eWasClass ||
           CN_OP        == eWasClass ) {
        
        pObject->vSetClassNo(CN_FREEDNOT);
        return;
      }
      else {
        // free everything else
      }
      
    }
    else {
      // dont free anything
      pObject->vSetClassNo(CN_FREEDNOT);
      return;
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // actually free the object and return its space for reuse
  
  eHT eHeap = gxaSizeHeap[eWasClass].m_eWhichHeap;
 
  if (! (eHeap > HT_INVALID && eHeap < HT_ENUMS) ) {

    vTrace("Heaps::vFree:: invalid heap %lld "_Stz
           "%p class %lld `%lls`\n"_Stz
           ,(i8)eHeap
           
           ,pObject
           ,(i8)eWasClass
           ,aCN[eWasClass]
           );
    RASSERT(eWasClass > 0 && eWasClass < CN_ENUMS);


  }
  
  Heap* pHeap = pHeaps->pGetHeap(eHeap);

  
//  if (bPersistent) {
//    switch(eHeap) {
//      case HT_T_TINY:   eHeap = HT_P_TINY;   break;
//      case HT_T_SMALL:  eHeap = HT_P_SMALL;  break;
//      case HT_T_MEDIUM: eHeap = HT_P_MEDIUM; break;
//      case HT_T_LARGE:  eHeap = HT_P_LARGE;  break;
//      default: break;
//    }
//  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pObject = aAdd<Object*>(pObject,gxaSizeHeap[eWasClass].m_nVtableAdjust);
  
  pHeap->vFreeObject(pObject);
}

/*----------------------------------------------------------------------------*/
/* free a non-object buffer                                                   */
/*----------------------------------------------------------------------------*/
void Heaps::vFree_NonObject(PoolBuffer* pBuffer)
{
  pBuffer->vSetMagic();
  
  AM::pHeaps()->Transient.m_pLarge->vFreeObject(pBuffer);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* heap memory report                                                         */
/*----------------------------------------------------------------------------*/
void Heaps::vSummaryReport(void)
{
  s256 sReport;

  eHT eHeap = HT_INVALID;
  
  for (Heap* pHeap = AM::pHeaps()->pEachHeap(eHeap);
       pHeap;
       pHeap = AM::pHeaps()->pEachHeap(eHeap)) {
    pHeap->vSummaryReport(sReport.pString());
    vTrace(" %s\n"_Stz,sReport.szStz());
  }
  
//  // tiny transient heap
//  AM::pHeaps()->Transient.m_pTiny->vSummaryReport(sReport.pString());
//  vTrace(" %s\n"_Stz,sReport.szStz());
//
//  // small transient heap
//  AM::pHeaps()->Transient.m_pSmall->vSummaryReport(sReport.pString());
//  vTrace(" %s\n"_Stz,sReport.szStz());
//
//  // medium transient heap
//  AM::pHeaps()->Transient.m_pMedium->vSummaryReport(sReport.pString());
//  vTrace(" %s\n"_Stz,sReport.szStz());
//
//  // large transient heap
//  AM::pHeaps()->Transient.m_pLarge->vSummaryReport(sReport.pString());
//  vTrace(" %s\n"_Stz,sReport.szStz());

  
//  // tiny persistent heap
//  AM::pHeaps()->Persistent.m_pTiny->vSummaryReport(sReport.pString());
//  vTrace(" %s\n"_Stz,sReport.szStz());
//
//  // small persistent heap
//  AM::pHeaps()->Persistent.m_pSmall->vSummaryReport(sReport.pString());
//  vTrace(" %s\n"_Stz,sReport.szStz());
//
//  // medium persistent heap
//  AM::pHeaps()->Persistent.m_pMedium->vSummaryReport(sReport.pString());
//  vTrace(" %s\n"_Stz,sReport.szStz());
//
//  // large persistent heap
//  AM::pHeaps()->Persistent.m_pLarge->vSummaryReport(sReport.pString());
//  vTrace(" %s\n"_Stz,sReport.szStz());
//
}

/*----------------------------------------------------------------------------*/
/* search free list                                                           */
/*----------------------------------------------------------------------------*/
void Heaps::vSearchFree(void)
{
  if (gbAddrAllocated) {
    for (FreeChunk* pTestChunk = AM::pHeaps()->Transient.m_pMedium->m_pFreeHead;
         pTestChunk;
         pTestChunk = pTestChunk->pNextFree()) {
      if ((void*)pTestChunk >= (void*)0x204828fc0038 &&
          (void*)pTestChunk < (void*)0x204828fcfd30) {
        int iDebug = true;
        UNUSED(iDebug);
      }
    }
  }
  
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* iterate each heap                                                          */
/*----------------------------------------------------------------------------*/
Heap* Heaps::pEachHeap(eHT& reLastHeap)
{
  Heap* pNextHeap = nullptr;
  i8 iLastHeap = reLastHeap;

  i8 iNextHeap;
  if (HT_INVALID == reLastHeap)
    iNextHeap = HT_T_TINY;
  else
    iNextHeap = ++ iLastHeap;

  // -- -- -- -- -- -- --
  
  if (HT_ENUMS == iNextHeap) {
    reLastHeap = HT_INVALID;
    return nullptr;
  }
  
  pNextHeap = pGetHeap((eHT) iNextHeap);
  
  reLastHeap = (eHT) iNextHeap;
  
  return pNextHeap;
}

/*----------------------------------------------------------------------------*/
/* iterate each heap                                                          */
/*----------------------------------------------------------------------------*/
Heap* Heaps::pGetHeap(eHT eHeap)
{
  _precondition_(_MAGIC_,CN_HEAPMEM);
  
  switch (eHeap) {
      
    case HT_P_TINY:    return Persistent.m_pTiny;
    case HT_P_SMALL:   return Persistent.m_pSmall;
    case HT_P_MEDIUM:  return Persistent.m_pMedium;
    case HT_P_LARGE:   return Persistent.m_pLarge;
      
    case HT_T_TINY:    return Transient.m_pTiny;
    case HT_T_SMALL:   return Transient.m_pSmall;
    case HT_T_MEDIUM:  return Transient.m_pMedium;
    case HT_T_LARGE:   return Transient.m_pLarge;
      
    case HT_V_TINY:    return Virtual.m_pTiny;
    case HT_V_SMALL:   return Virtual.m_pSmall;
    case HT_V_MEDIUM:  return Virtual.m_pMedium;
    case HT_V_LARGE:   return Virtual.m_pLarge;
      
    default:
      NEVER_REACH;
  }

}

/*----------------------------------------------------------------------------*/
/* initialise the various heaps                                               */
/*----------------------------------------------------------------------------*/
void Heaps::vInit(void)
{
  // create the persistent heaps
//  Persistent.m_pTiny   =
//    new Heap(CN_TINYPERSISTENT,  C_SIZE_SMALL_OBJECT,  HT_P_TINY);
//  Persistent.m_pSmall  =
//    new Heap(CN_SMALLPERSISTENT, C_SIZE_SMALL_OBJECT,  HT_P_SMALL);
//  Persistent.m_pMedium =
//    new Heap(CN_MEDIUMPERSISTENT,C_SIZE_MEDIUM_OBJECT, HT_P_MEDIUM);
//  Persistent.m_pLarge  =
//    new Heap(CN_LARGEPERSISTENT, C_SIZE_LARGE_OBJECT,  HT_P_LARGE);
  // create the transient heaps
  
  Transient.m_pTiny   =
    new Heap(CN_TINYTRANSIENT,  C_SIZE_TINY_OBJECT,  HT_T_TINY);
  Transient.m_pSmall  =
    new Heap(CN_SMALLTRANSIENT, C_SIZE_SMALL_OBJECT, HT_T_SMALL);
  Transient.m_pMedium =
    new Heap(CN_MEDIUMTRANSIENT,C_SIZE_MEDIUM_OBJECT,HT_T_MEDIUM);
  Transient.m_pLarge  =
    new Heap(CN_LARGETRANSIENT, C_SIZE_LARGE_OBJECT, HT_T_LARGE);

  // create the virtual heaps
  Virtual.m_pTiny   =
    new Heap(CN_TINYVIRTUAL,  C_SIZE_TINY_OBJECT,  HT_V_TINY);
  Virtual.m_pSmall  =
    new Heap(CN_SMALLVIRTUAL, C_SIZE_SMALL_OBJECT, HT_V_SMALL);
  Virtual.m_pMedium =
    new Heap(CN_MEDIUMVIRTUAL,C_SIZE_MEDIUM_OBJECT,HT_V_MEDIUM);
  Virtual.m_pLarge  =
    new Heap(CN_LARGEVIRTUAL, C_SIZE_LARGE_OBJECT, HT_V_LARGE);


  if (AM::AM::TRACE::bVerbose(C_VB_BOOT)) {
    
    //    vInfo("Created persistent tiny heap %p: %lldB objects\n"_Stz,
    //          Persistent.m_pTiny, C_SIZE_TINY_OBJECT);
    //    vInfo("Created persistent small heap %p: %lldB objects\n"_Stz,
    //          Persistent.m_pSmall, C_SIZE_SMALL_OBJECT);
    //    vInfo("Created persistent medium heap %p: %lldB objects\n"_Stz,
    //          Persistent.m_pMedium, C_SIZE_MEDIUM_OBJECT);
    //    vInfo("Created persistent large heap %p: %lldB objects\n"_Stz,
    //          Persistent.m_pLarge, C_SIZE_LARGE_OBJECT);
    
    vInfo("Created transient tiny heap %p: %lldB objects\n"_Stz,
          Transient.m_pTiny, C_SIZE_TINY_OBJECT);
    vInfo("Created transient small heap %p: %lldB objects\n"_Stz,
          Transient.m_pSmall, C_SIZE_SMALL_OBJECT);
    vInfo("Created transient medium heap %p: %lldB objects\n"_Stz,
          Transient.m_pMedium, C_SIZE_MEDIUM_OBJECT);
    vInfo("Created transient large heap %p: %lldB objects\n"_Stz,
          Transient.m_pLarge, C_SIZE_LARGE_OBJECT);
    
    vInfo("Created virtual tiny heap %p: %lldB objects\n"_Stz,
          Virtual.m_pTiny, C_SIZE_TINY_OBJECT);
    vInfo("Created virtual small heap %p: %lldB objects\n"_Stz,
          Virtual.m_pSmall, C_SIZE_SMALL_OBJECT);
    vInfo("Created virtual medium heap %p: %lldB objects\n"_Stz,
          Virtual.m_pMedium, C_SIZE_MEDIUM_OBJECT);
    vInfo("Created virtual large heap %p: %lldB objects\n"_Stz,
          Virtual.m_pLarge, C_SIZE_LARGE_OBJECT);
  }
    
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
