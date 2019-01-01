/*============================================================================*/
/* Job_SubPurger.cpp -- Sub Purge Job                                          */
/*============================================================================*/

// Job Short: SubPurge

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_SubPurger =
{
  "Job_SubPurger"_Stz,    // name of the job class
  CN_JOB_SUBPURGER,       // class enum
  JK_CHILD,               // job kind
  "table sub-purger"_Stz, // comment about the job
  nullptr,                // the prompt command
  nullptr,                // the second word
  "subPurger_run"_Stz,    // the _run task seed
  nullptr,                // the _req task seed
  nullptr,                // the _rcv task seed
  nullptr,                // the _exe task seed
  nullptr,                // the _end task seed
  nullptr,                // the _can task seed
  false,                  // check resources?
  false,                  // detached job?
  false,                  // buffer attached?
  JT_NONE,                // job trace
  pJob_SubPurger_pre,     // prompt pre-process function
  vJob_SubPurger_post,    // prompt post-process function
  nullptr,                // job new function
  vJob_SubPurger_delete,  // job delete function
  nullptr,                // job pre-send function
  nullptr,                // job post-receive function
  vJob_SubPurger_show,    // job show function
  
  // help and example
  false,
  "<none>"_Stz,
  "<none>"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* JobModel::pJob_SubPurger_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_SubTruncate_class;
  
  return &xJobClass_SubPurger;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_SubPurger* JobModel::_pJob_SubPurger(void* aAddress)
{
  Job_SubPurger* pJob_SubPurger = reinterpret_cast<Job_SubPurger*>(aAddress);
  
  RASSERT_TYPE(pJob_SubPurger,CN_JOB_SUBPURGER);
  
  return pJob_SubPurger;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_SubPurger::operator new(size_t)
{
  Job_SubPurger* pJob_SubPurger =
    static_cast<Job_SubPurger*>(HeapMem::pAllocate(CN_JOB_SUBPURGER));
  
  return pJob_SubPurger;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_SubPurger::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_SubPurger::operator delete(void* aAddress)
{
  Job_SubPurger* pJob_SubPurger = static_cast<Job_SubPurger*>(aAddress);
  
  HeapMem::vFree(pJob_SubPurger);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_SubPurger::Job_SubPurger(Interface::Session* pSession)
: Job(CN_JOB_SUBPURGER, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBPURGER,_FOOTER_);
    
  m_iPurgeJobId = -1;
  m_iSlot       = -1;
  
  _checkInitialised_(CN_JOB_SUBPURGER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBPURGER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_SubPurger::~Job_SubPurger(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBPURGER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_SubPurger::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_SubPurger* pJob_SubPurger = _pJob_SubPurger(pObject);

  pJob_SubPurger->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_SubPurger::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBPURGER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_SubPurger::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBPURGER);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobModel::pJob_SubPurger_pre(Interface::Session* pSession)
{
  Job_SubPurger* pJob_SubPurger = new Job_SubPurger(pSession);
  
  return pJob_SubPurger;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_SubPurger_post(Interface::Session* pSession, Job* pJob)
{
  Job_SubPurger* pJob_SubPurger = _pJob_SubPurger(pJob);
  
  if ( pJob_SubPurger->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_SubPurger->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_SubPurger_delete(Interface::Session* pSession, Job* pJob)
{
  Job_SubPurger* pJob_SubPurger = _pJob_SubPurger(pJob);
  
  delete pJob_SubPurger;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_SubPurger_show(Job* pJob)
{
  Job_SubPurger* pJob_SubPurger = _pJob_SubPurger(pJob);

  pJob_SubPurger->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
