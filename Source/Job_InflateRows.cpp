/*============================================================================*/
/* Job_InflateRows.cpp -- Fetch Rows Job                                        */
/*============================================================================*/

// Job Short: InflateRows

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_InflateRows =
{
  CN_JOB_INFLATEROWS,        // class enum
  JK_LOCAL,                  // job kind
  RK_DATA_ROWS,              // response kind
  "inflate rows"_Stz,        // comment about the job
  "inflate"_Stz,             // the prompt command
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
  JF_NONE,                   // flags

  eJob_checkCluster,         // prompt check function
  pJob_InflateRows_pre,        // prompt pre-process function
  vJob_InflateRows_post,       // prompt post-process function
  nullptr,                   // job new function
  vJob_InflateRows_delete,     // job delete function
  nullptr,                   // job pre-send function
  nullptr,                   // job post-receive function
  vJob_InflateRows_show,       // job show function
  
  // help and example
  true,
  "inflate <tableNm> [\"<where>\"] [displayOff | noPrint]"_Stz,
  "inflate supplier \"S_SUPPKEY 22 ==\";"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_InflateRows_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Ingest_class;
  
  return &xJobClass_InflateRows;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_InflateRows* Jobs::_pJob_InflateRows(void* aAddress)
{
  Job_InflateRows* pJob_InflateRows = reinterpret_cast<Job_InflateRows*>(aAddress);
  
  RASSERT_TYPE(pJob_InflateRows,CN_JOB_INFLATEROWS);
  
  return pJob_InflateRows;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_InflateRows::operator new(size_t)
{
  Job_InflateRows* pJob_InflateRows =
    static_cast<Job_InflateRows*>(HeapMem::pAllocate(CN_JOB_INFLATEROWS));
  
  return pJob_InflateRows;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_InflateRows::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_InflateRows::operator delete(void* aAddress)
{
  Job_InflateRows* pJob_InflateRows = static_cast<Job_InflateRows*>(aAddress);
  
  HeapMem::vFree(pJob_InflateRows);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_InflateRows::Job_InflateRows(Interface::Session* pSession)
: Job(CN_JOB_INFLATEROWS, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INFLATEROWS,_FOOTER_);
    
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
  
  _checkInitialised_(CN_JOB_INFLATEROWS);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_INFLATEROWS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_InflateRows::~Job_InflateRows(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INFLATEROWS,_FOOTER_);
  
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
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_InflateRows::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_InflateRows* pJob_InflateRows = _pJob_InflateRows(pObject);

  pJob_InflateRows->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* create row once object                                                     */
/*----------------------------------------------------------------------------*/
void Job_InflateRows::vCreateRowOnce(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INFLATEROWS,_FOOTER_);
  
  RASSERT(! m_pRowOnce);
  
  m_pRowOnce = new RowOnce;
}

/*----------------------------------------------------------------------------*/
/* delete last fetch buffer                                                   */
/*----------------------------------------------------------------------------*/
void Job_InflateRows::vDeleteBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INFLATEROWS,_FOOTER_);
  
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
bool Job_InflateRows::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INFLATEROWS,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("inflate"_Stz));
  
  // get number of items in token vector
  iNumTokens = pTokenVec->nItems();
    
  // set default to be no print
  m_bPrint = false;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the table name
  m_sObjectName = pTokenVec->pGet(1)->aText();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // inflate j10;
  if (iNumTokens == 2)
    return true;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // inflate j10 column;
  // inflate j10 print;
  if ( iNumTokens == 3) {
    if (pTokenVec->pGet(2)->eToken() == TN_NAME) {
      m_sColumnName = pTokenVec->pGet(2)->aText();
      return true;
    }
    
    if (pTokenVec->pGet(3)->aText()->bSame("print"_Stz))
        m_bPrint = true;
      else
        return false;
    
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - -
  // inflate j10 column print;
  if ( iNumTokens == 4) {
    if (pTokenVec->pGet(2)->eToken() == TN_NAME) {
      m_sColumnName = pTokenVec->pGet(2)->aText();
    }
    else
      return false;
    
    if (pTokenVec->pGet(3)->aText()->bSame("print"_Stz))
      m_bPrint = true;
    else
      return false;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test if row has been seen so far                                           */
/*----------------------------------------------------------------------------*/
bool Job_InflateRows::bRowSeen(Id xRowId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INFLATEROWS,_FOOTER_);
  
  if (! m_pRowOnce)
    return false;
  
  bool bSeen = m_pRowOnce->bRowSeen(xRowId);
  
  return bSeen;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_InflateRows::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INFLATEROWS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_InflateRows::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INFLATEROWS);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_InflateRows_pre(Interface::Session* pSession)
{
  Job_InflateRows* pJob_InflateRows = new Job_InflateRows(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommand->pTknVec();
  
  bool bResult = pJob_InflateRows->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    pJob_InflateRows->vAddError(EC_INVALID_COMMAND);
  }
  
  return pJob_InflateRows;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_InflateRows_post(Interface::Session* pSession, Job* pJob)
{
  Job_InflateRows* pJob_InflateRows = _pJob_InflateRows(pJob);
  
  if ( pJob_InflateRows->m_bHasError ) {
    
    vTrace("error: (node%lld) %s\n"_Stz, gpAttachMap->m_pGrid->iThisNode(),
             aEC[pJob_InflateRows->m_xResponse.m_eErrorCode]);
   
    return;
  }
  
  i8 nRowsProcessed = 0;
  i8 nLastBuffersScanned = 0;
  
  FetchBuffer* pFetchBuffer = nullptr;
    
  if (pJob_InflateRows->m_bPrint && gpAttachMap->m_bMetadata) {
    Column* pCol = pJob_InflateRows->m_pColumn;
    vTrace("metadata: (%s %s)\n"_Stz, pCol->sColumnName().aChar(), aCT[pCol->eColumnType()]);
  }
  
  // create a row for retrieval
  pJob_InflateRows->m_pRow = new Row(pJob_InflateRows->m_pTable->pColumnVec());
  
  while ( nLastBuffersScanned != pJob_InflateRows->m_nLastBuffers ) {
    
    pFetchBuffer = _pFetchBuffer(pJob_InflateRows->m_pBufferQueue->pPop());
    if (! pFetchBuffer) {
      //Pthread::vSleepMilli(1,CN_JOB_INFLATEROWS);
      Pthread::vSleepMicro(250,CN_JOB_INFLATEROWS);
      continue;
    }
    
    vRetrieveColumn(pJob_InflateRows, pFetchBuffer);
    nRowsProcessed += pFetchBuffer->nFetchedRows();
    
    if (pFetchBuffer->bIsLast()) {
      nLastBuffersScanned++;
    }
    delete pFetchBuffer;
    pFetchBuffer = nullptr;
  }
  
  if (pJob_InflateRows->m_bPrint)
    vTrace("success: %lld rows fetched\n"_Stz,nRowsProcessed);
  else
    vTrace("success: %lld rows affected\n"_Stz,nRowsProcessed);

}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_InflateRows_delete(Interface::Session* pSession, Job* pJob)
{
  Job_InflateRows* pJob_InflateRows = _pJob_InflateRows(pJob);
  
  delete pJob_InflateRows;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vIterateBuffer(Job_InflateRows* pJob_InflateRows,
                          FetchBuffer* pFetchBuffer,
                          i8& iRowsPrinted)
{
  ColumnVec* pColVec = pJob_InflateRows->m_pTable->pColumnVec();
  HV_Rec hvRec(pColVec);
  
  HeadLine** aaCompactRow_cursor = pFetchBuffer->aaFirstRowPtr();
  
  //pJob_InflateRows->m_pRow->vDontDecodeVch();
  
  while (aaCompactRow_cursor) {
    
    HeadLine* aCompact = *aaCompactRow_cursor;

    // inflate compact row into row
    RowCoder::vInflate(pJob_InflateRows->m_pRow,aCompact);
    
    if (pJob_InflateRows->m_bPrint) {
      
      hvRec->vAppend(pJob_InflateRows->m_pRow);
      
      if (pJob_InflateRows->m_bFirst) {
        if (iRowsPrinted < pJob_InflateRows->m_iRows) {
          hvRec->vPrintRow();
          iRowsPrinted++;
        }
      }
      else if (pJob_InflateRows->m_bLimit) {
        if (iRowsPrinted < pJob_InflateRows->m_iRows) {
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
    
    aaCompactRow_cursor = pFetchBuffer->aaNextRowPtr(aaCompactRow_cursor);
  }
  
  return;
}

/*----------------------------------------------------------------------------*/
/* iterate and retrieve column from rows in the buffer                        */
/*----------------------------------------------------------------------------*/
void Jobs::vRetrieveColumn(Job_InflateRows* pJob_InflateRows,
                           FetchBuffer* pFetchBuffer)
{
  //Datum oDatum;
  RowBuffer* pRowBuffer = nullptr;
  
  ColumnVec* pColVec = pJob_InflateRows->m_pTable->pColumnVec();
  Column* pCol = pJob_InflateRows->m_pColumn;
  
  HeadLine** aaHeadLine = pFetchBuffer->aaFirstRowPtr();
  
  HeadLine** aaLatter = (HeadLine**)pFetchBuffer->aLatter();
  
  pJob_InflateRows->m_pRow->m_bGetAll = false;
  pJob_InflateRows->m_pRow->m_abGetCol[pCol->iColumnNo()] = true;
  pJob_InflateRows->m_pRow->m_iLastCol = pCol->iColumnNo();
  
  while (aaHeadLine < aaLatter) {
    
    if ( ! aaHeadLine )
      break;
    
    HeadLine* aHeadLine = *aaHeadLine;
    
    // check if aHeadLine is from the last rowBuffer
    if (! RowBuffer::bCheckAddress(pRowBuffer,aHeadLine)) {
      pRowBuffer = RowBuffer::pToRowBuffer(aHeadLine);
    }
    
    RowCoder::vInflate(pJob_InflateRows->m_pRow,aHeadLine);
    
    if (pJob_InflateRows->m_bPrint) {
      HV_Rec hvRec(pColVec);
      
      hvRec->vAppend(pCol,pJob_InflateRows->m_pRow);
      vTrace("row: (%s)\n"_Stz,hvRec->szRecStr());
    }
    
    aaHeadLine = aAdd<HeadLine**>(aaHeadLine,sizeof(HeadLine*));
  }
  
  return;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_InflateRows_show(Job* pJob)
{
  Job_InflateRows* pJob_InflateRows = _pJob_InflateRows(pJob);

  pJob_InflateRows->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
