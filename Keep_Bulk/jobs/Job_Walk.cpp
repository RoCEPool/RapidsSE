/*============================================================================*/
/* Job_Walk.cpp -- walk index                                                 */
/*============================================================================*/

#include "Jobs.h"

JobClass xJobClass_Walk =
{
  CN_JOB_WALK,               // class number
  JK_DIST2PC,                // job kind
  RS_NODE_ZERO,              // response kind
  "walk"_Stz,                // comment about the job
  "walk"_Stz,                // the prompt command
  nullptr,                   // the second word
  nullptr,                   // the _run task seed
  "walk_req"_Stz,            // the _req task seed
  "walk_beg"_Stz,            // the _rcv task seed
  "walk_exe"_Stz,            // the _exe task seed
  "walk_end"_Stz,            // the _end task seed
  "walk_can"_Stz,            // the _can task seed
  true,                      // check resources?
  false,                     // detached job?
  false,                     // buffer attached?
  DL_GRAB_READ,              // ddl lock model
  PY_NORMAL,                 // priority
  JF_NONE,                   // flags

  eJob_checkCluster,         // prompt check function
  pJob_Walk_pre,             // prompt pre-process function
  vJob_post,                 // prompt post-process function
  nullptr,                   // job new function
  vJob_Walk_delete,          // job delete function
  vJob_send,                 // job pre-send function
  vJob_recv,                 // job post-receive function
  vJob_Walk_show,            // job show function
  
  // help and example
  true,
  "walk <tableNm>"_Stz,
  "tr SUPPLIER;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_Walk_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_WriteBuffer_class;
  
  return &xJobClass_Walk;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Walk* Jobs::_pJob_Walk(void* aAddress)
{
  Job_Walk* pJob_Walk = reinterpret_cast<Job_Walk*>(aAddress);
  
  RASSERT_TYPE(pJob_Walk,CN_JOB_WALK);
  
  return pJob_Walk;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Walk::operator new(size_t)
{
  Job_Walk* pJob_Walk =
    static_cast<Job_Walk*>(HeapMem::pAllocate(CN_JOB_WALK));
  
  return pJob_Walk;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Walk::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Walk::operator delete(void* aAddress)
{
  Job_Walk* pJob_Walk = static_cast<Job_Walk*>(aAddress);
  
  HeapMem::vFree(pJob_Walk);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Walk::Job_Walk(JobClass* aJobClass, ReqTuple* aReqTuple)
: Job(aJobClass, aReqTuple)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_WALK,_FOOTER_);
  
  m_pTable = nullptr;
  m_pIndex = nullptr;

  //m_sIndexName = ""_Stz;
  
  _checkInitialised_(CN_JOB_WALK);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_WALK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Walk::~Job_Walk(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_WALK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Walk::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_Walk* pJob_Walk = _pJob_Walk(pObject);

  pJob_Walk->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* begin walk job                                                             */
/*----------------------------------------------------------------------------*/
bool Job_Walk::bBegin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_WALK,_FOOTER_);
  
  //ActLog::vAdd(this,AN_JOB_BEGIN);
  
  m_pTable->vMarkTable();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* end walk job                                                               */
/*----------------------------------------------------------------------------*/
void Job_Walk::vEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_WALK,_FOOTER_);
  
  //ActLog::vAdd(this,AN_JOB_END);
}

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_Walk::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_WALK,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("walk"_Stz));
  
  // get number of items in token vector
  iNumTokens = pTokenVec->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 2 tokens
  if (iNumTokens < 2)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the table name
  m_oEnameSet.vAdd("<tableNm>"_Stz, pTokenVec->pGet(1)->szText());
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the index name
  if (iNumTokens == 3) {
    m_oEnameSet.vAdd("<indexNm>"_Stz, pTokenVec->pGet(2)->szText());
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Walk::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_WALK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Walk::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_WALK);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Walk_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession)
{
  Job_Walk* pJob_Walk = new Job_Walk(aJobClass,aReqTuple);
  //pJob->vDicPath(pSession);

  TokenVec* pTokenVec = pSession->m_pInput->pTknVec();
  
  bool bResult = pJob_Walk->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    pJob_Walk->vAddError(EC_INVALID_COMMAND);
  }
  
  return pJob_Walk;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Walk_delete(Job* pJob)
{
  Job_Walk* pJob_Walk = _pJob_Walk(pJob);
  
  delete pJob_Walk;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Walk_show(Job* pJob)
{
  Job_Walk* pJob_Walk = _pJob_Walk(pJob);

  pJob_Walk->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
