/*============================================================================*/
/* Job_SelectTable.cpp -- Fetch Rows Job                                        */
/*============================================================================*/

// Job Short: SelectTable

#include "JobsModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_SelectTable =
{
  "Job_SelectTable"_Stz,       // name of the job class
  CN_JOB_SELECTTABLE,          // class enum
  JK_DIST2PC,                // job kind
  "fetch rows"_Stz,          // comment about the job
  "fetch"_Stz,               // the prompt command
  nullptr,                   // the second word
  nullptr,                   // the _run task seed
  "SelectTable_req"_Stz,       // the _req task seed
  "SelectTable_beg"_Stz,       // the _rcv task seed
  "SelectTable_exe"_Stz,       // the _exe task seed
  "SelectTable_end"_Stz,       // the _end task seed
  "SelectTable_can"_Stz,       // the _can task seed
  true,                      // check resources?
  false,                     // detached job?
  false,                     // buffer attached?
  JT_NONE,                   // job trace
  pJob_SelectTable_pre,        // prompt pre-process function
  vJob_SelectTable_post,       // prompt post-process function
  nullptr,                   // job new function
  vJob_SelectTable_delete,     // job delete function
  vJob_send,                 // job pre-send function
  vJob_recv,                 // job post-receive function
  vJob_SelectTable_show,       // job show function
  
  // help and example
  true,
  "fetch <tableName> [\"<where>\"]"_Stz,
  "fetch supplier \"S_SUPPKEY 22 ==\";"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* JobsModel::pJob_SelectTable_class(pFUN** pNextFun)
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
Job_SelectTable* JobsModel::_pJob_SelectTable(void* aAddress)
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
    
  m_sWhere       = ""_Stz;
  m_pTable       = nullptr;
  m_iPartition   = -1;
  m_bDisplay     = true;
  m_pPlanner     = nullptr;
  m_pBufferQueue = new Queue;
  m_pFetchBuffer = nullptr;
  m_nLastBuffers = 0;
  m_nLastBuffersScanned = 0;
  m_pRowOnce     = nullptr;
  
  _checkInitialised_(CN_JOB_SELECTTABLE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_SELECTTABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_SelectTable::~Job_SelectTable(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SELECTTABLE,_FOOTER_);
  
  m_pBufferQueue->vFinalize();
  delete m_pBufferQueue;
  
  // in case excepton , we may not have deleted buffer
  if (m_pFetchBuffer)
    delete m_pFetchBuffer;
  
  delete m_pPlanner;
  
  if (m_pRowOnce) {
    delete m_pRowOnce;
    m_pRowOnce = nullptr;
  }
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
/* create row once object                                                     */
/*----------------------------------------------------------------------------*/
void Job_SelectTable::vCreateRowOnce(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SELECTTABLE,_FOOTER_);
  
  RASSERT(! m_pRowOnce);
  
  m_pRowOnce = new RowOnce;
}

/*----------------------------------------------------------------------------*/
/* delete last fetch buffer                                                   */
/*----------------------------------------------------------------------------*/
void Job_SelectTable::vDeleteBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SELECTTABLE,_FOOTER_);
  
  if (m_pFetchBuffer) {
    delete m_pFetchBuffer;
    m_pFetchBuffer = nullptr;
  }
  
//  if (m_pRowOnce) {
//    delete m_pRowOnce;
//    m_pRowOnce = nullptr;
//  }
}

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
  // there should be 2 tokens
  if (iNumTokens < 2)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the table name
  m_sObjectName = pTokenVec->pGet(1)->aText();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* test if row has been seen so far                                           */
/*----------------------------------------------------------------------------*/
bool Job_SelectTable::bRowSeen(Id xRowId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SELECTTABLE,_FOOTER_);
  
  if (! m_pRowOnce)
    return false;
  
  bool bSeen = m_pRowOnce->bRowSeen(xRowId);
  
  return bSeen;
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
Job* JobsModel::pJob_SelectTable_pre(Interface::Session* pSession)
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
void JobsModel::vJob_SelectTable_post(Interface::Session* pSession, Job* pJob)
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
void JobsModel::vJob_SelectTable_delete(Interface::Session* pSession, Job* pJob)
{
  Job_SelectTable* pJob_SelectTable = _pJob_SelectTable(pJob);
  
  delete pJob_SelectTable;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_SelectTable_show(Job* pJob)
{
  Job_SelectTable* pJob_SelectTable = _pJob_SelectTable(pJob);

  pJob_SelectTable->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
