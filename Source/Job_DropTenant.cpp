/*============================================================================*/
/* Job_DropTenant.cpp -- Drop Tenant Job                                      */
/*============================================================================*/

// Job Short: DropTenant

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_DropTenant =
{
  "Job_DropTenant"_Stz,   // name of the job class
  CN_JOB_DROPTENANT,      // class enum
  JK_DIST2PC,             // job kind
  "drop tenant"_Stz,      // comment about the job
  "drop"_Stz,             // the prompt command
  "tenant"_Stz,           // the second word
  nullptr,                // the _run task seed
  "dropTenant_req"_Stz,   // the _req task seed
  "dropTenant_beg"_Stz,   // the _rcv task seed
  "dropTenant_exe"_Stz,   // the _exe task seed
  "dropTenant_end"_Stz,   // the _end task seed
  "dropTenant_can"_Stz,   // the _can task seed
  true,                   // check resources?
  false,                  // detached job?
  false,                  // buffer attached?
  JT_NONE,                // job trace
  DL_NONE,                // ddl lock model

  eJob_checkCluster,      // prompt check function
  pJob_DropTenant_pre,    // prompt pre-process function
  vJob_DropTenant_post,   // prompt post-process function
  nullptr,                // job new function
  vJob_DropTenant_delete, // job delete function
  vJob_send,              // job pre-send function
  vJob_recv,              // job post-receive function
  vJob_DropTenant_show,   // job show function
  
  // help and example
  true,
  "drop tenant <tenantName>"_Stz,
  "drop tenant boray;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_DropTenant_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_DumpIndex_class;
  
  return &xJobClass_DropTenant;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_DropTenant* Jobs::_pJob_DropTenant(void* aAddress)
{
  Job_DropTenant* pJob_DropTenant = reinterpret_cast<Job_DropTenant*>(aAddress);
  
  RASSERT_TYPE(pJob_DropTenant,CN_JOB_DROPTENANT);
  
  return pJob_DropTenant;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_DropTenant::operator new(size_t)
{
  Job_DropTenant* pJob_DropTenant =
    static_cast<Job_DropTenant*>(HeapMem::pAllocate(CN_JOB_DROPTENANT));
  
  return pJob_DropTenant;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_DropTenant::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_DropTenant::operator delete(void* aAddress)
{
  Job_DropTenant* pJob_DropTenant = static_cast<Job_DropTenant*>(aAddress);
  
  HeapMem::vFree(pJob_DropTenant);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_DropTenant::Job_DropTenant(Interface::Session* pSession)
: Job(CN_JOB_DROPTENANT, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPTENANT,_FOOTER_);
    
  _checkInitialised_(CN_JOB_DROPTENANT);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPTENANT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_DropTenant::~Job_DropTenant(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPTENANT,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_DropTenant::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_DropTenant* pJob_DropTenant = _pJob_DropTenant(pObject);

  pJob_DropTenant->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_DropTenant::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPTENANT,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("drop"_Stz));
  ASSERT(pTokenVec->pGet(1)->bSame("tenant"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - -
  // get number of items in token
  iNumTokens = pTokenVec->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3)
  return false;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the table name
  m_sObjectName = pTokenVec->pGet(2)->aText();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_DropTenant::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPTENANT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_DropTenant::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPTENANT);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_DropTenant_pre(Interface::Session* pSession)
{
  Job_DropTenant* pJob_DropTenant = new Job_DropTenant(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommand->pTknVec();
  
  bool bResult = pJob_DropTenant->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_DropTenant->m_bHasError = true;
    pJob_DropTenant->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_DropTenant;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_DropTenant_post(Interface::Session* pSession, Job* pJob)
{
  Job_DropTenant* pJob_DropTenant = _pJob_DropTenant(pJob);
  
  if ( pJob_DropTenant->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_DropTenant->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
  else {
    vTrace("success: 0 rows affected\n"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_DropTenant_delete(Interface::Session* pSession, Job* pJob)
{
  Job_DropTenant* pJob_DropTenant = _pJob_DropTenant(pJob);
  
  delete pJob_DropTenant;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_DropTenant_show(Job* pJob)
{
  Job_DropTenant* pJob_DropTenant = _pJob_DropTenant(pJob);

  pJob_DropTenant->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
