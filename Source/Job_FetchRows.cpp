/*============================================================================*/
/* Job_FetchRows.cpp -- Fetch Rows Job                                        */
/*============================================================================*/

// Job Short: FetchRows

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobNote mJobNote_FetchRows (
  "Job_FetchRows"_Stz,       // name of the job class
  CN_JOB_FETCHROWS,          // class enum
  JK_DIST2PC,                // job kind
  "fetch rows"_Stz,          // comment about the job
  "fetch"_Stz,               // the prompt command
  nullptr,                   // the second word
  nullptr,                   // the _run task seed
  "fetchRows_req"_Stz,       // the _req task seed
  "fetchRows_beg"_Stz,       // the _rcv task seed
  "fetchRows_exe"_Stz,       // the _exe task seed
  "fetchRows_end"_Stz,       // the _end task seed
  "fetchRows_can"_Stz,       // the _can task seed
  true,                      // check resources?
  false,                     // detached job?
  false,                     // buffer attached?
  JT_NONE,                   // job trace
  pJob_FetchRows_pre,        // prompt pre-process function
  vJob_FetchRows_post,       // prompt post-process function
  nullptr,                   // job new function
  vJob_FetchRows_delete,     // job delete function
  vJob_send,                 // job pre-send function
  vJob_recv,                 // job post-receive function
  vJob_FetchRows_show,       // job show function
  true,                      // job is enabled
  "fetch <tableNm> <where>"_Stz,// help
  "<example>"_Stz            // example
);

