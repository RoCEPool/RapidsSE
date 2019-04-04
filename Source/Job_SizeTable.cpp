/*============================================================================*/
/* Job_SizeTable.cpp -- size table Job                                        */
/*============================================================================*/

// Job Short: SizeTable

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_SizeTable =
{
  CN_JOB_SIZETABLE,      // class enum
  JK_DIST2PC,            // job kind
  "size of table"_Stz,   // comment about the job
  "szTbl"_Stz,           // the prompt command
  nullptr,               // the second word
  nullptr,               // the _run task seed
  "sizeTable_req"_Stz,   // the _req task seed
  "sizeTable_beg"_Stz,   // the _rcv task seed
  "sizeTable_exe"_Stz,   // the _exe task seed
  "sizeTable_end"_Stz,   // the _end task seed
  "sizeTable_can"_Stz,   // the _can task seed
  true,                  // check resources?
  false,                 // detached job?
  false,                 // buffer attached?
  JT_NONE,               // job trace
  DL_GRAB_READ,          // ddl lock model
  PY_NORMAL,             // priority
  JF_NONE,               // flags

  eJob_checkCluster,     // prompt check function
  pJob_SizeTable_pre,    // prompt pre-process function
  vJob_SizeTable_post,   // prompt post-process function
  nullptr,               // job new function
  vJob_SizeTable_delete, // job delete function
  vJob_send,             // job pre-send function
  vJob_recv,             // job post-receive function
  vJob_SizeTable_show,   // job show function
  
  // help and example
  true,
  "sztbl <tableNm>"_Stz,
  "sztbl SUPPLIER;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_SizeTable_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Space_class;
  
  return &xJobClass_SizeTable;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_SizeTable* Jobs::_pJob_SizeTable(void* aAddress)
{
  Job_SizeTable* pJob_SizeTable = reinterpret_cast<Job_SizeTable*>(aAddress);
  
  RASSERT_TYPE(pJob_SizeTable,CN_JOB_SIZETABLE);
  
  return pJob_SizeTable;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_SizeTable::operator new(size_t)
{
  Job_SizeTable* pJob_SizeTable =
    static_cast<Job_SizeTable*>(HeapMem::pAllocate(CN_JOB_SIZETABLE));
  
  return pJob_SizeTable;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_SizeTable::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_SizeTable::operator delete(void* aAddress)
{
  Job_SizeTable* pJob_SizeTable = static_cast<Job_SizeTable*>(aAddress);
  
  HeapMem::vFree(pJob_SizeTable);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_SizeTable::Job_SizeTable(Interface::Session* pSession)
: Job(CN_JOB_SIZETABLE, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SIZETABLE,_FOOTER_);
    
  _checkInitialised_(CN_JOB_SIZETABLE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_SIZETABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_SizeTable::~Job_SizeTable(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SIZETABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_SizeTable::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_SizeTable* pJob_SizeTable = _pJob_SizeTable(pObject);

  pJob_SizeTable->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_SizeTable::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SIZETABLE,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("sztbl"_Stz));
  
  // get number of items in token vector
  iNumTokens = pTokenVec->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 2 tokens
  if (iNumTokens != 2)
    return false;
  
  // set the table name
  m_sObjectName = pTokenVec->pGet(1)->aText();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_SizeTable::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SIZETABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_SizeTable::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SIZETABLE);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_SizeTable_pre(Interface::Session* pSession)
{
  Job_SizeTable* pJob_SizeTable = new Job_SizeTable(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommand->pTknVec();
  
  bool bResult = pJob_SizeTable->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    pJob_SizeTable->vAddError(EC_INVALID_COMMAND);
  }
  
  return pJob_SizeTable;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_SizeTable_post(Interface::Session* pSession, Job* pJob)
{
  Job_SizeTable* pJob_SizeTable = _pJob_SizeTable(pJob);
  
  if ( pJob_SizeTable->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_SizeTable->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
  else {
    if (gpAttachMap->m_bMetadata) {
      vTrace("metadata: %s\n"_Stz, pJob_SizeTable->m_sMetadata.aStz());
    }
    
    i8 iRowsProcessed = 0;
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("%s\n"_Stz, pJob_SizeTable->m_xNode[i_Node].m_sResponse.aChar());
      iRowsProcessed += *pJob_SizeTable->m_xNode[i_Node].m_ixRowsProcessed;
    }
    vTrace("success: %lld rows affected\n"_Stz, iRowsProcessed);
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_SizeTable_delete(Interface::Session* pSession, Job* pJob)
{
  Job_SizeTable* pJob_SizeTable = _pJob_SizeTable(pJob);
  
  delete pJob_SizeTable;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_SizeTable_show(Job* pJob)
{
  Job_SizeTable* pJob_SizeTable = _pJob_SizeTable(pJob);

  pJob_SizeTable->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
