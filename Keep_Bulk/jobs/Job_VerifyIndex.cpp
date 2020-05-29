/*============================================================================*/
/* Job_VerifyIndex.cpp -- verify index Job                                    */
/*============================================================================*/

#include "Jobs.h"

JobClass xJobClass_VerifyIndex =
{
  CN_JOB_VERIFYINDEX,         // class number
  JK_DIST2PC,                 // job kind
  RS_NODE_ROWS,               // response kind
  "verify index"_Stz,         // comment about the job
  "vfyidx"_Stz,               // the prompt command
  nullptr,                    // the second word
  nullptr,                    // the _run task seed
  "verifyIndex_req"_Stz,      // the _req task seed
  "verifyIndex_beg"_Stz,      // the _rcv task seed
  "verifyIndex_exe"_Stz,      // the _exe task seed
  "verifyIndex_end"_Stz,      // the _end task seed
  "verifyIndex_can"_Stz,      // the _can task seed
  true,                       // check resources?
  false,                      // detached job?
  false,                      // buffer attached?
  DL_GRAB_READ,               // ddl lock model
  PY_NORMAL,                  // priority
  JF_NONE,                    // flags

  eJob_checkCluster,          // prompt check function
  pJob_VerifyIndex_pre,       // prompt pre-process function
  vJob_post,                  // prompt post-process function
  nullptr,                    // job new function
  vJob_VerifyIndex_delete,    // job delete function
  vJob_send,                  // job pre-send function
  vJob_recv,                  // job post-receive function
  vJob_VerifyIndex_show,      // job show function
  
  // help and example
  true,
  "vfyidx <tableNm> <indexNm>"_Stz,
  "vfyidx SUPPPLIER idx1;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_VerifyIndex_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_VerifyTable_class;
  
  return &xJobClass_VerifyIndex;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_VerifyIndex* Jobs::_pJob_VerifyIndex(void* aAddress)
{
  Job_VerifyIndex* pJob_VerifyIndex = reinterpret_cast<Job_VerifyIndex*>(aAddress);
  
  RASSERT_TYPE(pJob_VerifyIndex,CN_JOB_VERIFYINDEX);
  
  return pJob_VerifyIndex;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_VerifyIndex::operator new(size_t)
{
  Job_VerifyIndex* pJob_VerifyIndex =
    static_cast<Job_VerifyIndex*>(HeapMem::pAllocate(CN_JOB_VERIFYINDEX));
  
  return pJob_VerifyIndex;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_VerifyIndex::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_VerifyIndex::operator delete(void* aAddress)
{
  Job_VerifyIndex* pJob_VerifyIndex = static_cast<Job_VerifyIndex*>(aAddress);
  
  HeapMem::vFree(pJob_VerifyIndex);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_VerifyIndex::Job_VerifyIndex(JobClass* aJobClass, ReqTuple* aReqTuple)
: Job(aJobClass, aReqTuple)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_VERIFYINDEX,_FOOTER_);
    
  _checkInitialised_(CN_JOB_VERIFYINDEX);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_VERIFYINDEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_VerifyIndex::~Job_VerifyIndex(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_VERIFYINDEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_VerifyIndex::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_VerifyIndex* pJob_VerifyIndex = _pJob_VerifyIndex(pObject);

  pJob_VerifyIndex->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_VerifyIndex::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_VERIFYINDEX,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("vfyidx"_Stz));
  
  // get number of items in token vector
  iNumTokens = pTokenVec->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the index name
  m_oEnameSet.vAdd("<indexNm>"_Stz, pTokenVec->pGet(2)->szText());
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the table name
  m_oEnameSet.vAdd("<tableNm>"_Stz, pTokenVec->pGet(1)->szText());
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_VerifyIndex::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_VERIFYINDEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_VerifyIndex::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_VERIFYINDEX);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_VerifyIndex_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession)
{
  Job_VerifyIndex* pJob_VerifyIndex = new Job_VerifyIndex(aJobClass,aReqTuple);
  //pJob->vDicPath(pSession);

  TokenVec* pTokenVec = pSession->m_pInput->pTknVec();
  
  bool bResult = pJob_VerifyIndex->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    pJob_VerifyIndex->vAddError(EC_INVALID_COMMAND);
  }
  
  return pJob_VerifyIndex;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_VerifyIndex_delete(Job* pJob)
{
  Job_VerifyIndex* pJob_VerifyIndex = _pJob_VerifyIndex(pJob);
  
  delete pJob_VerifyIndex;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_VerifyIndex_show(Job* pJob)
{
  Job_VerifyIndex* pJob_VerifyIndex = _pJob_VerifyIndex(pJob);

  pJob_VerifyIndex->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
