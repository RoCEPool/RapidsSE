/*============================================================================*/
/* Job_AnalyzeIndex.cpp -- analyze index Job                                  */
/*============================================================================*/

// Job Short: AnalyzeIndex

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_AnalyzeIndex =
{
  "Job_AnalyzeIndex"_Stz,    // name of the job class
  CN_JOB_ANALYZEINDEX,       // class enum
  JK_DIST2PC,                // job kind
  "analyse index"_Stz,       // comment about the job
  "anidx"_Stz,               // the prompt command
  nullptr,                   // the second word
  nullptr,                   // the _run task seed
  "analyzeIndex_req"_Stz,    // the _req task seed
  "analyzeIndex_beg"_Stz,    // the _rcv task seed
  "analyzeIndex_exe"_Stz,    // the _exe task seed
  "analyzeIndex_end"_Stz,    // the _end task seed
  "analyzeIndex_can"_Stz,    // the _can task seed
  true,                      // check resources?
  false,                     // detached job?
  false,                     // buffer attached?
  JT_NONE,                   // job trace
  pJob_AnalyzeIndex_pre,     // prompt pre-process function
  vJob_AnalyzeIndex_post,    // prompt post-process function
  pJob_AnalyzeIndex_new,     // job new function
  vJob_AnalyzeIndex_delete,  // job delete function
  vJob_send,                 // job pre-send function
  vJob_recv,                 // job post-receive function
  vJob_AnalyzeIndex_show,    // job show function
  
  // help and example
  true,
  "anidx <tableNm> <indexNm> [0/1]"_Stz,
  "anidx SUPPLIER idx1 0;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* JobModel::pJob_AnalyzeIndex_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_AnalyzeTable_class;
  
  return &xJobClass_AnalyzeIndex;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_AnalyzeIndex* JobModel::_pJob_AnalyzeIndex(void* aAddress)
{
  Job_AnalyzeIndex* pJob_AnalyzeTable = reinterpret_cast<Job_AnalyzeIndex*>(aAddress);
  
  RASSERT_TYPE(pJob_AnalyzeTable,CN_JOB_ANALYZEINDEX);
  
  return pJob_AnalyzeTable;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_AnalyzeIndex::operator new(size_t)
{
  Job_AnalyzeIndex* pJob_AnalyzeIndex =
    static_cast<Job_AnalyzeIndex*>(HeapMem::pAllocate(CN_JOB_ANALYZEINDEX));
  
  return pJob_AnalyzeIndex;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_AnalyzeIndex::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_AnalyzeIndex::operator delete(void* aAddress)
{
  Job_AnalyzeIndex* pJob_AnalyzeIndex = static_cast<Job_AnalyzeIndex*>(aAddress);
  
  HeapMem::vFree(pJob_AnalyzeIndex);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_AnalyzeIndex::Job_AnalyzeIndex(Interface::Session* pSession)
: Job(CN_JOB_ANALYZEINDEX, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_ANALYZEINDEX,_FOOTER_);
    
  m_bIsDetailed      = false;
  
  _checkInitialised_(CN_JOB_ANALYZEINDEX);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_ANALYZEINDEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_AnalyzeIndex::~Job_AnalyzeIndex(void) noexcept(false)
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
  m_sObjectName = pTokenVec->pGet(1)->aText();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the index name
  m_sIndexName = pTokenVec->pGet(2)->aText();
  
  if (iNumTokens == 4)
    m_bIsDetailed = pTokenVec->pGet(3)->bSame("true"_Stz) == 1 ? true : false;
  
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
Job* JobModel::pJob_AnalyzeIndex_pre(Interface::Session* pSession)
{
  Job_AnalyzeIndex* pJob_AnalyzeIndex = new Job_AnalyzeIndex(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommandProcessor->pTknVec();
  
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
void JobModel::vJob_AnalyzeIndex_post(Interface::Session* pSession, Job* pJob)
{
  Job_AnalyzeIndex* pJob_AnalyzeIndex = _pJob_AnalyzeIndex(pJob);
  
  if ( pJob_AnalyzeIndex->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_AnalyzeIndex->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
  else {
    if (gpAttachMap->m_bMetadata) {
      vTrace("metadata: %s\n"_Stz, pJob_AnalyzeIndex->m_sMetadata.aStz());
    }
    
    i8 iRowsProcessed = 0;
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("%s\n"_Stz, pJob_AnalyzeIndex->m_xNode[i_Node].m_sResponse.aChar());
      iRowsProcessed += *pJob_AnalyzeIndex->m_xNode[i_Node].m_ziRowsProcessed;
    }
    if (gpAttachMap->m_bMetadata) {
      vTrace("success: %lld rows affected\n"_Stz, iRowsProcessed);
    }
  }
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* JobModel::pJob_AnalyzeIndex_new(Job* pArg)
{
  Job_AnalyzeIndex* pJob_AnalyzeIndex = new Job_AnalyzeIndex(nullptr);
  
  return pJob_AnalyzeIndex;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_AnalyzeIndex_delete(Interface::Session* pSession, Job* pJob)
{
  Job_AnalyzeIndex* pJob_AnalyzeIndex = _pJob_AnalyzeIndex(pJob);
  
  delete pJob_AnalyzeIndex;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_AnalyzeIndex_show(Job* pJob)
{
  Job_AnalyzeIndex* pJob_AnalyzeIndex = _pJob_AnalyzeIndex(pJob);

  pJob_AnalyzeIndex->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
