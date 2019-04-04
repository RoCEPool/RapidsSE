/*============================================================================*/
/* Job_FetchRows.cpp -- Fetch Rows Job                                        */
/*============================================================================*/

// Job Short: FetchRows

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_FetchRows =
{
  CN_JOB_FETCHROWS,          // class enum
  JK_LOCAL,                  // job kind
  "fetch rows"_Stz,          // comment about the job
  "fetch"_Stz,               // the prompt command
  nullptr,                   // the second word
  nullptr,                   // the _run task seed
  "fetchRows_req"_Stz,       // the _req task seed
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
  JF_NONE,                  // flags

  eJob_checkCluster,         // prompt check function
  pJob_FetchRows_pre,        // prompt pre-process function
  vJob_FetchRows_post,       // prompt post-process function
  nullptr,                   // job new function
  vJob_FetchRows_delete,     // job delete function
  nullptr,                   // job pre-send function
  nullptr,                   // job post-receive function
  vJob_FetchRows_show,       // job show function
  
  // help and example
  true,
  "fetch <tableNm> [\"<where>\"] [displayOff | noPrint]"_Stz,
  "fetch supplier \"S_SUPPKEY 22 ==\";"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_FetchRows_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Generate_class;
  
  return &xJobClass_FetchRows;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_FetchRows* Jobs::_pJob_FetchRows(void* aAddress)
{
  Job_FetchRows* pJob_FetchRows = reinterpret_cast<Job_FetchRows*>(aAddress);
  
  RASSERT_TYPE(pJob_FetchRows,CN_JOB_FETCHROWS);
  
  return pJob_FetchRows;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_FetchRows::operator new(size_t)
{
  Job_FetchRows* pJob_FetchRows =
    static_cast<Job_FetchRows*>(HeapMem::pAllocate(CN_JOB_FETCHROWS));
  
  return pJob_FetchRows;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_FetchRows::operator new(size_t, void* aAddress)
{
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
Job_FetchRows::Job_FetchRows(Interface::Session* pSession)
: Job(CN_JOB_FETCHROWS, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_FETCHROWS,_FOOTER_);
    
  m_sWhere       = ""_Stz;
  m_pTable       = nullptr;
  m_iPartition   = -1;
  m_bDisplay     = true;
  m_bPrint       = true;
  m_pPlanner     = nullptr;
  m_pBufferQueue = new Queue;
  m_pFetchBuffer = nullptr;
  m_nLastBuffers = 0;
  m_nLastBuffersScanned = 0;
  m_pRowOnce     = nullptr;
  
  _checkInitialised_(CN_JOB_FETCHROWS);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_FETCHROWS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_FetchRows::~Job_FetchRows(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_FETCHROWS,_FOOTER_);
  
  //m_pBufferQueue->vFinalize();
  delete m_pBufferQueue;
  
  // in case excepton , we may not have deleted buffer
  if (m_pFetchBuffer)
    delete m_pFetchBuffer;
  
  delete m_pPlanner;
  
  if (m_pRowOnce) {
    delete m_pRowOnce;
    m_pRowOnce = nullptr;
  }
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_FetchRows::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(pObject);

  pJob_FetchRows->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* create row once object                                                     */
/*----------------------------------------------------------------------------*/
void Job_FetchRows::vCreateRowOnce(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_FETCHROWS,_FOOTER_);
  
  RASSERT(! m_pRowOnce);
  
  m_pRowOnce = new RowOnce;
}

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
  
//  if (m_pRowOnce) {
//    delete m_pRowOnce;
//    m_pRowOnce = nullptr;
//  }
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
  //fetch j10 displayOff|noPrint;
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
    
      if ( ! pTokenVec->pGet(2)->aText()->bSame("displayOff"_Stz) &&
           ! pTokenVec->pGet(2)->aText()->bSame("noPrint"_Stz))
        return false;
    
      // set display off
      if (pTokenVec->pGet(2)->aText()->bSame("displayOff"_Stz))
        m_bDisplay = false;
      else {
        ASSERT(pTokenVec->pGet(2)->aText()->bSame("noPrint"_Stz));
        m_bPrint = false;
      }
      return true;
    }
    
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - -
  // 4 tokens
  //fetch j10 "where" 1;
  //fetch j10 1 displayOff|noPrint;
  //fetch j10 "where" displayOff|noPrint;
  if (iNumTokens == 4) {
    if (pTokenVec->pGet(2)->eToken() == TN_STRING) {
      
      m_sWhere = pTokenVec->pGet(2)->aText();
      
      if (pTokenVec->pGet(3)->eToken() == TN_INTEGER) {
        m_iPartition = pTokenVec->pGet(3)->iToI8();
        return true;
      }
      
      if (pTokenVec->pGet(3)->eToken() != TN_NAME)
        return false;
      
      if ( ! pTokenVec->pGet(3)->aText()->bSame("displayOff"_Stz) &&
          ! pTokenVec->pGet(3)->aText()->bSame("noPrint"_Stz))
        return false;
      
      if (pTokenVec->pGet(3)->aText()->bSame("displayOff"_Stz))
        m_bDisplay = false;
      else {
        ASSERT(pTokenVec->pGet(3)->aText()->bSame("noPrint"_Stz));
        m_bPrint = false;
      }
      return true;
    }
    
    if (pTokenVec->pGet(2)->eToken() == TN_INTEGER) {
      
      m_iPartition = pTokenVec->pGet(2)->iToI8();
      
      if (pTokenVec->pGet(3)->eToken() != TN_NAME)
        return false;
      
      if ( ! pTokenVec->pGet(3)->aText()->bSame("displayOff"_Stz) &&
          ! pTokenVec->pGet(3)->aText()->bSame("noPrint"_Stz))
        return false;
      
      if (pTokenVec->pGet(3)->aText()->bSame("displayOff"_Stz))
        m_bDisplay = false;
      else {
        ASSERT(pTokenVec->pGet(3)->aText()->bSame("noPrint"_Stz));
        m_bPrint = false;
      }
      return true;
    }
    
    return false;
    
  }
  
  // - - - - - - - - - - - - - - - - - - - - -
  // 5 tokens
  //fetch j10 "where" 1 displayOff|noPrint;
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
        m_bDisplay = false;
        return true;
      }
      if ( pTokenVec->pGet(4)->aText()->bSame("noPrint"_Stz) ) {
        // set print off
        m_bPrint = false;
        return true;
      }
    }
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* test if row has been seen so far                                           */
/*----------------------------------------------------------------------------*/
bool Job_FetchRows::bRowSeen(Id xRowId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_FETCHROWS,_FOOTER_);
  
  if (! m_pRowOnce)
    return false;
  
  bool bSeen = m_pRowOnce->bRowSeen(xRowId);
  
  return bSeen;
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
Job* Jobs::pJob_FetchRows_pre(Interface::Session* pSession)
{
  Job_FetchRows* pJob_FetchRows = new Job_FetchRows(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommand->pTknVec();
  
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
void Jobs::vJob_FetchRows_post(Interface::Session* pSession, Job* pJob)
{
  Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(pJob);
  
  GenVar xVariable;
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
    vTrace("metadata: %s\n"_Stz, pJob_FetchRows->m_sMetadata.aStz());
  }
  
  while ( nLastBuffersScanned != pJob_FetchRows->m_nLastBuffers ) {
    
    // attempt to pop buffer off the buffer queue
//    if ( ! pJob_FetchRows->m_pBufferQueue->bPop(&xVariable) ) {
//      Pthread::vSleepMilli(1);
//      continue;
//    }
//    PtrVal* aPtrVal = xVariable.aNestedPtrVal();
//    pFetchBuffer = (FetchBuffer*)(aPtrVal->pObj());
    
    pFetchBuffer = _pFetchBuffer(pJob_FetchRows->m_pBufferQueue->pPop());
    if (! pFetchBuffer) {
      Pthread::vSleepMilli(1,CN_JOB_FETCHROWS);
      continue;
    }
    
    if (pJob_FetchRows->m_bDisplay) {
      vIterateBuffer(pJob_FetchRows, pFetchBuffer, pJob_FetchRows->m_bPrint);
    }
    
    nRowsProcessed += pFetchBuffer->nFetchedRows();
    
    if (pFetchBuffer->bIsLast()) {
      nLastBuffersScanned++;
    }
    delete pFetchBuffer;
    pFetchBuffer = nullptr;
  }
    
  vTrace("success: %lld rows fetched\n"_Stz, nRowsProcessed);
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_FetchRows_delete(Interface::Session* pSession, Job* pJob)
{
  Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(pJob);
  
  delete pJob_FetchRows;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vIterateBuffer(Job_FetchRows* pJob_FetchRows, FetchBuffer* pFetchBuffer, bool bPrint)
{
  ColumnVec* pColVec = pJob_FetchRows->m_pTable->pColumnVec();
  CacheLine** aaCacheLine = nullptr;
  aaCacheLine = pFetchBuffer->aaFirstRowPtr();
  
  //Record* pRecord = new Record;
  HV_Rec hvRec;
  
  while (aaCacheLine) {
    
    //s48K sRowString;
    //RowCoder::vString(sRowString, pColumnVec,*aaCacheLine);
    
    RowCoder::vRecord(hvRec,nullptr,0,pColVec);

    if (bPrint)
      vTrace("row: (%s)\n"_Stz, hvRec->szRecStr());
    
    aaCacheLine = pFetchBuffer->aaNextRowPtr(aaCacheLine);
  }
  
  //delete pRecord;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_FetchRows_show(Job* pJob)
{
  Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(pJob);

  pJob_FetchRows->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
