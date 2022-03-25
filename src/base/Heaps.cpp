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

__aFooterFunction(Heaps)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Heaps::Heaps(i8 bNoFree)
: Object(CN_HEAPMEM)
{
  _precondition_(_MAGIC_,CN_HEAPMEM);
  
  m_bNoFree     = bNoFree;
  
  m_bTraceAlloc = false;
  m_eTraceAlloc = CN_NONE;
  m_pTaskAlloc  = nullptr;
  
  m_bTraceFree  = false;
  m_eTraceFree  = CN_NONE;
  m_pTaskFree   = nullptr;

  Transient.m_pTiny   = nullptr;
  Transient.m_pSmall  = nullptr;
  Transient.m_pMedium = nullptr;
  Transient.m_pLarge  = nullptr;
  Transient.m_pData   = nullptr;

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
  
  if ((pHeaps->m_bTraceAlloc || eAllocClass == pHeaps->m_eTraceAlloc)
      && (! pHeaps->m_pTaskAlloc || (MY::pTask() == pHeaps->m_pTaskAlloc))) {
    vTrace("Alloc: %s %p from %s\n"_Stz
           ,aCN[eAllocClass],pObject,aHT[eHeap]);
  }
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* detailed dump of all heaps                                                 */
/*----------------------------------------------------------------------------*/
void Heaps::vDumpHeaps(void)
{
  eHT eHeap = HT_INVALID;
  
  for (Heap* pHeap = AM::pHeaps()->pEachHeap(eHeap);
       pHeap;
       pHeap = AM::pHeaps()->pEachHeap(eHeap)) {
    
    pHeap->vDumpHeap();
  }
}

/*----------------------------------------------------------------------------*/
/* free an object back to heap                                                */
/*----------------------------------------------------------------------------*/
void Heaps::vFree(Object* pObject, bool bPersistent)
{  
  UNUSED(bPersistent);
  
  eCN eFreeClass = pObject->eGetClassNo();

  Heaps* pHeaps = AM::pHeaps();
    
  if (! (eFreeClass > CN_NONE && eFreeClass < CN_ENUMS) ) {
    vTrace("Heaps::vFree:: invalid class %lld\n"_Stz
           ,(i8)eFreeClass
           );
    RASSERT(eFreeClass > CN_NONE && eFreeClass < CN_ENUMS);
  }

  if (! (CN_FREECHUNK != eFreeClass) ) {
    FreeChunk* pFreeChunk = (FreeChunk*)(pObject);
    eCN eWasClass = pFreeChunk->eWasClass();

    vTrace("Heaps::vFree:: %p "
           "attempting to free a CN_FREECHUNK(was %lld `%s`)\n"_Stz
           ,pObject
           ,eWasClass
           ,aCN[eWasClass]
           );
    
    
    RASSERT(CN_FREECHUNK != eFreeClass);
  }

  RU_Memory* pRU_Memory = AM::pRU_Memory();
  
  if (pRU_Memory)
    AM::vFreeInc(pRU_Memory, eFreeClass);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (pHeaps->m_bNoFree) {
    if (   CN_PIPESPEC == eFreeClass
        || CN_STAGESPEC    == eFreeClass
        || CN_OPSPEC       == eFreeClass
        )
    return;
  }

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
      
      if ( CN_JOB       == eFreeClass ||
           CN_LOGICUNIT == eFreeClass ||
           CN_OP        == eFreeClass ) {
        
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
  
  eHT eHeap = gxaSizeHeap[eFreeClass].m_eWhichHeap;
  
  if (! (eHeap > HT_INVALID && eHeap < HT_ENUMS) ) {

    vTrace("Heaps::vFree:: invalid heap %lld "_Stz
           "%p class %lld %s\n"_Stz
           ,(i8)eHeap
           
           ,pObject
           ,(i8)eFreeClass
           ,aCN[eFreeClass]
           );
    RASSERT(eHeap > HT_INVALID && eHeap < HT_ENUMS);
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
  
  pObject = aAdd<Object*>(pObject,gxaSizeHeap[eFreeClass].m_nVtableAdjust);
  
  if ((pHeaps->m_bTraceFree || eFreeClass == pHeaps->m_eTraceFree)
      && (! pHeaps->m_pTaskFree || (MY::pTask() == pHeaps->m_pTaskFree))) {
    vTrace("Free : %s %p into %s\n"_Stz
           ,aCN[eFreeClass],pObject,aHT[eHeap]);
  }

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
/* set trace alloc of objects                                                 */
/*----------------------------------------------------------------------------*/
void Heaps::vSetTraceAlloc(bool bTraceAlloc, Task* pTaskAlloc)
{
  m_bTraceAlloc = bTraceAlloc;
  m_pTaskAlloc  = pTaskAlloc;
}

/*----------------------------------------------------------------------------*/
/* set trace alloc of objects                                                 */
/*----------------------------------------------------------------------------*/
void Heaps::vSetTraceAlloc(eCN eTraceAlloc)
{
  m_eTraceAlloc = eTraceAlloc;
}

/*----------------------------------------------------------------------------*/
/* set trace free of objects                                                  */
/*----------------------------------------------------------------------------*/
void Heaps::vSetTraceFree(bool bTraceFree, Task* pTaskFree)
{
  m_bTraceFree = bTraceFree;
  m_pTaskFree  = pTaskFree;
}

/*----------------------------------------------------------------------------*/
/* set trace free of objects                                                  */
/*----------------------------------------------------------------------------*/
void Heaps::vSetTraceFree(eCN eTraceFree)
{
  m_eTraceFree = eTraceFree;
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
    case HT_T_DATA:    return Transient.m_pData;

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
  Transient.m_pData  =
    new Heap(CN_DATATRANSIENT,  C_SIZE_DATA_OBJECT, HT_T_DATA);

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
    
    //    vTrace("Created persistent tiny heap %p: %lldB objects\n"_Stz,
    //          Persistent.m_pTiny, C_SIZE_TINY_OBJECT);
    //    vTrace("Created persistent small heap %p: %lldB objects\n"_Stz,
    //          Persistent.m_pSmall, C_SIZE_SMALL_OBJECT);
    //    vTrace("Created persistent medium heap %p: %lldB objects\n"_Stz,
    //          Persistent.m_pMedium, C_SIZE_MEDIUM_OBJECT);
    //    vTrace("Created persistent large heap %p: %lldB objects\n"_Stz,
    //          Persistent.m_pLarge, C_SIZE_LARGE_OBJECT);
    
    vTrace("Created transient tiny heap %p: %lldB objects\n"_Stz,
          Transient.m_pTiny, C_SIZE_TINY_OBJECT);
    vTrace("Created transient small heap %p: %lldB objects\n"_Stz,
          Transient.m_pSmall, C_SIZE_SMALL_OBJECT);
    vTrace("Created transient medium heap %p: %lldB objects\n"_Stz,
          Transient.m_pMedium, C_SIZE_MEDIUM_OBJECT);
    vTrace("Created transient large heap %p: %lldB objects\n"_Stz,
          Transient.m_pLarge, C_SIZE_LARGE_OBJECT);
    vTrace("Created transient data heap %p: %lldB objects\n"_Stz,
          Transient.m_pData, C_SIZE_DATA_OBJECT);

    vTrace("Created virtual tiny heap %p: %lldB objects\n"_Stz,
          Virtual.m_pTiny, C_SIZE_TINY_OBJECT);
    vTrace("Created virtual small heap %p: %lldB objects\n"_Stz,
          Virtual.m_pSmall, C_SIZE_SMALL_OBJECT);
    vTrace("Created virtual medium heap %p: %lldB objects\n"_Stz,
          Virtual.m_pMedium, C_SIZE_MEDIUM_OBJECT);
    vTrace("Created virtual large heap %p: %lldB objects\n"_Stz,
          Virtual.m_pLarge, C_SIZE_LARGE_OBJECT);
  }
    
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
