/*============================================================================*/
/* Job_Universal.cpp -- Define Node Job                                      */
/*============================================================================*/

#include "Jobs.h"

JobClass xJobClass_Universal =
{
  CN_JOB_DEFINENODE,      // class number
  JK_DIST2PC,             // job kind
  RS_NODE_ZERO,           // response kind
  "define node"_Stz,      // comment about the job
  "forget"_Stz,           // the prompt command
  "node"_Stz,             // the second word
  nullptr,                // the _run task seed
  "forgetNode_req"_Stz,   // the _req task seed
  "forgetNode_beg"_Stz,   // the _rcv task seed
  "forgetNode_exe"_Stz,   // the _exe task seed
  "forgetNode_end"_Stz,   // the _end task seed
  "forgetNode_can"_Stz,   // the _can task seed
  true,                   // check resources?
  false,                  // detached job?
  false,                  // buffer attached?
  DL_GRAB_WRITE,          // ddl lock model
  PY_NORMAL,              // priority
  JF_LOWMEM,              // flags

  eJob_checkCluster,      // prompt check function
  pJob_Universal_pre,    // prompt pre-process function
  vJob_post,              // prompt post-process function
  pJob_Universal_new,    // job new function
  vJob_Universal_delete, // job delete function
  vJob_send,              // job pre-send function
  vJob_recv,              // job post-receive function
  vJob_Universal_show,   // job show function
  
  // help
  true,
  "create node <nodeNm> ( <nodeinfo> )"_Stz,
  
  ""_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_Universal_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_UseSchema_class;
  
  return &xJobClass_Universal;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Universal* Jobs::_pJob_Universal(void* aAddress)
{
  Job_Universal* pJob_Universal = reinterpret_cast<Job_Universal*>(aAddress);
  
  RASSERT_TYPE(pJob_Universal,CN_JOB_DEFINENODE);
  
  return pJob_Universal;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Universal::operator new(size_t)
{
  Job_Universal* pJob_Universal =
    static_cast<Job_Universal*>(HeapMem::pAllocate(CN_JOB_DEFINENODE));
  
  return pJob_Universal;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Universal::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Universal::operator delete(void* aAddress)
{
  Job_Universal* pJob_Universal = static_cast<Job_Universal*>(aAddress);
  
  HeapMem::vFree(pJob_Universal);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Universal::Job_Universal(JobClass* aJobClass, ReqTuple* aReqTuple, i8 iForId, Session* pSession)
: Job(aJobClass, aReqTuple, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DEFINENODE,_FOOTER_);
  
  _checkInitialised_(CN_JOB_DEFINENODE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_DEFINENODE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Universal::~Job_Universal(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DEFINENODE,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Universal::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_Universal* pJob_Universal = _pJob_Universal(pObject);

  pJob_Universal->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate node                                                              */
/*----------------------------------------------------------------------------*/
eEC Job_Universal::ePopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DEFINENODE,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // assert we got a create table
  ASSERT(pTokenVec->pGet(0)->bSame("forget"_Stz));
  ASSERT(pTokenVec->pGet(1)->bSame("node"_Stz));
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // some validations
  bool bValid = bValidateUniversal(pTokenVec);
  if ( ! bValid ) {
    return EC_INVALID_COMMAND;
  }
  
  // set the node name
  vNodeName(pTokenVec->pGet(2)->szText());
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Universal::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DEFINENODE,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* validate a create table request                                            */
/*----------------------------------------------------------------------------*/
bool Job_Universal::bValidateUniversal(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DEFINENODE,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get tokens in token vector
  i8 iNumTokens = pTokenVec->nTokens();
  
  // there should be a minumum of 3 tokens
  if (iNumTokens < 3) {
    return false;
  }
  
  Tokeniser::vToEnmSet(&m_oEnmSet,pTokenVec);
  
  m_oEnmSet.vPrint();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Universal::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DEFINENODE);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Universal_pre(JobClass* aJobClass, ReqTuple* aReqTuple, i8 iForId, Session* pSession)
{
  Job_Universal* pJob_Universal = new Job_Universal(aJobClass,aReqTuple,iForId,pSession);
  //pJob->vDicPath(pSession);

  TokenVec* pTokenVec = pSession->m_pInput->pTknVec();

  eEC eError = pJob_Universal->ePopulateJob(pTokenVec);
  
  if ( eError != EC_NONE ) {
    pJob_Universal->vAddError(eError);
  }
  
  return pJob_Universal;
}

/*----------------------------------------------------------------------------*/
/* print success                                                              */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Universal_post(Job* pJob)
{
  vTrace("I logNote here\n"_Stz);
  
  i8 iResponses = pJob->m_pResponseVec->nItems();
  
  if (pJob->m_bHasError) {
    for (i8 i_Node = 0; i_Node < iResponses; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob->m_pResponseVec->pGet(i_Node)->m_eErrorCode]);
    }
    return;
  }
  
  RASSERT(iResponses == AM::nDefinedNodes());
  
  vTrace("success: 0 rows affected\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Universal_new(JobClass* aJobClass)
{
  Job_Universal* pJob_Universal =
    new Job_Universal(aJobClass,nullptr,C_UNKNOWN,nullptr);
  
  return pJob_Universal;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Universal_delete(Job* pJob)
{
  Job_Universal* pJob_Universal = _pJob_Universal(pJob);
    
  delete pJob_Universal;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Universal_show(Job* pJob)
{
  Job_Universal* pJob_Universal = _pJob_Universal(pJob);

  pJob_Universal->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
