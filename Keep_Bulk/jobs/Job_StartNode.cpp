/*============================================================================*/
/* Job_StartNode.cpp -- Start Node Job                                        */
/*============================================================================*/

#include "Jobs.h"

JobClass xJobClass_StartNode =
{
  CN_JOB_STARTNODE,       // class number
  JK_LOCAL,               // job kind
  RS_NONE,                // response kind
  "start node"_Stz,       // comment about the job
  "start"_Stz,            // the prompt command
  "node"_Stz,             // the second word
  nullptr,                // the _run task seed
  "StartNode_req"_Stz,    // the _req task seed
  nullptr,                // the _rcv task seed
  nullptr,                // the _exe task seed
  nullptr,                // the _end task seed
  nullptr,                // the _can task seed
  true,                   // check resources?
  false,                  // detached job?
  false,                  // buffer attached?
  DL_NONE,                // ddl lock model
  PY_NORMAL,              // priority
  JF_NONE,                // flags

  eJob_checkCluster,      // prompt check function
  pJob_StartNode_pre,     // prompt pre-process function
  vJob_post,              // prompt post-process function
  nullptr,                // job new function
  vJob_StartNode_delete,  // job delete function
  nullptr,                // job pre-send function
  nullptr,                // job post-receive function
  vJob_StartNode_show,    // job show function
  
  // help
  true,
  "start node <nodeNm>"_Stz,
  
  ""_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_StartNode_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Status_class;
  
  return &xJobClass_StartNode;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_StartNode* Jobs::_pJob_StartNode(void* aAddress)
{
  Job_StartNode* pJob_StartNode = reinterpret_cast<Job_StartNode*>(aAddress);
  
  RASSERT_TYPE(pJob_StartNode,CN_JOB_STARTNODE);
  
  return pJob_StartNode;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_StartNode::operator new(size_t)
{
  Job_StartNode* pJob_StartNode =
    static_cast<Job_StartNode*>(HeapMem::pAllocate(CN_JOB_STARTNODE));
  
  return pJob_StartNode;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_StartNode::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_StartNode::operator delete(void* aAddress)
{
  Job_StartNode* pJob_StartNode = static_cast<Job_StartNode*>(aAddress);
  
  HeapMem::vFree(pJob_StartNode);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_StartNode::Job_StartNode(JobClass* aJobClass, ReqTuple* aReqTuple)
: Job(aJobClass, aReqTuple)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_STARTNODE,_FOOTER_);
  
  _checkInitialised_(CN_JOB_STARTNODE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_STARTNODE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_StartNode::~Job_StartNode(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_STARTNODE,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_StartNode::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_StartNode* pJob_StartNode = _pJob_StartNode(pObject);

  pJob_StartNode->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate table                                                             */
/*----------------------------------------------------------------------------*/
eEC Job_StartNode::ePopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_STARTNODE,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // assert we got a create table
  ASSERT(pTokenVec->pGet(0)->bSame("start"_Stz));
  ASSERT(pTokenVec->pGet(1)->bSame("node"_Stz));
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // some validations
  bool bValid = bValidateStartNode(pTokenVec);
  if ( ! bValid ) {
    return EC_INVALID_COMMAND;
  }
  
  // set the node name
  //vNodeName(pTokenVec->pGet(2)->szText());
  m_oEnameSet.vAdd("<nodeNm>"_Stz, pTokenVec->pGet(2)->szText());
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_StartNode::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_STARTNODE,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* validate a create table request                                            */
/*----------------------------------------------------------------------------*/
bool Job_StartNode::bValidateStartNode(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_STARTNODE,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get tokens in token vector
  i8 iNumTokens = pTokenVec->nTokens();
  
  // there should be a minumum of 3 tokens
  if (iNumTokens < 3) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_StartNode::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_STARTNODE);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_StartNode_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession)
{
  Job_StartNode* pJob_StartNode = new Job_StartNode(aJobClass,aReqTuple);
  //pJob->vDicPath(pSession);

  TokenVec* pTokenVec = pSession->m_pInput->pTknVec();

  eEC eError = pJob_StartNode->ePopulateJob(pTokenVec);
  
  if ( eError != EC_NONE ) {
    pJob_StartNode->vAddError(eError);
  }
  
  return pJob_StartNode;
}

/*----------------------------------------------------------------------------*/
/* print success                                                              */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_StartNode_post(Job* pJob)
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
Job* Jobs::pJob_StartNode_new(JobClass* aJobClass, ReqTuple* aReqTuple)
{
  Job_StartNode* pJob_StartNode = new Job_StartNode(aJobClass,nullptr);
  
  return pJob_StartNode;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_StartNode_delete(Job* pJob)
{
  Job_StartNode* pJob_StartNode = _pJob_StartNode(pJob);
    
  delete pJob_StartNode;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_StartNode_show(Job* pJob)
{
  Job_StartNode* pJob_StartNode = _pJob_StartNode(pJob);

  pJob_StartNode->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
