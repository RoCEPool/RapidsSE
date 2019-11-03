/*============================================================================*/
/* Job_DropCatalog.cpp -- Drop Catalog Job                                    */
/*============================================================================*/

// Job Short: DropCatalog

#include "Jobs.h"
JobClass xJobClass_DropCatalog =
{
  CN_JOB_DROPCATALOG,      // class number
  JK_DIST2PC,              // job kind
  RS_NODE_ZERO,            // response kind
  "drop catalog"_Stz,      // comment about the job
  "drop"_Stz,              // the prompt command
  "catalog"_Stz,           // the second word
  nullptr,                 // the _run task seed
  "dropCatalog_req"_Stz,   // the _req task seed
  "dropCatalog_beg"_Stz,   // the _rcv task seed
  "dropCatalog_exe"_Stz,   // the _exe task seed
  "dropCatalog_end"_Stz,   // the _end task seed
  "dropCatalog_can"_Stz,   // the _can task seed
  true,                    // check resources?
  false,                   // detached job?
  false,                   // buffer attached?
  DL_GRAB_WRITE,           // ddl lock model
  PY_NORMAL,               // priority
  JF_NONE,                 // flags

  eJob_checkCluster,       // prompt check function
  pJob_DropCatalog_pre,    // prompt pre-process function
  vJob_post,               // prompt post-process function
  nullptr,                 // job new function
  vJob_DropCatalog_delete, // job delete function
  vJob_send,               // job pre-send function
  vJob_recv,               // job post-receive function
  vJob_DropCatalog_show,   // job show function
  
  // help and example
  true,
  "drop catalog <catalogNm>"_Stz,
  "drop catalog tpchCatalog;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_DropCatalog_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_DropIndex_class;
  
  return &xJobClass_DropCatalog;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_DropCatalog* Jobs::_pJob_DropCatalog(void* aAddress)
{
  Job_DropCatalog* pJob_DropCatalog = reinterpret_cast<Job_DropCatalog*>(aAddress);
  
  RASSERT_TYPE(pJob_DropCatalog,CN_JOB_DROPCATALOG);
  
  return pJob_DropCatalog;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_DropCatalog::operator new(size_t)
{
  Job_DropCatalog* pJob_DropSchema =
    static_cast<Job_DropCatalog*>(HeapMem::pAllocate(CN_JOB_DROPCATALOG));
  
  return pJob_DropSchema;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_DropCatalog::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_DropCatalog::operator delete(void* aAddress)
{
  Job_DropCatalog* pJob_DropCatalog = static_cast<Job_DropCatalog*>(aAddress);
  
  HeapMem::vFree(pJob_DropCatalog);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_DropCatalog::Job_DropCatalog(JobClass* aJobClass, ReqTuple* aReqTuple)
: Job(aJobClass, aReqTuple)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPCATALOG,_FOOTER_);
    
  _checkInitialised_(CN_JOB_DROPCATALOG);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPCATALOG,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_DropCatalog::~Job_DropCatalog(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPCATALOG,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_DropCatalog::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_DropCatalog* pJob_DropCatalog = _pJob_DropCatalog(pObject);

  pJob_DropCatalog->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_DropCatalog::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPCATALOG,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("drop"_Stz));
  ASSERT(pTokenVec->pGet(1)->bSame("catalog"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - -
  // get number of items in token
  iNumTokens = pTokenVec->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3)
  return false;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the table name
  m_oEnameSet.vAdd("<catalogNm>"_Stz, pTokenVec->pGet(2)->szText());
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_DropCatalog::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPCATALOG,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_DropCatalog::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPCATALOG);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_DropCatalog_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession)
{
  Job_DropCatalog* pJob_DropCatalog = new Job_DropCatalog(aJobClass,aReqTuple);
  //pJob->vDicPath(pSession);

  TokenVec* pTokenVec = pSession->m_pInput->pTknVec();
  
  bool bResult = pJob_DropCatalog->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    pJob_DropCatalog->vAddError(EC_INVALID_COMMAND);
  }
  
  return pJob_DropCatalog;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_DropCatalog_delete(Job* pJob)
{
  Job_DropCatalog* pJob_DropCatalog = _pJob_DropCatalog(pJob);
  
  delete pJob_DropCatalog;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_DropCatalog_show(Job* pJob)
{
  Job_DropCatalog* pJob_DropCatalog = _pJob_DropCatalog(pJob);

  pJob_DropCatalog->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
