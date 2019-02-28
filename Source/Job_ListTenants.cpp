/*============================================================================*/
/* Job_ListTenants.cpp -- List Tenants Job                                    */
/*============================================================================*/

// Job Short: ListTenants

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_ListTenants =
{
  "Job_ListTenants"_Stz,  // name of the job class
  CN_JOB_LISTTENANTS,     // class enum
  JK_LOCAL,               // job kind
  "list tenants"_Stz,     // comment about the job
  "lsten"_Stz,            // the prompt command
  nullptr,                // the second word
  nullptr,                // the _run task seed
  "listTenants_req"_Stz,  // the _req task seed
  nullptr,                // the _rcv task seed
  nullptr,                // the _exe task seed
  nullptr,                // the _end task seed
  nullptr,                // the _can task seed
  false,                  // check resources?
  false,                  // detached job?
  false,                  // buffer attached?
  JT_NONE,                // job trace
  DL_NONE,                // ddl lock model

  nullptr,                // prompt check function
  pJob_ListTenants_pre,   // prompt pre-process function
  vJob_ListTenants_post,  // prompt post-process function
  nullptr,                // job new function
  vJob_ListTenants_delete,// job delete function
  nullptr,                // job pre-send function
  nullptr,                // job post-receive function
  vJob_ListTenants_show,  // job show function
  
  // help and example
  true,
  "lsten"_Stz,
  "lsten;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_ListTenants_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_LoadTable_class;
  
  return &xJobClass_ListTenants;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_ListTenants* Jobs::_pJob_ListTenants(void* aAddress)
{
  Job_ListTenants* pJob_ListTenants = reinterpret_cast<Job_ListTenants*>(aAddress);
  
  RASSERT_TYPE(pJob_ListTenants,CN_JOB_LISTTENANTS);
  
  return pJob_ListTenants;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_ListTenants::operator new(size_t)
{
  Job_ListTenants* pJob_ListTenants =
    static_cast<Job_ListTenants*>(HeapMem::pAllocate(CN_JOB_LISTTENANTS));
  
  return pJob_ListTenants;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_ListTenants::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_ListTenants::operator delete(void* aAddress)
{
  Job_ListTenants* pJob_ListColumns = static_cast<Job_ListTenants*>(aAddress);
  
  HeapMem::vFree(pJob_ListColumns);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_ListTenants::Job_ListTenants(Interface::Session* pSession)
: Job(CN_JOB_LISTTENANTS, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTTENANTS,_FOOTER_);
  
   _checkInitialised_(CN_JOB_LISTTENANTS);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTTENANTS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_ListTenants::~Job_ListTenants(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTTENANTS,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_ListTenants::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_ListTenants* pJob_ListTenants = _pJob_ListTenants(pObject);

  pJob_ListTenants->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_ListTenants::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTTENANTS,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("lsten"_Stz));
  
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
void Job_ListTenants::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTTENANTS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_ListTenants::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTTENANTS);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_ListTenants_pre(Interface::Session* pSession)
{
  Job_ListTenants* pJob_ListTenants = new Job_ListTenants(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommand->pTknVec();
  
  bool bResult = pJob_ListTenants->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_ListTenants->m_bHasError = true;
    pJob_ListTenants->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_ListTenants;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_ListTenants_post(Interface::Session* pSession, Job* pJob)
{
  Job_ListTenants* pJob_ListTenants = _pJob_ListTenants(pJob);
  
  if ( pJob_ListTenants->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_ListTenants->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
  else {
    i8 iRowsProcessed = 0;
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("%s"_Stz, pJob_ListTenants->m_xNode[i_Node].m_sResponse.aChar());
      iRowsProcessed += *pJob_ListTenants->m_xNode[i_Node].m_ixRowsProcessed;
    }
    vTrace("success: %lld rows affected\n"_Stz, iRowsProcessed);
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_ListTenants_delete(Interface::Session* pSession, Job* pJob)
{
  Job_ListTenants* pJob_ListTenants = _pJob_ListTenants(pJob);
  
  delete pJob_ListTenants;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_ListTenants_show(Job* pJob)
{
  Job_ListTenants* pJob_ListTenants = _pJob_ListTenants(pJob);

  pJob_ListTenants->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
