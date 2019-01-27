/*============================================================================*/
/* Job_Purge.cpp -- Purge Job                                                 */
/*============================================================================*/

// Job Short: Ingest

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_Purge =
{
  "Job_Purge"_Stz,       // name of the job class
  CN_JOB_PURGE,          // class enum
  JK_DIST2PC,            // job kind
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
  JT_NONE,               // job trace
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
  *pNextFun = (pFUN*)pJob_Pwf_class;
  
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
Job_Purge::Job_Purge(Interface::Session* pSession)
: Job(CN_JOB_PURGE, pSession)
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
Job* Jobs::pJob_Purge_pre(Interface::Session* pSession)
{
  i8 nTokens;

  Job_Purge* pJob_Purge = new Job_Purge(pSession);

  TokenVec* pTknVec = pSession->m_pCommand->pTknVec();

  // get no of tokens in token vector
  nTokens = pTknVec->nTokens();

  // there should be 3 tokens
  if (nTokens < 3)
    return pJob_Purge;

  // set the table name
  pJob_Purge->m_sObjectName = pTknVec->pGet(1)->aText();

  pJob_Purge->m_iOlderThanSeconds = pTknVec->pGet(2)->iToI8();

  return pJob_Purge;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Purge_post(Interface::Session* pSession, Job* pJob)
{
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Purge_new(Job* pArg)
{
  Job_Purge* pJob_Purge = new Job_Purge(nullptr);
  
  return pJob_Purge;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Purge_delete(Interface::Session* pSession, Job* pJob)
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
