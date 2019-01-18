/*============================================================================*/
/* Job_CreateIndex.cpp -- Create Index Job                                    */
/*============================================================================*/

// Job Short: CreateIndex

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_CreateIndex =
{
  "Job_CreateIndex"_Stz,   // name of the job class
  CN_JOB_CREATEINDEX,      // class enum
  JK_DIST2PC,              // job kind
  "create index"_Stz,      // comment about the job
  "create"_Stz,            // the prompt command
  "index"_Stz,             // the second word
  nullptr,                 // the _run task seed
  "createIndex_req"_Stz,   // the _req task seed
  "createIndex_beg"_Stz,   // the _rcv task seed
  "createIndex_exe"_Stz,   // the _exe task seed
  "createIndex_end"_Stz,   // the _end task seed
  "createIndex_can"_Stz,   // the _can task seed
  true,                    // check resources?
  false,                   // detached job?
  false,                   // buffer attached?
  JT_NONE,                 // job trace
  pJob_CreateIndex_pre,    // prompt pre-process function
  vJob_CreateIndex_post,   // prompt post-process function
  nullptr,                 // job new function
  vJob_CreateIndex_delete, // job delete function
  vJob_send,               // job pre-send function
  vJob_recv,               // job post-receive function
  vJob_CreateIndex_show,   // job show function
  
  // help and example
  true,
  "create index <indexNm> on <tableNamem> ( <columnList> )"_Stz,
  "create index idx1 on supplier (S_SUPPKEY);"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

JobClass xJobClass_CreateIndexUnique =
{
  "Job_CreateIndex"_Stz,   // name of the job class
  CN_JOB_CREATEINDEX,      // class enum
  JK_DIST2PC,              // job kind
  "create unique index"_Stz, // comment about the job
  "create"_Stz,            // the prompt command
  "unique"_Stz,            // the second word
  nullptr,                 // the _run task seed
  "createIndex_req"_Stz,   // the _req task seed
  "createIndex_beg"_Stz,   // the _rcv task seed
  "createIndex_exe"_Stz,   // the _exe task seed
  "createIndex_end"_Stz,   // the _end task seed
  "createIndex_can"_Stz,   // the _can task seed
  true,                    // check resources?
  false,                   // detached job?
  false,                   // buffer attached?
  JT_NONE,                 // job trace
  pJob_CreateIndex_pre,    // prompt pre-process function
  vJob_CreateIndex_post,   // prompt post-process function
  nullptr,                 // job new function
  vJob_CreateIndex_delete, // job delete function
  vJob_send,               // job pre-send function
  vJob_recv,               // job post-receive function
  vJob_CreateIndex_show,   // job show function
  
  // help
  true,
  "create unique index <indexNamem> on <tableNaem> ( <columnList> )"_Stz,
  "create unique index idx1 on supplier (S_SUPPKEY);"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_CreateIndex_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_CreateIndexUnique_class;
  
  return &xJobClass_CreateIndex;
}

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_CreateIndexUnique_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_CreateSchema_class;
  
  return &xJobClass_CreateIndexUnique;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_CreateIndex* Jobs::_pJob_CreateIndex(void* aAddress)
{
  Job_CreateIndex* pJob_CreateIndex = reinterpret_cast<Job_CreateIndex*>(aAddress);
  
  RASSERT_TYPE(pJob_CreateIndex,CN_JOB_CREATEINDEX);
  
  return pJob_CreateIndex;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_CreateIndex::operator new(size_t)
{
  Job_CreateIndex* pJob_CreateIndex =
    static_cast<Job_CreateIndex*>(HeapMem::pAllocate(CN_JOB_CREATEINDEX));
  
  return pJob_CreateIndex;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_CreateIndex::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_CreateIndex::operator delete(void* aAddress)
{
  Job_CreateIndex* pJob_CreateIndex = static_cast<Job_CreateIndex*>(aAddress);
  
  HeapMem::vFree(pJob_CreateIndex);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_CreateIndex::Job_CreateIndex(Interface::Session* pSession)
: Job(CN_JOB_CREATEINDEX, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATEINDEX,_FOOTER_);
  
  m_bIsUnique  = false;
  m_iColumns   = 0;
  
  for (i8 i_Column = 0; i_Column < C_MAX_COLUMNS; i_Column++) {
    m_sColumnName[i_Column] = ""_Stz;
  }
  
  _checkInitialised_(CN_JOB_CREATEINDEX);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATEINDEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_CreateIndex::~Job_CreateIndex(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATEINDEX,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_CreateIndex::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_CreateIndex* pJob_CreateIndex = _pJob_CreateIndex(pObject);

  pJob_CreateIndex->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate table                                                             */
/*----------------------------------------------------------------------------*/
bool Job_CreateIndex::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATEINDEX,_FOOTER_);
  
  i8 iIfUnique = 0;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // assert we got a create table
  ASSERT(pTokenVec->pGet(0)->bSame("create"_Stz));
  ASSERT((pTokenVec->pGet(1)->bSame("index"_Stz)) ||
         (pTokenVec->pGet(1)->bSame("unique"_Stz) && pTokenVec->pGet(2)->bSame("index"_Stz)));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get tokens in token vector
  i8 iNumTokens = pTokenVec->nTokens();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // some validations
  bool bValid = bValidateCreateIndex(pTokenVec, iNumTokens);
  if ( ! bValid )
    return false;
  
  if (pTokenVec->pGet(1)->bSame("unique"_Stz))
    iIfUnique = 1;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the index name
  m_sIndexName = pTokenVec->pGet(2 + iIfUnique)->aText();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // uniqueness
  if (iIfUnique) {
    m_bIsUnique = true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the table name
  m_sObjectName = pTokenVec->pGet(4 + iIfUnique)->aText();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create the index columns
  
  // now collect the column names and their types
  i8 iCurrentToken = 6 + iIfUnique;
  i8 iColumn = 0;
  while (iCurrentToken < iNumTokens - 1) {
    
    if (iCurrentToken != 6 + iIfUnique && !pTokenVec->pGet(iCurrentToken++)->bSame(","_Stz)) {
      return false;
    }
    m_sColumnName[iColumn++] = pTokenVec->pGet(iCurrentToken++)->aText();
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set no of columns
  m_iColumns = iColumn;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_CreateIndex::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATEINDEX,_FOOTER_);
  
  vTrace("\n\nJob_CreateIndex: %s\n"_Stz, m_sIndexName.aChar());
  
  //vTrace("m_sExecute = %s\n"_Stz,m_sExecute.aChar());
  vTrace("TableName = %s\n"_Stz,m_sObjectName.aChar());
  vTrace("m_iColumns = %lld\n"_Stz,m_iColumns);
  
  for (i8 i_Column = 0; i_Column < m_iColumns; i_Column++) {
    vTrace("m_sColumnName[%lld] = %s\n"_Stz, i_Column, m_sColumnName[i_Column].aChar());
  }
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* validate a create index request                                            */
/*----------------------------------------------------------------------------*/
bool Job_CreateIndex::bValidateCreateIndex(TokenVec* pTokenVec, i8 iNumTokens)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATEINDEX,_FOOTER_);
  
  i8 iIfUnique = 0;
  
  if (pTokenVec->pGet(1)->bSame("unique"_Stz)) {
    iIfUnique = 1;
    if ( ! pTokenVec->pGet(2)->bSame("index"_Stz)) {
      return false;
    }
  }
  else {
    if ( ! pTokenVec->pGet(1)->bSame("index"_Stz)) {
      return false;
    }
  }
  
  // there should be a minumum of 8 tokens
  if (iNumTokens < 8 + iIfUnique) {
    return false;
  }
  
  // token[3] should be 'on'
  if (!pTokenVec->pGet(3 + iIfUnique)->bSame("on"_Stz)) {
    return false;
  }
  
  // token[5] should be '('
  if (!pTokenVec->pGet(5 + iIfUnique)->bSame("("_Stz)) {
    return false;
  }
  
  // last-1 token should be ')'
  if (!pTokenVec->pGet(iNumTokens - 1)->bSame(")"_Stz)) {
    return false;
  }
  
  return true;
}
/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_CreateIndex::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATEINDEX);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_CreateIndex_pre(Interface::Session* pSession)
{
  Job_CreateIndex* pJob_CreateIndex = new Job_CreateIndex(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommandProcessor->pTknVec();
  
  bool bResult = pJob_CreateIndex->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_CreateIndex->m_bHasError = true;
    pJob_CreateIndex->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  return pJob_CreateIndex;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_CreateIndex_post(Interface::Session* pSession, Job* pJob)
{
  Job_CreateIndex* pJob_CreateIndex = _pJob_CreateIndex(pJob);
  
  if ( pJob_CreateIndex->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_CreateIndex->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
  else {
    vTrace("success: 0 rows affected\n"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_CreateIndex_delete(Interface::Session* pSession, Job* pJob)
{
  Job_CreateIndex* pJob_CreateIndex = _pJob_CreateIndex(pJob);
  
  delete pJob_CreateIndex;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_CreateIndex_show(Job* pJob)
{
  Job_CreateIndex* pJob_CreateIndex = _pJob_CreateIndex(pJob);

  pJob_CreateIndex->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
