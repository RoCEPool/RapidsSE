/*============================================================================*/
/* Job_VerifyIndex.cpp -- verify index Job                                    */
/*============================================================================*/

// Job Short: VerifyIndex

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_VerifyIndex =
{
  "Job_VerifyIndex"_Stz,      // name of the job class
  CN_JOB_VERIFYINDEX,         // class enum
  JK_DIST2PC,                 // job kind
  "verify index"_Stz,         // comment about the job
  "vfyidx"_Stz,               // the prompt command
  nullptr,                    // the second word
  nullptr,                    // the _run task seed
  "verifyIndex_req"_Stz,      // the _req task seed
  "verifyIndex_beg"_Stz,      // the _rcv task seed
  "verifyIndex_exe"_Stz,      // the _exe task seed
  "verifyIndex_end"_Stz,      // the _end task seed
  "verifyIndex_can"_Stz,      // the _can task seed
  true,                       // check resources?
  false,                      // detached job?
  false,                      // buffer attached?
  JT_NONE,                    // job trace
  pJob_VerifyIndex_pre,       // prompt pre-process function
  vJob_VerifyIndex_post,      // prompt post-process function
  nullptr,                    // job new function
  vJob_VerifyIndex_delete,    // job delete function
  vJob_send,                  // job pre-send function
  vJob_recv,                  // job post-receive function
  vJob_VerifyIndex_show,      // job show function
  
  // help and example
  true,
  "vfyidx <tableName> <indexName>"_Stz,
  "vfyidx SUPPPLIER idx1;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* JobModel::pJob_VerifyIndex_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_VisualizeIndex_class;
  
  return &xJobClass_VerifyIndex;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_VerifyIndex* JobModel::_pJob_VerifyIndex(void* aAddress)
{
  Job_VerifyIndex* pJob_VerifyIndex = reinterpret_cast<Job_VerifyIndex*>(aAddress);
  
  RASSERT_TYPE(pJob_VerifyIndex,CN_JOB_VERIFYINDEX);
  
  return pJob_VerifyIndex;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_VerifyIndex::operator new(size_t)
{
  Job_VerifyIndex* pJob_VerifyIndex =
    static_cast<Job_VerifyIndex*>(HeapMem::pAllocate(CN_JOB_VERIFYINDEX));
  
  return pJob_VerifyIndex;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_VerifyIndex::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_VerifyIndex::operator delete(void* aAddress)
{
  Job_VerifyIndex* pJob_VerifyIndex = static_cast<Job_VerifyIndex*>(aAddress);
  
  HeapMem::vFree(pJob_VerifyIndex);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_VerifyIndex::Job_VerifyIndex(Interface::Session* pSession)
: Job(CN_JOB_VERIFYINDEX, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_VERIFYINDEX,_FOOTER_);
    
  _checkInitialised_(CN_JOB_VERIFYINDEX);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_VERIFYINDEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_VerifyIndex::~Job_VerifyIndex(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_VERIFYINDEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_VerifyIndex::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_VerifyIndex* pJob_VerifyIndex = _pJob_VerifyIndex(pObject);

  pJob_VerifyIndex->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_VerifyIndex::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_VERIFYINDEX,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("vfyidx"_Stz));
  
  // get number of items in token vector
  iNumTokens = pTokenVec->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the index name
  m_sIndexName = pTokenVec->pGet(2)->aText();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the table name
  m_sObjectName = pTokenVec->pGet(1)->aText();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_VerifyIndex::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_VERIFYINDEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_VerifyIndex::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_VERIFYINDEX);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobModel::pJob_VerifyIndex_pre(Interface::Session* pSession)
{
  Job_VerifyIndex* pJob_VerifyIndex = new Job_VerifyIndex(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommandProcessor->pTknVec();
  
  bool bResult = pJob_VerifyIndex->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_VerifyIndex->m_bHasError = true;
    pJob_VerifyIndex->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_VerifyIndex;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_VerifyIndex_post(Interface::Session* pSession, Job* pJob)
{
  Job_VerifyIndex* pJob_VerifyIndex = _pJob_VerifyIndex(pJob);
  
  if ( pJob_VerifyIndex->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_VerifyIndex->m_xNode[i_Node].m_eErrorCode]);
    }
  }
  else {
    if (gpAttachMap->m_bMetadata) {
      vTrace("metadata: %s\n"_Stz, pJob_VerifyIndex->m_sMetadata.aStz());
    }
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("%s\n"_Stz, pJob_VerifyIndex->m_xNode[i_Node].m_sResponse.aChar());
    }
    vTrace("success: %lld rows affected\n"_Stz, gpAttachMap->m_pGrid->m_nNodes);
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_VerifyIndex_delete(Interface::Session* pSession, Job* pJob)
{
  Job_VerifyIndex* pJob_VerifyIndex = _pJob_VerifyIndex(pJob);
  
  delete pJob_VerifyIndex;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_VerifyIndex_show(Job* pJob)
{
  Job_VerifyIndex* pJob_VerifyIndex = _pJob_VerifyIndex(pJob);

  pJob_VerifyIndex->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
