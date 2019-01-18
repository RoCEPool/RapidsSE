/*============================================================================*/
/* Job_TruncateTable.cpp -- Truncate Table Job                                */
/*============================================================================*/

// Job Short: TruncateTable

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_TruncateTable =
{
  "Job_TruncateTable"_Stz,   // name of the job class
  CN_JOB_TRUNCATETABLE,      // class enum
  JK_DIST2PC,                // job kind
  "truncate table"_Stz,      // comment about the job
  "tr"_Stz,                  // the prompt command
  nullptr,                   // the second word
  nullptr,                   // the _run task seed
  "truncateTable_req"_Stz,   // the _req task seed
  "truncateTable_beg"_Stz,   // the _rcv task seed
  "truncateTable_exe"_Stz,   // the _exe task seed
  "truncateTable_end"_Stz,   // the _end task seed
  "truncateTable_can"_Stz,   // the _can task seed
  true,                      // check resources?
  false,                     // detached job?
  false,                     // buffer attached?
  JT_NONE,                   // job trace
  pJob_TruncateTable_pre,    // prompt pre-process function
  vJob_TruncateTable_post,   // prompt post-process function
  nullptr,                   // job new function
  vJob_TruncateTable_delete, // job delete function
  vJob_send,                 // job pre-send function
  vJob_recv,                 // job post-receive function
  vJob_TruncateTable_show,   // job show function
  
  // help and example
  true,
  "tr <tableName>"_Stz,
  "tr SUPPLIER;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_TruncateTable_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_UseSchema_class;
  
  return &xJobClass_TruncateTable;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_TruncateTable* Jobs::_pJob_TruncateTable(void* aAddress)
{
  Job_TruncateTable* pJob_TruncateTable = reinterpret_cast<Job_TruncateTable*>(aAddress);
  
  RASSERT_TYPE(pJob_TruncateTable,CN_JOB_TRUNCATETABLE);
  
  return pJob_TruncateTable;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_TruncateTable::operator new(size_t)
{
  Job_TruncateTable* pJob_TruncateTable =
    static_cast<Job_TruncateTable*>(HeapMem::pAllocate(CN_JOB_TRUNCATETABLE));
  
  return pJob_TruncateTable;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_TruncateTable::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_TruncateTable::operator delete(void* aAddress)
{
  Job_TruncateTable* pJob_TruncateTable = static_cast<Job_TruncateTable*>(aAddress);
  
  HeapMem::vFree(pJob_TruncateTable);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_TruncateTable::Job_TruncateTable(Interface::Session* pSession)
: Job(CN_JOB_TRUNCATETABLE, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_TRUNCATETABLE,_FOOTER_);
  
  _checkInitialised_(CN_JOB_TRUNCATETABLE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_TRUNCATETABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_TruncateTable::~Job_TruncateTable(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_TRUNCATETABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_TruncateTable::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_TruncateTable* pJob_TruncateTable = _pJob_TruncateTable(pObject);

  pJob_TruncateTable->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_TruncateTable::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_TRUNCATETABLE,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("tr"_Stz));
  
  // get number of items in token vector
  iNumTokens = pTokenVec->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 2 tokens
  if (iNumTokens != 2)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the table name
  m_sObjectName = pTokenVec->pGet(1)->aText();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_TruncateTable::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_TRUNCATETABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_TruncateTable::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_TRUNCATETABLE);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_TruncateTable_pre(Interface::Session* pSession)
{
  Job_TruncateTable* pJob_TruncateTable = new Job_TruncateTable(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommandProcessor->pTknVec();
  
  bool bResult = pJob_TruncateTable->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_TruncateTable->m_bHasError = true;
    pJob_TruncateTable->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_TruncateTable;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_TruncateTable_post(Interface::Session* pSession, Job* pJob)
{
  Job_TruncateTable* pJob_TruncateTable = _pJob_TruncateTable(pJob);
  
  if ( pJob_TruncateTable->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_TruncateTable->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
  else {
    vTrace("success: 0 rows affected\n"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_TruncateTable_delete(Interface::Session* pSession, Job* pJob)
{
  Job_TruncateTable* pJob_TruncateTable = _pJob_TruncateTable(pJob);
  
  delete pJob_TruncateTable;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_TruncateTable_show(Job* pJob)
{
  Job_TruncateTable* pJob_TruncateTable = _pJob_TruncateTable(pJob);

  pJob_TruncateTable->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
