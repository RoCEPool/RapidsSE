/*============================================================================*/
/* Job_AnalyzeTable.cpp -- analyze table Job                                  */
/*============================================================================*/

// Job Short: AnalyzeTable

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_AnalyzeTable =
{
  "Job_AnalyzeTable"_Stz,    // name of the job class
  CN_JOB_ANALYZETABLE,       // class enum
  JK_DIST2PC,                // job kind
  "analyze table"_Stz,       // comment about the job
  "antbl"_Stz,               // the prompt command
  nullptr,                   // the second word
  nullptr,                   // the _run task seed
  "analyzeTable_req"_Stz,    // the _req task seed
  "analyzeTable_beg"_Stz,    // the _rcv task seed
  "analyzeTable_exe"_Stz,    // the _exe task seed
  "analyzeTable_end"_Stz,    // the _end task seed
  "analyzeTable_can"_Stz,    // the _can task seed
  true,                      // check resources?
  false,                     // detached job?
  false,                     // buffer attached?
  JT_NONE,                   // job trace
  pJob_AnalyzeTable_pre,     // prompt pre-process function
  vJob_AnalyzeTable_post,    // prompt post-process function
  nullptr,                   // job new function
  vJob_AnalyzeTable_delete,  // job delete function
  vJob_send,                 // job pre-send function
  vJob_recv,                 // job post-receive function
  vJob_AnalyzeTable_show,    // job show function
  
  // help and example
  true,
  "antbl <tableNm> [0/1]"_Stz,
  "<example>"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_AnalyzeTable_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Answer_class;
  
  return &xJobClass_AnalyzeTable;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_AnalyzeTable* Jobs::_pJob_AnalyzeTable(void* aAddress)
{
  Job_AnalyzeTable* pJob_AnalyzeTable = reinterpret_cast<Job_AnalyzeTable*>(aAddress);
  
  RASSERT_TYPE(pJob_AnalyzeTable,CN_JOB_ANALYZETABLE);
  
  return pJob_AnalyzeTable;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_AnalyzeTable::operator new(size_t)
{
  Job_AnalyzeTable* pJob_AnalyzeTable =
    static_cast<Job_AnalyzeTable*>(HeapMem::pAllocate(CN_JOB_ANALYZETABLE));
  
  return pJob_AnalyzeTable;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_AnalyzeTable::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_AnalyzeTable::operator delete(void* aAddress)
{
  Job_AnalyzeTable* pJob_AnalyzeTable = static_cast<Job_AnalyzeTable*>(aAddress);
  
  HeapMem::vFree(pJob_AnalyzeTable);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_AnalyzeTable::Job_AnalyzeTable(Interface::Session* pSession)
: Job(CN_JOB_ANALYZETABLE, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_ANALYZETABLE,_FOOTER_);
    
  m_bIsDetailed = false;
  
  _checkInitialised_(CN_JOB_ANALYZETABLE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_ANALYZETABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_AnalyzeTable::~Job_AnalyzeTable(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_ANALYZETABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_AnalyzeTable::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_AnalyzeTable* pJob_AnalyzeTable = _pJob_AnalyzeTable(pObject);

  pJob_AnalyzeTable->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_AnalyzeTable::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_ANALYZETABLE,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("antbl"_Stz));
  
  // get number of items in token vector
  iNumTokens = pTokenVec->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 2 or 3 tokens
  if (iNumTokens != 2 && iNumTokens != 3)
    return false;
  
  // set the table name
  m_sObjectName = pTokenVec->pGet(1)->aText();
  
  if (iNumTokens == 3)
    m_bIsDetailed = pTokenVec->pGet(2)->bSame("true"_Stz) == 1 ? true : false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_AnalyzeTable::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_ANALYZETABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_AnalyzeTable::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_ANALYZETABLE);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_AnalyzeTable_pre(Interface::Session* pSession)
{
  Job_AnalyzeTable* pJob_AnalyzeTable = new Job_AnalyzeTable(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommand->pTknVec();
  
  bool bResult = pJob_AnalyzeTable->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_AnalyzeTable->m_bHasError = true;
    pJob_AnalyzeTable->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_AnalyzeTable;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_AnalyzeTable_post(Interface::Session* pSession, Job* pJob)
{
  Job_AnalyzeTable* pJob_AnalyzeTable = _pJob_AnalyzeTable(pJob);
  
  if ( pJob_AnalyzeTable->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_AnalyzeTable->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
  else {
    if (gpAttachMap->m_bMetadata) {
      vTrace("metadata: %s\n"_Stz, pJob_AnalyzeTable->m_sMetadata.aStz());
    }
    
    i8 iRowsProcessed = 0;
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("%s\n"_Stz, pJob_AnalyzeTable->m_xNode[i_Node].m_sResponse.aChar());
      iRowsProcessed += *pJob_AnalyzeTable->m_xNode[i_Node].m_ziRowsProcessed;
    }
    
    vTrace("success: %lld rows affected\n"_Stz, iRowsProcessed);
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_AnalyzeTable_delete(Interface::Session* pSession, Job* pJob)
{
  Job_AnalyzeTable* pJob_AnalyzeTable = _pJob_AnalyzeTable(pJob);
  
  delete pJob_AnalyzeTable;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_AnalyzeTable_show(Job* pJob)
{
  Job_AnalyzeTable* pJob_AnalyzeTable = _pJob_AnalyzeTable(pJob);

  pJob_AnalyzeTable->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
