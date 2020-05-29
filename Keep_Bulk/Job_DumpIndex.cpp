/*============================================================================*/
/* Job_DumpIndex.cpp -- dump index Job                                        */
/*============================================================================*/

#include "Jobs.h"

JobClass xJobClass_DumpIndex =
{
  CN_JOB_DUMPINDEX,      // class number
  JK_LOCAL,              // job kind
  RS_LOCAL_ZERO,         // response kind
  "dump index"_Stz,      // comment about the job
  "dumpidx"_Stz,         // the prompt command
  nullptr,               // the second word
  nullptr,               // the _run task seed
  "dumpIndex_req"_Stz,   // the _req task seed
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
  pJob_DumpIndex_pre,    // prompt pre-process function
  vJob_post,             // prompt post-process function
  nullptr,               // job new function
  vJob_DumpIndex_delete, // job delete function
  nullptr,               // job pre-send function
  nullptr,               // job post-receive function
  vJob_DumpIndex_show,   // job show function
  
  // help and example
  true,
  "dumpidx <tableNm> <indexNm>"_Stz,
  "dumpidx supplier idx1;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_DumpIndex_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_DumpTable_class;
  
  return &xJobClass_DumpIndex;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_DumpIndex* Jobs::_pJob_DumpIndex(void* aAddress)
{
  Job_DumpIndex* pJob_DumpIndex = reinterpret_cast<Job_DumpIndex*>(aAddress);
  
  RASSERT_TYPE(pJob_DumpIndex,CN_JOB_DUMPINDEX);
  
  return pJob_DumpIndex;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_DumpIndex::operator new(size_t)
{
  Job_DumpIndex* pJob_DumpIndex =
    static_cast<Job_DumpIndex*>(HeapMem::pAllocate(CN_JOB_DUMPINDEX));
  
  return pJob_DumpIndex;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_DumpIndex::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_DumpIndex::operator delete(void* aAddress)
{
  Job_DumpIndex* pJob_DumpIndex = static_cast<Job_DumpIndex*>(aAddress);
  
  HeapMem::vFree(pJob_DumpIndex);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_DumpIndex::Job_DumpIndex(JobClass* aJobClass, ReqTuple* aReqTuple)
: Job(aJobClass, aReqTuple)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DUMPINDEX,_FOOTER_);
    
  _checkInitialised_(CN_JOB_DUMPINDEX);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_DUMPINDEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_DumpIndex::~Job_DumpIndex(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DUMPINDEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_DumpIndex::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_DumpIndex* pJob_DumpIndex = _pJob_DumpIndex(pObject);

  pJob_DumpIndex->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_DumpIndex::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DUMPINDEX,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("dumpidx"_Stz));
  
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
void Job_DumpIndex::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DUMPINDEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_DumpIndex::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DUMPINDEX);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_DumpIndex_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession)
{
  Job_DumpIndex* pJob_DumpIndex = new Job_DumpIndex(aJobClass,aReqTuple);
  //pJob->vDicPath(pSession);

  TokenVec* pTokenVec = pSession->m_pInput->pTknVec();
  
  bool bResult = pJob_DumpIndex->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    pJob_DumpIndex->vAddError(EC_INVALID_COMMAND);
  }
  
  return pJob_DumpIndex;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_DumpIndex_delete(Job* pJob)
{
  Job_DumpIndex* pJob_DumpIndex = _pJob_DumpIndex(pJob);
  
  delete pJob_DumpIndex;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_DumpIndex_show(Job* pJob)
{
  Job_DumpIndex* pJob_DumpIndex = _pJob_DumpIndex(pJob);

  pJob_DumpIndex->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
