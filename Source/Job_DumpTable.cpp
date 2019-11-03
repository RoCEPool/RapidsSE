/*============================================================================*/
/* Job_DumpTable.cpp -- dump index Job                                        */
/*============================================================================*/

#include "Jobs.h"

JobClass xJobClass_DumpTable =
{
  CN_JOB_DUMPTABLE,      // class number
  JK_LOCAL,              // job kind
  RS_LOCAL_ZERO,         // response kind
  "dump table"_Stz,      // comment about the job
  "dumptbl"_Stz,         // the prompt command
  nullptr,               // the second word
  nullptr,               // the _run task seed
  "dumpTable_req"_Stz,   // the _req task seed
  nullptr,               // the _rcv task seed
  nullptr,               // the _exe task seed
  nullptr,               // the _end task seed
  nullptr,               // the _can task seed
  true,                  // check resources?
  false,                 // detached job?
  false,                 // buffer attached?
  DL_GRAB_READ,          // ddl lock model
  PY_NORMAL,             // priority
  JF_NONE,               // flags

  eJob_checkCluster,     // prompt check function
  pJob_DumpTable_pre,    // prompt pre-process function
  vJob_post,             // prompt post-process function
  nullptr,               // job new function
  vJob_DumpTable_delete, // job delete function
  nullptr,               // job pre-send function
  nullptr,               // job post-receive function
  vJob_DumpTable_show,   // job show function
  
  // help and example
  true,
  "dumptbl <tableNm>"_Stz,
  "dumptbl supplier;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_DumpTable_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Export_class;

  return &xJobClass_DumpTable;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_DumpTable* Jobs::_pJob_DumpTable(void* aAddress)
{
  Job_DumpTable* pJob_DumpTable = reinterpret_cast<Job_DumpTable*>(aAddress);
  
  RASSERT_TYPE(pJob_DumpTable,CN_JOB_DUMPTABLE);
  
  return pJob_DumpTable;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_DumpTable::operator new(size_t)
{
  Job_DumpTable* pJob_DumpTable =
    static_cast<Job_DumpTable*>(HeapMem::pAllocate(CN_JOB_DUMPTABLE));
  
  return pJob_DumpTable;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_DumpTable::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_DumpTable::operator delete(void* aAddress)
{
  Job_DumpTable* pJob_DumpTable = static_cast<Job_DumpTable*>(aAddress);
  
  HeapMem::vFree(pJob_DumpTable);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_DumpTable::Job_DumpTable(JobClass* aJobClass, ReqTuple* aReqTuple)
: Job(aJobClass, aReqTuple)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DUMPTABLE,_FOOTER_);
    
  _checkInitialised_(CN_JOB_DUMPTABLE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_DUMPTABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_DumpTable::~Job_DumpTable(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DUMPTABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_DumpTable::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_DumpTable* pJob_DumpTable = _pJob_DumpTable(pObject);

  pJob_DumpTable->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_DumpTable::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DUMPTABLE,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("dumptbl"_Stz));
  
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
void Job_DumpTable::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DUMPTABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_DumpTable::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DUMPTABLE);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_DumpTable_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession)
{
  Job_DumpTable* pJob_DumpTable = new Job_DumpTable(aJobClass,aReqTuple);
  //pJob->vDicPath(pSession);

  TokenVec* pTokenVec = pSession->m_pInput->pTknVec();
  
  bool bResult = pJob_DumpTable->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    pJob_DumpTable->vAddError(EC_INVALID_COMMAND);
  }
  
  return pJob_DumpTable;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_DumpTable_delete(Job* pJob)
{
  Job_DumpTable* pJob_DumpTable = _pJob_DumpTable(pJob);
  
  delete pJob_DumpTable;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_DumpTable_show(Job* pJob)
{
  Job_DumpTable* pJob_DumpTable = _pJob_DumpTable(pJob);

  pJob_DumpTable->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
