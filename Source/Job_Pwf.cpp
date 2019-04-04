/*============================================================================*/
/* Job_Pwf.cpp -- Present Working Folder Job                                  */
/*============================================================================*/

// Job Short: Pwf

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_Pwf =
{
  CN_JOB_PWF,            // class enum
  JK_LOCAL,              // job kind
  "working folder"_Stz,  // comment about the job
  "pwf"_Stz,             // the prompt command
  nullptr,               // the second word
  nullptr,               // the _run task seed
  "pwf_req"_Stz,         // the _req task seed
  nullptr,               // the _rcv task seed
  nullptr,               // the _exe task seed
  nullptr,               // the _end task seed
  nullptr,               // the _can task seed
  false,                 // check resources?
  false,                 // detached job?
  false,                 // buffer attached?
  JT_NONE,               // job trace
  DL_NONE,               // ddl lock model
  PY_NORMAL,             // priority
  JF_NONE,               // flags

  nullptr,               // prompt check function
  pJob_Pwf_pre,          // prompt pre-process function
  vJob_Pwf_post,         // prompt post-process function
  nullptr,               // job new function
  vJob_Pwf_delete,       // job delete function
  nullptr,               // job pre-send function
  nullptr,               // job post-receive function
  vJob_Pwf_show,         // job show function
  
  // help and example
  true,
  "pwf"_Stz,
  "pwf;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_Pwf_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Receiver_class;
  
  return &xJobClass_Pwf;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Pwf* Jobs::_pJob_Pwf(void* aAddress)
{
  Job_Pwf* pJob_Pwf = reinterpret_cast<Job_Pwf*>(aAddress);
  
  RASSERT_TYPE(pJob_Pwf,CN_JOB_PWF);
  
  return pJob_Pwf;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Pwf::operator new(size_t)
{
  Job_Pwf* pJob_Pwf =
    static_cast<Job_Pwf*>(HeapMem::pAllocate(CN_JOB_PWF));
  
  return pJob_Pwf;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Pwf::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Pwf::operator delete(void* aAddress)
{
  Job_Pwf* pJob_Pwf = static_cast<Job_Pwf*>(aAddress);
  
  HeapMem::vFree(pJob_Pwf);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Pwf::Job_Pwf(Interface::Session* pSession)
: Job(CN_JOB_PWF, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PWF,_FOOTER_);
  
   _checkInitialised_(CN_JOB_PWF);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_PWF,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Pwf::~Job_Pwf(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PWF,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Pwf::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_Pwf* pJob_Pwf = _pJob_Pwf(pObject);

  pJob_Pwf->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_Pwf::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PWF,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("pwf"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - -
  // get no of tokens in token vector
  iNumTokens = pTokenVec->nTokens();;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be only 1 tokens
  if (iNumTokens != 1) {
    return nullptr;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Pwf::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PWF,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Pwf::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PWF);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Pwf_pre(Interface::Session* pSession)
{
  Job_Pwf* pJob_Pwf = new Job_Pwf(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommand->pTknVec();
  
  bool bResult = pJob_Pwf->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    pJob_Pwf->vAddError(EC_INVALID_COMMAND);
  }
  
  return pJob_Pwf;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Pwf_post(Interface::Session* pSession, Job* pJob)
{
  Job_Pwf* pJob_Pwf = _pJob_Pwf(pJob);
  
  if ( pJob_Pwf->m_bHasError ) {
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_Pwf->m_xNode[i_Node].m_eErrorCode]);
    }
  }
  else {
    if (gpAttachMap->m_bMetadata) {
      vTrace("metadata: %s\n"_Stz, pJob_Pwf->m_sMetadata.aStz());
    }
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("%s"_Stz, pJob_Pwf->m_xNode[i_Node].m_sResponse.aChar());
    }
    vTrace("success: %lld rows affected\n"_Stz, gpAttachMap->m_pGrid->m_nNodes);
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Pwf_delete(Interface::Session* pSession, Job* pJob)
{
  Job_Pwf* pJob_Pwf = _pJob_Pwf(pJob);
  
  delete pJob_Pwf;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Pwf_show(Job* pJob)
{
  Job_Pwf* pJob_Pwf = _pJob_Pwf(pJob);

  pJob_Pwf->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
