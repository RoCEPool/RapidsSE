/*============================================================================*/
/* Job_FetchRows.cpp -- Fetch Rows Job                                        */
/*============================================================================*/

#include "Jobs.h"

JobClass xJobClass_FetchRows =
{
  CN_JOB_FETCHROWS,          // class number
  JK_LOCAL,                  // job kind
  RS_NONE,                   // response kind
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
  DL_GRAB_READ,              // ddl lock model
  PY_NORMAL,                 // priority
  JF_NONE,                   // flags

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

JobClass xJobClass_FetchRetrieveRows =
{
  CN_JOB_FETCHROWS,          // class number
  JK_LOCAL,                  // job kind
  RS_NONE,                   // response kind
  "fetch retrive rows"_Stz,  // comment about the job
  "fetch"_Stz,               // the prompt command
  "retrieve"_Stz,            // the second word
  nullptr,                   // the _run task seed
  "fetchRows_req"_Stz,       // the _req task seed
  nullptr,                   // the _rcv task seed
  nullptr,                   // the _exe task seed
  nullptr,                   // the _end task seed
  nullptr,                   // the _can task seed
  true,                      // check resources?
  false,                     // detached job?
  false,                     // buffer attached?
  DL_GRAB_READ,              // ddl lock model
  PY_NORMAL,                 // priority
  JF_NONE,                   // flags
  
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
  "fetch retrieve <tableNm> <columnNm> |print|"_Stz,
  "fetch retrieve supplier S_SUPPKEY"_Stz,
  
  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_FetchRows_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_LoadTable_class;
  
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
Job_FetchRows::Job_FetchRows(JobClass* aJobClass, ReqTuple* aReqTuple)
: Job(aJobClass, aReqTuple)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_FETCHROWS,_FOOTER_);
    
  m_sWhere       = ""_Stz;
  m_sColumnName  = ""_Stz;
  m_pTable       = nullptr;
  m_pColumn      = nullptr;
  m_iPartition   = -1;
  m_bDisplay     = true;
  m_bPrint       = true;
  m_bLimit       = false;
  m_bRandom      = false;
  m_bFirst       = false;
  m_iRows        = C_UNKNOWN;
  m_pPlanner     = nullptr;
  m_pBufferQueue = new Queue;
  m_pFetchBuffer = nullptr;
  m_nLastBuffers = 0;
  m_nLastBuffersScanned = 0;
  m_pRowOnce     = nullptr;
  m_bRetrieve    = false;
  
  m_pRow         = nullptr;
  
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
  
  if (m_pRow)
    delete m_pRow;
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append a row to a working buffer                                           */
/*----------------------------------------------------------------------------*/
bool Job_FetchRows::bAppendRow(FetchBuffer* pFetchBuffer, Ent* aItem,
                               Job_FetchRows* pJob_FetchRows)
{
  Ref xRef = aItem->xRef();
  
  if (pJob_FetchRows && pJob_FetchRows->bRowSeen(xRef)) {
    // row already inserted
    //vTrace("RowData already inserted %p \n"_Stz, aItem);
    return true;
  }
  
//  // get the rowBuffer
//  CrwBuffer* pCrwBuffer = CrwBuffer::pRefdCrwBuffer(xRef);
//
//  // get the dataRow
//  Crw* aCrw = pCrwBuffer->pCrw(xRef.iItem());
 
  Crw* aCrw = CrwBuffer::pRefdCrw(xRef);

  if (pFetchBuffer->bAppendCrw(aCrw))
    return true;
  else
    return false;
}

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
  if (iNumTokens < 2 || iNumTokens > 6)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // is it a fetch retrieve ?
  if (pTokenVec->pGet(1)->bSame("retrieve"_Stz)) {
    
    //fetch retrieve <tableNm> <columnNm> |print|
    
    // set default to be no print
    m_bPrint = false;
    
    // - - - - - - - - - - - - - - - - - - - - -
    // there should be 4 or 5 tokens
    if (iNumTokens != 4 && iNumTokens != 5)
      return false;
    
    // - - - - - - - - - - - - - - - - - - - - -
    // set retrive
    m_bRetrieve = true;
    
    // - - - - - - - - - - - - - - - - - - - - -
    // set the table name
    pJobTags()->vInsert(C_TAG_TABLENM, pTokenVec->pGet(2)->szChars());
    
    // - - - - - - - - - - - - - - - - - - - - -
    // fetch retrieve j10 column;
    if (pTokenVec->pGet(3)->eToken() == TN_NAME) {
      m_sColumnName = pTokenVec->pGet(3)->szChars();
      
      if (iNumTokens == 5) {
        if (pTokenVec->pGet(4)->bSame("print"_Stz))
          m_bPrint = true;
        else
          return false;
      }
      return true;
    }

    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the table name
  pJobTags()->vInsert(C_TAG_TABLENM, pTokenVec->pGet(1)->szChars());
  
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
      m_sWhere = pTokenVec->pGet(2)->szChars();
      return true;
    }
    
    if (pTokenVec->pGet(2)->eToken() == TN_INTEGER) {
      m_iPartition = pTokenVec->pGet(2)->iToI8();
      return true;
    }
    
    if (pTokenVec->pGet(2)->eToken() == TN_NAME) {
    
      if ( ! pTokenVec->pGet(2)->bSame("displayOff"_Stz) &&
           ! pTokenVec->pGet(2)->bSame("noPrint"_Stz))
        return false;
    
      // set display off
      if (pTokenVec->pGet(2)->bSame("displayOff"_Stz))
        m_bDisplay = false;
      else {
        ASSERT(pTokenVec->pGet(2)->bSame("noPrint"_Stz));
        m_bPrint = false;
      }
      return true;
    }
    
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - -
  // 4 tokens
  //fetch j10 "where" 1;
  //fetch j10 |limit 10|first 10|random 10|;
  //fetch j10 1 |displayOff|noPrint;
  //fetch j10 "where" |displayOff|noPrint;
  if (iNumTokens == 4) {
    if (pTokenVec->pGet(2)->eToken() == TN_STRING) {
      
      m_sWhere = pTokenVec->pGet(2)->szChars();
      
      if (pTokenVec->pGet(3)->eToken() == TN_INTEGER) {
        m_iPartition = pTokenVec->pGet(3)->iToI8();
        return true;
      }
      
      if (pTokenVec->pGet(3)->eToken() != TN_NAME)
        return false;
      
      if ( ! pTokenVec->pGet(3)->bSame("displayOff"_Stz) &&
           ! pTokenVec->pGet(3)->bSame("noPrint"_Stz) &&
           ! pTokenVec->pGet(3)->bSame("noPrint"_Stz))
        return false;
      
      if (pTokenVec->pGet(3)->bSame("displayOff"_Stz))
        m_bDisplay = false;
      else {
        ASSERT(pTokenVec->pGet(3)->bSame("noPrint"_Stz));
        m_bPrint = false;
      }
      return true;
    }
    
    if (pTokenVec->pGet(2)->eToken() == TN_INTEGER) {
      
      m_iPartition = pTokenVec->pGet(2)->iToI8();
      
      if (pTokenVec->pGet(3)->eToken() != TN_NAME)
        return false;
      
      if ( ! pTokenVec->pGet(3)->bSame("displayOff"_Stz) &&
          ! pTokenVec->pGet(3)->bSame("noPrint"_Stz))
        return false;
      
      if (pTokenVec->pGet(3)->bSame("displayOff"_Stz))
        m_bDisplay = false;
      else {
        ASSERT(pTokenVec->pGet(3)->bSame("noPrint"_Stz));
        m_bPrint = false;
      }
      return true;
    }
    
    //fetch j10 |limit 10|first 10|random 10|;
    if (pTokenVec->pGet(2)->eToken() == TN_NAME) {
      
      if (pTokenVec->pGet(2)->bSame("limit"_Stz))
        m_bLimit = true;
      else if (pTokenVec->pGet(2)->bSame("first"_Stz))
        m_bFirst = true;
      else if (pTokenVec->pGet(2)->bSame("random"_Stz))
        m_bRandom = true;
      else
        return false;
      
      if (pTokenVec->pGet(3)->eToken() == TN_INTEGER) {
        m_iRows = pTokenVec->pGet(3)->iToI8();
        return true;
      }
      
      return false;
      
    }
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - -
  // 5 tokens
  //fetch j10 "where" 1 displayOff|noPrint;
  //fetch j10 "where" displayOff|noPrint;
  //fetch j10 1 |limit 10|first 10|random 10|;
  if (iNumTokens == 5) {
    
    // get where
    if (pTokenVec->pGet(2)->eToken() == TN_STRING) {
      m_sWhere = pTokenVec->pGet(2)->szChars();
    
    // get partition
    if (pTokenVec->pGet(3)->eToken() == TN_INTEGER) {
      m_iPartition = pTokenVec->pGet(3)->iToI8();
    }
    else if (pTokenVec->pGet(3)->eToken() == TN_NAME) {
      
      if (pTokenVec->pGet(3)->bSame("limit"_Stz))
        m_bLimit = true;
      else if (pTokenVec->pGet(3)->bSame("first"_Stz))
        m_bFirst = true;
      else if (pTokenVec->pGet(3)->bSame("random"_Stz))
        m_bRandom = true;
      else
        return false;
      
      if (pTokenVec->pGet(4)->eToken() == TN_INTEGER) {
        m_iRows = pTokenVec->pGet(4)->iToI8();
        return true;
      }
      return false;
    }
    
    // get displayOff
    if (pTokenVec->pGet(4)->eToken() == TN_NAME) {
      if ( pTokenVec->pGet(4)->bSame("displayOff"_Stz) ) {
        // set display off
        m_bDisplay = false;
        return true;
      }
      if ( pTokenVec->pGet(4)->bSame("noPrint"_Stz) ) {
        // set print off
        m_bPrint = false;
        return true;
      }
    }
    return false;
  }
    
  //fetch j10 1 |limit 10|first 10|random 10|;
  if (pTokenVec->pGet(2)->eToken() == TN_INTEGER) {
    
    m_iPartition = pTokenVec->pGet(2)->iToI8();
    
    if (pTokenVec->pGet(3)->eToken() == TN_NAME) {
        
      if (pTokenVec->pGet(3)->bSame("limit"_Stz))
        m_bLimit = true;
      else if (pTokenVec->pGet(3)->bSame("first"_Stz))
        m_bFirst = true;
      else if (pTokenVec->pGet(3)->bSame("random"_Stz))
        m_bRandom = true;
      else
        return false;
    }
    else
      return false;
    
    if (pTokenVec->pGet(4)->eToken() == TN_INTEGER) {
      m_iRows = pTokenVec->pGet(4)->iToI8();
      return true;
    }
  
    return false;
  }

  return false;

}
  
  // - - - - - - - - - - - - - - - - - - - - -
  // 6 tokens
  //fetch j10 "where" 1 |limit 10|first 10|random 10|;
  if (iNumTokens == 6) {
    
    // get where
    if (pTokenVec->pGet(2)->eToken() == TN_STRING) {
      m_sWhere = pTokenVec->pGet(2)->szChars();
      
      // get partition
      if (pTokenVec->pGet(3)->eToken() == TN_INTEGER) {
        m_iPartition = pTokenVec->pGet(3)->iToI8();
      }
      else
        return false;
      
      if (pTokenVec->pGet(4)->eToken() == TN_NAME) {
        
        if (pTokenVec->pGet(4)->bSame("limit"_Stz))
          m_bLimit = true;
        else if (pTokenVec->pGet(4)->bSame("first"_Stz))
          m_bFirst = true;
        else if (pTokenVec->pGet(4)->bSame("random"_Stz))
          m_bRandom = true;
        else
          return false;
      }
      else
        return false;
      
      if (pTokenVec->pGet(5)->eToken() == TN_INTEGER) {
        m_iRows = pTokenVec->pGet(5)->iToI8();
        return true;
      }
      return false;
    }
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* test if row has been seen so far                                           */
/*----------------------------------------------------------------------------*/
bool Job_FetchRows::bRowSeen(Ref xRowRef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_FETCHROWS,_FOOTER_);
  
  if (! m_pRowOnce)
    return false;
  
  bool bSeen = m_pRowOnce->bRowSeen(xRowRef);
  
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
Job* Jobs::pJob_FetchRows_pre(JobClass* aJobClass, ReqTuple* aReqTuple, TokenVec* pTknVec)
{
  Job_FetchRows* pJob_FetchRows = new Job_FetchRows(aJobClass,aReqTuple);
  
  bool bResult = pJob_FetchRows->bPopulateJob(pTknVec);
  
  if ( !bResult) {
    pJob_FetchRows->vJobError(EC_INVALID_COMMAND);
  }
  
  return pJob_FetchRows;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_FetchRows_post(JobClass* aJobClass, Job* pJob)
{
  Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(pJob);
  
  if ( pJob_FetchRows->bJobHasError() ) {
    
    vTrace("error: (node%lld) %s\n"_Stz, AM::pGrid()->iThisNode(),
             aEC[pJob_FetchRows->eJobError()]);
   
    return;
  }
  
  i8 nRowsProcessed = 0;
  i8 nLastBuffersScanned = 0;
  i8 iRowsPrinted = 0;
  
  FetchBuffer* pFetchBuffer = nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // retrieve
  if ( pJob_FetchRows->m_bRetrieve) {
    
    if (pJob_FetchRows->m_bPrint && AM::bMetadata()) {
      Column* pCol = pJob_FetchRows->m_pColumn;
      vTrace("metadata: (%s %s)\n"_Stz, pCol->szColNm(), aCT[pCol->eColumnType()]);
    }
    
    // create a row for retrieval
    pJob_FetchRows->m_pRow = new RowSingle(pJob_FetchRows->m_pTable->pStreamDef(),0);
    
    while ( nLastBuffersScanned != pJob_FetchRows->m_nLastBuffers ) {
      
      pFetchBuffer = _pFetchBuffer(pJob_FetchRows->m_pBufferQueue->pPop());
      if (! pFetchBuffer) {
        //PsxThd::vSleepMilli(1,CN_JOB_FETCHROWS);
        AM::vNapUsec(250,CN_JOB_FETCHROWS);
        continue;
      }
      
      vRetrieveColumn(pJob_FetchRows, pFetchBuffer);
      nRowsProcessed += pFetchBuffer->nFetchedRows();
      
      if (pFetchBuffer->bIsLast()) {
        nLastBuffersScanned++;
      }
      delete pFetchBuffer;
      pFetchBuffer = nullptr;
    }
    
    if (pJob_FetchRows->m_bPrint)
      vTrace("success: %lld rows fetched\n"_Stz,nRowsProcessed);
    else
      vTrace("success: %lld rows affected\n"_Stz,nRowsProcessed);
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // fetch
  if (AM::bMetadata()) {
    vTrace("metadata: %s\n"_Stz, pJob_FetchRows->szMetaData());
  }
  
  // create a row for retrieval
  pJob_FetchRows->m_pRow = new RowSingle(pJob_FetchRows->m_pTable->pStreamDef(), 0);
                   
  while ( nLastBuffersScanned != pJob_FetchRows->m_nLastBuffers ) {
    
    pFetchBuffer = _pFetchBuffer(pJob_FetchRows->m_pBufferQueue->pPop());
    if (! pFetchBuffer) {
      //PsxThd::vSleepMilli(1,CN_JOB_FETCHROWS);
      AM::vNapMsec(1,CN_JOB_FETCHROWS);
      continue;
    }
    
    if (pJob_FetchRows->m_bDisplay) {
      if (pJob_FetchRows->m_bLimit && iRowsPrinted != pJob_FetchRows->m_iRows)
        vIterateBuffer(pJob_FetchRows, pFetchBuffer, iRowsPrinted);
      else
        vIterateBuffer(pJob_FetchRows, pFetchBuffer, iRowsPrinted);
    }
    
    nRowsProcessed += pFetchBuffer->nFetchedRows();
    
    if (pFetchBuffer->bIsLast()) {
      nLastBuffersScanned++;
    }
    delete pFetchBuffer;
    pFetchBuffer = nullptr;
  }
  
  if (pJob_FetchRows->m_iRows == C_UNKNOWN)
    vTrace("success: %lld rows fetched\n"_Stz,nRowsProcessed);
  else {
    if (pJob_FetchRows->m_bFirst)
      vTrace("success: %lld rows fetched; first %lld rows printed\n"_Stz, nRowsProcessed, pJob_FetchRows->m_iRows);
    else if (pJob_FetchRows->m_bLimit)
      vTrace("success: %lld rows fetched\n"_Stz, pJob_FetchRows->m_iRows);
    else
      RASSERT(0);
  }
  
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_FetchRows_delete(Job* pJob)
{
  Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(pJob);
  
  delete pJob_FetchRows;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vIterateBuffer(Job_FetchRows* pJob_FetchRows,
                          FetchBuffer* pFetchBuffer,
                          i8& iRowsPrinted)
{
  StreamDef* pStreamDef = pJob_FetchRows->m_pTable->pStreamDef();
  HV_Rec hvRec(pStreamDef, C_ALL);
  
  Crw** aaCompactRow_cursor = pFetchBuffer->aaFirstCrwPtr();
  
  //pJob_FetchRows->m_pRow->vDontDecodeVch();
  
  while (aaCompactRow_cursor) {
    
    Crw* aCompact = *aaCompactRow_cursor;

    // inflate compact row into row
    RowInflator::vInflateRow(pJob_FetchRows->m_pRow,aCompact);
    
    if (pJob_FetchRows->m_bPrint) {
      
      hvRec->vAppend(pJob_FetchRows->m_pRow);
      
      if (pJob_FetchRows->m_bFirst) {
        if (iRowsPrinted < pJob_FetchRows->m_iRows) {
          hvRec->vPrintRow();
          iRowsPrinted++;
        }
      }
      else if (pJob_FetchRows->m_bLimit) {
        if (iRowsPrinted < pJob_FetchRows->m_iRows) {
          hvRec->vPrintRow();
          iRowsPrinted++;
        }
        else {
          return;
        }
      }
      else
        hvRec->vPrintRow();
    }
    
    aaCompactRow_cursor = pFetchBuffer->aaNextCrwPtr(aaCompactRow_cursor);
  }
  
  return;
}

/*----------------------------------------------------------------------------*/
/* iterate and retrieve column from rows in the buffer                        */
/*----------------------------------------------------------------------------*/
void Jobs::vRetrieveColumn(Job_FetchRows* pJob_FetchRows,
                           FetchBuffer* pFetchBuffer)
{
  //Datum oDatum;
  CrwBuffer* pCrwBuffer = nullptr;
  
  StreamDef* pStreamDef = pJob_FetchRows->m_pTable->pStreamDef();
  Column* pCol = pJob_FetchRows->m_pColumn;
  
  Crw** aaHeadLine = pFetchBuffer->aaFirstCrwPtr();
  
  Crw** aaCrwPtrLimit = (Crw**)pFetchBuffer->aCrwPtrLimit();
  
  //pJob_FetchRows->m_pRow->m_bGetAll = false;
  //pJob_FetchRows->m_pRow->m_abGetCol[pCol->iColumnNo()] = true;
  //pJob_FetchRows->m_pRow->m_iLastCol = pCol->iColumnNo();
  
  while (aaHeadLine < aaCrwPtrLimit) {
    
    if ( ! aaHeadLine )
      break;
    
    Crw* aHeadLine = *aaHeadLine;
    
    // check if aHeadLine is from the last rowBuffer
    if (! CrwBuffer::bCheckAddress(pCrwBuffer,aHeadLine)) {
      pCrwBuffer = CrwBuffer::pToCrwBuffer(aHeadLine);
    }
    
    RowInflator::vInflateColumn(pJob_FetchRows->m_pRow,aHeadLine,pCol->iColumnNo());
    
    if (pJob_FetchRows->m_bPrint) {
      HV_Rec hvRec(pStreamDef,C_ALL);
      
      hvRec->vAppendCol(pCol,pJob_FetchRows->m_pRow);
      vTrace("row: (%s)\n"_Stz,hvRec->szRecStr());
    }
    
    aaHeadLine = aAdd<Crw**>(aaHeadLine,sizeof(Crw*));
  }
  
  return;
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
