/*============================================================================*/
/* Job_ListColumns.cpp -- List Columns Job                                    */
/*============================================================================*/

// Job Short: ListColumns

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_ListColumns =
{
  CN_JOB_LISTCOLUMNS,     // class enum
  JK_LOCAL,               // job kind
  "list table columns"_Stz, // comment about the job
  "lscol"_Stz,            // the prompt command
  nullptr,                // the second word
  nullptr,                // the _run task seed
  "listColumns_req"_Stz,  // the _req task seed
  nullptr,                // the _rcv task seed
  nullptr,                // the _exe task seed
  nullptr,                // the _end task seed
  nullptr,                // the _can task seed
  false,                  // check resources?
  false,                  // detached job?
  false,                  // buffer attached?
  JT_NONE,                // job trace
  DL_GRAB_READ,           // ddl lock model
  PY_NORMAL,              // priority
  JF_NONE,                // flags

  eJob_checkCluster,      // prompt check function
  pJob_ListColumns_pre,   // prompt pre-process function
  vJob_ListColumns_post,  // prompt post-process function
  nullptr,                // job new function
  vJob_ListColumns_delete,// job delete function
  nullptr,                // job pre-send function
  nullptr,                // job post-receive function
  vJob_ListColumns_show,  // job show function
  
  // help and example
  true,
  "lscol <tableNm>"_Stz,
  "lscol SUPPLIER;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_ListColumns_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_ListIdxColumns_class;
  
  return &xJobClass_ListColumns;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_ListColumns* Jobs::_pJob_ListColumns(void* aAddress)
{
  Job_ListColumns* pJob_ListColumns = reinterpret_cast<Job_ListColumns*>(aAddress);
  
  RASSERT_TYPE(pJob_ListColumns,CN_JOB_LISTCOLUMNS);
  
  return pJob_ListColumns;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_ListColumns::operator new(size_t)
{
  Job_ListColumns* pJob_ListColumns =
    static_cast<Job_ListColumns*>(HeapMem::pAllocate(CN_JOB_LISTCOLUMNS));
  
  return pJob_ListColumns;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_ListColumns::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_ListColumns::operator delete(void* aAddress)
{
  Job_ListColumns* pJob_ListColumns = static_cast<Job_ListColumns*>(aAddress);
  
  HeapMem::vFree(pJob_ListColumns);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_ListColumns::Job_ListColumns(Interface::Session* pSession)
: Job(CN_JOB_LISTCOLUMNS, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTCOLUMNS,_FOOTER_);
  
  _checkInitialised_(CN_JOB_LISTCOLUMNS);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTCOLUMNS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_ListColumns::~Job_ListColumns(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTCOLUMNS,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_ListColumns::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_ListColumns* pJob_ListColumns = _pJob_ListColumns(pObject);

  pJob_ListColumns->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_ListColumns::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTCOLUMNS,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("lscol"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - -
  // get no of tokens in token vector
  iNumTokens = pTokenVec->nTokens();;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 1 tokens
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
void Job_ListColumns::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTCOLUMNS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_ListColumns::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTCOLUMNS);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_ListColumns_pre(Interface::Session* pSession)
{
  Job_ListColumns* pJob_ListColumns = new Job_ListColumns(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommand->pTknVec();
  
  bool bResult = pJob_ListColumns->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_ListColumns->m_bHasError = true;
    pJob_ListColumns->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_ListColumns;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_ListColumns_post(Interface::Session* pSession, Job* pJob)
{
  Job_ListColumns* pJob_ListColumns = _pJob_ListColumns(pJob);
  
  if ( pJob_ListColumns->m_bHasError ) {
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_ListColumns->m_xNode[i_Node].m_eErrorCode]);
    }
  }
  else {
    if (gpAttachMap->m_bMetadata) {
      vTrace("metadata: %s\n"_Stz, pJob_ListColumns->m_sMetadata.aStz());
    }
    
    i8 iRowsProcessed = 0;
    Table* pTable = (Table*)StrLib::iToI8(pJob_ListColumns->m_xNode[gpAttachMap->m_pGrid->m_iThisNode].m_sResponse.aChar());
    
    ColumnVec* pColumnVec = pTable->pColumnVec();
    i8 iColumns = pColumnVec->nItems();
    for (i8 iColumn = 0; iColumn < iColumns; iColumn++) {
      Column* pColumn = pColumnVec->pGet(iColumn);
      vTrace("row: (%lld, '%s', '%s', %lld, %lld, %s, %s)\n"_Stz,
             pColumn->iColumnNo(),
             pColumn->sColumnName().aChar(),
             aCT[pColumn->eColumnType()],
             pColumn->iColumnLength(),
             pColumn->iColumnScale(),
             pColumn->bIsNullable() ? "true"_Stz : "false"_Stz,
             pColumn->bIsPartitionKey() ? "true"_Stz : "false"_Stz
      );
      
    }
    iRowsProcessed = iColumns;
    vTrace("success: %lld rows affected\n"_Stz, iRowsProcessed);
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_ListColumns_delete(Interface::Session* pSession, Job* pJob)
{
  Job_ListColumns* pJob_ListColumns = _pJob_ListColumns(pJob);
  
  // now delete the job
  delete pJob_ListColumns;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_ListColumns_show(Job* pJob)
{
  Job_ListColumns* pJob_ListColumns = _pJob_ListColumns(pJob);

  pJob_ListColumns->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
