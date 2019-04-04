/*============================================================================*/
/* Job_VisualizeTable.cpp -- visualize index Job                              */
/*============================================================================*/

// Job Short: VisualizeIndex

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_VisualizeTable =
{
  CN_JOB_VISUALIZETABLE,      // class enum
  JK_LOCAL,                   // job kind
  RK_NONE,                    // response kind
  "visualize table"_Stz,      // comment about the job
  "vistbl"_Stz,               // the prompt command
  nullptr,                    // the second word
  nullptr,                    // the _run task seed
  "visualizeTable_req"_Stz,   // the _req task seed
  nullptr,                    // the _rcv task seed
  nullptr,                    // the _exe task seed
  nullptr,                    // the _end task seed
  nullptr,                    // the _can task seed
  true,                       // check resources?
  false,                      // detached job?
  false,                      // buffer attached?
  JT_NONE,                    // job trace
  DL_GRAB_READ,               // ddl lock model
  PY_NORMAL,                  // priority
  JF_NONE,                    // flags

  eJob_checkCluster,          // prompt check function
  pJob_VisualizeTable_pre,    // prompt pre-process function
  vJob_VisualizeTable_post,   // prompt post-process function
  nullptr,                    // job new function
  vJob_VisualizeTable_delete, // job delete function
  nullptr,                    // job pre-send function
  nullptr,                    // job post-receive function
  vJob_VisualizeTable_show,   // job show function
  
  // help and example
  true,
  "vistbl <tableNm>"_Stz,
  "vistbl SUPPLIER"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_VisualizeTable_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_WaitCluster_class;
  
  return &xJobClass_VisualizeTable;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_VisualizeTable* Jobs::_pJob_VisualizeTable(void* aAddress)
{
  Job_VisualizeTable* pJob_VisualizeTable = reinterpret_cast<Job_VisualizeTable*>(aAddress);
  
  RASSERT_TYPE(pJob_VisualizeTable,CN_JOB_VISUALIZETABLE);
  
  return pJob_VisualizeTable;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_VisualizeTable::operator new(size_t)
{
  Job_VisualizeTable* pJob_VisualizeTable =
    static_cast<Job_VisualizeTable*>(HeapMem::pAllocate(CN_JOB_VISUALIZETABLE));
  
  return pJob_VisualizeTable;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_VisualizeTable::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_VisualizeTable::operator delete(void* aAddress)
{
  Job_VisualizeTable* pJob_VisualizeTable = static_cast<Job_VisualizeTable*>(aAddress);
  
  HeapMem::vFree(pJob_VisualizeTable);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_VisualizeTable::Job_VisualizeTable(Interface::Session* pSession)
: Job(CN_JOB_VISUALIZETABLE, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_VISUALIZETABLE,_FOOTER_);
    
  _checkInitialised_(CN_JOB_VISUALIZETABLE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_VISUALIZETABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_VisualizeTable::~Job_VisualizeTable(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_VISUALIZETABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_VisualizeTable::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_VisualizeTable* pJob_VisualizeTable = _pJob_VisualizeTable(pObject);

  pJob_VisualizeTable->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_VisualizeTable::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_VISUALIZETABLE,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("vistbl"_Stz));
  
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
void Job_VisualizeTable::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_VISUALIZETABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_VisualizeTable::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_VISUALIZETABLE);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_VisualizeTable_pre(Interface::Session* pSession)
{
  Job_VisualizeTable* pJob_VisualizeTable = new Job_VisualizeTable(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommand->pTknVec();
  
  bool bResult = pJob_VisualizeTable->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    pJob_VisualizeTable->vAddError(EC_INVALID_COMMAND);
  }
  
  return pJob_VisualizeTable;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_VisualizeTable_post(Interface::Session* pSession, Job* pJob)
{
  Job_VisualizeTable* pJob_VisualizeTable = _pJob_VisualizeTable(pJob);
  
  if ( pJob_VisualizeTable->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_VisualizeTable->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_VisualizeTable_delete(Interface::Session* pSession, Job* pJob)
{
  Job_VisualizeTable* pJob_VisualizeTable = _pJob_VisualizeTable(pJob);
  
  delete pJob_VisualizeTable;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_VisualizeTable_show(Job* pJob)
{
  Job_VisualizeTable* pJob_VisualizeTable = _pJob_VisualizeTable(pJob);

  pJob_VisualizeTable->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
