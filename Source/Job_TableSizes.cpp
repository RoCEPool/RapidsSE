/*============================================================================*/
/* Job_TableSizes.cpp -- Table Sizes Job                                      */
/*============================================================================*/

// Job Short: TableSizes

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_TableSizes =
{
  "Job_TableSizes"_Stz,  // name of the job class
  CN_JOB_TABLESIZES,     // class enum
  JK_DIST2PC,            // job kind
  "table sizes"_Stz,     // comment about the job
  "tableSizes"_Stz,      // the prompt command
  nullptr,               // the second word
  nullptr,               // the _run task seed
  "tableSizes_req"_Stz,  // the _req task seed
  "tableSizes_beg"_Stz,  // the _rcv task seed
  "tableSizes_exe"_Stz,  // the _exe task seed
  "tableSizes_end"_Stz,  // the _end task seed
  "tableSizes_can"_Stz,  // the _can task seed
  true,                  // check resources?
  false,                 // detached job?
  false,                 // buffer attached?
  JT_NONE,               // job trace
  pJob_TableSizes_pre,   // prompt pre-process function
  vJob_TableSizes_post,  // prompt post-process function
  nullptr,               // job new function
  vJob_TableSizes_delete,// job delete function
  vJob_send,             // job pre-send function
  vJob_recv,             // job pre-receive function
  vJob_TableSizes_show,  // job show function
  
  // help and example
  true,
  "tableSizes"_Stz,
  "tableSizes;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_TableSizes_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_TruncateTable_class;
  
  return &xJobClass_TableSizes;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_TableSizes* Jobs::_pJob_TableSizes(void* aAddress)
{
  Job_TableSizes* pJob_TableSizes = reinterpret_cast<Job_TableSizes*>(aAddress);
  
  RASSERT_TYPE(pJob_TableSizes,CN_JOB_TABLESIZES);
  
  return pJob_TableSizes;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_TableSizes::operator new(size_t)
{
  Job_TableSizes* pJob_TableSizes =
    static_cast<Job_TableSizes*>(HeapMem::pAllocate(CN_JOB_TABLESIZES));
  
  return pJob_TableSizes;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_TableSizes::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_TableSizes::operator delete(void* aAddress)
{
  Job_TableSizes* pJob_TableSizes = static_cast<Job_TableSizes*>(aAddress);
  
  HeapMem::vFree(pJob_TableSizes);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_TableSizes::Job_TableSizes(Interface::Session* pSession)
: Job(CN_JOB_TABLESIZES, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_TABLESIZES,_FOOTER_);
    
  _checkInitialised_(CN_JOB_TABLESIZES);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_TABLESIZES,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_TableSizes::~Job_TableSizes(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_TABLESIZES,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_TableSizes::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_TableSizes* pJob_TableSizes = _pJob_TableSizes(pObject);

  pJob_TableSizes->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_TableSizes::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_TABLESIZES,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("tableSizes"_Stz));
  
  // get number of items in token vector
  iNumTokens = pTokenVec->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 1 tokens
  if (iNumTokens != 1)
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_TableSizes::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_TABLESIZES,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_TableSizes::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_TABLESIZES);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_TableSizes_pre(Interface::Session* pSession)
{
  Job_TableSizes* pJob_TableSizes = new Job_TableSizes(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommand->pTknVec();
  
  bool bResult = pJob_TableSizes->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_TableSizes->m_bHasError = true;
    pJob_TableSizes->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_TableSizes;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_TableSizes_post(Interface::Session* pSession, Job* pJob)
{
  Job_TableSizes* pJob_TableSizes = _pJob_TableSizes(pJob);
  
  if ( pJob_TableSizes->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_TableSizes->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
  else {
    if (gpAttachMap->m_bMetadata) {
      vTrace("metadata: %s\n"_Stz, pJob_TableSizes->m_sMetadata.aStz());
    }
    
    i8 iRowsProcessed = 0;
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("%s\n"_Stz, pJob_TableSizes->m_xNode[i_Node].m_sResponse.aChar());
      iRowsProcessed += *pJob_TableSizes->m_xNode[i_Node].m_ziRowsProcessed;
    }
    vTrace("success: %lld rows affected\n"_Stz, iRowsProcessed);
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_TableSizes_delete(Interface::Session* pSession, Job* pJob)
{
  Job_TableSizes* pJob_TableSizes = _pJob_TableSizes(pJob);
  
  delete pJob_TableSizes;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_TableSizes_show(Job* pJob)
{
  Job_TableSizes* pJob_TableSizes = _pJob_TableSizes(pJob);

  pJob_TableSizes->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
