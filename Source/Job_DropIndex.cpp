/*============================================================================*/
/* Job_DropIndex.cpp -- drop index Job                                        */
/*============================================================================*/

// Job Short: DropIndex

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_DropIndex =
{
  "Job_DropIndex"_Stz,   // name of the job class
  CN_JOB_DROPINDEX,      // class enum
  JK_DIST2PC,            // job kind
  "drop index"_Stz,      // comment about the job
  "drop"_Stz,            // the prompt command
  "index"_Stz,           // the second word
  nullptr,               // the _run task seed
  "dropIndex_req"_Stz,   // the _req task seed
  "dropIndex_beg"_Stz,   // the _rcv task seed
  "dropIndex_exe"_Stz,   // the _exe task seed
  "dropIndex_end"_Stz,   // the _end task seed
  "dropIndex_can"_Stz,   // the _can task seed
  true,                  // check resources?
  false,                 // detached job?
  false,                 // buffer attached?
  JT_NONE,               // job trace
  pJob_DropIndex_pre,    // prompt pre-process function
  vJob_DropIndex_post,   // prompt post-process function
  nullptr,               // job new function
  vJob_DropIndex_delete, // job delete function
  vJob_send,             // job pre-send function
  vJob_recv,             // job post-receive function
  vJob_DropIndex_show,   // job show function
  
  // true
  true,
  "drop index <tableName> . <indexName>"_Stz,
  "drop index supplier.idx1;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_DropIndex_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_DropSchema_class;
  
  return &xJobClass_DropIndex;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_DropIndex* Jobs::_pJob_DropIndex(void* aAddress)
{
  Job_DropIndex* pJob_DropIndex = reinterpret_cast<Job_DropIndex*>(aAddress);
  
  RASSERT_TYPE(pJob_DropIndex,CN_JOB_DROPINDEX);
  
  return pJob_DropIndex;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_DropIndex::operator new(size_t)
{
  Job_DropIndex* pJob_DropIndex =
    static_cast<Job_DropIndex*>(HeapMem::pAllocate(CN_JOB_DROPINDEX));
  
  return pJob_DropIndex;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_DropIndex::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_DropIndex::operator delete(void* aAddress)
{
  Job_DropIndex* pJob_DropIndex = static_cast<Job_DropIndex*>(aAddress);
  
  HeapMem::vFree(pJob_DropIndex);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_DropIndex::Job_DropIndex(Interface::Session* pSession)
: Job(CN_JOB_DROPINDEX, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPINDEX,_FOOTER_);
    
  _checkInitialised_(CN_JOB_DROPINDEX);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPINDEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_DropIndex::~Job_DropIndex(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPINDEX,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_DropIndex::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_DropIndex* pJob_DropIndex = _pJob_DropIndex(pObject);

  pJob_DropIndex->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_DropIndex::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPINDEX,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("drop"_Stz));
  ASSERT(pTokenVec->pGet(1)->bSame("index"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - -
  // get number of items in token
  iNumTokens = pTokenVec->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 5 tokens
  if (iNumTokens != 5)
    return false;
  
  // 4th token should be '.'
  if (!pTokenVec->pGet(3)->bSame("."_Stz)) {
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the table name
  m_sObjectName = pTokenVec->pGet(2)->aText();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the index name
  m_sIndexName = pTokenVec->pGet(4)->aText();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_DropIndex::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPINDEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_DropIndex::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPINDEX);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_DropIndex_pre(Interface::Session* pSession)
{
  Job_DropIndex* pJob_DropIndex = new Job_DropIndex(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommandProcessor->pTknVec();
  
  bool bResult = pJob_DropIndex->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_DropIndex->m_bHasError = true;
    pJob_DropIndex->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_DropIndex;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_DropIndex_post(Interface::Session* pSession, Job* pJob)
{
  Job_DropIndex* pJob_DropIndex = _pJob_DropIndex(pJob);
  
  if ( pJob_DropIndex->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_DropIndex->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
  else {
    vTrace("success: 0 rows affected\n"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_DropIndex_delete(Interface::Session* pSession, Job* pJob)
{
  Job_DropIndex* pJob_DropIndex = _pJob_DropIndex(pJob);
  
  delete pJob_DropIndex;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_DropIndex_show(Job* pJob)
{
  Job_DropIndex* pJob_DropIndex = _pJob_DropIndex(pJob);

  pJob_DropIndex->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
