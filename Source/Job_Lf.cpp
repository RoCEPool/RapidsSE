/*============================================================================*/
/* Job_Lf.cpp -- List Folder Job                                              */
/*============================================================================*/

// Job Short: Lf

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_Lf =
{
  "Job_Lf"_Stz,      // name of the job class
  CN_JOB_LF,         // class enum
  JK_LOCAL,          // job kind
  "list folder"_Stz, // comment about the job
  "lf"_Stz,          // the prompt command
  nullptr,           // the second word
  nullptr,           // the _run task seed
  "lf_req"_Stz,      // the _req task seed
  nullptr,           // the _exe task seed
  nullptr,           // the _rcv task seed
  nullptr,           // the _end task seed
  nullptr,           // the _can task seed
  false,             // check resources?
  false,             // detached job?
  false,             // buffer attached?
  JT_NONE,           // job trace
  DL_NONE,           // ddl lock model

  nullptr,           // prompt check function
  pJob_Lf_pre,       // prompt pre-process function
  vJob_Lf_post,      // prompt post-process function
  nullptr,           // job new function
  vJob_Lf_delete,    // job delete function
  nullptr,           // job pre-send function
  nullptr,           // job post-receive function
  vJob_Lf_show,      // job show function
  
  // help and example
  true,
  "lf"_Stz,
  "lf;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_Lf_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_ListCatalogs_class;
  
  return &xJobClass_Lf;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Lf* Jobs::_pJob_Lf(void* aAddress)
{
  Job_Lf* pJob_Lf = reinterpret_cast<Job_Lf*>(aAddress);
  
  RASSERT_TYPE(pJob_Lf,CN_JOB_LF);
  
  return pJob_Lf;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Lf::operator new(size_t)
{
  Job_Lf* pJob_Lf =
    static_cast<Job_Lf*>(HeapMem::pAllocate(CN_JOB_LF));
  
  return pJob_Lf;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Lf::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Lf::operator delete(void* aAddress)
{
  Job_Lf* pJob_Lf = static_cast<Job_Lf*>(aAddress);
  
  HeapMem::vFree(pJob_Lf);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Lf::Job_Lf(Interface::Session* pSession)
: Job(CN_JOB_LF, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LF,_FOOTER_);
  
  _checkInitialised_(CN_JOB_LF);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_LF,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Lf::~Job_Lf(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LF,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Lf::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_Lf* pJob_Lf = _pJob_Lf(pObject);

  pJob_Lf->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_Lf::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LF,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("lf"_Stz));
  
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
void Job_Lf::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LF,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Lf::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LF);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Lf_pre(Interface::Session* pSession)
{
  Job_Lf* pJob_Lf = new Job_Lf(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommand->pTknVec();
  
  bool bResult = pJob_Lf->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_Lf->m_bHasError = true;
    pJob_Lf->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_Lf;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Lf_post(Interface::Session* pSession, Job* pJob)
{
  Job_Lf* pJob_Lf = _pJob_Lf(pJob);
  
  if ( pJob_Lf->m_bHasError ) {
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_Lf->m_xNode[i_Node].m_eErrorCode]);
    }
  }
  else {
    if (gpAttachMap->m_bMetadata) {
      vTrace("metadata: %s\n"_Stz, pJob_Lf->m_sMetadata.aStz());
    }
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("%s"_Stz, pJob_Lf->m_xNode[i_Node].m_sResponse.aChar());
    }
    vTrace("success: %lld rows affected\n"_Stz, gpAttachMap->m_pGrid->m_nNodes);
  }
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Lf_delete(Interface::Session* pSession, Job* pJob)
{
  Job_Lf* pJob_Lf = _pJob_Lf(pJob);
  
  delete pJob_Lf;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Lf_show(Job* pJob)
{
  Job_Lf* pJob_Lf = _pJob_Lf(pJob);

  pJob_Lf->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
