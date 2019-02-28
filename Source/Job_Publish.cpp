/*============================================================================*/
/* Job_Publish.cpp -- Publish Job                                             */
/*============================================================================*/

// Job Short: Ingest

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_Publish =
{
  "Job_Publish"_Stz,     // name of the job class
  CN_JOB_PUBLISH,        // class enum
  JK_DIST2PC,            // job kind
  "publish table"_Stz,   // comment about the job
  "publish"_Stz,         // the prompt command
  nullptr,               // the second word
  nullptr,               // the _run task seed
  "publish_req"_Stz,     // the _req task seed
  "publish_beg"_Stz,     // the _rcv task seed
  "publish_exe"_Stz,     // the _exe task seed
  "publish_end"_Stz,     // the _end task seed
  "publish_can"_Stz,     // the _can task seed
  false,                 // check resources?
  false,                 // detached job?
  false,                 // buffer attached?
  JT_NONE,               // job trace
  DL_NONE,               // ddl lock model

  eJob_checkCluster,     // prompt check function
  pJob_Publish_pre,      // prompt pre-process function
  vJob_Publish_post,     // prompt post-process function
  pJob_Publish_new,      // job new function
  vJob_Publish_delete,   // job delete function
  vJob_send,             // job pre-send function
  vJob_recv,             // job post-receive function
  vJob_Publish_show,     // job show function
  
  // help and example
  true,
  "publish <tableNm>"_Stz,
  "publish SensorEvents;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_Publish_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Purge_class;
  
  return &xJobClass_Publish;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Publish* Jobs::_pJob_Publish(void* aAddress)
{
  Job_Publish* pJob_Publish = reinterpret_cast<Job_Publish*>(aAddress);
  
  RASSERT_TYPE(pJob_Publish,CN_JOB_PUBLISH);
  
  return pJob_Publish;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Publish::operator new(size_t)
{
  Job_Publish* pJob_Publish =
    static_cast<Job_Publish*>(HeapMem::pAllocate(CN_JOB_PUBLISH));
  
  return pJob_Publish;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Publish::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Publish::operator delete(void* aAddress)
{
  Job_Publish* pJob_Publish = static_cast<Job_Publish*>(aAddress);
  
  HeapMem::vFree(pJob_Publish);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Publish::Job_Publish(Interface::Session* pSession)
: Job(CN_JOB_PUBLISH, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PUBLISH,_FOOTER_);
  
  //vTrace("::Job_Publish: job %p\n"_Stz,this);

  m_pTable = nullptr;
  
  m_okLock.vInitLock(LK_PIPE);
  
  _checkInitialised_(CN_JOB_PUBLISH);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_PUBLISH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Publish::~Job_Publish(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PUBLISH,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Publish::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_Publish* pJob_Publish = _pJob_Publish(pObject);

  pJob_Publish->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Publish::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PUBLISH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Publish::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PUBLISH);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Publish_pre(Interface::Session* pSession)
{
  i8 nTokens;

  Job_Publish* pJob_Publish = new Job_Publish(pSession);

  TokenVec* pTknVec = pSession->m_pCommand->pTknVec();

  // get no of tokens in token vector
  nTokens = pTknVec->nTokens();

  // there should be 3 tokens
  if (nTokens < 3)
    return pJob_Publish;

  // set the table name
  pJob_Publish->m_sObjectName = pTknVec->pGet(1)->aText();

  // set the output file name
  pJob_Publish->m_sFilePath = pTknVec->pGet(2)->sText();
  
  return pJob_Publish;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Publish_post(Interface::Session* pSession, Job* pJob)
{
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Publish_new(Job* pArg)
{
  Job_Publish* pJob_Publish = new Job_Publish(nullptr);
  
  return pJob_Publish;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Publish_delete(Interface::Session* pSession, Job* pJob)
{
  Job_Publish* pJob_Publish = _pJob_Publish(pJob);
  
  delete pJob_Publish;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Publish_show(Job* pJob)
{
  Job_Publish* pJob_Publish = _pJob_Publish(pJob);

  pJob_Publish->vShow();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* publish data for table                                                     */
/*----------------------------------------------------------------------------*/
void Job_Publish::vPublish(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PUBLISH,_FOOTER_);
  
  vTrace("Publishing table %s\n"_Stz, m_sObjectName.aStz());
}

/*----------------------------------------------------------------------------*/
/* get table                                                                  */
/*----------------------------------------------------------------------------*/
Table* Job_Publish::pTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PUBLISH,_FOOTER_);
  
  return m_pTable;
}

/*----------------------------------------------------------------------------*/
/* set table                                                                  */
/*----------------------------------------------------------------------------*/
void Job_Publish::vTable(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PUBLISH,_FOOTER_);
  
  m_pTable = pTable;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
