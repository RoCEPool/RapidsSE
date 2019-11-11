/*============================================================================*/
/* Job_DefineNode.cpp -- Define Node Job                                      */
/*============================================================================*/

#include "Jobs.h"

JobClass xJobClass_DefineNode =
{
  CN_JOB_DEFINENODE,      // class number
  JK_DIST2PC,             // job kind
  RS_NODE_ZERO,           // response kind
  "define node"_Stz,      // comment about the job
  nullptr,//"define"_Stz,           // the prompt command
  nullptr,//"node"_Stz,             // the second word
  nullptr,                // the _run task seed
  "defineNode_req"_Stz,   // the _req task seed
  "defineNode_beg"_Stz,   // the _rcv task seed
  "defineNode_exe"_Stz,   // the _exe task seed
  "defineNode_end"_Stz,   // the _end task seed
  "defineNode_can"_Stz,   // the _can task seed
  true,                   // check resources?
  false,                  // detached job?
  false,                  // buffer attached?
  DL_GRAB_WRITE,          // ddl lock model
  PY_NORMAL,              // priority
  JF_LOWMEM,              // flags

  eJob_checkCluster,      // prompt check function
  pJob_DefineNode_pre,    // prompt pre-process function
  vJob_post,              // prompt post-process function
  pJob_DefineNode_new,    // job new function
  vJob_DefineNode_delete, // job delete function
  vJob_send,              // job pre-send function
  vJob_recv,              // job post-receive function
  vJob_DefineNode_show,   // job show function
  
  // help
  true,
  "define node <nodeNm> { <nodeinfo> }"_Stz,
  
  ""_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_DefineNode_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Export_class;
  
  return &xJobClass_DefineNode;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_DefineNode* Jobs::_pJob_CreateNode(void* aAddress)
{
  Job_DefineNode* pJob_CreateNode = reinterpret_cast<Job_DefineNode*>(aAddress);
  
  RASSERT_TYPE(pJob_CreateNode,CN_JOB_DEFINENODE);
  
  return pJob_CreateNode;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_DefineNode::operator new(size_t)
{
  Job_DefineNode* pJob_CreateNode =
    static_cast<Job_DefineNode*>(HeapMem::pAllocate(CN_JOB_DEFINENODE));
  
  return pJob_CreateNode;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_DefineNode::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_DefineNode::operator delete(void* aAddress)
{
  Job_DefineNode* pJob_CreateNode = static_cast<Job_DefineNode*>(aAddress);
  
  HeapMem::vFree(pJob_CreateNode);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_DefineNode::Job_DefineNode(JobClass* aJobClass, ReqTuple* aReqTuple)
: Job(aJobClass, aReqTuple)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DEFINENODE,_FOOTER_);
  
  _checkInitialised_(CN_JOB_DEFINENODE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_DEFINENODE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_DefineNode::~Job_DefineNode(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DEFINENODE,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_DefineNode::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_DefineNode* pJob_CreateNode = _pJob_CreateNode(pObject);

  pJob_CreateNode->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate node                                                              */
/*----------------------------------------------------------------------------*/
eEC Job_DefineNode::ePopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DEFINENODE,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Dic* pRootDic = AM::pRootDic();

  Word* pWord_syn = pRootDic->pFindWord("defineNode_syn"_Stz);

  bool b = JobHelper::bExtractArgs(&m_oEnameSet,pWord_syn->pEnameSet(),pTokenVec);
  if (! b) {
    vTrace("Unable to match job syntax\n"_Stz);
    return EC_INVALID_COMMAND;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // assert we got a create table
  ASSERT(pTokenVec->pGet(0)->bSame("define"_Stz));
  ASSERT(pTokenVec->pGet(1)->bSame("node"_Stz));
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // some validations
  bool bValid = bValidateCreateNode(pTokenVec);
  if ( ! bValid ) {
    return EC_INVALID_COMMAND;
  }
  
  // set the node name
  m_oEnameSet.vAdd("<nodeNm>"_Stz,pTokenVec->pGet(2)->szText());

  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_DefineNode::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DEFINENODE,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* validate a create table request                                            */
/*----------------------------------------------------------------------------*/
bool Job_DefineNode::bValidateCreateNode(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DEFINENODE,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get tokens in token vector
  i8 iNumTokens = pTokenVec->nTokens();
  
  // there should be a minumum of 3 tokens
  if (iNumTokens < 3) {
    return false;
  }
  
  //Tokeniser::vToEnmSet(&m_oEnameSet,pTokenVec);
  
  //m_oEnameSet.vPrint();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_DefineNode::aFooter(void)
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
Job* Jobs::pJob_DefineNode_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession)
{
  Job_DefineNode* pJob_CreateNode = new Job_DefineNode(aJobClass,aReqTuple);
  //pJob->vDicPath(pSession);

  TokenVec* pTokenVec = pSession->m_pInput->pTknVec();

  eEC eError = pJob_CreateNode->ePopulateJob(pTokenVec);
  
  if ( eError != EC_NONE ) {
    pJob_CreateNode->vAddError(eError);
  }
  
  return pJob_CreateNode;
}

/*----------------------------------------------------------------------------*/
/* print success                                                              */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_DefineNode_post(Job* pJob)
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
Job* Jobs::pJob_DefineNode_new(JobClass* aJobClass, ReqTuple* aReqTuple)
{
  Job_DefineNode* pJob_CreateNode = new Job_DefineNode(aJobClass,nullptr);
  
  return pJob_CreateNode;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_DefineNode_delete(Job* pJob)
{
  Job_DefineNode* pJob_CreateNode = _pJob_CreateNode(pJob);
    
  delete pJob_CreateNode;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_DefineNode_show(Job* pJob)
{
  Job_DefineNode* pJob_CreateNode = _pJob_CreateNode(pJob);

  pJob_CreateNode->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
