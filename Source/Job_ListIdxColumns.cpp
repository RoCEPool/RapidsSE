/*============================================================================*/
/* Job_ListIdxColumns.cpp -- List Index Columns Job                           */
/*============================================================================*/

// Job Short: ListIdxColumns

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_ListIdxColumns =
{
  CN_JOB_LISTIDXCOLUMNS,     // class enum
  JK_LOCAL,                  // job kind
  "list index columns"_Stz,  // comment about the job
  "lsidxcol"_Stz,            // the prompt command
  nullptr,                   // the second word
  nullptr,                   // the _run task seed
  "listIdxColumns_req"_Stz,  // the _req task seed
  nullptr,                   // the _rcv task seed
  nullptr,                   // the _exe task seed
  nullptr,                   // the _end task seed
  nullptr,                   // the _can task seed
  false,                     // check resources?
  false,                     // detached job?
  false,                     // buffer attached?
  JT_NONE,                   // job trace
  DL_GRAB_READ,              // ddl lock model
  PY_NORMAL,                 // priority
  JF_NONE,                   // flags

  eJob_checkCluster,         // prompt check function
  pJob_ListIdxColumns_pre,   // prompt pre-process function
  vJob_ListIdxColumns_post,  // prompt post-process function
  nullptr,                   // job new function
  vJob_ListIdxColumns_delete,// job delete function
  nullptr,                   // job pre-send function
  nullptr,                   // job post-receive function
  vJob_ListIdxColumns_show,  // job show function
  
  // help and example
  true,
  "lsidxcol <tableNm> <indexNm>"_Stz,
  "lsidxcol supplier idx1;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_ListIdxColumns_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_ListIndexes_class;
  
  return &xJobClass_ListIdxColumns;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_ListIdxColumns* Jobs::_pJob_ListIdxColumns(void* aAddress)
{
  Job_ListIdxColumns* pJob_ListIdxColumns = reinterpret_cast<Job_ListIdxColumns*>(aAddress);
  
  RASSERT_TYPE(pJob_ListIdxColumns,CN_JOB_LISTIDXCOLUMNS);
  
  return pJob_ListIdxColumns;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_ListIdxColumns::operator new(size_t)
{
  Job_ListIdxColumns* pJob_ListIdxColumns =
    static_cast<Job_ListIdxColumns*>(HeapMem::pAllocate(CN_JOB_LISTIDXCOLUMNS));
  
  return pJob_ListIdxColumns;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_ListIdxColumns::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_ListIdxColumns::operator delete(void* aAddress)
{
  Job_ListIdxColumns* pJob_ListIdxColumns = static_cast<Job_ListIdxColumns*>(aAddress);
  
  HeapMem::vFree(pJob_ListIdxColumns);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_ListIdxColumns::Job_ListIdxColumns(Interface::Session* pSession)
: Job(CN_JOB_LISTIDXCOLUMNS, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTIDXCOLUMNS,_FOOTER_);
  
  _checkInitialised_(CN_JOB_LISTIDXCOLUMNS);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTIDXCOLUMNS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_ListIdxColumns::~Job_ListIdxColumns(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTIDXCOLUMNS,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_ListIdxColumns::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_ListIdxColumns* pJob_ListIdxColumns = _pJob_ListIdxColumns(pObject);

  pJob_ListIdxColumns->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_ListIdxColumns::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTIDXCOLUMNS,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("lsidxcol"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - -
  // get no of tokens in token vector
  iNumTokens = pTokenVec->nTokens();;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3) {
    return nullptr;
  }
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the table name
  m_sObjectName = pTokenVec->pGet(1)->aText();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the index name
  m_sIndexName = pTokenVec->pGet(2)->aText();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_ListIdxColumns::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTIDXCOLUMNS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_ListIdxColumns::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTIDXCOLUMNS);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_ListIdxColumns_pre(Interface::Session* pSession)
{
  Job_ListIdxColumns* pJob_ListIdxColumns = new Job_ListIdxColumns(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommand->pTknVec();
  
  bool bResult = pJob_ListIdxColumns->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_ListIdxColumns->m_bHasError = true;
    pJob_ListIdxColumns->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_ListIdxColumns;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_ListIdxColumns_post(Interface::Session* pSession, Job* pJob)
{
  Job_ListIdxColumns* pJob_ListIdxColumns = _pJob_ListIdxColumns(pJob);
  
  if ( pJob_ListIdxColumns->m_bHasError ) {
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_ListIdxColumns->m_xNode[i_Node].m_eErrorCode]);
    }
  }
  else {
    if (gpAttachMap->m_bMetadata) {
      vTrace("metadata: %s\n"_Stz, pJob_ListIdxColumns->m_sMetadata.aStz());
    }
    
    i8 iRowsProcessed = 0;
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("%s"_Stz, pJob_ListIdxColumns->m_xNode[i_Node].m_sResponse.aChar());
      iRowsProcessed += *pJob_ListIdxColumns->m_xNode[i_Node].m_ixRowsProcessed;
    }
    vTrace("success: %lld rows affected\n"_Stz, iRowsProcessed);
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_ListIdxColumns_delete(Interface::Session* pSession, Job* pJob)
{
  Job_ListIdxColumns* pJob_ListIdxColumns = _pJob_ListIdxColumns(pJob);
  
  delete pJob_ListIdxColumns;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_ListIdxColumns_show(Job* pJob)
{
  Job_ListIdxColumns* pJob_ListIdxColumns = _pJob_ListIdxColumns(pJob);

  pJob_ListIdxColumns->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
