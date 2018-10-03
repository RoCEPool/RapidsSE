/*============================================================================*/
/* Job_FetchRows.cpp -- Fetch Rows Job                                        */
/*============================================================================*/

// Job Short: FetchRows

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_FetchRows =
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

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* JobsModel::pJob_FetchRows_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_InsertBuffer_class;
  
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
    static_cast<Job_FetchRows*>(HeapMem::pAllocate(CN_JOB_FETCHROWS));
  
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
  
  HeapMem::vFree(pJob_FetchRows);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_FetchRows::Job_FetchRows(Interface::SEitf* pSEitf)
: Job(CN_JOB_FETCHROWS, pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_FETCHROWS,_FOOTER_);
    
  m_sWhere       = ""_Stz;
  m_pTable       = nullptr;
  m_iPartition   = -1;
  m_bDisplayOn   = true;
  m_pPlanner     = nullptr;
  m_pBufferQueue = new Queue;
  m_pFetchBuffer = nullptr;
  m_nLastBuffers = 0;
  m_nLastBuffersScanned = 0;

 
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
  
  m_pBufferQueue->vFinalize();
  delete m_pBufferQueue;
  
  // in case excepton , we may not have deleted buffer
  if (m_pFetchBuffer)
    delete m_pFetchBuffer;
  
  delete m_pPlanner;
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
/* delete last fetch buffer                                                   */
/*----------------------------------------------------------------------------*/
void Job_FetchRows::vDeleteBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_FETCHROWS,_FOOTER_);
  
  if (m_pFetchBuffer) {
    delete m_pFetchBuffer;
    m_pFetchBuffer = nullptr;
  }
}

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
  // there should be 2 to 5 tokens
  if (iNumTokens < 2 || iNumTokens > 5)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the table name
  m_sObjectName = pTokenVec->pGet(1)->aText();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // 2 tokens
  //fetch j10;
  if (iNumTokens == 2)
    return true;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // 3 tokens
  //fetch j10 "where";
  //fetch j10 1;
  //fetch j10 displayOff;
  if (iNumTokens == 3) {
    if (pTokenVec->pGet(2)->eToken() == TN_STRING) {
      m_sWhere = pTokenVec->pGet(2)->aText();
      return true;
    }
    
    if (pTokenVec->pGet(2)->eToken() == TN_INTEGER) {
      m_iPartition = pTokenVec->pGet(2)->iToI8();
      return true;
    }
    
    if (pTokenVec->pGet(2)->eToken() == TN_NAME) {
    
      if ( ! pTokenVec->pGet(2)->aText()->bSame("displayOff"_Stz) )
        return false;
    
      // set display off
      m_bDisplayOn = false;
      return true;
    }
    
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - -
  // 4 tokens
  //fetch j10 "where" 1;
  //fetch j10 1 displayOff;
  if (iNumTokens == 4) {
    if (pTokenVec->pGet(2)->eToken() == TN_STRING) {
      
      m_sWhere = pTokenVec->pGet(2)->aText();
      
      if (pTokenVec->pGet(3)->eToken() != TN_INTEGER)
        return false;
      
      m_iPartition = pTokenVec->pGet(3)->iToI8();
      return true;
    }
    
    if (pTokenVec->pGet(2)->eToken() == TN_INTEGER) {
      
      m_iPartition = pTokenVec->pGet(2)->iToI8();
      
      if (pTokenVec->pGet(3)->eToken() != TN_NAME)
        return false;
      
      if ( ! pTokenVec->pGet(3)->aText()->bSame("displayOff"_Stz) )
        return false;
      
      // set display off
      m_bDisplayOn = false;
      return true;
    }
    
    return false;
    
  }
  
  // - - - - - - - - - - - - - - - - - - - - -
  // 5 tokens
  //fetch j10 "where" 1 displayOff;
  if (iNumTokens == 5) {
    
    // get where
    if (pTokenVec->pGet(2)->eToken() == TN_STRING) {
      m_sWhere = pTokenVec->pGet(2)->aText();
    }
    else
      return false;
    
    // get partition
    if (pTokenVec->pGet(3)->eToken() == TN_INTEGER) {
      m_iPartition = pTokenVec->pGet(3)->iToI8();
    }
    else
      return false;
    
    // get displayOff
    if (pTokenVec->pGet(4)->eToken() == TN_NAME) {
      if ( pTokenVec->pGet(4)->aText()->bSame("displayOff"_Stz) ) {
        // set display off
        m_bDisplayOn = false;
        return true;
      }
    }
    return false;
  }
  
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
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_FetchRows->m_xNode[i_Node].m_eErrorCode]);
    }
   
    return;
  }
  
  if (gpAttachMap->m_bMetadata) {
    vTrace("metadata: %s\n"_Stz, pJob_FetchRows->m_sMetadata.aStr());
  }
  
  while ( nLastBuffersScanned != pJob_FetchRows->m_nLastBuffers ) {
    
    // attempt to pop buffer off the buffer queue
    if ( ! pJob_FetchRows->m_pBufferQueue->bPop(&xValWrk) ) {
      PosixThread::vSleepMilli(1);
      continue;
    }
    PtrVal* pPtrVal = xValWrk.pNestedPtrVal();
    pFetchBuffer = (FetchBuffer*)(pPtrVal->pPtr());
    
    if (pJob_FetchRows->m_bDisplayOn) {
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
  s48K sRowString;
  
  ppRow = pFetchBuffer->pFirstRowPtr();
  while (ppRow) {
    Row* pRow = *ppRow;
    pRow->vAsBinaryStringVLQ(sRowString, pJob_FetchRows->m_pTable->pColumnVec());
    vTrace("row: (%s)\n"_Stz, sRowString.aChar());
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
