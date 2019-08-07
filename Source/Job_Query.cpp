/*============================================================================*/
/* Job_Query.cpp -- Query Job                                                 */
/*============================================================================*/

#include "Jobs.h"

JobClass xJobClass_Query =
{
  CN_JOB_QUERY,          // class number
  JK_LOCAL,              // job kind
  RK_LOCAL_ROWS,         // response kind
  "query"_Stz,           // comment about the job
  "query"_Stz,           // the prompt command
  nullptr,               // the second word
  nullptr,               // the _run task seed
  "query_req"_Stz,       // the _req task seed
  nullptr,               // the _rcv task seed
  nullptr,               // the _exe task seed
  nullptr,               // the _end task seed
  nullptr,               // the _can task seed
  false,                 // check resources?
  false,                 // detached job?
  false,                 // buffer attached?
  JT_NONE,               // job trace
  DL_GRAB_READ,          // ddl lock model
  PY_NORMAL,             // priority
  JF_NONE,               // flags

  nullptr,               // prompt check function
  pJob_Query_pre,          // prompt pre-process function
  vJob_post,             // prompt post-process function
  nullptr,               // job new function
  vJob_Query_delete,       // job delete function
  nullptr,               // job pre-send function
  nullptr,               // job post-receive function
  vJob_Query_show,         // job show function
  
  // help and example
  true,
  "query"_Stz,
  "query;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_Query_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Receiver_class;
  
  return &xJobClass_Query;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Query* Jobs::_pJob_Query(void* aAddress)
{
  Job_Query* pJob_Query = reinterpret_cast<Job_Query*>(aAddress);
  
  RASSERT_TYPE(pJob_Query,CN_JOB_QUERY);
  
  return pJob_Query;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Query::operator new(size_t)
{
  Job_Query* pJob_Query =
    static_cast<Job_Query*>(HeapMem::pAllocate(CN_JOB_QUERY));
  
  return pJob_Query;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Query::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Query::operator delete(void* aAddress)
{
  Job_Query* pJob_Query = static_cast<Job_Query*>(aAddress);
  
  HeapMem::vFree(pJob_Query);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Query::Job_Query(Session* pSession)
: JobMark2(CN_JOB_QUERY, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_QUERY,_FOOTER_);
  
  m_pDigest = new Digest(this);
  
  m_pScanOp  = new ScanOp ("S_0"_Stz,m_pDigest,nullptr,false);
  m_pOutputOp = new OutputOp("O_0"_Stz,m_pDigest,nullptr,false);
  m_pZedOp    = new ZedOp   ("Z_0"_Stz,m_pDigest,nullptr,false);

  _checkInitialised_(CN_JOB_QUERY);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_QUERY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Query::~Job_Query(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_QUERY,_FOOTER_);
  
  delete m_pScanOp;
  delete m_pOutputOp;
  delete m_pZedOp;
  
  delete m_pDigest;
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Query::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_Query* pJob_Query = _pJob_Query(pObject);

  pJob_Query->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_Query::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_QUERY,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("query"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - -
  // get no of tokens in token vector
  iNumTokens = pTokenVec->nTokens();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be only 2 tokens
  if (iNumTokens != 2) {
    return nullptr;
  }
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the table name
  m_sObjectName = pTokenVec->pGet(1)->aText();

  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Query::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_QUERY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* query begin                                                                */
/*----------------------------------------------------------------------------*/
void Job_Query::vBegin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_QUERY,_FOOTER_);
  
  AM::vSetAcksRmg(m_pExecutingTask,3);

  m_pScanOp->vOpOut(m_pOutputOp);
  m_pOutputOp->vOpOut(m_pZedOp);
  m_pZedOp->vOpOut(nullptr);
  //m_pZedOp->vAckJobEnd(true);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  TablePtn* pTablePtn = m_pTable->pPartition(0);
  m_pScanOp->vTablePtn(pTablePtn);
}

/*----------------------------------------------------------------------------*/
/* query end                                                                  */
/*----------------------------------------------------------------------------*/
void Job_Query::vEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_QUERY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* query execute                                                               */
/*----------------------------------------------------------------------------*/
void Job_Query::vExecute(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_QUERY,_FOOTER_);
  
  Session* pSession = AM::pThisTLSSession();
  
  bool bSpawned;
  
  bSpawned = AM::bSpawnOp(pSession,m_pZedOp);
  ASSERT(bSpawned);
  bSpawned = AM::bSpawnOp(pSession,m_pOutputOp);
  ASSERT(bSpawned);
  bSpawned = AM::bSpawnOp(pSession,m_pScanOp);
  ASSERT(bSpawned);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Query::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_QUERY);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Query_pre(Session* pSession)
{
  Job_Query* pJob_Query = new Job_Query(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommand->pTknVec();
  
  bool bResult = pJob_Query->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    pJob_Query->vAddError(EC_INVALID_COMMAND);
  }
  
  return pJob_Query;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Query_delete(Session* pSession, Job* pJob)
{
  Job_Query* pJob_Query = _pJob_Query(pJob);
  
  delete pJob_Query;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Query_show(Job* pJob)
{
  Job_Query* pJob_Query = _pJob_Query(pJob);

  pJob_Query->vShow();
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
