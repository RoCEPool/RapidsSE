/*============================================================================*/
/* Job_ListCatalogs.cpp -- List Catalogs Job                                  */
/*============================================================================*/

// Job Short: ListCatalogs

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_ListCatalogs =
{
  CN_JOB_LISTCATALOGS,     // class enum
  JK_LOCAL,                // job kind
  RK_NONE,                 // response kind
  "list catalogs"_Stz,     // comment about the job
  "lscat"_Stz,             // the prompt command
  nullptr,                 // the second word
  nullptr,                 // the _run task seed
  "listCatalogs_req"_Stz,  // the _req task seed
  nullptr,                 // the _rcv task seed
  nullptr,                 // the _exe task seed
  nullptr,                 // the _end task seed
  nullptr,                 // the _can task seed
  false,                   // check resources?
  false,                   // detached job?
  false,                   // buffer attached?
  JT_NONE,                 // job trace
  DL_GRAB_READ,            // ddl lock model
  PY_NORMAL,               // priority
  JF_NONE,                 // flags

  nullptr,                 // prompt check function
  pJob_ListCatalogs_pre,   // prompt pre-process function
  vJob_ListCatalogs_post,  // prompt post-process function
  nullptr,                 // job new function
  vJob_ListCatalogs_delete,// job delete function
  nullptr,                 // job pre-send function
  nullptr,                 // job post-receive function
  vJob_ListCatalogs_show,  // job show function
  
  // help and example
  true,
  "lscat"_Stz,
  "lscat;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_ListCatalogs_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_ListColumns_class;
  
  return &xJobClass_ListCatalogs;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_ListCatalogs* Jobs::_pJob_ListCatalogs(void* aAddress)
{
  Job_ListCatalogs* pJob_ListCatalogs = reinterpret_cast<Job_ListCatalogs*>(aAddress);
  
  RASSERT_TYPE(pJob_ListCatalogs,CN_JOB_LISTCATALOGS);
  
  return pJob_ListCatalogs;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_ListCatalogs::operator new(size_t)
{
  Job_ListCatalogs* pJob_ListCatalogs =
    static_cast<Job_ListCatalogs*>(HeapMem::pAllocate(CN_JOB_LISTCATALOGS));
  
  return pJob_ListCatalogs;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_ListCatalogs::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_ListCatalogs::operator delete(void* aAddress)
{
  Job_ListCatalogs* pJob_ListCatalogs = static_cast<Job_ListCatalogs*>(aAddress);
  
  HeapMem::vFree(pJob_ListCatalogs);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_ListCatalogs::Job_ListCatalogs(Interface::Session* pSession)
: Job(CN_JOB_LISTCATALOGS, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTCATALOGS,_FOOTER_);
  
  _checkInitialised_(CN_JOB_LISTCATALOGS);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTCATALOGS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_ListCatalogs::~Job_ListCatalogs(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTCATALOGS,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_ListCatalogs::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_ListCatalogs* pJob_ListCatalogs = _pJob_ListCatalogs(pObject);

  pJob_ListCatalogs->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_ListCatalogs::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTCATALOGS,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("lscat"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - -
  // get no of tokens in token vector
  iNumTokens = pTokenVec->nTokens();;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 1 tokens
  if (iNumTokens != 1) {
    return nullptr;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_ListCatalogs::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTCATALOGS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_ListCatalogs::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTCATALOGS);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_ListCatalogs_pre(Interface::Session* pSession)
{
  Job_ListCatalogs* pJob_ListCatalogs = new Job_ListCatalogs(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommand->pTknVec();
  
  bool bResult = pJob_ListCatalogs->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    pJob_ListCatalogs->vAddError(EC_INVALID_COMMAND);
  }
  
  return pJob_ListCatalogs;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_ListCatalogs_post(Interface::Session* pSession, Job* pJob)
{
  Job_ListCatalogs* pJob_ListCatalogs = _pJob_ListCatalogs(pJob);
  
  if ( pJob_ListCatalogs->m_bHasError ) {
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_ListCatalogs->m_xNode[i_Node].m_eErrorCode]);
    }
  }
  else {
    if (gpAttachMap->m_bMetadata) {
      vTrace("metadata: %s\n"_Stz, pJob_ListCatalogs->m_sMetadata.aStz());
    }
    
    i8 iRowsProcessed = 0;
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("%s"_Stz, pJob_ListCatalogs->m_xNode[i_Node].m_sResponse.aChar());
      iRowsProcessed += *pJob_ListCatalogs->m_xNode[i_Node].m_ixRowsProcessed;
    }
    if (gpAttachMap->m_bMetadata) {
      vTrace("success: %lld rows affected\n"_Stz, iRowsProcessed);
    }
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_ListCatalogs_delete(Interface::Session* pSession, Job* pJob)
{
  Job_ListCatalogs* pJob_ListCatalogs = _pJob_ListCatalogs(pJob);
  
  delete pJob_ListCatalogs;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_ListCatalogs_show(Job* pJob)
{
  Job_ListCatalogs* pJob_ListCatalogs = _pJob_ListCatalogs(pJob);

  pJob_ListCatalogs->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
