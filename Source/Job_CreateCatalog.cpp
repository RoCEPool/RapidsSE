/*============================================================================*/
/* Job_CreateCatalog.cpp -- Create Catalog Job                                */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobNote mJobNote_CreateCatalog (
  "Job_CreateCatalog"_Stz,           // name of the job class
  CN_JOB_CREATECATALOG,              // class enum
  "create catalog catalogName"_Stz,  // comment about the job
  "create"_Stz,                      // the prompt command
  "catalog"_Stz,                     // the second word
  "createCatalog"_Stz,               // the task seed for this job
  false,                             // detached job?
  pJob_CreateCatalog_pre,            // prompt pre-process function
  vJob_CreateCatalog_post,           // prompt post-process function
  true
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_CreateCatalog* JobsModel::_pJob_CreateCatalog(void* aAddress)
{
  Job_CreateCatalog* pJob_CreateCatalog = reinterpret_cast<Job_CreateCatalog*>(aAddress);
  
  RASSERT( !pJob_CreateCatalog || CN_JOB_CREATECATALOG == pJob_CreateCatalog->eClass());
  
  return pJob_CreateCatalog;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_CreateCatalog::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_CreateCatalog* pJob_CreateCatalog =
    static_cast<Job_CreateCatalog*>(Heap::pAllocate(CN_JOB_CREATECATALOG));
  
  return pJob_CreateCatalog;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_CreateCatalog::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_CreateCatalog::operator delete(void* aAddress)
{
  Job_CreateCatalog* pJob_CreateCatalog = static_cast<Job_CreateCatalog*>(aAddress);
  
  Heap::vFree(pJob_CreateCatalog);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_CreateCatalog::Job_CreateCatalog(Interface::SEitf* pSEitf)
: Job(CN_JOB_CREATECATALOG), JobDicPath(pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATECATALOG,_FOOTER_);
    
  m_sSeed = mJobNote_CreateCatalog.pJobClass()->m_aSeed;
  m_bCheckResources   = true;
  
  _checkInitialised_(CN_JOB_CREATECATALOG);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATECATALOG,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_CreateCatalog::~Job_CreateCatalog(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATECATALOG,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_CreateCatalog::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_CREATECATALOG == eClass);
  RASSERT(pObject->eClass() == CN_JOB_CREATECATALOG);
  
  Job_CreateCatalog* pJob_CreateCatalog = _pJob_CreateCatalog(pObject);

  pJob_CreateCatalog->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_CreateCatalog::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATECATALOG,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("create"_Stz));
  ASSERT(pTokenVec->pGet(1)->bSame("catalog"_Stz));
  
  // get number of items in token vector
  iNumTokens = pTokenVec->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3)
  return false;
  
  // set the table name
  m_sObjectName = pTokenVec->pGet(2)->aText();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_CreateCatalog::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATECATALOG,_FOOTER_);
  
                                                               //! Alka to write
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_CreateCatalog::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATECATALOG);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Prompt ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_CreateCatalog_pre(Interface::SEitf* pSEitf, Prompt::SePrompt* pPrompt)
{
  //SEitf* pSEitf = pPrompt->pSEitf();
  Job_CreateCatalog* pJob_CreateCatalog = new Job_CreateCatalog(pSEitf);
  
  TokenVec* pTokenVec = pPrompt->m_pCommandProcessor->pTknVec();
  
  bool bResult = pJob_CreateCatalog->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_CreateCatalog->m_bHasError = true;
    pJob_CreateCatalog->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_CreateCatalog;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_CreateCatalog_post(Prompt::SePrompt* pPrompt, Job* pJob)
{
  Job_CreateCatalog* pJob_CreateCatalog = _pJob_CreateCatalog(pJob);
  
  if ( pJob_CreateCatalog->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("ERROR (NODE%lld): %s\n"_Stz, i_Node, aEC[pJob_CreateCatalog->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
  
  // now delete the job
  delete pJob_CreateCatalog;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
