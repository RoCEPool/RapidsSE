/*============================================================================*/
/* Job_TruncateIndex.cpp -- analyze index Job                                  */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobNote mJobNote_TruncateIndex (
  "Job_TruncateIndex"_Stz,   // name of the job class
  CN_JOB_TRUNCATEINDEX,      // class enum
  JK_DIST2PC,                // job kind
  "tr tableNm"_Stz,          // comment about the job
  "tr"_Stz,                  // the prompt command
  nullptr,                   // the second word
  "truncateIndex"_Stz,       // the task seed for this job
  true,                      // check resources?
  false,                     // detached job?
  JT_NONE,                   // job trace
  pJob_TruncateIndex_pre,    // prompt pre-process function
  vJob_TruncateIndex_post,   // prompt post-process function
  nullptr,                   // job new function
  vJob_TruncateIndex_delete, // job delete function
  nullptr,                   // job send function
  nullptr,                   // job receive function
  vJob_TruncateIndex_show,   // job show function
  true,                      // job is enabled
  "<help>"_Stz,              // help
  "<example>"_Stz            // example
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_TruncateIndex* JobsModel::_pJob_TruncateIndex(void* aAddress)
{
  Job_TruncateIndex* pJob_TruncateIndex = reinterpret_cast<Job_TruncateIndex*>(aAddress);
  
  RASSERT( !pJob_TruncateIndex || CN_JOB_TRUNCATEINDEX == pJob_TruncateIndex->eClass());
  
  return pJob_TruncateIndex;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_TruncateIndex::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_TruncateIndex* pJob_TruncateIndex =
    static_cast<Job_TruncateIndex*>(Heap::pAllocate(CN_JOB_TRUNCATEINDEX));
  
  return pJob_TruncateIndex;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_TruncateIndex::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_TruncateIndex::operator delete(void* aAddress)
{
  Job_TruncateIndex* pJob_TruncateIndex = static_cast<Job_TruncateIndex*>(aAddress);
  
  Heap::vFree(pJob_TruncateIndex);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_TruncateIndex::Job_TruncateIndex(Interface::SEitf* pSEitf)
: Job(CN_JOB_TRUNCATEINDEX, pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_TRUNCATEINDEX,_FOOTER_);
    
  m_sSeed = mJobNote_TruncateIndex.pJobClass()->m_aSeed;
  m_bCheckResources  = true;
  m_sTableName       = ""_Stz;
  m_bIsDetailed      = false;
  
  _checkInitialised_(CN_JOB_ANALYZETABLE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_TRUNCATEINDEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_TruncateIndex::~Job_TruncateIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_TRUNCATEINDEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_TruncateIndex::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_ANALYZEINDEX == eClass);
  RASSERT(pObject->eClass() == CN_JOB_TRUNCATEINDEX);
  
  Job_TruncateIndex* pJob_TruncateIndex = _pJob_TruncateIndex(pObject);

  pJob_TruncateIndex->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_TruncateIndex::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_TRUNCATEINDEX,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("anidx"_Stz));
  
  // get number of items in token vector
  iNumTokens = pTokenVec->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 2 tokens
  if (iNumTokens != 2)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the table name
  m_sObjectName = pTokenVec->pGet(1)->aText();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_TruncateIndex::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_TRUNCATEINDEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_TruncateIndex::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_TRUNCATEINDEX);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_TruncateIndex_pre(Interface::SEitf* pSEitf)
{
  Job_TruncateIndex* pJob_TruncateIndex = new Job_TruncateIndex(pSEitf);
  
  TokenVec* pTokenVec = pSEitf->m_pCommandProcessor->pTknVec();
  
  bool bResult = pJob_TruncateIndex->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_TruncateIndex->m_bHasError = true;
    pJob_TruncateIndex->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_TruncateIndex;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_TruncateIndex_post(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_TruncateIndex* pJob_TruncateIndex = _pJob_TruncateIndex(pJob);
  
  if ( pJob_TruncateIndex->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("ERROR (NODE%lld): %s\n"_Stz, i_Node, aEC[pJob_TruncateIndex->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_TruncateIndex_delete(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_TruncateIndex* pJob_TruncateIndex = _pJob_TruncateIndex(pJob);
  
  delete pJob_TruncateIndex;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_TruncateIndex_show(Job* pJob)
{
  Job_TruncateIndex* pJob_TruncateIndex = _pJob_TruncateIndex(pJob);

  pJob_TruncateIndex->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
