/*============================================================================*/
/* Job_SubPurger.cpp -- Sub Purge Job                                         */
/*============================================================================*/

#include "Jobs.h"

JobClass xJobClass_SubPurger =
{
  CN_JOB_SUBPURGER,       // class number
  JK_CHILD,               // job kind
  RS_NODE_ERRORS,         // response kind
  "table sub-purger"_Stz, // comment about the job
  "_SUBPURGER"_Stz,       // the prompt command
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
  DL_NONE,                // ddl lock model
  PY_NORMAL,              // priority
  JF_NONE,                // flags

  nullptr,                // prompt check function
  pJob_SubPurger_pre,     // prompt pre-process function
  vJob_post,              // prompt post-process function
  nullptr,                // job new function
  vJob_SubPurger_delete,  // job delete function
  nullptr,                // job pre-send function
  nullptr,                // job post-receive function
  vJob_SubPurger_show,    // job show function
  
  // help and example
  true,
  C_NO_NAME,
  C_NO_NAME,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_SubPurger_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_SubScan_class;
  
  return &xJobClass_SubPurger;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_SubPurger* Jobs::_pJob_SubPurger(void* aAddress)
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
Job_SubPurger::Job_SubPurger(JobClass* aJobClass, ReqTuple* aReqTuple)
: Job(aJobClass, aReqTuple)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBPURGER,_FOOTER_);
    
  m_iPurgeJobId = -1;
  
  m_pJob_Purge  = nullptr;
  m_pTable      = nullptr;
  m_nPurgeCount = 0;
  
  _checkInitialised_(CN_JOB_SUBPURGER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBPURGER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_SubPurger::~Job_SubPurger(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBPURGER,_FOOTER_);
  
  //vTrace("Sub_Purger %lld ~destructor\n"_Stz,m_i_Slot);
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
/* begin sub purger                                                           */
/*----------------------------------------------------------------------------*/
bool Job_SubPurger::bBegin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBPURGER,_FOOTER_);
  
  Job* pJobFound = AM::pParticipatingJobs()->pFindJob(m_iPurgeJobId);
  RASSERT(pJobFound);
  
  m_pJob_Purge = _pJob_Purge(pJobFound);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* attempt to purge oldest buffer                                             */
/*----------------------------------------------------------------------------*/
bool Job_SubPurger::bChunk(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBPURGER,_FOOTER_);
  
  i8 iOlderThanSeconds = m_pJob_Purge->m_iOlderThanSeconds;
  
  bool bPurged = m_pTable->bPurge(i_Slot(),iOlderThanSeconds);
  
  if (bPurged) {
    m_nPurgeCount++;
    if (0 == m_nPurgeCount % 250) {
      vTrace("SubPurger %lld: Purged %lld buffers from %s\n"_Stz,
             i_Slot(),m_nPurgeCount,m_pTable->szTableNm()->azChar());
    }
  }
  
//  if (! bPurged) {
//    PsxThd::vSleepMilli(10);
//    return true;
//  }

  return bPurged;
}

/*----------------------------------------------------------------------------*/
/* end sub purger                                                             */
/*----------------------------------------------------------------------------*/
void Job_SubPurger::vEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBPURGER,_FOOTER_);
  
  vTrace("Sub_Purger %lld ends\n"_Stz,i_Slot());
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
Job* Jobs::pJob_SubPurger_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession)
{
  Job_SubPurger* pJob_SubPurger = new Job_SubPurger(aJobClass,aReqTuple);
  //pJob->vDicPath(pSession);

  return pJob_SubPurger;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_SubPurger_delete(Job* pJob)
{
  Job_SubPurger* pJob_SubPurger = _pJob_SubPurger(pJob);
  
  delete pJob_SubPurger;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_SubPurger_show(Job* pJob)
{
  Job_SubPurger* pJob_SubPurger = _pJob_SubPurger(pJob);

  pJob_SubPurger->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
