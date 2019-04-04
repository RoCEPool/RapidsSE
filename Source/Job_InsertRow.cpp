/*============================================================================*/
/* Job_InsertRow.cpp -- Single Row Insert Job                                 */
/*============================================================================*/

// Job Short: InsertRow

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_InsertRow =
{
  CN_JOB_INSERTROW,       // class enum
  JK_DIST2PC,             // job kind
  RK_NONE,                // response kind
  "insert into tableNm"_Stz, // comment about the job
  "insert"_Stz,           // the prompt command
  "into"_Stz,             // the second word
  nullptr,                // the _run task seed
  "insertRow_req"_Stz,    // the _req task seed
  "insertRow_beg"_Stz,    // the _rcv task seed
  "insertRow_exe"_Stz,    // the _exe task seed
  "insertRow_end"_Stz,    // the _end task seed
  "insertRow_can"_Stz,    // the _can task seed
  true,                   // check resources?
  false,                  // detached job?
  false,                  // buffer attached?
  JT_NONE,                // job trace
  DL_GRAB_READ,           // ddl lock model
  PY_NORMAL,              // priority
  JF_NONE,                // flags

  eJob_checkCluster,      // prompt check function
  pJob_InsertRow_pre,     // prompt pre-process function
  vJob_InsertRow_post,    // prompt post-process function
  pJob_InsertRow_new,     // job new function
  vJob_InsertRow_delete,  // job delete function
  vJob_send,              // job pre-send function
  vJob_recv,              // job post-receive function
  vJob_InsertRow_show,    // job show function
  
  // help and example
  true,
  "insert into <tableNm> values ( <values> );"_Stz,
  "insert into t1 values (10,'aaa');"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_InsertRow_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_InsertTable_class;
  
  return &xJobClass_InsertRow;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_InsertRow* Jobs::_pJob_InsertRow(void* aAddress)
{
  Job_InsertRow* pJob_InsertRow = reinterpret_cast<Job_InsertRow*>(aAddress);
  
  RASSERT_TYPE(pJob_InsertRow,CN_JOB_INSERTROW);
  
  return pJob_InsertRow;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_InsertRow::operator new(size_t)
{
  Job_InsertRow* pJob_LoadTable =
    static_cast<Job_InsertRow*>(HeapMem::pAllocate(CN_JOB_INSERTROW));
  
  return pJob_LoadTable;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_InsertRow::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_InsertRow::operator delete(void* aAddress)
{
  Job_InsertRow* pJob_InsertRow = static_cast<Job_InsertRow*>(aAddress);
  
  HeapMem::vFree(pJob_InsertRow);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_InsertRow::Job_InsertRow(Interface::Session* pSession)
: Job(CN_JOB_INSERTROW, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INSERTROW,_FOOTER_);
  m_pTable = nullptr;
  
  m_pRecord = new Record;
  m_nRawDataBytes = C_UNKNOWN;
  m_iPartitionId = C_UNKNOWN;
  
  _checkInitialised_(CN_JOB_INSERTROW);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_INSERTROW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_InsertRow::~Job_InsertRow(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INSERTROW,_FOOTER_);
  
  delete m_pRecord;
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_InsertRow::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_InsertRow* pJob_InsertRow = _pJob_InsertRow(pObject);

  pJob_InsertRow->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_InsertRow::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INSERTROW,_FOOTER_);
  
  ASSERT(pTokenVec->pGet(0)->bSame("insert"_Stz));
  ASSERT(pTokenVec->pGet(1)->bSame("into"_Stz));
  
  if ( ! pTokenVec->pGet(3)->bSame("values"_Stz))
    return false;
  
//  if ( ! pTokenVec->pGet(4)->bSame("("_Stz))
//    return false;
//
//  // get no of tokens in token vector
//  i8 nTokens = pTokenVec->nTokens();;
//
//  // there should be at least 6 tokens
//  if (nTokens < 6)
//    return false;
//
//  // get the table name
//  m_sObjectName = pTokenVec->pGet(2)->aText();
//
//  s17KB s;
//  pTokenVec->vToString(s);
//
//  // get the rawRecord
//  i8 i = 5;
//  m_aRawData.vCopy(""_Stz);
//  while ( ! pTokenVec->pGet(i)->bSame(")"_Stz)) {
//    m_aRawData.vConcat(pTokenVec->pGet(i++)->aText());
//    if ( ! pTokenVec->pGet(i)->bSame(","_Stz))
//      continue;
//    m_aRawData.vConcat(pTokenVec->pGet(i++)->aText());
//  }
//
//  if ( ! pTokenVec->pGet(i)->bSame(")"_Stz))
//    return false;
//
  // get the length of the record
  //m_nRawDataBytes = StrLib::iSize(m_aRawData.aChar());

  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_InsertRow::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INSERTROW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_InsertRow::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INSERTROW);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_InsertRow_pre(Interface::Session* pSession)
{
  Job_InsertRow* pJob_InsertRow = new Job_InsertRow(pSession);
  
//  TokenVec* pTokenVec = pSession->m_pCommand->pTknVec();
  
//  bool bResult = pJob_InsertRow->bPopulateJob(pTokenVec);
//
//  if ( !bResult) {
//    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
//    pJob_InsertRow->m_bHasError = true;
//    pJob_InsertRow->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
//  }
  
  // get a jobId for the insertRow job
  //i8 iJobId = gpAttachMap->m_pOriginatingJobs->iNextJobId();
  //pJob_InsertRow->vJobId(iJobId);
  //vTrace("InsertRow id %lld\n"_Stz, iJobId);
  
  return pJob_InsertRow;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_InsertRow_post(Interface::Session* pSession, Job* pJob)
{
  Job_InsertRow* pJob_InsertRow = _pJob_InsertRow(pJob);
  
  if ( pJob_InsertRow->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_InsertRow->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_InsertRow_new(Job* pArg)
{
  Job_InsertRow* pJob_InsertRow = new Job_InsertRow(nullptr);
  
  return pJob_InsertRow;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_InsertRow_delete(Interface::Session* pSession, Job* pJob)
{
  Job_InsertRow* pJob_InsertRow = _pJob_InsertRow(pJob);
  
  //vTrace("DELETING INSERTROW\n"_Stz);
  delete pJob_InsertRow;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_InsertRow_show(Job* pJob)
{
  Job_InsertRow* pJob_InsertRow = _pJob_InsertRow(pJob);

  pJob_InsertRow->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
