/*============================================================================*/
/* Job_SubWalker.cpp -- Sub Walker Job                                        */
/*============================================================================*/

// Job Short: SubWalker

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_SubWalker =
{
  "Job_SubWalker"_Stz,       // name of the job class
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
  *pNextFun = (pFUN*)pJob_TableSizes_class;
  
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
  
  TablePtn* pTablePtn = m_pJobWalk->m_pTable->pSlots()->pPartition(m_i_Slot);
  
  m_pIndexTree = pTablePtn->pIndexTree(m_pJobWalk->m_pIndex->iIndexNo());
  
  m_pEntryBuffer = m_pIndexTree->pFirstH0Buffer();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* attempt to purge oldest buffer                                             */
/*----------------------------------------------------------------------------*/
bool Job_SubWalker::bChunk(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBWALKER,_FOOTER_);
  
  if (! m_pEntryBuffer)
    return false;

  //vTrace("SubWalker[%lld]: %p\n"_Stz,m_i_Slot,m_pEntryBuffer);
  
  m_pEntryBuffer = m_pEntryBuffer->pNextBuffer();
  
  return true;
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