JobClass_s xJobClass_FetchRows =
{
  "Job_FetchRows"_Stz,       // name of the job class
  CN_JOB_FETCHROWS,          // class enum
  JK_DIST2PC,                // job kind
  "fetch rows"_Stz,          // comment about the job
  "fetch"_Stz,               // the prompt command
  nullptr,                   // the second word
  nullptr,                   // the _run task seed
  "fetchRows_req"_Stz,       // the _req task seed
  "fetchRows_beg"_Stz,       // the _rcv task seed
  "fetchRows_exe"_Stz,       // the _exe task seed
  "fetchRows_end"_Stz,       // the _end task seed
  "fetchRows_can"_Stz,       // the _can task seed
  true,                      // check resources?
  false,                     // detached job?
  false,                     // buffer attached?
  JT_NONE,                   // job trace
  pJob_FetchRows_pre,        // prompt pre-process function
  vJob_FetchRows_post,       // prompt post-process function
  nullptr,                   // job new function
  vJob_FetchRows_delete,     // job delete function
  vJob_send,                 // job pre-send function
  vJob_recv,                 // job post-receive function
  vJob_FetchRows_show,       // job show function
  true,                      // job is enabled
  "fetch <tableNm> <where>"_Stz,// help
  "<example>"_Stz,           // example

  { 0, 0, 0, 0 ,0, 0 }    // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass_s* JobsModel::pJob_FetchRows_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Lf_class;
  
  return &xJobClass_FetchRows;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_FetchRows* JobsModel::_pJob_FetchRows(void* aAddress)
{
  Job_FetchRows* pJob_FetchRows = reinterpret_cast<Job_FetchRows*>(aAddress);
  
  RASSERT( !pJob_FetchRows || CN_JOB_FETCHROWS == pJob_FetchRows->eClass());
  
  return pJob_FetchRows;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_FetchRows::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_FetchRows* pJob_FetchRows =
    static_cast<Job_FetchRows*>(Heap::pAllocate(CN_JOB_FETCHROWS));
  
  return pJob_FetchRows;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_FetchRows::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_FetchRows::operator delete(void* aAddress)
{
  Job_FetchRows* pJob_FetchRows = static_cast<Job_FetchRows*>(aAddress);
  
  Heap::vFree(pJob_FetchRows);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_FetchRows::Job_FetchRows(Interface::SEitf* pSEitf)
: Job(CN_JOB_FETCHROWS, pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_FETCHROWS,_FOOTER_);
    
  m_sWhere       = ""_Stz;
  m_pBufferQueue = new Queue;
  m_pTable       = nullptr;
  m_pPlanner     = nullptr;
  m_nLastBuffers = -1;
 
  m_pTask        = nullptr;
  
  _checkInitialised_(CN_JOB_FETCHROWS);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_FETCHROWS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_FetchRows::~Job_FetchRows(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_FETCHROWS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_FetchRows::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_FETCHROWS == eClass);
  RASSERT(pObject->eClass() == CN_JOB_FETCHROWS);
  
  Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(pObject);

  pJob_FetchRows->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_FetchRows::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_FETCHROWS,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("fetch"_Stz));
  
  // get number of items in token vector
  iNumTokens = pTokenVec->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 2/3 tokens
  if (iNumTokens != 2 && iNumTokens != 3)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the table name
  m_sObjectName = pTokenVec->pGet(1)->aText();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the where clause
  if (iNumTokens == 3)
    m_sWhere = pTokenVec->pGet(2)->aText();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_FetchRows::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_FETCHROWS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_FetchRows::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_FETCHROWS);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_FetchRows_pre(Interface::SEitf* pSEitf)
{
  Job_FetchRows* pJob_FetchRows = new Job_FetchRows(pSEitf);
  
  TokenVec* pTokenVec = pSEitf->m_pCommandProcessor->pTknVec();
  
  bool bResult = pJob_FetchRows->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_FetchRows->m_bHasError = true;
    pJob_FetchRows->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_FetchRows;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_FetchRows_post(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(pJob);
  
  ValWrk xValWrk;
  i8 nRowsProcessed = 0;
  i8 nLastBuffersScanned = 0;
  
  FetchBuffer* pFetchBuffer = nullptr;
  
  if ( pJob_FetchRows->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("ERROR (NODE%lld): %s\n"_Stz, i_Node, aEC[pJob_FetchRows->m_xNode[i_Node].m_eErrorCode]);
    }
   
    return;
  }
    
  while ( nLastBuffersScanned != pJob_FetchRows->m_nLastBuffers ) {
    
    // attempt to pop buffer off the buffer queue
    if ( ! pJob_FetchRows->m_pBufferQueue->bPop(&xValWrk) ) {
      PosixThread::vSleepMilli(1);
      continue;
    }
    PtrVal* pPtrVal = xValWrk.pNestedPtrVal();
    pFetchBuffer = (FetchBuffer*)(pPtrVal->pPtr());
    
    if (pSEitf->m_bDisplay) {
      vIterateBuffer(pJob_FetchRows, pFetchBuffer);      
    }
    
    nRowsProcessed += pFetchBuffer->nFetchedRows();
    
    if (pFetchBuffer->bIsLast()) {
      nLastBuffersScanned++;
    }
    delete pFetchBuffer;
    pFetchBuffer = nullptr;
  }
    
  vTrace("success: %lld rows affected\n"_Stz, nRowsProcessed);
  
  // delete the planner
  delete pJob_FetchRows->m_pPlanner;
  pJob_FetchRows->m_pPlanner = nullptr;
  
  // delete the buffer queue
  pJob_FetchRows->m_pBufferQueue->vFinalize();
  delete pJob_FetchRows->m_pBufferQueue;
  pJob_FetchRows->m_pBufferQueue = nullptr;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_FetchRows_delete(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(pJob);
  
  delete pJob_FetchRows;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vIterateBuffer(Job_FetchRows* pJob_FetchRows, FetchBuffer* pFetchBuffer) {
  Row** ppRow = nullptr;
  i8 nPartition = pFetchBuffer->nPartitionNo();
  s48K sRowString;
  
  ppRow = pFetchBuffer->pFirstRowPtr();
  while (ppRow) {
    Row* pRow = *ppRow;
    pRow->vAsBinaryStringVLQ(sRowString, pJob_FetchRows->m_pTable->pColumnVec());
    vTrace("row: (%lld, %s)\n"_Stz, nPartition, sRowString.aChar());
    ppRow = pFetchBuffer->pNextRowPtr(ppRow);
  }
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_FetchRows_show(Job* pJob)
{
  Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(pJob);

  pJob_FetchRows->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
