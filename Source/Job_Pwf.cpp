/*============================================================================*/
/* Job_PresentWorkingFolder.cpp -- Present Working Folder Job                 */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobNote mJobNote_Pwf (
  "Job_Pwf"_Stz,         // name of the job class
  CN_JOB_PWF,            // class enum
  JK_SINGLENODE,         // job kind
  "change folder"_Stz,   // comment about the job
  "pwf"_Stz,             // the prompt command
  nullptr,               // the second word
  "pwf"_Stz,             // the task seed for this job
  false,                 // check resources?
  false,                 // detached job?
  pJob_Pwf_pre,          // prompt pre-process function
  vJob_Pwf_post,         // prompt post-process function
  true                   // job is enabled
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Pwf* JobsModel::_pJob_Pwf(void* aAddress)
{
  Job_Pwf* pJob_Pwf = reinterpret_cast<Job_Pwf*>(aAddress);
  
  //RASSERT( !pJob_Pwf || CN_JOB_PWF == pJob_Pwf->eClass());
  
  return pJob_Pwf;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Pwf::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_Pwf* pJob_Pwf =
    static_cast<Job_Pwf*>(Heap::pAllocate(CN_JOB_PWF));
  
  return pJob_Pwf;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Pwf::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Pwf::operator delete(void* aAddress)
{
  Job_Pwf* pJob_Pwf = static_cast<Job_Pwf*>(aAddress);
  
  Heap::vFree(pJob_Pwf);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Pwf::Job_Pwf(Interface::SEitf* pSEitf)
: Job(CN_JOB_PWF), JobDicPath(pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PWF,_FOOTER_);
  
  m_sSeed    = mJobNote_Pwf.pJobClass()->m_aSeed;

  _checkInitialised_(CN_JOB_PWF);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_PWF,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Pwf::~Job_Pwf(void)
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
  RASSERT(CN_JOB_DOCOL == eClass);
  RASSERT(pObject->eClass() == CN_JOB_PWF);
  
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
  
                                                               //! Alka to write
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
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_Pwf_pre(Interface::SEitf* pSEitf)
{
  Job_Pwf* pJob_Pwf = new Job_Pwf(pSEitf);
  
  TokenVec* pTokenVec = pSEitf->m_pCommandProcessor->pTknVec();
  
  bool bResult = pJob_Pwf->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_Pwf->m_bHasError = true;
    pJob_Pwf->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_Pwf;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_Pwf_post(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_Pwf* pJob_Pwf = _pJob_Pwf(pJob);
  
  if ( pJob_Pwf->m_bHasError ) {
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("ERROR (NODE%lld): %s\n"_Stz, i_Node, aEC[pJob_Pwf->m_xNode[i_Node].m_eErrorCode]);
    }
  }
  else {
    if (gpAttachMap->m_bMetadata) {
      vTrace("metadata: %s\n"_Stz, pJob_Pwf->m_sMetadata.aStr());
    }
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("%s\n"_Stz, pJob_Pwf->m_xNode[i_Node].m_sResponse.aChar());
    }
    if (gpAttachMap->m_bMetadata) {
      vTrace("success: %lld rows affected\n"_Stz, gpAttachMap->m_pGrid->m_nNodes);
    }
  }

  // now delete the job
  delete pJob_Pwf;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
