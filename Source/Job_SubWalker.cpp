/*============================================================================*/
/* Job_SubWalker.cpp -- Sub Walker Job                                        */
/*============================================================================*/

// Job Short: SubWalker

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_SubWalker =
{
  CN_JOB_SUBWALKER,          // class enum
  JK_CHILD,                  // job kind
  "sub-walker"_Stz,          // comment about the job
  nullptr,                   // the prompt command
  nullptr,                   // the second word
  "subWalker_run"_Stz,       // the _run task seed
  nullptr,                   // the _req task seed
  nullptr,                   // the _rcv task seed
  nullptr,                   // the _exe task seed
  nullptr,                   // the _end task seed
  nullptr,                   // the _can task seed
  false,                     // check resources?
  false,                     // detached job?
  false,                     // buffer attached?
  JT_NONE,                   // job trace
  DL_NONE,                   // ddl lock model
  PY_NORMAL,                 // priority
  JF_NONE,                   // flags

  nullptr,                   // prompt check function
  pJob_SubWalker_pre,        // prompt pre-process function
  vJob_SubWalker_post,       // prompt post-process function
  nullptr,                   // job new function
  vJob_SubWalker_delete,     // job delete function
  nullptr,                   // job pre-send function
  nullptr,                   // job post-receive function
  vJob_SubWalker_show,       // job show function
  
  // help and example
  false,
  "<none>"_Stz,
  "<none>"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_SubWalker_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Supervisor_class;
  
  return &xJobClass_SubWalker;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_SubWalker* Jobs::_pJob_SubWalker(void* aAddress)
{
  Job_SubWalker* pJob_TruncateTable = reinterpret_cast<Job_SubWalker*>(aAddress);
  
  RASSERT_TYPE(pJob_TruncateTable,CN_JOB_SUBWALKER);
  
  return pJob_TruncateTable;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_SubWalker::operator new(size_t)
{
  Job_SubWalker* pJob_SubWalker =
    static_cast<Job_SubWalker*>(HeapMem::pAllocate(CN_JOB_SUBWALKER));
  
  return pJob_SubWalker;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_SubWalker::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_SubWalker::operator delete(void* aAddress)
{
  Job_SubWalker* pJob_SubWalker = static_cast<Job_SubWalker*>(aAddress);
  
  HeapMem::vFree(pJob_SubWalker);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_SubWalker::Job_SubWalker(Interface::Session* pSession)
: Job(CN_JOB_SUBWALKER, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBWALKER,_FOOTER_);
    
  m_pJobWalk     = nullptr;
  m_pIndexTree   = nullptr;
  m_pEntryBuffer = nullptr;
  
  _checkInitialised_(CN_JOB_SUBWALKER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBWALKER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_SubWalker::~Job_SubWalker(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBWALKER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_SubWalker::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_SubWalker* pJob_SubWalker = _pJob_SubWalker(pObject);

  pJob_SubWalker->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_SubWalker::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBWALKER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* begin sub purger                                                           */
/*----------------------------------------------------------------------------*/
bool Job_SubWalker::bBegin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBWALKER,_FOOTER_);
  
//  TablePtn* pTablePtn = m_pJobWalk->m_pTable->pSlots()->pPartition(m_i_Slot);
//
//  m_pIndexTree = pTablePtn->pIndexTree(m_pJobWalk->m_pIndex->iIndexNo());
//
//  m_pEntryBuffer = m_pIndexTree->pFirstH0Buffer();
  
  return true;
}

#define C_LINES (C_1M * 10)

i8* apiCacheRnd[C_LINES+2048];
i8* apiCacheSeq[C_LINES+2048];

/*----------------------------------------------------------------------------*/
/* attempt to purge oldest buffer                                             */
/*----------------------------------------------------------------------------*/
bool Job_SubWalker::bChunk(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBWALKER,_FOOTER_);
  
//  if (! m_pEntryBuffer)
//    return false;
//
//  for (Item* aItem = m_pEntryBuffer->aFirstItem();
//       aItem;
//       aItem = m_pEntryBuffer->aAdjacentItem(aItem))
//    {;}
//
//  //vTrace("SubWalker[%lld]: %p\n"_Stz,m_i_Slot,m_pEntryBuffer);
//
//  m_pEntryBuffer = m_pEntryBuffer->pNextBuffer();
//
//  return true;
  
  i8 i_CacheLine = 0;
  i8 iSum = 0;
  Timer oTimer;

  for (i_CacheLine = 0; i_CacheLine < C_LINES+2048; i_CacheLine++) {
    apiCacheRnd[i_CacheLine] = (i8*)gpAttachMap + Random::iInteger(C_LINES) * 8;
    apiCacheSeq[i_CacheLine] = (i8*)gpAttachMap + i_CacheLine * 8;
    //vTrace("%p\n"_Stz,apiCacheRnd[i_CacheLine]);
  }
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // prefetch T0
  vTrace("-- prefetch T0 ----------------------------------------------\n"_Stz);
  
  const i8 C_DIST  = 2048;
  const i8 C_AHEAD = 2048;
  const i8 C_PRINT = 0;
  const i8 C_PRE   = 0;
  
  for (i8 i_Dist = 2; i_Dist < C_DIST; i_Dist *= 2) {

    for (i8 nAhead = 1; nAhead < C_AHEAD; nAhead *= 2) {
      
      if (C_PRE) {
        for (i8 nfetch = 0; nfetch < i_Dist*nAhead; nfetch++) {
          i8 i_Prefetch = 0 + 0 + nfetch;
          i8* piPre = apiCacheRnd[i_Prefetch];
          iSum += (i8)piPre;
          vPrefetchT2(piPre);
          if (C_PRINT)
            vTrace("i_Prefetch %lld %p\n"_Stz,i_Prefetch,piPre);
        }
        iCpuid(0,0);
      }

      oTimer.vStart();
      
      for (i_CacheLine = 0; i_CacheLine < C_LINES; i_CacheLine++) {
        
        if (C_PRE) {
          if (0 == i_CacheLine % i_Dist) {
            //iCpuid(0,0);
            for (i8 nFetch = 0; nFetch < i_Dist; nFetch++) {
              i8 i_Prefetch = i_CacheLine + i_Dist*nAhead + nFetch;
              i8* piPre = apiCacheRnd[i_Prefetch];
              iSum += (i8)piPre;
              vPrefetchT2(piPre);
              if (C_PRINT)
                vTrace("i_Prefetch %lld %p\n"_Stz,i_Prefetch,piPre);
            }
          }
        }
        
        i8* piCacheLine = apiCacheSeq[i_CacheLine];
        iSum += *piCacheLine;
        if (C_PRINT)
          vTrace("i_Cache %lld %p\n"_Stz,i_CacheLine,piCacheLine);
      }
      
      oTimer.vUpdate();
      vTrace("i_Dist %lld Ahead %lld Time %lld "
             ": Lines %lld sum %lld (%lldns/f)\n"_Stz,
             i_Dist,nAhead,oTimer.iMilliseconds(),
             C_LINES,iSum,oTimer.iNanoseconds()/C_LINES);
    }
  }
  

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // sequential
  vTrace("-- sequential -----------------------------------------------\n"_Stz);
  for (i8 i_Dist = 1; i_Dist < 2048; i_Dist *= 2) {
    oTimer.vStart();
    for (i_CacheLine = 0; i_CacheLine < C_LINES; i_CacheLine++) {
      i8* piPre = apiCacheRnd[i_CacheLine + i_Dist];
      iSum += (i8)piPre;
      //vPrefetchT0(piPre);
      i8* piCacheLine = apiCacheSeq[i_CacheLine];
      iSum += *piCacheLine;
    }
    oTimer.vUpdate();
    vTrace("i_Dist %lld Time %lld : i_CacheLines %lld sum %lld (%lldns/f)\n"_Stz,
           i_Dist,oTimer.iMilliseconds(),i_CacheLine,iSum,oTimer.iNanoseconds()/C_LINES);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // no prefetch
  vTrace("-- no prefetch ----------------------------------------------\n"_Stz);
  for (i8 i_Dist = 1; i_Dist < 2048; i_Dist *= 2) {
    oTimer.vStart();
    for (i_CacheLine = 0; i_CacheLine < C_LINES; i_CacheLine++) {
      i8* piPre = apiCacheRnd[i_CacheLine + i_Dist];
      iSum += (i8)piPre;
      //vPrefetchT0(piPre);
      i8* piCacheLine = apiCacheRnd[i_CacheLine];
      iSum += *piCacheLine;
    }
    oTimer.vUpdate();
    vTrace("i_Dist %lld Time %lld : i_CacheLines %lld sum %lld (%lldns/f)\n"_Stz,
           i_Dist,oTimer.iMilliseconds(),i_CacheLine,iSum,oTimer.iNanoseconds()/C_LINES);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // prefetch T0
  vTrace("-- prefetch T0 ----------------------------------------------\n"_Stz);
  for (i8 i_Dist = 1; i_Dist < 2048; i_Dist *= 2) {
    oTimer.vStart();
    for (i_CacheLine = 0; i_CacheLine < C_LINES; i_CacheLine++) {
      i8* piPre = apiCacheRnd[i_CacheLine + i_Dist];
      iSum += (i8)piPre;
      vPrefetchT0(piPre);
      i8* piCacheLine = apiCacheRnd[i_CacheLine];
      iSum += *piCacheLine;
    }
    oTimer.vUpdate();
    vTrace("i_Dist %lld Time %lld : i_CacheLines %lld sum %lld (%lldns/f)\n"_Stz,
           i_Dist,oTimer.iMilliseconds(),i_CacheLine,iSum,oTimer.iNanoseconds()/C_LINES);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // prefetch T1
  vTrace("-- prefetch T1 ----------------------------------------------\n"_Stz);
  for (i8 i_Dist = 1; i_Dist < 2048; i_Dist *= 2) {
    oTimer.vStart();
    for (i_CacheLine = 0; i_CacheLine < C_LINES; i_CacheLine++) {
      i8* piPre = apiCacheRnd[i_CacheLine + i_Dist];
      iSum += (i8)piPre;
      vPrefetchT1(piPre);
      i8* piCacheLine = apiCacheRnd[i_CacheLine];
      iSum += *piCacheLine;
    }
    oTimer.vUpdate();
    vTrace("i_Dist %lld Time %lld : i_CacheLines %lld sum %lld (%lldns/f)\n"_Stz,
           i_Dist,oTimer.iMilliseconds(),i_CacheLine,iSum,oTimer.iNanoseconds()/C_LINES);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // prefetch T2
  vTrace("-- prefetch T2 ----------------------------------------------\n"_Stz);
  for (i8 i_Dist = 1; i_Dist < 2048; i_Dist *= 2) {
    oTimer.vStart();
    for (i_CacheLine = 0; i_CacheLine < C_LINES; i_CacheLine++) {
      i8* piPre = apiCacheRnd[i_CacheLine + i_Dist];
      iSum += (i8)piPre;
      vPrefetchT2(piPre);
      i8* piCacheLine = apiCacheRnd[i_CacheLine];
      iSum += *piCacheLine;
    }
    oTimer.vUpdate();
    vTrace("i_Dist %lld Time %lld : i_CacheLines %lld sum %lld (%lldns/f)\n"_Stz,
           i_Dist,oTimer.iMilliseconds(),i_CacheLine,iSum,oTimer.iNanoseconds()/C_LINES);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // prefetch NTA
  vTrace("-- prefetch NTA ---------------------------------------------\n"_Stz);
  for (i8 i_Dist = 1; i_Dist < 2048; i_Dist *= 2) {
    oTimer.vStart();
    for (i_CacheLine = 0; i_CacheLine < C_LINES; i_CacheLine++) {
      i8* piPre = apiCacheRnd[i_CacheLine + i_Dist];
      iSum += (i8)piPre;
      vPrefetchNta(piPre);
      i8* piCacheLine = apiCacheRnd[i_CacheLine];
      iSum += *piCacheLine;
    }
    oTimer.vUpdate();
    vTrace("i_Dist %lld Time %lld : i_CacheLines %lld sum %lld (%lldns/f)\n"_Stz,
           i_Dist,oTimer.iMilliseconds(),i_CacheLine,iSum,oTimer.iNanoseconds()/C_LINES);
  }
  
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  //if (iSum == 1)
    vTrace("i_CacheLines %lld sum %lld\n"_Stz,i_CacheLine,iSum);
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* end sub purger                                                             */
/*----------------------------------------------------------------------------*/
void Job_SubWalker::vEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBWALKER,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_SubWalker::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBWALKER);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_SubWalker_pre(Interface::Session* pSession)
{
  Job_SubWalker* pJob_SubWalker = new Job_SubWalker(pSession);
  
  return pJob_SubWalker;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_SubWalker_post(Interface::Session* pSession, Job* pJob)
{
  Job_SubWalker* pJob_SubWalker = _pJob_SubWalker(pJob);
  
  if ( pJob_SubWalker->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_SubWalker->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_SubWalker_delete(Interface::Session* pSession, Job* pJob)
{
  Job_SubWalker* pJob_SubWalker = _pJob_SubWalker(pJob);
  
  delete pJob_SubWalker;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_SubWalker_show(Job* pJob)
{
  Job_SubWalker* pJob_SubWalker = _pJob_SubWalker(pJob);

  pJob_SubWalker->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
