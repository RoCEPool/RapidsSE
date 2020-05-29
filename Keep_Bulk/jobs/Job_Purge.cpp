/*============================================================================*/
/* Job_Purge.cpp -- Purge Job                                                 */
/*============================================================================*/

#include "Jobs.h"

JobClass xJobClass_Purge =
{
  CN_JOB_PURGE,          // class number
  JK_DIST2PC,            // job kind
  RS_NONE,               // response kind
  "purge table"_Stz,     // comment about the job
  "purge"_Stz,           // the prompt command
  nullptr,               // the second word
  nullptr,               // the _run task seed
  "purge_req"_Stz,       // the _req task seed
  "purge_beg"_Stz,       // the _rcv task seed
  "purge_exe"_Stz,       // the _exe task seed
  "purge_end"_Stz,       // the _end task seed
  "purge_can"_Stz,       // the _can task seed
  true,                  // check resources?
  false,                 // detached job?
  false,                 // buffer attached?
  DL_GRAB_READ,          // ddl lock model
  PY_NORMAL,             // priority
  JF_NONE,               // flags

  eJob_checkCluster,     // prompt check function
  pJob_Purge_pre,        // prompt pre-process function
  vJob_Purge_post,       // prompt post-process function
  pJob_Purge_new,        // job new function
  vJob_Purge_delete,     // job delete function
  vJob_send,             // job pre-send function
  vJob_recv,             // job post-receive function
  vJob_Purge_show,       // job show function
  
  // help and example
  true,
  "purge <tableNm>"_Stz,
  "purge SensorEvents;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_Purge_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Receiver_class;
  
  return &xJobClass_Purge;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Purge* Jobs::_pJob_Purge(void* aAddress)
{
  Job_Purge* pJob_Purge = reinterpret_cast<Job_Purge*>(aAddress);
  
  RASSERT_TYPE(pJob_Purge,CN_JOB_PURGE);
  
  return pJob_Purge;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Purge::operator new(size_t)
{
  Job_Purge* pJob_Purge =
    static_cast<Job_Purge*>(HeapMem::pAllocate(CN_JOB_PURGE));
  
  return pJob_Purge;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Purge::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Purge::operator delete(void* aAddress)
{
  Job_Purge* pJob_Purge = static_cast<Job_Purge*>(aAddress);
  
  HeapMem::vFree(pJob_Purge);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Purge::Job_Purge(JobClass* aJobClass, ReqTuple* aReqTuple)
: Job(aJobClass, aReqTuple)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PURGE,_FOOTER_);
  
  m_pTable = nullptr;
  m_iOlderThanSeconds = 0;
  
  _checkInitialised_(CN_JOB_PURGE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_PURGE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Purge::~Job_Purge(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PURGE,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Purge::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_Purge* pJob_Purge = _pJob_Purge(pObject);

  pJob_Purge->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Purge::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PURGE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Purge::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PURGE);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Purge_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession)
{
  i8 nTokens;

  Job_Purge* pJob_Purge = new Job_Purge(aJobClass,aReqTuple);
  //pJob->vDicPath(pSession);

  TokenVec* pTknVec = pSession->m_pInput->pTknVec();

  // get no of tokens in token vector
  nTokens = pTknVec->nTokens();

  // there should be 3 tokens
  if (nTokens < 3)
    return pJob_Purge;

  // set the table name
  pJob_Purge->m_oEnameSet.vAdd("<tableNm>"_Stz, pTknVec->pGet(1)->szText());
  pJob_Purge->m_iOlderThanSeconds = pTknVec->pGet(2)->iToI8();

  return pJob_Purge;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Purge_post(Job* pJob)
{
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Purge_new(JobClass* aJobClass, ReqTuple* aReqTuple)
{
  Job_Purge* pJob_Purge = new Job_Purge(aJobClass,nullptr);
  
  return pJob_Purge;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Purge_delete(Job* pJob)
{
  Job_Purge* pJob_Purge = _pJob_Purge(pJob);
  
  delete pJob_Purge;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Purge_show(Job* pJob)
{
  Job_Purge* pJob_Purge = _pJob_Purge(pJob);

  pJob_Purge->vShow();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get table                                                                  */
/*----------------------------------------------------------------------------*/
Table* Job_Purge::pTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PURGE,_FOOTER_);
  
  return m_pTable;
}

/*----------------------------------------------------------------------------*/
/* set table                                                                  */
/*----------------------------------------------------------------------------*/
void Job_Purge::vTable(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PURGE,_FOOTER_);
  
  m_pTable = pTable;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
