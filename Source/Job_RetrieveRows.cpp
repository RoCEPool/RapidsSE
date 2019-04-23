/*============================================================================*/
/* Job_RetrieveRows.cpp -- Retrieve Rows Job                                        */
/*============================================================================*/

// Job Short: RetrieveRows

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_RetrieveRows =
{
  CN_JOB_RETRIEVEROWS,       // class enum
  JK_LOCAL,                  // job kind
  RK_DATA_ROWS,              // response kind
  "Retrieve rows"_Stz,       // comment about the job
  "Retrieve"_Stz,            // the prompt command
  nullptr,                   // the second word
  nullptr,                   // the _run task seed
  "FetchRows_req"_Stz,       // the _req task seed
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
  pJob_RetrieveRows_pre,     // prompt pre-process function
  vJob_RetrieveRows_post,    // prompt post-process function
  nullptr,                   // job new function
  vJob_RetrieveRows_delete,  // job delete function
  nullptr,                   // job pre-send function
  nullptr,                   // job post-receive function
  vJob_RetrieveRows_show,    // job show function
  
  // help and example
  true,
  "Retrieve <tableNm> columnName"_Stz,
  "Retrieve supplier S_SUPPKEY;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_RetrieveRows_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Sanity_class;
  
  return &xJobClass_RetrieveRows;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_RetrieveRows* Jobs::_pJob_RetrieveRows(void* aAddress)
{
  Job_RetrieveRows* pJob_RetrieveRows = reinterpret_cast<Job_RetrieveRows*>(aAddress);
  
  RASSERT_TYPE(pJob_RetrieveRows,CN_JOB_RETRIEVEROWS);
  
  return pJob_RetrieveRows;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_RetrieveRows::operator new(size_t)
{
  Job_RetrieveRows* pJob_RetrieveRows =
    static_cast<Job_RetrieveRows*>(HeapMem::pAllocate(CN_JOB_RETRIEVEROWS));
  
  return pJob_RetrieveRows;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_RetrieveRows::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_RetrieveRows::operator delete(void* aAddress)
{
  Job_RetrieveRows* pJob_RetrieveRows = static_cast<Job_RetrieveRows*>(aAddress);
  
  HeapMem::vFree(pJob_RetrieveRows);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_RetrieveRows::Job_RetrieveRows(Interface::Session* pSession)
: Job(CN_JOB_RETRIEVEROWS, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_RETRIEVEROWS,_FOOTER_);
    
  m_sWhere       = ""_Stz;
  m_pTable       = nullptr;
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
  
  _checkInitialised_(CN_JOB_RETRIEVEROWS);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_RETRIEVEROWS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_RetrieveRows::~Job_RetrieveRows(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_RETRIEVEROWS,_FOOTER_);
  
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
void Job_RetrieveRows::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_RetrieveRows* pJob_RetrieveRows = _pJob_RetrieveRows(pObject);

  pJob_RetrieveRows->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* create row once object                                                     */
/*----------------------------------------------------------------------------*/
void Job_RetrieveRows::vCreateRowOnce(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_RETRIEVEROWS,_FOOTER_);
  
  RASSERT(! m_pRowOnce);
  
  m_pRowOnce = new RowOnce;
}

/*----------------------------------------------------------------------------*/
/* delete last Retrieve buffer                                                   */
/*----------------------------------------------------------------------------*/
void Job_RetrieveRows::vDeleteBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_RETRIEVEROWS,_FOOTER_);
  
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
bool Job_RetrieveRows::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_RETRIEVEROWS,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("Retrieve"_Stz));
  
  // get number of items in token vector
  iNumTokens = pTokenVec->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 2 to 5 tokens
  if (iNumTokens < 2 || iNumTokens > 6)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the table name
  m_sObjectName = pTokenVec->pGet(1)->aText();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // 2 tokens
  //Retrieve j10;
  if (iNumTokens == 2)
    return true;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // 3 tokens
  //Retrieve j10 "where";
  //Retrieve j10 1;
  //Retrieve j10 displayOff|noPrint;
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
  //Retrieve j10 "where" 1;
  //Retrieve j10 |limit 10|first 10|random 10|;
  //Retrieve j10 1 |displayOff|noPrint;
  //Retrieve j10 "where" |displayOff|noPrint;
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
           ! pTokenVec->pGet(3)->aText()->bSame("noPrint"_Stz) &&
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
    
    //Retrieve j10 |limit 10|first 10|random 10|;
    if (pTokenVec->pGet(2)->eToken() == TN_NAME) {
      
      if (pTokenVec->pGet(2)->aText()->bSame("limit"_Stz))
        m_bLimit = true;
      else if (pTokenVec->pGet(2)->aText()->bSame("first"_Stz))
        m_bFirst = true;
      else if (pTokenVec->pGet(2)->aText()->bSame("random"_Stz))
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
  //Retrieve j10 "where" 1 displayOff|noPrint;
  //Retrieve j10 "where" displayOff|noPrint;
  //Retrieve j10 1 |limit 10|first 10|random 10|;
  if (iNumTokens == 5) {
    
    // get where
    if (pTokenVec->pGet(2)->eToken() == TN_STRING) {
      m_sWhere = pTokenVec->pGet(2)->aText();
    
    // get partition
    if (pTokenVec->pGet(3)->eToken() == TN_INTEGER) {
      m_iPartition = pTokenVec->pGet(3)->iToI8();
    }
    else if (pTokenVec->pGet(3)->eToken() == TN_NAME) {
      
      if (pTokenVec->pGet(3)->aText()->bSame("limit"_Stz))
        m_bLimit = true;
      else if (pTokenVec->pGet(3)->aText()->bSame("first"_Stz))
        m_bFirst = true;
      else if (pTokenVec->pGet(3)->aText()->bSame("random"_Stz))
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
    
  //Retrieve j10 1 |limit 10|first 10|random 10|;
  if (pTokenVec->pGet(2)->eToken() == TN_INTEGER) {
    
    m_iPartition = pTokenVec->pGet(2)->iToI8();
    
    if (pTokenVec->pGet(3)->eToken() == TN_NAME) {
        
      if (pTokenVec->pGet(3)->aText()->bSame("limit"_Stz))
        m_bLimit = true;
      else if (pTokenVec->pGet(3)->aText()->bSame("first"_Stz))
        m_bFirst = true;
      else if (pTokenVec->pGet(3)->aText()->bSame("random"_Stz))
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
  //Retrieve j10 "where" 1 |limit 10|first 10|random 10|;
  if (iNumTokens == 6) {
    
    // get where
    if (pTokenVec->pGet(2)->eToken() == TN_STRING) {
      m_sWhere = pTokenVec->pGet(2)->aText();
      
      // get partition
      if (pTokenVec->pGet(3)->eToken() == TN_INTEGER) {
        m_iPartition = pTokenVec->pGet(3)->iToI8();
      }
      else
        return false;
      
      if (pTokenVec->pGet(4)->eToken() == TN_NAME) {
        
        if (pTokenVec->pGet(4)->aText()->bSame("limit"_Stz))
          m_bLimit = true;
        else if (pTokenVec->pGet(4)->aText()->bSame("first"_Stz))
          m_bFirst = true;
        else if (pTokenVec->pGet(4)->aText()->bSame("random"_Stz))
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
bool Job_RetrieveRows::bRowSeen(Id xRowId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_RETRIEVEROWS,_FOOTER_);
  
  if (! m_pRowOnce)
    return false;
  
  bool bSeen = m_pRowOnce->bRowSeen(xRowId);
  
  return bSeen;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_RetrieveRows::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_RETRIEVEROWS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_RetrieveRows::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_RETRIEVEROWS);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_RetrieveRows_pre(Interface::Session* pSession)
{
  Job_RetrieveRows* pJob_RetrieveRows = new Job_RetrieveRows(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommand->pTknVec();
  
  bool bResult = pJob_RetrieveRows->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    pJob_RetrieveRows->vAddError(EC_INVALID_COMMAND);
  }
  
  return pJob_RetrieveRows;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_RetrieveRows_post(Interface::Session* pSession, Job* pJob)
{
  Job_RetrieveRows* pJob_RetrieveRows = _pJob_RetrieveRows(pJob);
  
  GenVar xVariable;
  i8 nRowsProcessed = 0;
  i8 nLastBuffersScanned = 0;
  i8 iRowsPrinted = 0;
  
  FetchBuffer* pFetchBuffer = nullptr;
  bool bDone = false;
  
  if ( pJob_RetrieveRows->m_bHasError ) {
    
    vTrace("error: (node%lld) %s\n"_Stz, gpAttachMap->m_pGrid->iThisNode(),
             aEC[pJob_RetrieveRows->m_xResponse.m_eErrorCode]);
   
    return;
  }
  
  if (gpAttachMap->m_bMetadata) {
    vTrace("metadata: %s\n"_Stz, pJob_RetrieveRows->m_sMetadata.aStz());
  }
  
  while ( nLastBuffersScanned != pJob_RetrieveRows->m_nLastBuffers ) {
    
    pFetchBuffer = _pFetchBuffer(pJob_RetrieveRows->m_pBufferQueue->pPop());
    if (! pFetchBuffer) {
      Pthread::vSleepMilli(1,CN_JOB_RETRIEVEROWS);
      continue;
    }
    
    if (pJob_RetrieveRows->m_bDisplay) {
      if (pJob_RetrieveRows->m_bLimit && iRowsPrinted != pJob_RetrieveRows->m_iRows)
        vIterateBuffer(pJob_RetrieveRows, pFetchBuffer, iRowsPrinted);
      else
        vIterateBuffer(pJob_RetrieveRows, pFetchBuffer, iRowsPrinted);
    }
    
    nRowsProcessed += pFetchBuffer->nFetchedRows();
    
    if (pFetchBuffer->bIsLast()) {
      nLastBuffersScanned++;
    }
    delete pFetchBuffer;
    pFetchBuffer = nullptr;
  }
  
  if (pJob_RetrieveRows->m_iRows == C_UNKNOWN)
    vTrace("success: %lld rows Retrieveed\n"_Stz,nRowsProcessed);
  else {
    if (pJob_RetrieveRows->m_bFirst)
      vTrace("success: %lld rows Retrieveed; first %lld rows printed\n"_Stz, nRowsProcessed, pJob_RetrieveRows->m_iRows);
    else if (pJob_RetrieveRows->m_bLimit)
      vTrace("success: %lld rows Retrieveed\n"_Stz, pJob_RetrieveRows->m_iRows);
    else
      RASSERT(0);
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_RetrieveRows_delete(Interface::Session* pSession, Job* pJob)
{
  Job_RetrieveRows* pJob_RetrieveRows = _pJob_RetrieveRows(pJob);
  
  delete pJob_RetrieveRows;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vIterateBuffer(Job_RetrieveRows* pJob_RetrieveRows,
                          FetchBuffer* pFetchBuffer,
                          i8& iRowsPrinted)
{
  HV_Rec hvRec;

  ColumnVec* pColVec = pJob_RetrieveRows->m_pTable->pColumnVec();
  
  HeadLine** aaCacheLine = pFetchBuffer->aaFirstRowPtr();
  while (aaCacheLine) {
    
    HeadLine* aHeadLine = *aaCacheLine;

    RowCoder::vRecord(hvRec,aHeadLine,pColVec);

    if (pJob_RetrieveRows->m_bPrint) {
      if (pJob_RetrieveRows->m_bFirst) {
        if (iRowsPrinted < pJob_RetrieveRows->m_iRows) {
          hvRec->vPrintRow();
          iRowsPrinted++;
        }
      }
      else if (pJob_RetrieveRows->m_bLimit) {
        if (iRowsPrinted < pJob_RetrieveRows->m_iRows) {
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
    
    aaCacheLine = pFetchBuffer->aaNextRowPtr(aaCacheLine);
  }
  
  return;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_RetrieveRows_show(Job* pJob)
{
  Job_RetrieveRows* pJob_RetrieveRows = _pJob_RetrieveRows(pJob);

  pJob_RetrieveRows->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
