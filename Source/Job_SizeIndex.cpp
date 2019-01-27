/*============================================================================*/
/* Job_SizeIndex.cpp -- size index Job                                        */
/*============================================================================*/

// Job Short: SizeIndex

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_SizeIndex =
{
  "Job_SizeIndex"_Stz,   // name of the job class
  CN_JOB_SIZEINDEX,      // class enum
  JK_DIST2PC,            // job kind
  "size of index"_Stz,   // comment about the job
  "szidx"_Stz,           // the prompt command
  nullptr,               // the second word
  nullptr,               // the _run task seed
  "sizeIndex_req"_Stz,   // the _req task seed
  "sizeIndex_beg"_Stz,   // the _rcv task seed
  "sizeIndex_exe"_Stz,   // the _exe task seed
  "sizeIndex_end"_Stz,   // the _end task seed
  "sizeIndex_can"_Stz,   // the _can task seed
  true,                  // check resources?
  false,                 // detached job?
  false,                 // buffer attached?
  JT_NONE,               // job trace
  pJob_SizeIndex_pre,    // prompt pre-process function
  vJob_SizeIndex_post,   // prompt post-process function
  nullptr,               // job new function
  vJob_SizeIndex_delete, // job delete function
  vJob_send,             // job pre-send function
  vJob_recv,             // job post-receive function
  vJob_SizeIndex_show,   // job show function
  
  // help and example
  true,
  "szidx <tableNam> <indexName>"_Stz,
  "szidx supplier idx1;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_SizeIndex_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_SizeTable_class;
  
  return &xJobClass_SizeIndex;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_SizeIndex* Jobs::_pJob_SizeIndex(void* aAddress)
{
  Job_SizeIndex* pJob_SizeIndex = reinterpret_cast<Job_SizeIndex*>(aAddress);
  
  RASSERT_TYPE(pJob_SizeIndex,CN_JOB_SIZEINDEX);
  
  return pJob_SizeIndex;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_SizeIndex::operator new(size_t)
{
  Job_SizeIndex* pJob_SizeIndex =
    static_cast<Job_SizeIndex*>(HeapMem::pAllocate(CN_JOB_SIZEINDEX));
  
  return pJob_SizeIndex;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_SizeIndex::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_SizeIndex::operator delete(void* aAddress)
{
  Job_SizeIndex* pJob_SizeIndex = static_cast<Job_SizeIndex*>(aAddress);
  
  HeapMem::vFree(pJob_SizeIndex);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_SizeIndex::Job_SizeIndex(Interface::Session* pSession)
: Job(CN_JOB_SIZEINDEX, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SIZEINDEX,_FOOTER_);
    
  _checkInitialised_(CN_JOB_SIZEINDEX);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_SIZEINDEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_SizeIndex::~Job_SizeIndex(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SIZEINDEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_SizeIndex::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_SizeIndex* pJob_SizeIndex = _pJob_SizeIndex(pObject);

  pJob_SizeIndex->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_SizeIndex::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SIZEINDEX,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("szidx"_Stz));
  
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
void Job_SizeIndex::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SIZEINDEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_SizeIndex::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SIZEINDEX);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_SizeIndex_pre(Interface::Session* pSession)
{
  Job_SizeIndex* pJob_SizeIndex = new Job_SizeIndex(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommand->pTknVec();
  
  bool bResult = pJob_SizeIndex->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_SizeIndex->m_bHasError = true;
    pJob_SizeIndex->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_SizeIndex;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_SizeIndex_post(Interface::Session* pSession, Job* pJob)
{
  Job_SizeIndex* pJob_SizeIndex = _pJob_SizeIndex(pJob);
  
  if ( pJob_SizeIndex->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_SizeIndex->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
  else {
    if (gpAttachMap->m_bMetadata) {
      vTrace("metadata: %s\n"_Stz, pJob_SizeIndex->m_sMetadata.aStz());
    }
    
    i8 iRowsProcessed = 0;
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("%s\n"_Stz, pJob_SizeIndex->m_xNode[i_Node].m_sResponse.aChar());
      iRowsProcessed += *pJob_SizeIndex->m_xNode[i_Node].m_ziRowsProcessed;
    }
    vTrace("success: %lld rows affected\n"_Stz, iRowsProcessed);
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_SizeIndex_delete(Interface::Session* pSession, Job* pJob)
{
  Job_SizeIndex* pJob_SizeTable = _pJob_SizeIndex(pJob);
  
  delete pJob_SizeTable;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_SizeIndex_show(Job* pJob)
{
  Job_SizeIndex* pJob_SizeIndex = _pJob_SizeIndex(pJob);

  pJob_SizeIndex->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
