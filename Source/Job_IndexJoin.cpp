/*============================================================================*/
/* Job_IndexJoin.cpp -- Fetch Rows Job                                        */
/*============================================================================*/

// Job Short: IndexJoin

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_IndexJoin =
{
  CN_JOB_INDEXJOIN,          // class enum
  JK_LOCAL,                  // job kind
  RK_DATA_ROWS,              // response kind
  "join tables"_Stz,         // comment about the job
  "join"_Stz,                // the prompt command
  nullptr,                   // the second word
  nullptr,                   // the _run task seed
  "IndexJoin_req"_Stz,       // the _req task seed
  nullptr,                   // the _rcv task seed
  nullptr,                   // the _exe task seed
  nullptr,                   // the _end task seed
  nullptr,                   // the _can task seed
  true,                      // check resources?
  false,                     // detached job?
  false,                     // buffer attached?
  JT_NONE,                   // job trace
  DL_GRAB_READ,              // ddl lock model
  PY_NORMAL,                 // priority
  JF_NONE,                   // flags

  eJob_checkCluster,         // prompt check function
  pJob_IndexJoin_pre,        // prompt pre-process function
  vJob_IndexJoin_post,       // prompt post-process function
  nullptr,                   // job new function
  vJob_IndexJoin_delete,     // job delete function
  nullptr,                   // job pre-send function
  nullptr,                   // job post-receive function
  vJob_IndexJoin_show,       // job show function
  
  // help and example
  true,
  "join <Left-Table> <Left-Index> <Right-Table> <Right-Index>"_Stz,
  "join T1 T1_idx1 T2 T2_idx2"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_IndexJoin_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_InflateRows_class;
  
  return &xJobClass_IndexJoin;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_IndexJoin* Jobs::_pJob_IndexJoin(void* aAddress)
{
  Job_IndexJoin* pJob_IndexJoin = reinterpret_cast<Job_IndexJoin*>(aAddress);
  
  RASSERT_TYPE(pJob_IndexJoin,CN_JOB_INDEXJOIN);
  
  return pJob_IndexJoin;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_IndexJoin::operator new(size_t)
{
  Job_IndexJoin* pJob_IndexJoin =
    static_cast<Job_IndexJoin*>(HeapMem::pAllocate(CN_JOB_INDEXJOIN));
  
  return pJob_IndexJoin;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_IndexJoin::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_IndexJoin::operator delete(void* aAddress)
{
  Job_IndexJoin* pJob_IndexJoin = static_cast<Job_IndexJoin*>(aAddress);
  
  HeapMem::vFree(pJob_IndexJoin);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_IndexJoin::Job_IndexJoin(Interface::Session* pSession)
: Job(CN_JOB_INDEXJOIN, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INDEXJOIN,_FOOTER_);
  
  m_sRightTable  = ""_Stz;
  m_sRightIndex  = ""_Stz;
  m_pLeftTable   = nullptr;
  m_pRightTable  = nullptr;
  m_pLeftIndex   = nullptr;
  m_pRightIndex  = nullptr;
  m_iPartition   = -1;
  m_pBufferQueue = new Queue;
  m_nLastBuffers = 0;
  m_nLastBuffersScanned = 0;
  
  _checkInitialised_(CN_JOB_INDEXJOIN);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_INDEXJOIN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_IndexJoin::~Job_IndexJoin(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INDEXJOIN,_FOOTER_);
  
  delete m_pBufferQueue;
  
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_IndexJoin::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_IndexJoin* pJob_IndexJoin = _pJob_IndexJoin(pObject);

  pJob_IndexJoin->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_IndexJoin::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INDEXJOIN,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("join"_Stz));
  
  // get number of items in token vector
  iNumTokens = pTokenVec->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // join T1 T1_idx1 T2 T2_idx2
  
  if (iNumTokens != 5)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the left table name
  m_sObjectName = pTokenVec->pGet(1)->aText();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the left index name
  m_sIndexName = pTokenVec->pGet(2)->aText();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the right table name
  m_sRightTable = pTokenVec->pGet(3)->aText();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the right index name
  m_sRightIndex = pTokenVec->pGet(4)->aText();

  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_IndexJoin::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INDEXJOIN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_IndexJoin::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INDEXJOIN);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_IndexJoin_pre(Interface::Session* pSession)
{
  Job_IndexJoin* pJob_IndexJoin = new Job_IndexJoin(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommand->pTknVec();
  
  bool bResult = pJob_IndexJoin->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    pJob_IndexJoin->vAddError(EC_INVALID_COMMAND);
  }
  
  return pJob_IndexJoin;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_IndexJoin_post(Interface::Session* pSession, Job* pJob)
{
  Job_IndexJoin* pJob_IndexJoin = _pJob_IndexJoin(pJob);
  
  if ( pJob_IndexJoin->m_bHasError ) {
    
    vTrace("error: (node%lld) %s\n"_Stz, gpAttachMap->m_pGrid->iThisNode(),
             aEC[pJob_IndexJoin->m_xResponse.m_eErrorCode]);
   
    return;
  }
  
  //! to do
  Pthread::vSleepMilli(1000000,CN_JOB_FETCHROWS);
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_IndexJoin_delete(Interface::Session* pSession, Job* pJob)
{
  Job_IndexJoin* pJob_IndexJoin = _pJob_IndexJoin(pJob);
  
  delete pJob_IndexJoin;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_IndexJoin_show(Job* pJob)
{
  Job_IndexJoin* pJob_IndexJoin = _pJob_IndexJoin(pJob);

  pJob_IndexJoin->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
