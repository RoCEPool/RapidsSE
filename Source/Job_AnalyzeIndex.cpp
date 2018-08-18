/*============================================================================*/
/* Job_AnalyzeIndex.cpp -- analyze index Job                                  */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobNote mJobNote_AnalyzeIndex (
  "Job_AnalyzeIndex"_Stz,    // name of the job class
  CN_JOB_ANALYZEINDEX,       // class enum
  JK_DIST2PC,                // job kind
  "anidx tableNm indexNm [0/1]"_Stz,   // comment about the job
  "anidx"_Stz,               // the prompt command
  nullptr,                   // the second word
  "analyzeIndex"_Stz,        // the task seed for this job
  "analyzeIndex_node"_Stz,   // the second task seed for this job
  nullptr,                   // the third task seed for this job
  true,                      // check resources?
  false,                     // detached job?
  false,                     // buffer attached?
  JT_NONE,                   // job trace
  pJob_AnalyzeIndex_pre,     // prompt pre-process function
  vJob_AnalyzeIndex_post,    // prompt post-process function
  nullptr,                   // job new function
  vJob_AnalyzeIndex_delete,  // job delete function
  nullptr,                   // job pre-send function
  nullptr,                   // job post-receive function
  vJob_AnalyzeIndex_show,    // job show function
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
Job_AnalyzeIndex* JobsModel::_pJob_AnalyzeIndex(void* aAddress)
{
  Job_AnalyzeIndex* pJob_AnalyzeTable = reinterpret_cast<Job_AnalyzeIndex*>(aAddress);
  
  RASSERT( !pJob_AnalyzeTable || CN_JOB_ANALYZEINDEX == pJob_AnalyzeTable->eClass());
  
  return pJob_AnalyzeTable;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_AnalyzeIndex::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_AnalyzeIndex* pJob_AnalyzeIndex =
    static_cast<Job_AnalyzeIndex*>(Heap::pAllocate(CN_JOB_ANALYZEINDEX));
  
  return pJob_AnalyzeIndex;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_AnalyzeIndex::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_AnalyzeIndex::operator delete(void* aAddress)
{
  Job_AnalyzeIndex* pJob_AnalyzeIndex = static_cast<Job_AnalyzeIndex*>(aAddress);
  
  Heap::vFree(pJob_AnalyzeIndex);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_AnalyzeIndex::Job_AnalyzeIndex(Interface::SEitf* pSEitf)
: Job(CN_JOB_ANALYZEINDEX, pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_ANALYZEINDEX,_FOOTER_);
    
  m_sSeed = mJobNote_AnalyzeIndex.pJobClass()->m_aSeed1;
  m_bCheckResources  = true;
  m_sTableName       = ""_Stz;
  m_bIsDetailed      = false;
  
  _checkInitialised_(CN_JOB_ANALYZEINDEX);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_ANALYZEINDEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_AnalyzeIndex::~Job_AnalyzeIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_ANALYZEINDEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_AnalyzeIndex::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_ANALYZEINDEX == eClass);
  RASSERT(pObject->eClass() == CN_JOB_ANALYZEINDEX);
  
  Job_AnalyzeIndex* pJob_AnalyzeIndex = _pJob_AnalyzeIndex(pObject);

  pJob_AnalyzeIndex->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_AnalyzeIndex::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_ANALYZEINDEX,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("anidx"_Stz));
  
  // get number of items in token vector
  iNumTokens = pTokenVec->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 or 4 tokens
  if (iNumTokens != 3 && iNumTokens != 4)
    return false;
  
  // set the table name
  m_sTableName = pTokenVec->pGet(1)->aText();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the index name
  m_sObjectName = pTokenVec->pGet(2)->aText();
  
  if (iNumTokens == 4)
    m_bIsDetailed = pTokenVec->pGet(3)->aText()->iToI8();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_AnalyzeIndex::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_ANALYZEINDEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_AnalyzeIndex::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_ANALYZEINDEX);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_AnalyzeIndex_pre(Interface::SEitf* pSEitf)
{
  Job_AnalyzeIndex* pJob_AnalyzeIndex = new Job_AnalyzeIndex(pSEitf);
  
  TokenVec* pTokenVec = pSEitf->m_pCommandProcessor->pTknVec();
  
  bool bResult = pJob_AnalyzeIndex->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_AnalyzeIndex->m_bHasError = true;
    pJob_AnalyzeIndex->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_AnalyzeIndex;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_AnalyzeIndex_post(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_AnalyzeIndex* pJob_AnalyzeIndex = _pJob_AnalyzeIndex(pJob);
  
  if ( pJob_AnalyzeIndex->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("ERROR (NODE%lld): %s\n"_Stz, i_Node, aEC[pJob_AnalyzeIndex->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
  else {
    if (gpAttachMap->m_bMetadata) {
      vTrace("metadata: %s\n"_Stz, pJob_AnalyzeIndex->m_sMetadata.aStr());
    }
    
    i8 iRowsProcessed = 0;
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("%s\n"_Stz, pJob_AnalyzeIndex->m_xNode[i_Node].m_sResponse.aChar());
      iRowsProcessed += pJob_AnalyzeIndex->m_xNode[i_Node].m_iRowsProcessed;
    }
    if (gpAttachMap->m_bMetadata) {
      vTrace("success: %lld rows affected\n"_Stz, iRowsProcessed);
    }
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_AnalyzeIndex_delete(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_AnalyzeIndex* pJob_AnalyzeIndex = _pJob_AnalyzeIndex(pJob);
  
  delete pJob_AnalyzeIndex;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_AnalyzeIndex_show(Job* pJob)
{
  Job_AnalyzeIndex* pJob_AnalyzeIndex = _pJob_AnalyzeIndex(pJob);

  pJob_AnalyzeIndex->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
