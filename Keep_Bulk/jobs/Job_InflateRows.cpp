/*============================================================================*/
/* Job_InflateRows.cpp -- Fetch Rows Job                                        */
/*============================================================================*/

#include "Jobs.h"

JobClass xJobClass_InflateRows =
{
  CN_JOB_INFLATEROWS,        // class number
  JK_LOCAL,                  // job kind
  RS_DATA_ROWS,              // response kind
  "inflate rows"_Stz,        // comment about the job
  "inflate"_Stz,             // the prompt command
  nullptr,                   // the second word
  nullptr,                   // the _run task seed
  "inflateRows_req"_Stz,     // the _req task seed
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
Job_InflateRows::Job_InflateRows(JobClass* aJobClass, ReqTuple* aReqTuple)
: Job(aJobClass, aReqTuple)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INFLATEROWS,_FOOTER_);
    
  m_sWhere       = ""_Stz;
  m_sColumnName  = ""_Stz;
  m_pTable       = nullptr;
  m_pColumn      = nullptr;
  m_bPrint       = true;
  m_nSubInflateTasks = 1;
  m_pBufferQueue = new Queue;

  m_inBufferCount = 0;
  m_inBuffersScanned = 0;
  
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
  m_oEnameSet.vAdd("<tableNm>"_Stz, pTokenVec->pGet(1)->szText());
  
  // - - - - - - - - - - - - - - - - - - - - -
  // inflate j10;
  if (iNumTokens == 2)
    return true;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // inflate j10 column;
  // inflate j10 print;
  // inflate j10 threads;
  if ( iNumTokens == 3) {
    if (pTokenVec->pGet(2)->eToken() == TN_NAME) {
      
      if (pTokenVec->pGet(2)->szText()->bSame("print"_Stz)) {
        m_bPrint = true;
        return true;
      }
      
      m_sColumnName = pTokenVec->pGet(2)->szText();
      return true;
    }
    
    if (pTokenVec->pGet(2)->eToken() == TN_INTEGER) {
      m_nSubInflateTasks = pTokenVec->pGet(2)->iToI8();
      return true;
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - -
  // inflate j10 column print;
  // inflate j10 column threads;
  // inflate j10 print threads;
  if ( iNumTokens == 4) {
    
    if (pTokenVec->pGet(2)->eToken() == TN_NAME) {
      
      // inflate j10 print threads;
      if (pTokenVec->pGet(2)->szText()->bSame("print"_Stz)) {
        m_bPrint = true;
        
        if (pTokenVec->pGet(3)->eToken() == TN_INTEGER) {
          m_nSubInflateTasks = pTokenVec->pGet(3)->iToI8();
          return true;
        }
        return false;
      }
      
      // inflate j10 column print;
      // inflate j10 column threads;
      m_sColumnName = pTokenVec->pGet(2)->szText();
      
      // inflate j10 column print;
      if (pTokenVec->pGet(3)->eToken() == TN_NAME) {
        if (pTokenVec->pGet(3)->szText()->bSame("print"_Stz)) {
          m_bPrint = true;
          return true;
        }
        return false;
      }
      
      // inflate j10 column threads;
      if (pTokenVec->pGet(3)->eToken() == TN_INTEGER) {
        m_nSubInflateTasks = pTokenVec->pGet(2)->iToI8();
        return true;
      }

    }
    
  }
  
  return false;
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
Job* Jobs::pJob_InflateRows_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession)
{
  Job_InflateRows* pJob_InflateRows = new Job_InflateRows(aJobClass,aReqTuple);
  //pJob->vDicPath(pSession);

  TokenVec* pTokenVec = pSession->m_pInput->pTknVec();
  
  bool bResult = pJob_InflateRows->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    pJob_InflateRows->vAddError(EC_INVALID_COMMAND);
  }
  
  return pJob_InflateRows;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_InflateRows_post(Job* pJob)
{
  Job_InflateRows* pJob_InflateRows = _pJob_InflateRows(pJob);
  
  if ( pJob_InflateRows->m_bHasError ) {
    
    vTrace("error: (node%lld) %s\n"_Stz, AM::pGrid()->iThisNode(),
           aEC[pJob_InflateRows->m_xResponse.m_eErrorCode]);
    
    return;
  }
  
  if (pJob_InflateRows->m_bPrint)
    vTrace("success: %lld rows fetched\n"_Stz,*pJob_InflateRows->m_inRowsProcessed);
  else
    vTrace("success: %lld rows affected\n"_Stz,*pJob_InflateRows->m_inRowsProcessed);

}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_InflateRows_delete(Job* pJob)
{
  Job_InflateRows* pJob_InflateRows = _pJob_InflateRows(pJob);
  
  delete pJob_InflateRows;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vRetrieveRows(Job_InflateRows* pJob_InflateRows,
                          CrwBuffer* pCrwBuffer, Row* pRow)
{
  ColumnVec* pColVec = pJob_InflateRows->m_pTable->pColumnVec();
  HV_Rec hvRec(pColVec);
  
  //pJob_InflateRows->m_pRow->vDontDecodeVch();
  
  i8 nRowCount = pCrwBuffer->nRowCount();
  for (i8 i_Row = 0; i_Row < nRowCount; i_Row++) {
    
    // get the inflated compact row
    pCrwBuffer->vGetRow(pRow, i_Row);
    
    if (pJob_InflateRows->m_bPrint) {
      
      hvRec->vAppend(pRow);
      hvRec->vPrintRow();
    }
  }
}

/*----------------------------------------------------------------------------*/
/* iterate and retrieve column from rows in the buffer                        */
/*----------------------------------------------------------------------------*/
void Jobs::vRetrieveColumns(Job_InflateRows* pJob_InflateRows,
                            CrwBuffer* pCrwBuffer, Row* pRow)
{
  ColumnVec* pColVec = pJob_InflateRows->m_pTable->pColumnVec();
  Column* pCol = pJob_InflateRows->m_pColumn;
  
  pRow->m_bGetAll = false;
  pRow->m_abGetCol[pCol->iColumnNo()] = true;
  pRow->m_iLastCol = pCol->iColumnNo();
  
  i8 nRowCount = pCrwBuffer->nRowCount();
  for (i8 i_Row = 0; i_Row < nRowCount; i_Row++) {
    
    // get the inflated compact row
    pCrwBuffer->vGetRow(pRow, i_Row);
    
    if (pJob_InflateRows->m_bPrint) {
      HV_Rec hvRec(pColVec);
      
      hvRec->vAppend(pCol,pRow);
      vTrace("row: (%s)\n"_Stz,hvRec->szRecStr());
    }
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
