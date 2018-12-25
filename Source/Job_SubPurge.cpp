/*============================================================================*/
/* Job_SubPurge.cpp -- Sub Purge Job                                          */
/*============================================================================*/

// Job Short: SubPurge

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_SubPurge =
{
  "Job_SubPurge"_Stz,     // name of the job class
  CN_JOB_SUBPURGE,        // class enum
  JK_CHILD,               // job kind
  "table sub-purge"_Stz,  // comment about the job
  nullptr,                // the prompt command
  nullptr,                // the second word
  "subPurge_run"_Stz,     // the _run task seed
  nullptr,                // the _req task seed
  nullptr,                // the _rcv task seed
  nullptr,                // the _exe task seed
  nullptr,                // the _end task seed
  nullptr,                // the _can task seed
  false,                  // check resources?
  false,                  // detached job?
  false,                  // buffer attached?
  JT_NONE,                // job trace
  pJob_SubPurge_pre,      // prompt pre-process function
  vJob_SubPurge_post,     // prompt post-process function
  nullptr,                   // job new function
  vJob_SubPurge_delete,   // job delete function
  nullptr,                   // job pre-send function
  nullptr,                   // job post-receive function
  vJob_SubPurge_show,     // job show function
  
  // help and example
  false,
  "<none>"_Stz,
  "<none>"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* JobModel::pJob_SubPurge_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_SubTruncate_class;
  
  return &xJobClass_SubPurge;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_SubPurge* JobModel::_pJob_SubPurge(void* aAddress)
{
  Job_SubPurge* pJob_SubPurge = reinterpret_cast<Job_SubPurge*>(aAddress);
  
  RASSERT_TYPE(pJob_SubPurge,CN_JOB_SUBPURGE);
  
  return pJob_SubPurge;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_SubPurge::operator new(size_t)
{
  Job_SubPurge* pJob_SubPurge =
    static_cast<Job_SubPurge*>(HeapMem::pAllocate(CN_JOB_SUBPURGE));
  
  return pJob_SubPurge;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_SubPurge::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_SubPurge::operator delete(void* aAddress)
{
  Job_SubPurge* pJob_SubPurge = static_cast<Job_SubPurge*>(aAddress);
  
  HeapMem::vFree(pJob_SubPurge);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_SubPurge::Job_SubPurge(Interface::Session* pSession)
: Job(CN_JOB_SUBPURGE, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBPURGE,_FOOTER_);
    
  m_iPurgeJobId = -1;
  m_iSlot       = -1;
  
  _checkInitialised_(CN_JOB_SUBPURGE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBPURGE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_SubPurge::~Job_SubPurge(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBPURGE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_SubPurge::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_SubPurge* pJob_SubPurge = _pJob_SubPurge(pObject);

  pJob_SubPurge->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_SubPurge::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBPURGE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_SubPurge::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBPURGE);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobModel::pJob_SubPurge_pre(Interface::Session* pSession)
{
  Job_SubPurge* pJob_SubPurge = new Job_SubPurge(pSession);
  
  return pJob_SubPurge;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_SubPurge_post(Interface::Session* pSession, Job* pJob)
{
  Job_SubPurge* pJob_SubPurge = _pJob_SubPurge(pJob);
  
  if ( pJob_SubPurge->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_SubPurge->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_SubPurge_delete(Interface::Session* pSession, Job* pJob)
{
  Job_SubPurge* pJob_SubPurge = _pJob_SubPurge(pJob);
  
  delete pJob_SubPurge;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_SubPurge_show(Job* pJob)
{
  Job_SubPurge* pJob_SubPurge = _pJob_SubPurge(pJob);

  pJob_SubPurge->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
