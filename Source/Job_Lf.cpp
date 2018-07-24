/*============================================================================*/
/* Job_Lf.cpp -- List Folder Job                                              */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobNote mJobNote_Lf (
  "Job_Lf"_Stz,      // name of the job class
  CN_JOB_LF,         // class enum
  "list folder"_Stz, // comment about the job
  "lf"_Stz,          // the prompt command
  nullptr,           // the second word
  "lf"_Stz,          // the task seed for this job
  false,             // detached job?
  pJob_Lf_pre,       // prompt pre-process function
  vJob_Lf_post,      // prompt post-process function
  true               // job is enabled
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Lf* JobsModel::_pJob_Lf(void* aAddress)
{
  Job_Lf* pJob_Lf = reinterpret_cast<Job_Lf*>(aAddress);
  
  //RASSERT( !pJob_Lf || CN_JOB_LF == pJob_Lf->eClass());
  
  return pJob_Lf;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Lf::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_Lf* pJob_Lf =
    static_cast<Job_Lf*>(Heap::pAllocate(CN_JOB_LF));
  
  return pJob_Lf;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Lf::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Lf::operator delete(void* aAddress)
{
  Job_Lf* pJob_Lf = static_cast<Job_Lf*>(aAddress);
  
  Heap::vFree(pJob_Lf);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Lf::Job_Lf(Interface::SEitf* pSEitf)
: Job(CN_JOB_LF), JobDicPath(pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LF,_FOOTER_);
  
  m_sSeed    = mJobNote_Lf.pJobClass()->m_aSeed;

  _checkInitialised_(CN_JOB_LF);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_LF,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Lf::~Job_Lf(void)
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
  RASSERT(CN_JOB_DOCOL == eClass);
  RASSERT(pObject->eClass() == CN_JOB_LF);
  
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
  
                                                               //! Alka to write
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
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_Lf_pre(Prompt::SePrompt* pPrompt)
{
  SEitf* pSEitf = pPrompt->pSEitf();
  
  Job_Lf* pJob_Lf = new Job_Lf(pSEitf);
  
  TokenVec* pTokenVec = pPrompt->m_pCommandProcessor->pTknVec();
  
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
void JobsModel::vJob_Lf_post(Prompt::SePrompt* pPrompt, Job* pJob)
{
  Job_Lf* pJob_Lf = _pJob_Lf(pJob);
  
  if ( pJob_Lf->m_bHasError ) {
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("ERROR (NODE%lld): %s\n"_Stz, i_Node, aEC[pJob_Lf->m_xNode[i_Node].m_eErrorCode]);
    }
  }
  else {
    if (gpAttachMap->m_bMetadata) {
      vTrace("metadata: %s\n"_Stz, pJob_Lf->m_sMetadata.aStr());
    }
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("%s\n"_Stz, pJob_Lf->m_xNode[i_Node].m_sResponse.aChar());
    }
    if (gpAttachMap->m_bMetadata) {
      vTrace("success: %lld rows affected\n"_Stz, gpAttachMap->m_pGrid->m_nNodes);
    }
  }
  
  // now delete the job
  delete pJob_Lf;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
