/*============================================================================*/
/* Job_SelectTable.cpp -- Fetch Rows Job                                      */
/*============================================================================*/

#include "Jobs.h"

JobClass xJobClass_SelectTable =
{
  CN_JOB_SELECTTABLE,        // class number
  JK_LOCAL,                  // job kind
  RS_NONE,                   // response kind
  "select table"_Stz,        // comment about the job
  "select"_Stz,              // the prompt command
  "table"_Stz,               // the second word
  nullptr,                   // the _run task seed
  "selectTable_req"_Stz,     // the _req task seed
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
  pJob_SelectTable_pre,      // prompt pre-process function
  vJob_post,                 // prompt post-process function
  nullptr,                   // job new function
  vJob_SelectTable_delete,   // job delete function
  nullptr,                   // job pre-send function
  nullptr,                   // job post-receive function
  vJob_SelectTable_show,     // job show function
  
  // help and example
  true,
  "select <tableNm> <partitionNo>"_Stz,
  "fetch supplier 1;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_SelectTable_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Sink_class;
  
  return &xJobClass_SelectTable;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_SelectTable* Jobs::_pJob_SelectTable(void* aAddress)
{
  Job_SelectTable* pJob_SelectTable = reinterpret_cast<Job_SelectTable*>(aAddress);
  
  RASSERT_TYPE(pJob_SelectTable,CN_JOB_SELECTTABLE);
  
  return pJob_SelectTable;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_SelectTable::operator new(size_t)
{
  Job_SelectTable* pJob_SelectTable =
    static_cast<Job_SelectTable*>(HeapMem::pAllocate(CN_JOB_SELECTTABLE));
  
  return pJob_SelectTable;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_SelectTable::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_SelectTable::operator delete(void* aAddress)
{
  Job_SelectTable* pJob_SelectTable = static_cast<Job_SelectTable*>(aAddress);
  
  HeapMem::vFree(pJob_SelectTable);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_SelectTable::Job_SelectTable(JobClass* aJobClass, ReqTuple* aReqTuple)
: Job(aJobClass, aReqTuple)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SELECTTABLE,_FOOTER_);
    
  m_pTable       = nullptr;
  m_iPartition   = -1;
  
  _checkInitialised_(CN_JOB_SELECTTABLE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_SELECTTABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_SelectTable::~Job_SelectTable(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SELECTTABLE,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_SelectTable::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_SelectTable* pJob_SelectTable = _pJob_SelectTable(pObject);

  pJob_SelectTable->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_SelectTable::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SELECTTABLE,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("select"_Stz));
  
  // get number of items in token vector
  iNumTokens = pTokenVec->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens < 3)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the table name
  m_oEnameSet.vAdd("<tableNm>"_Stz, pTokenVec->pGet(1)->szText());
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the partition number
  m_iPartition = pTokenVec->pGet(2)->iToI8();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_SelectTable::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SELECTTABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_SelectTable::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SELECTTABLE);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_SelectTable_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession)
{
  Job_SelectTable* pJob_SelectTable = new Job_SelectTable(aJobClass,aReqTuple);
  //pJob->vDicPath(pSession);

  TokenVec* pTokenVec = pSession->m_pInput->pTknVec();
  
  bool bResult = pJob_SelectTable->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    pJob_SelectTable->vAddError(EC_INVALID_COMMAND);
  }
  
  return pJob_SelectTable;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_SelectTable_delete(Job* pJob)
{
  Job_SelectTable* pJob_SelectTable = _pJob_SelectTable(pJob);
  
  delete pJob_SelectTable;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_SelectTable_show(Job* pJob)
{
  Job_SelectTable* pJob_SelectTable = _pJob_SelectTable(pJob);

  pJob_SelectTable->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
