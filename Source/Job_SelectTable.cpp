/*============================================================================*/
/* Job_SelectTable.cpp -- Fetch Rows Job                                        */
/*============================================================================*/

// Job Short: SelectTable

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_SelectTable =
{
  "Job_SelectTable"_Stz,     // name of the job class
  CN_JOB_SELECTTABLE,        // class enum
  JK_LOCAL,                  // job kind
  "select table"_Stz,        // comment about the job
  "select"_Stz,              // the prompt command
  nullptr,                   // the second word
  nullptr,                   // the _run task seed
  "selectTable_req"_Stz,     // the _req task seed
  nullptr,                   // the _rcv task seed
  nullptr,                   // the _exe task seed
  nullptr,                   // the _end task seed
  nullptr,                   // the _can task seed
  true,                      // check resources?
  false,                     // detached job?
  false,                     // buffer attached?
  JT_NONE,                   // job trace
  pJob_SelectTable_pre,      // prompt pre-process function
  vJob_SelectTable_post,     // prompt post-process function
  nullptr,                   // job new function
  vJob_SelectTable_delete,   // job delete function
  nullptr,                   // job pre-send function
  nullptr,                   // job post-receive function
  vJob_SelectTable_show,     // job show function
  
  // help and example
  true,
  "select <tableName> P#"_Stz,
  "fetch supplier 1;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* JobModel::pJob_SelectTable_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_ShowCluster_class;
  
  return &xJobClass_SelectTable;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_SelectTable* JobModel::_pJob_SelectTable(void* aAddress)
{
  Job_SelectTable* pJob_SelectTable = reinterpret_cast<Job_SelectTable*>(aAddress);
  
  RASSERT_TYPE(pJob_SelectTable,CN_JOB_SELECTTABLE);
  
  return pJob_SelectTable;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_SelectTable::operator new(size_t)
{
  Job_SelectTable* pJob_SelectTable =
    static_cast<Job_SelectTable*>(HeapMem::pAllocate(CN_JOB_SELECTTABLE));
  
  return pJob_SelectTable;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_SelectTable::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_SelectTable::operator delete(void* aAddress)
{
  Job_SelectTable* pJob_SelectTable = static_cast<Job_SelectTable*>(aAddress);
  
  HeapMem::vFree(pJob_SelectTable);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_SelectTable::Job_SelectTable(Interface::Session* pSession)
: Job(CN_JOB_SELECTTABLE, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SELECTTABLE,_FOOTER_);
    
  m_pTable       = nullptr;
  m_iPartition   = -1;
  
  _checkInitialised_(CN_JOB_SELECTTABLE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_SELECTTABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_SelectTable::~Job_SelectTable(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SELECTTABLE,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_SelectTable::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_SelectTable* pJob_SelectTable = _pJob_SelectTable(pObject);

  pJob_SelectTable->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_SelectTable::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SELECTTABLE,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("select"_Stz));
  
  // get number of items in token vector
  iNumTokens = pTokenVec->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens < 3)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the table name
  m_sObjectName = pTokenVec->pGet(1)->aText();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the partition number
  m_iPartition = pTokenVec->pGet(2)->iToI8();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_SelectTable::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SELECTTABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_SelectTable::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SELECTTABLE);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobModel::pJob_SelectTable_pre(Interface::Session* pSession)
{
  Job_SelectTable* pJob_SelectTable = new Job_SelectTable(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommandProcessor->pTknVec();
  
  bool bResult = pJob_SelectTable->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_SelectTable->m_bHasError = true;
    pJob_SelectTable->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_SelectTable;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_SelectTable_post(Interface::Session* pSession, Job* pJob)
{
  Job_SelectTable* pJob_SelectTable = _pJob_SelectTable(pJob);
  
  if ( pJob_SelectTable->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_SelectTable->m_xNode[i_Node].m_eErrorCode]);
    }
   
    return;
  }
  
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_SelectTable_delete(Interface::Session* pSession, Job* pJob)
{
  Job_SelectTable* pJob_SelectTable = _pJob_SelectTable(pJob);
  
  delete pJob_SelectTable;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_SelectTable_show(Job* pJob)
{
  Job_SelectTable* pJob_SelectTable = _pJob_SelectTable(pJob);

  pJob_SelectTable->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
