/*============================================================================*/
/* Job_VerifyTable.cpp -- verify index Job                                    */
/*============================================================================*/

#include "Jobs.h"

JobClass xJobClass_VerifyTable =
{
  CN_JOB_VERIFYTABLE,         // class number
  JK_DIST2PC,                 // job kind
  RS_NODE_ROWS,               // response kind
  "verify table"_Stz,         // comment about the job
  "vfytbl"_Stz,               // the prompt command
  nullptr,                    // the second word
  nullptr,                    // the _run task seed
  "verifyTable_req"_Stz,      // the _req task seed
  "verifyTable_beg"_Stz,      // the _rcv task seed
  "verifyTable_exe"_Stz,      // the _exe task seed
  "verifyTable_end"_Stz,      // the _end task seed
  "verifyTable_can"_Stz,      // the _can task seed
  true,                       // check resources?
  false,                      // detached job?
  false,                      // buffer attached?
  DL_GRAB_READ,               // ddl lock model
  PY_NORMAL,                  // priority
  JF_NONE,                    // flags

  eJob_checkCluster,          // prompt check function
  pJob_VerifyTable_pre,       // prompt pre-process function
  vJob_post,                  // prompt post-process function
  nullptr,                    // job new function
  vJob_VerifyTable_delete,    // job delete function
  vJob_send,                  // job pre-send function
  vJob_recv,                  // job post-receive function
  vJob_VerifyTable_show,      // job show function
  
  // help and example
  true,
  "vfytbl <tableNm>"_Stz,
  "vfytbl SUPPPLIER;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_VerifyTable_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_WaitCluster_class;
  
  return &xJobClass_VerifyTable;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_VerifyTable* Jobs::_pJob_VerifyTable(void* aAddress)
{
  Job_VerifyTable* pJob_VerifyTable = reinterpret_cast<Job_VerifyTable*>(aAddress);
  
  RASSERT_TYPE(pJob_VerifyTable,CN_JOB_VERIFYTABLE);
  
  return pJob_VerifyTable;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_VerifyTable::operator new(size_t)
{
  Job_VerifyTable* pJob_VerifyTable =
    static_cast<Job_VerifyTable*>(HeapMem::pAllocate(CN_JOB_VERIFYTABLE));
  
  return pJob_VerifyTable;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_VerifyTable::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_VerifyTable::operator delete(void* aAddress)
{
  Job_VerifyTable* pJob_VerifyTable = static_cast<Job_VerifyTable*>(aAddress);
  
  HeapMem::vFree(pJob_VerifyTable);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_VerifyTable::Job_VerifyTable(JobClass* aJobClass, ReqTuple* aReqTuple)
: Job(aJobClass, aReqTuple)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_VERIFYTABLE,_FOOTER_);
    
  _checkInitialised_(CN_JOB_VERIFYTABLE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_VERIFYTABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_VerifyTable::~Job_VerifyTable(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_VERIFYTABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_VerifyTable::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_VerifyTable* pJob_VerifyTable = _pJob_VerifyTable(pObject);

  pJob_VerifyTable->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_VerifyTable::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_VERIFYTABLE,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("vfytbl"_Stz));
  
  // get number of items in token vector
  iNumTokens = pTokenVec->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 2 tokens
  if (iNumTokens != 2)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the table name
  m_oEnameSet.vAdd("<tableNm>"_Stz, pTokenVec->pGet(1)->szText());
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_VerifyTable::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_VERIFYTABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_VerifyTable::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_VERIFYTABLE);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_VerifyTable_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession)
{
  Job_VerifyTable* pJob_VerifyTable = new Job_VerifyTable(aJobClass,aReqTuple);
  //pJob->vDicPath(pSession);

  TokenVec* pTokenVec = pSession->m_pInput->pTknVec();
  
  bool bResult = pJob_VerifyTable->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    pJob_VerifyTable->vAddError(EC_INVALID_COMMAND);
  }
  
  return pJob_VerifyTable;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_VerifyTable_delete(Job* pJob)
{
  Job_VerifyTable* pJob_VerifyTable = _pJob_VerifyTable(pJob);
  
  delete pJob_VerifyTable;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_VerifyTable_show(Job* pJob)
{
  Job_VerifyTable* pJob_VerifyTable = _pJob_VerifyTable(pJob);

  pJob_VerifyTable->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
