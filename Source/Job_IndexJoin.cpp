/*============================================================================*/
/* Job_IndexJoin.cpp -- Fetch Rows Job                                        */
/*============================================================================*/

#include "Jobs.h"

JobClass xJobClass_IndexJoin =
{
  CN_JOB_INDEXJOIN,          // class number
  JK_LOCAL,                  // job kind
  RS_DATA_ROWS,              // response kind
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
Job_IndexJoin::Job_IndexJoin(JobClass* aJobClass, ReqTuple* aReqTuple)
: Job(aJobClass, aReqTuple)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INDEXJOIN,_FOOTER_);
  
  m_sRightTable  = ""_Stz;
  m_sRightIndex  = ""_Stz;
  m_pLeftTable   = nullptr;
  m_pRightTable  = nullptr;
  m_pLeftIndex   = nullptr;
  m_pRightIndex  = nullptr;
  m_bPrint       = false;
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
  
  if (iNumTokens != 5 && iNumTokens != 6)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the left table name
  m_oEnameSet.vAdd("<tableNm>"_Stz, pTokenVec->pGet(1)->szText());
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the left index name
  m_oEnameSet.vAdd("<indexNm>"_Stz, pTokenVec->pGet(2)->szText());
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the right table name
  m_sRightTable = pTokenVec->pGet(3)->szText();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the right index name
  m_sRightIndex = pTokenVec->pGet(4)->szText();

  if (iNumTokens == 6) {
    if (pTokenVec->pGet(5)->eToken() == TN_NAME) {
      if (pTokenVec->pGet(5)->szText()->bSame("print"_Stz)) {
        m_bPrint = true;
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
Job* Jobs::pJob_IndexJoin_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession)
{
  Job_IndexJoin* pJob_IndexJoin = new Job_IndexJoin(aJobClass,aReqTuple);
  //pJob->vDicPath(pSession);

  TokenVec* pTokenVec = pSession->m_pInput->pTknVec();
  
  bool bResult = pJob_IndexJoin->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    pJob_IndexJoin->vAddError(EC_INVALID_COMMAND);
  }
  
  return pJob_IndexJoin;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_IndexJoin_post(Job* pJob)
{
  Job_IndexJoin* pJob_IndexJoin = _pJob_IndexJoin(pJob);
  
  if ( pJob_IndexJoin->m_bHasError ) {
    
    vTrace("error: (node%lld) %s\n"_Stz, AM::pGrid()->iThisNode(),
             aEC[pJob_IndexJoin->m_xResponse.m_eErrorCode]);
   
    return;
  }
  
  i8 nLastBuffersScanned = 0;
  i8 nRowsProcessed = 0;
  
  RefBuffer* pRefBuffer = nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // fetch
  if (AM::bMetadata()) {
    vTrace("metadata: %s\n"_Stz, pJob_IndexJoin->m_sMetadata.szStz());
  }
  
  while ( nLastBuffersScanned != pJob_IndexJoin->m_nLastBuffers ) {
    
    pRefBuffer = _pRefBuffer(pJob_IndexJoin->m_pBufferQueue->pPop());
    if (! pRefBuffer) {
      //PsxThd::vSleepMilli(1,CN_JOB_FETCHROWS);
      AM::vNapUsec(250,CN_JOB_FETCHROWS);
      continue;
    }
    
    if (pJob_IndexJoin->m_bPrint) {
      vIterateBuffer(pJob_IndexJoin, pRefBuffer);
    }
    
    nRowsProcessed += pRefBuffer->nItems();
    
    if (pRefBuffer->bIsLast()) {
      nLastBuffersScanned++;
    }
    delete pRefBuffer;
    pRefBuffer = nullptr;
  }
  
  if (pJob_IndexJoin->m_bPrint)
    vTrace("success: %lld rows fetched\n"_Stz,nRowsProcessed);
  else
    vTrace("success: %lld rows affected\n"_Stz,nRowsProcessed);
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_IndexJoin_delete(Job* pJob)
{
  Job_IndexJoin* pJob_IndexJoin = _pJob_IndexJoin(pJob);
  
  delete pJob_IndexJoin;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vIterateBuffer(Job_IndexJoin* pJob_IndexJoin,
                          RefBuffer* pRefBuffer)
{
  // dump reference buffer
  //pRefBuffer->vShow();
  
  //vTrace("Rows (by reference):\n"_Stz);
  
  ColumnVec* pLeftColVec  = pJob_IndexJoin->m_pLeftTable->pColumnVec();
  ColumnVec* pRightColVec = pJob_IndexJoin->m_pRightTable->pColumnVec();
  
  i8 nJoinRows = pRefBuffer->nItems();
  RefSet* pRefSet = pRefBuffer->aFirstRefSet();
  
  HV_Rec hvRecLeft(pLeftColVec);
  HV_Rec hvRecRight(pRightColVec);
  
  // create a left and a right row
  HV_Row hLeftRow(pLeftColVec);
  HV_Row hRightRow(pRightColVec);
  
  while (nJoinRows-- > 0) {
    
    // get the left and right rows from the refSet
    Crw* aLeftRow = (Crw*)pRefSet->iRowRef(0);
    Crw* aRightRow = (Crw*)pRefSet->iRowRef(1);
    
    // inflate left row
    if (aLeftRow) {
      RowInflator::vInflateRow(hLeftRow,aLeftRow);
      hvRecLeft->vAppend(hLeftRow);
    }
    
    // inflate right row
    if (aRightRow) {
      RowInflator::vInflateRow(hRightRow,aRightRow);
      hvRecRight->vAppend(hRightRow);
    }
    
    vTrace("row: (%s) ⋈ (%s)\n"_Stz,
           aLeftRow  ? hvRecLeft->szRecStr()  : "NULL"_Stz,
           aRightRow ? hvRecRight->szRecStr() : "NULL"_Stz);
    
    // get next row ref
    pRefSet = pRefBuffer->aNextRefSet(pRefSet);
  }
  
  return;
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
