/*============================================================================*/
/* Heaps.cpp -- Set of Heaps in Pool                                        */
/*============================================================================*/

#include "BASE.h"
#include "TagConstants.h"
#include "Job.hpp"
#include "CmdLineArgs.hpp"
#include "Decider.hpp"

Decider gxThreadsRemaining;

extern SizeHeap  gxaSizeHeap[];
extern CmdLineArgs  gxCmdLineArgs;

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

  Heaps* pHeaps = AM::pHeaps();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  eHT eHeap = gxaSizeHeap[eAllocClass].m_eWhichHeap;

  // The Job persistent flag is set via pipeline tag
  // This causes all allocs as part of this job to use the persistent heap
  Job* pJob = MY::pJob();
  if ((pJob && pJob->bPersistent()) || bPersistent) {
    switch(eHeap) {
      case HT_T_TINY:   eHeap = HT_P_TINY;   break;
      case HT_T_SMALL:  eHeap = HT_P_SMALL;  break;
      case HT_T_MEDIUM: eHeap = HT_P_MEDIUM; break;
      case HT_T_LARGE:  eHeap = HT_P_LARGE;  break;
      default: break;
    }
  }

  Heap* pHeap = pHeaps->pGetHeap(eHeap);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Object* pObject = nullptr;

  pObject = pHeap->pAllocObject(eAllocClass);

  if(pObject) {
    if (pJob && pJob->bPersistent()) {
      pObject->vSetPersistent(true);
    } else {
      pObject->vSetPersistent(false);
    }
  }
  
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
    if (   CN_VERTEXSPEC == eFreeClass
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

  // Note: Virtual Heap is ignored for now, even if the Object persistent bit is set.
  if (pObject->bGetPersistent()) {
    switch(eHeap) {
      case HT_T_TINY:   eHeap = HT_P_TINY;   break;
      case HT_T_SMALL:  eHeap = HT_P_SMALL;  break;
      case HT_T_MEDIUM: eHeap = HT_P_MEDIUM; break;
      case HT_T_LARGE:  eHeap = HT_P_LARGE;  break;
      default: break;
    }
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
/* Find Ptn0 reload file path                                                 */
/*----------------------------------------------------------------------------*/
void Heaps::vFindPtn0FilePath(String* psFilePath)
{
  i8 nDiskpaths = gxCmdLineArgs.m_xSysArgs.m_nDiskPaths;
  if(nDiskpaths <= 0)
  {
    vTrace("SE was not started with the '--disk <path-to-disk-root>' option. Cannot reload.\n"_Stz);
    return;
  }

  for(i8 i = 0; i < nDiskpaths; i++)
  {
    s256 szTmp = ""_Stz;
    szTmp.vSprint("%s%ssaveFile_%s_0"_Stz,
                  gxCmdLineArgs.m_xSysArgs.m_sDiskRootDir[i].szStz(),
                  gxCmdLineArgs.m_xSysArgs.m_sReloadDir.szStz(),
                  gxCmdLineArgs.m_xSysArgs.m_sNodeName.szStz());

    if(File::bExists(szTmp))
    {
      psFilePath->vCopy(szTmp);
      return;
    }
  }

  vTrace("Unable to find %ssaveFile_%s_0 in given root disk directories.\n"
         "Check the paths given with '--disk <path-to-disk-root>' and '--reload <dir>' argument.\n"_Stz,
         gxCmdLineArgs.m_xSysArgs.m_sReloadDir.szStz(),
         gxCmdLineArgs.m_xSysArgs.m_sNodeName.szStz());
  THROW("Unable to find saveFile_<node-name>_0!!\n"_Stz);
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
  if(gxCmdLineArgs.m_xSysArgs.m_bReload) {

    Timer oTimer;
    oTimer.vStart();

    vReloadPHeaps();

    oTimer.vUpdate();
    vTrace("Loading of Persistent Heaps from disk took %lld ms\n"_Stz, oTimer.iMilliseconds());

  } else {
    // create the persistent heaps
    Persistent.m_pTiny   =
        new Heap(CN_TINYPERSISTENT,  C_SIZE_TINY_OBJECT,  HT_P_TINY);
    Persistent.m_pSmall  =
        new Heap(CN_SMALLPERSISTENT, C_SIZE_SMALL_OBJECT,  HT_P_SMALL);
    Persistent.m_pMedium =
        new Heap(CN_MEDIUMPERSISTENT,C_SIZE_MEDIUM_OBJECT, HT_P_MEDIUM);
    Persistent.m_pLarge  =
        new Heap(CN_LARGEPERSISTENT, C_SIZE_LARGE_OBJECT,  HT_P_LARGE);
  }

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

/*----------------------------------------------------------------------------*/
/* Load the Persistent heaps, spawn threads to load the persistent buffers    */
/* from disk                                                                  */
/*----------------------------------------------------------------------------*/
void Heaps::vReloadPHeaps(void)
{
  /*
  Notes:
    This section is where we load the persistent heaps from file.
    The file consists of pool buffers and heap buffers. They are
    C_SIZE_POOL_BUFFER in size.

    We start by opening the file, and reading the header information.
    The header is of size sizeof(PoolBuffer).

    The object information tells us the Class type, and the pool buffer
    member called m_pThis, tells us the address of the buffer when it was
    written to disk.

    We do a sanity check of this address and directly copy the header info
    into the free chain buffer (since these addresses correspond to an
    address in the free chain). Next, we read the rest of the info from
    the file into this buffer. Note, we do not lock the free chain.

    After the main thread loads in the various persistent heaps from disk,
    we spawn threads based on the number of partitions. Each thread reads
    it's corresponding file and performs the same task as what the main
    thread did earlier, i.e read header info -> get the address -> check
    the address -> read body into this address.

    All threads are directly writing to the buffer addresses it reads
    from the reload files. This drastically speeds up reload times, but
    leaves the free chain in an unstable/unusable state since different
    parts of the free chain have been written to.

    Hence, once all threads are done reloading, the main thread cleans up
    the free chain, and repairs all the pointers, and other metadata info
    about it, like number of allocs, number of free buffers, etc.

  */
  i8 nSizeHeader = sizeof(PoolBuffer);

  char caObjectHeader[nSizeHeader];

  Header* aHeader = (Header*) & caObjectHeader[0];

  i8 nSizeBody = ((i8) C_SIZE_POOL_BUFFER) - nSizeHeader;


  // Get the name of the first file, which contains the P Heaps.
  // Main thread first loads these in, before spawning threads to read
  // the rest of the buffers.

  s256 szNode0Path = ""_Stz;
  vFindPtn0FilePath(szNode0Path);

  RASSERT( ! szNode0Path.bIsEmpty());

  File* pFile = new(C_VIA_MALLOC) File(nullptr);
  bool bResult = pFile->bOpenRead(szNode0Path);
  ASSERT(bResult);
  UNUSED(bResult);

  // Read the Header information to get the address of this pool buffer
  i8 nBytes = pFile->nReadBytes(aHeader, nSizeHeader);
  RASSERT(nBytes == nSizeHeader);

  while(nBytes != 0)
  {
    PoolBuffer* pPoolBufferHeader = reinterpret_cast<PoolBuffer*>(aHeader);
    void *aBody;

    switch(pPoolBufferHeader->eGetClassNo()) {
      case CN_TINYPERSISTENT: {
        RASSERT(gpBufferPool->bReloadSanityCheckAddr(pPoolBufferHeader->m_pThis) );
        PoolBuffer *pBuffer = reinterpret_cast<PoolBuffer*>(pPoolBufferHeader->m_pThis);

        MemLib::vCopy(pBuffer, pPoolBufferHeader, nSizeHeader, pPoolBufferHeader->eGetClassNo());
        aBody = aAdd<void *>(pBuffer, nSizeHeader);
        nBytes = pFile->nReadBytes(aBody, nSizeBody);
        RASSERT(nBytes == nSizeBody);

        Persistent.m_pTiny = reinterpret_cast<Heap*>(pBuffer);

        break;
      }

      case CN_SMALLPERSISTENT: {
        RASSERT(gpBufferPool->bReloadSanityCheckAddr(pPoolBufferHeader->m_pThis) );
        PoolBuffer *pBuffer = reinterpret_cast<PoolBuffer*>(pPoolBufferHeader->m_pThis);

        MemLib::vCopy(pBuffer, pPoolBufferHeader, nSizeHeader, pPoolBufferHeader->eGetClassNo());
        aBody = aAdd<void *>(pBuffer, nSizeHeader);
        nBytes = pFile->nReadBytes(aBody, nSizeBody);
        RASSERT(nBytes == nSizeBody);

        Persistent.m_pSmall = reinterpret_cast<Heap*>(pBuffer);

        break;
      }

      case CN_MEDIUMPERSISTENT: {
        RASSERT(gpBufferPool->bReloadSanityCheckAddr(pPoolBufferHeader->m_pThis) );
        PoolBuffer *pBuffer = reinterpret_cast<PoolBuffer*>(pPoolBufferHeader->m_pThis);

        MemLib::vCopy(pBuffer, pPoolBufferHeader, nSizeHeader, pPoolBufferHeader->eGetClassNo());
        aBody = aAdd<void *>(pBuffer, nSizeHeader);
        nBytes = pFile->nReadBytes(aBody, nSizeBody);
        RASSERT(nBytes == nSizeBody);

        Persistent.m_pMedium = reinterpret_cast<Heap*>(pBuffer);

        break;
      }

      case CN_LARGEPERSISTENT: {
        RASSERT(gpBufferPool->bReloadSanityCheckAddr(pPoolBufferHeader->m_pThis) );
        PoolBuffer *pBuffer = reinterpret_cast<PoolBuffer*>(pPoolBufferHeader->m_pThis);

        MemLib::vCopy(pBuffer, pPoolBufferHeader, nSizeHeader, pPoolBufferHeader->eGetClassNo());
        aBody = aAdd<void *>(pBuffer, nSizeHeader);
        nBytes = pFile->nReadBytes(aBody, nSizeBody);
        RASSERT(nBytes == nSizeBody);

        Persistent.m_pLarge = reinterpret_cast<Heap*>(pBuffer);

        // Now that the Persistent heaps have been loaded in, we want to exit this loop
        // and spawn threads to read in the rest of the buffers.
        // So, force the loop to break by setting nBytes to zero.
        nBytes = 0;
        continue;
      }

      default:
        THROW("Error: Read invalid object during reload of Persistent Heaps..\n"_Stz);
    }
    nBytes = pFile->nReadBytes(aHeader, nSizeHeader);
  }

  RASSERT(Persistent.m_pTiny);
  RASSERT(Persistent.m_pSmall);
  RASSERT(Persistent.m_pMedium);
  RASSERT(Persistent.m_pLarge);

  // Main thread's work is done, so we close the file.
  if(pFile->bIsOpen())
    pFile->vClose();
  delete pFile;

  i8 iNoPtns = gxCmdLineArgs.m_xSysArgs.m_nPtnsPerNode;

  // We are using an atomic counter to figure out when the threads have
  // finished their work.
  gxThreadsRemaining.vSetRemaining(iNoPtns);

  // Spawn the threads.
  for(i8 i = 0; i < iNoPtns; i++)
  {
    PsxThd::iSpawnPosixThread(pLoadHeapBuffers, (void *) i);
  }

  vTrace("Main thread waiting...\n"_Stz);
  do{
    sleep(1);
  } while(gxThreadsRemaining.bIsSet());

  Persistent.m_pTiny->vDropWriteLock();
  Persistent.m_pSmall->vDropWriteLock();
  Persistent.m_pMedium->vDropWriteLock();
  Persistent.m_pLarge->vDropWriteLock();

  // Clean up the free chain
  gpBufferPool->vCleanup(false, true);
}
//__attribute__((optimize("O0")))
/*----------------------------------------------------------------------------*/
/* Threads call this function, to load heap buffers from disk                 */
/*----------------------------------------------------------------------------*/
void* pLoadHeapBuffers(void *pFuncObj)
{

  i8 iPtnNo = (i8) pFuncObj;
  i8 nDiskpaths = gxCmdLineArgs.m_xSysArgs.m_nDiskPaths;

  s256 szFilePath = ""_Stz;
  szFilePath.vSprint("%s%ssaveFile_%s_%lld"_Stz,
                     gxCmdLineArgs.m_xSysArgs.m_sDiskRootDir[iPtnNo % nDiskpaths].szStz(),
                     gxCmdLineArgs.m_xSysArgs.m_sReloadDir.szStz(),
                     gxCmdLineArgs.m_xSysArgs.m_sNodeName.szStz(),
                     iPtnNo);

  // Search the other disks for the reload file if we can't find it in the current dir
  if( ! File::bExists(szFilePath)) {
    i8 i = 0;
    for (i = 0; i < nDiskpaths; i++) {
      Stz *sRootDir = gxCmdLineArgs.m_xSysArgs.m_sDiskRootDir[i];
      szFilePath.vSprint("%s%ssaveFile_%s_%lld"_Stz,
                         sRootDir,
                         gxCmdLineArgs.m_xSysArgs.m_sReloadDir.szStz(),
                         gxCmdLineArgs.m_xSysArgs.m_sNodeName.szStz(),
                         iPtnNo);
      if (File::bExists(szFilePath))
        break;
    }
    if(i >= nDiskpaths)
    {
      vTrace("%s does not exist. Please check the arguments for --disk and --reload options given\n"_Stz, szFilePath.szStz());
      THROW("Could not open savefile. Please check the arguments for --disk and --reload options given\n"_Stz);
    }
  }

  if(AM::AM::TRACE::bVerbose(C_VB_BOOT))
    vTrace("Thread got filepath %s\n"_Stz, szFilePath.szStz());

  i8 nSizeHeader = sizeof(PoolBuffer);

  char caObjectHeader[nSizeHeader];

  Header* aHeader = (Header*) & caObjectHeader[0];

  i8 nSizeBody = ((i8) C_SIZE_POOL_BUFFER) - nSizeHeader;

  File* pFile = new(C_VIA_MALLOC) File(nullptr);
  bool bResult = pFile->bOpenRead(szFilePath);
  RASSERT(bResult);

  if(iPtnNo == 0)
  {
    // The saveFile_0 contains the persistent heaps at the start of the file.
    // The main thread first loads these heaps into memory.
    // Hence, when this thread is loading the rest of the buffers, it needs
    // to seek past the first four buffers (PTiny, PSmall, PMedium, PLarge).
    pFile->vSeek((((i8) C_SIZE_POOL_BUFFER) * 4));
  }

  // Read the Header information to get the address of this pool buffer
  i8 nBytes = pFile->nReadBytes(aHeader, nSizeHeader);
  RASSERT(nBytes == nSizeHeader);

  while(nBytes == nSizeHeader)
  {
    PoolBuffer* pPoolBufferHeader = reinterpret_cast<PoolBuffer*>(aHeader);
    void *aBody;

    if(pPoolBufferHeader->eGetClassNo() == CN_HEAPBUFFER)
    {
      RASSERT(gpBufferPool->bReloadSanityCheckAddr(pPoolBufferHeader->m_pThis) );
      PoolBuffer *pPoolBuffer = pPoolBufferHeader->m_pThis;

      MemLib::vCopy(pPoolBuffer, pPoolBufferHeader, nSizeHeader, pPoolBufferHeader->eGetClassNo());
      aBody = aAdd<void *>(pPoolBuffer, nSizeHeader);
      nBytes = pFile->nReadBytes(aBody, nSizeBody);
      RASSERT(nBytes == nSizeBody);
    } else {
      vTrace("Thread %lld read a non-heap buffer...\n"_Stz, iPtnNo);
      THROW("Thread read a non-heap buffer...\n"_Stz);
    }

    nBytes = pFile->nReadBytes(aHeader, nSizeHeader);
  }

  if(pFile->bIsOpen())
    pFile->vClose();
  delete pFile;

  // Atomic decrement. Last thread to finish causes the main thread to wake up.
  i8 nAcksWereRemaining;
  gxThreadsRemaining.bDecAndDecide(nAcksWereRemaining);
  RASSERT(nAcksWereRemaining >= 0);

  return nullptr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
