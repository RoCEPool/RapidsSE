/*============================================================================*/
/* Job_ListPartColumns.cpp -- List Partition Columns Job                      */
/*============================================================================*/

// Job Short: ListPartColumns

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_ListPartColumns =
{
  CN_JOB_LISTPARTCOLUMNS,           // class enum
  JK_LOCAL,                         // job kind
  "list partition key"_Stz,         // comment about the job
  "lspartcol"_Stz,                  // the prompt command
  nullptr,                          // the second word
  nullptr,                          // the _run task seed
  "listPartColumns_req"_Stz,        // the _req task seed
  nullptr,                          // the _rcv task seed
  nullptr,                          // the _exe task seed
  nullptr,                          // the _end task seed
  nullptr,                          // the _can task seed
  false,                            // check resources?
  false,                            // detached job?
  false,                            // buffer attached?
  JT_NONE,                          // job trace
  DL_GRAB_READ,                     // ddl lock model
  PY_NORMAL,                        // priority
  JF_NONE,                          // flags

  eJob_checkCluster,                // prompt check function
  pJob_ListPartColumns_pre,         // prompt pre-process function
  vJob_ListPartColumns_post,        // prompt post-process function
  nullptr,                          // job new function
  vJob_ListPartColumns_delete,      // job delete function
  nullptr,                          // job pre-send function
  nullptr,                          // job post-receive function
  vJob_ListPartColumns_show,        // job show function
  
  // help and example
  true,
  "lspartcol <tableName>"_Stz,
  "lspartcol SUPPLIER;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_ListPartColumns_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_ListPartitions_class;
  
  return &xJobClass_ListPartColumns;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_ListPartColumns* Jobs::_pJob_ListPartColumns(void* aAddress)
{
  Job_ListPartColumns* pJob_ListPartColumns = reinterpret_cast<Job_ListPartColumns*>(aAddress);
  
  RASSERT_TYPE(pJob_ListPartColumns,CN_JOB_LISTPARTCOLUMNS);
  
  return pJob_ListPartColumns;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_ListPartColumns::operator new(size_t)
{
  Job_ListPartColumns* pJob_ListPartColumns =
    static_cast<Job_ListPartColumns*>(HeapMem::pAllocate(CN_JOB_LISTPARTCOLUMNS));
  
  return pJob_ListPartColumns;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_ListPartColumns::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_ListPartColumns::operator delete(void* aAddress)
{
  Job_ListPartColumns* pJob_ListPartColumns = static_cast<Job_ListPartColumns*>(aAddress);
  
  HeapMem::vFree(pJob_ListPartColumns);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_ListPartColumns::Job_ListPartColumns(Interface::Session* pSession)
: Job(CN_JOB_LISTPARTCOLUMNS, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTPARTCOLUMNS,_FOOTER_);
  
  _checkInitialised_(CN_JOB_LISTPARTCOLUMNS);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTPARTCOLUMNS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_ListPartColumns::~Job_ListPartColumns(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTPARTCOLUMNS,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_ListPartColumns::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_ListPartColumns* pJob_ListPartColumns = _pJob_ListPartColumns(pObject);

  pJob_ListPartColumns->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_ListPartColumns::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTPARTCOLUMNS,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("lspartcol"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - -
  // get no of tokens in token vector
  iNumTokens = pTokenVec->nTokens();;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 2 tokens
  if (iNumTokens != 2) {
    return nullptr;
  }
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the table name
  m_sObjectName = pTokenVec->pGet(1)->aText();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_ListPartColumns::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTPARTCOLUMNS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_ListPartColumns::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTPARTCOLUMNS);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_ListPartColumns_pre(Interface::Session* pSession)
{
  Job_ListPartColumns* pJob_ListPartColumns = new Job_ListPartColumns(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommand->pTknVec();
  
  bool bResult = pJob_ListPartColumns->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_ListPartColumns->m_bHasError = true;
    pJob_ListPartColumns->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_ListPartColumns;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_ListPartColumns_post(Interface::Session* pSession, Job* pJob)
{
  Job_ListPartColumns* pJob_ListPartColumns = _pJob_ListPartColumns(pJob);
  
  if ( pJob_ListPartColumns->m_bHasError ) {
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_ListPartColumns->m_xNode[i_Node].m_eErrorCode]);
    }
  }
  else {
    if (gpAttachMap->m_bMetadata) {
      vTrace("metadata: %s\n"_Stz, pJob_ListPartColumns->m_sMetadata.aStz());
    }
    
    i8 iRowsProcessed = 0;
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("%s"_Stz, pJob_ListPartColumns->m_xNode[i_Node].m_sResponse.aChar());
      iRowsProcessed += *pJob_ListPartColumns->m_xNode[i_Node].m_ixRowsProcessed;
    }
    vTrace("success: %lld rows affected\n"_Stz, iRowsProcessed);
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_ListPartColumns_delete(Interface::Session* pSession, Job* pJob)
{
  Job_ListPartColumns* pJob_ListPartColumns = _pJob_ListPartColumns(pJob);
  
  delete pJob_ListPartColumns;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_ListPartColumns_show(Job* pJob)
{
  Job_ListPartColumns* pJob_ListPartColumns = _pJob_ListPartColumns(pJob);

  pJob_ListPartColumns->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
