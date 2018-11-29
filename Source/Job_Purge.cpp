/*============================================================================*/
/* Job_Purge.cpp -- Purge Job                                                 */
/*============================================================================*/

// Job Short: Ingest

#include "JobsModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_Purge =
{
  "Job_Purge"_Stz,       // name of the job class
  CN_JOB_PURGE,          // class enum
  JK_LOCAL,              // job kind
  "purge table"_Stz,     // comment about the job
  "purge"_Stz,           // the prompt command
  nullptr,               // the second word
  nullptr,               // the _run task seed
  "purge_req"_Stz,       // the _req task seed
  nullptr,               // the _rcv task seed
  nullptr,               // the _exe task seed
  nullptr,               // the _end task seed
  nullptr,               // the _can task seed
  false,                 // check resources?
  false,                 // detached job?
  false,                 // buffer attached?
  JT_NONE,               // job trace
  pJob_Purge_pre,        // prompt pre-process function
  vJob_Purge_post,       // prompt post-process function
  pJob_Purge_new,        // job new function
  vJob_Purge_delete,     // job delete function
  nullptr,               // job pre-send function
  nullptr,               // job post-receive function
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
JobClass* JobsModel::pJob_Purge_class(pFUN** pNextFun)
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
Job_Purge* JobsModel::_pJob_Purge(void* aAddress)
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
Job* JobsModel::pJob_Purge_pre(Interface::Session* pSession)
{
  i8 nTokens;

  Job_Purge* pJob_Purge = new Job_Purge(pSession);

  TokenVec* pTknVec = pSession->m_pCommandProcessor->pTknVec();

  // get no of tokens in token vector
  nTokens = pTknVec->nTokens();

  // there should be 2 tokens
  if (nTokens < 2)
    return pJob_Purge;

  // set the table name
  pJob_Purge->m_sObjectName = pTknVec->pGet(1)->aText();

  return pJob_Purge;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_Purge_post(Interface::Session* pSession, Job* pJob)
{
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_Purge_new(Job* pArg)
{
  Job_Purge* pJob_Purge = new Job_Purge(nullptr);
  
  return pJob_Purge;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_Purge_delete(Interface::Session* pSession, Job* pJob)
{
  Job_Purge* pJob_Purge = _pJob_Purge(pJob);
  
  delete pJob_Purge;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_Purge_show(Job* pJob)
{
  Job_Purge* pJob_Purge = _pJob_Purge(pJob);

  pJob_Purge->vShow();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* ingest data for table                                                      */
/*----------------------------------------------------------------------------*/
void Job_Purge::vPurge(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PURGE,_FOOTER_);
  
  vTrace("Purging table %s\n"_Stz,
         m_sObjectName.aStr());
  
}

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
