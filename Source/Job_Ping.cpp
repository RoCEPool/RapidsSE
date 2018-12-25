/*============================================================================*/
/* Job_Ping.cpp -- Ping Job                                                   */
/*============================================================================*/

// Job Short: Lf

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_Ping =
{
  "Job_Ping"_Stz,    // name of the job class
  CN_JOB_PING,       // class enum
  JK_LOCAL,          // job kind
  "ping"_Stz,        // comment about the job
  "ping"_Stz,        // the prompt command
  nullptr,           // the second word
  nullptr,           // the _run task seed
  "ping_req"_Stz,    // the _req task seed
  nullptr,           // the _exe task seed
  nullptr,           // the _rcv task seed
  nullptr,           // the _end task seed
  nullptr,           // the _can task seed
  false,             // check resources?
  false,             // detached job?
  false,             // buffer attached?
  JT_NONE,           // job trace
  pJob_Ping_pre,     // prompt pre-process function
  vJob_Ping_post,    // prompt post-process function
  nullptr,           // job new function
  vJob_Ping_delete,  // job delete function
  nullptr,           // job pre-send function
  nullptr,           // job post-receive function
  vJob_Ping_show,    // job show function
  
  // help and example
  true,
  "ping"_Stz,
  "ping;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* JobModel::pJob_Ping_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Publish_class;
  
  return &xJobClass_Ping;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Ping* JobModel::_pJob_Ping(void* aAddress)
{
  Job_Ping* pJob_Ping = reinterpret_cast<Job_Ping*>(aAddress);
  
  RASSERT_TYPE(pJob_Ping,CN_JOB_PING);
  
  return pJob_Ping;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Ping::operator new(size_t)
{
  Job_Ping* pJob_Ping =
    static_cast<Job_Ping*>(HeapMem::pAllocate(CN_JOB_PING));
  
  return pJob_Ping;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Ping::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Ping::operator delete(void* aAddress)
{
  Job_Ping* pJob_Lf = static_cast<Job_Ping*>(aAddress);
  
  HeapMem::vFree(pJob_Lf);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Ping::Job_Ping(Interface::Session* pSession)
: Job(CN_JOB_PING, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PING,_FOOTER_);
  
  _checkInitialised_(CN_JOB_PING);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_PING,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Ping::~Job_Ping(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PING,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Ping::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_Ping* pJob_Ping = _pJob_Ping(pObject);

  pJob_Ping->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_Ping::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PING,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("ping"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - -
  // get no of tokens in token vector
  iNumTokens = pTokenVec->nTokens();;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 1 tokens
  if (iNumTokens != 1) {
    return nullptr;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Ping::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PING,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Ping::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PING);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobModel::pJob_Ping_pre(Interface::Session* pSession)
{
  Job_Ping* pJob_Ping = new Job_Ping(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommandProcessor->pTknVec();
  
  bool bResult = pJob_Ping->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_Ping->m_bHasError = true;
    pJob_Ping->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_Ping;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_Ping_post(Interface::Session* pSession, Job* pJob)
{
  Job_Ping* pJob_Ping = _pJob_Ping(pJob);
  
  if ( pJob_Ping->m_bHasError ) {
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_Ping->m_xNode[i_Node].m_eErrorCode]);
    }
  }
  else {
    if (gpAttachMap->m_bMetadata) {
      vTrace("metadata: %s\n"_Stz, pJob_Ping->m_sMetadata.aStr());
    }
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("%s"_Stz, pJob_Ping->m_xNode[i_Node].m_sResponse.aChar());
    }
    vTrace("success: %lld rows affected\n"_Stz, gpAttachMap->m_pGrid->m_nNodes);
  }
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_Ping_delete(Interface::Session* pSession, Job* pJob)
{
  Job_Ping* pJob_Ping = _pJob_Ping(pJob);
  
  delete pJob_Ping;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_Ping_show(Job* pJob)
{
  Job_Ping* pJob_Ping= _pJob_Ping(pJob);

  pJob_Ping->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
