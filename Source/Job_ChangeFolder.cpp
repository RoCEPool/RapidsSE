/*============================================================================*/
/* Job_ChangeFolder.cpp -- Change Folder Job                                  */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobNote mJobNote_ChangeFolder (
  "Job_ChangeFolder"_Stz,     // name of the job class
  CN_JOB_CHANGEFOLDER,        // class enum
  "change folder"_Stz,        // comment about the job
  "cf"_Stz,                   // the prompt command
  nullptr,                    // the second word
  "cf"_Stz,                   // the task seed for this job
  false,                      // detached job?
  pJob_ChangeFolder_pre,      // prompt pre-process function
  vJob_ChangeFolder_post,     // prompt post-process function
  true                        // job is enabled
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_ChangeFolder* JobsModel::_pJob_ChangeFolder(void* aAddress)
{
  Job_ChangeFolder* pJob_ChangeFolder = reinterpret_cast<Job_ChangeFolder*>(aAddress);
  
  //RASSERT( !pJob_ChangeFolder || CN_JOB_CHANGEFOLDER == pJob_ChangeFolder->eClass());
  
  return pJob_ChangeFolder;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_ChangeFolder::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_ChangeFolder* pJob_ChangeFolder =
    static_cast<Job_ChangeFolder*>(Heap::pAllocate(CN_JOB_CHANGEFOLDER));
  
  return pJob_ChangeFolder;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_ChangeFolder::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_ChangeFolder::operator delete(void* aAddress)
{
  Job_ChangeFolder* pJob_ChangeFolder = static_cast<Job_ChangeFolder*>(aAddress);
  
  Heap::vFree(pJob_ChangeFolder);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_ChangeFolder::Job_ChangeFolder(Interface::SEitf* pSEitf)
: Job(CN_JOB_CHANGEFOLDER), JobDicPath(pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CHANGEFOLDER,_FOOTER_);
  
  m_sSeed    = mJobNote_ChangeFolder.pJobClass()->m_aSeed;

  _checkInitialised_(CN_JOB_DOCOL);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_CHANGEFOLDER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_ChangeFolder::~Job_ChangeFolder(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CHANGEFOLDER,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_ChangeFolder::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_DOCOL == eClass);
  RASSERT(pObject->eClass() == CN_JOB_CHANGEFOLDER);
  
  Job_ChangeFolder* pJob_ChangeFolder = _pJob_ChangeFolder(pObject);

  pJob_ChangeFolder->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_ChangeFolder::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CHANGEFOLDER,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("cf"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - -
  // get no of tokens in token vector
  iNumTokens = pTokenVec->nTokens();;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 2 tokens
  if (iNumTokens != 2) {
    return nullptr;
  }
  
  // set the docol command name
  m_sObjectName = pTokenVec->pGet(1)->aText();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_ChangeFolder::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CHANGEFOLDER,_FOOTER_);
  
                                                               //! Alka to write
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_ChangeFolder::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CHANGEFOLDER);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_ChangeFolder_pre(Prompt::SePrompt* pPrompt)
{
  SEitf* pSEitf = pPrompt->pSEitf();
  
  Job_ChangeFolder* pJob_ChangeFolder = new Job_ChangeFolder(pSEitf);
  
  TokenVec* pTokenVec = pPrompt->m_pCommandProcessor->pTknVec();
  
  bool bResult = pJob_ChangeFolder->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_ChangeFolder->m_bHasError = true;
    pJob_ChangeFolder->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_ChangeFolder;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_ChangeFolder_post(Prompt::SePrompt* pPrompt, Job* pJob)
{
  Job_ChangeFolder* pJob_ChangeFolder = _pJob_ChangeFolder(pJob);
  
  // Note: take care of errors / meta data ????
  for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
    vTrace("%s\n"_Stz, pJob_ChangeFolder->m_xNode[i_Node].m_sResponse.aChar());
  }

  // now delete the job
  delete pJob_ChangeFolder;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
