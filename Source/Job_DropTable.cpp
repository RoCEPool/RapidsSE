/*============================================================================*/
/* Job_DropTable.cpp -- drop table Job                                        */
/*============================================================================*/

#include "Jobs.h"

JobDesc xJobClass_DropTable (
  CN_JOB_DROPTABLE,      // class number
  JK_DIST2PC,            // job kind
  RS_NODE_ZERO,          // response kind
  "drop table"_Stz,      // comment about the job
  "drop"_Stz,            // the prompt command
  "table"_Stz,           // the second word
  nullptr,               // the _run task seed
  "dropTable_req"_Stz,   // the _req task seed
  "dropTable_beg"_Stz,   // the _rcv task seed
  "dropTable_exe"_Stz,   // the _exe task seed
  "dropTable_end"_Stz,   // the _end task seed
  "dropTable_can"_Stz,   // the _can task seed
  true,                  // check resources?
  false,                 // detached job?
  false,                 // buffer attached?
  DL_GRAB_WRITE,         // ddl lock model
  PY_NORMAL,             // priority
  JF_NONE,               // flags

  eJob_checkCluster,     // prompt check function
  pJob_DropTable_pre,    // prompt pre-process function
  vJob_post,             // prompt post-process function
  nullptr,               // job new function
  vJob_DropTable_delete, // job delete function
  vJob_send,             // job pre-send function
  vJob_recv,             // job post-receive function
  vJob_DropTable_show,   // job show function
  
  // help and example
  true,
  "drop table <tableNm>"_Stz,
  "<example>"_Stz
);

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
//JobClass* Jobs::pJob_DropTable_class(pFUN** pNextFun)
//{
//  *pNextFun = (pFUN*)pJob_DropTenant_class;
//  
//  return &xJobClass_DropTable;
//}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_DropTable* Jobs::_pJob_DropTable(void* aAddress)
{
  Job_DropTable* pJob_DropTable = reinterpret_cast<Job_DropTable*>(aAddress);
  
  RASSERT_TYPE(pJob_DropTable,CN_JOB_DROPTABLE);
  
  return pJob_DropTable;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_DropTable::operator new(size_t)
{
  Job_DropTable* pJob_DropTable =
    static_cast<Job_DropTable*>(HeapMem::pAllocate(CN_JOB_DROPTABLE));
  
  return pJob_DropTable;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_DropTable::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_DropTable::operator delete(void* aAddress)
{
  Job_DropTable* pJob_DropTable = static_cast<Job_DropTable*>(aAddress);
  
  HeapMem::vFree(pJob_DropTable);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_DropTable::Job_DropTable(JobClass* aJobClass, ReqTuple* aReqTuple)
: Job(aJobClass, aReqTuple)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPTABLE,_FOOTER_);
    
  _checkInitialised_(CN_JOB_DROPTABLE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPTABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_DropTable::~Job_DropTable(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPTABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_DropTable::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_DropTable* pJob_DropTable = _pJob_DropTable(pObject);

  pJob_DropTable->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_DropTable::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPTABLE,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("drop"_Stz));
  ASSERT(pTokenVec->pGet(1)->bSame("table"_Stz));
  
  // get number of items in token vector
  iNumTokens = pTokenVec->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3)
    return false;
  
  // set the table name
  m_oEnameSet.vAdd("<tableNm>"_Stz, pTokenVec->pGet(2)->szText());
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_DropTable::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPTABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_DropTable::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPTABLE);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_DropTable_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession)
{
  Job_DropTable* pJob_DropTable = new Job_DropTable(aJobClass,aReqTuple);
  //pJob->vDicPath(pSession);

  TokenVec* pTokenVec = pSession->m_pInput->pTknVec();
  
  bool bResult = pJob_DropTable->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    pJob_DropTable->vAddError(EC_INVALID_COMMAND);
  }
  
  return pJob_DropTable;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_DropTable_delete(Job* pJob)
{
  Job_DropTable* pJob_DropTable = _pJob_DropTable(pJob);
  
  delete pJob_DropTable;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_DropTable_show(Job* pJob)
{
  Job_DropTable* pJob_DropTable = _pJob_DropTable(pJob);

  pJob_DropTable->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
