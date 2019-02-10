/*============================================================================*/
/* Job_ListSchemas.cpp -- List Schemas Job                                    */
/*============================================================================*/

// Job Short: ListSchemas

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_ListSchemas =
{
  "Job_ListSchemas"_Stz,  // name of the job class
  CN_JOB_LISTSCHEMAS,     // class enum
  JK_LOCAL,               // job kind
  "list schemas"_Stz,     // comment about the job
  "lssch"_Stz,            // the prompt command
  nullptr,                // the second word
  nullptr,                // the _run task seed
  "listSchemas_req"_Stz,  // the _req task seed
  nullptr,                // the _rcv task seed
  nullptr,                // the _exe task seed
  nullptr,                // the _end task seed
  nullptr,                // the _can task seed
  false,                  // check resources?
  false,                  // detached job?
  false,                  // buffer attached?
  JT_NONE,                // job trace
  
  eJob_checkCluster,      // prompt check function
  pJob_ListSchemas_pre,   // prompt pre-process function
  vJob_ListSchemas_post,  // prompt post-process function
  nullptr,                // job new function
  vJob_ListSchemas_delete,// job delete function
  nullptr,                // job pre-send function
  nullptr,                // job post-receive function
  vJob_ListSchemas_show,  // job show function
  
  // help and example
  true,
  "lssch"_Stz,
  "lssch;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_ListSchemas_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_ListTables_class;
  
  return &xJobClass_ListSchemas;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_ListSchemas* Jobs::_pJob_ListSchemas(void* aAddress)
{
  Job_ListSchemas* pJob_ListSchemas = reinterpret_cast<Job_ListSchemas*>(aAddress);
  
  RASSERT_TYPE(pJob_ListSchemas,CN_JOB_LISTSCHEMAS);
  
  return pJob_ListSchemas;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_ListSchemas::operator new(size_t)
{
  Job_ListSchemas* pJob_ListSchemas =
    static_cast<Job_ListSchemas*>(HeapMem::pAllocate(CN_JOB_LISTSCHEMAS));
  
  return pJob_ListSchemas;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_ListSchemas::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_ListSchemas::operator delete(void* aAddress)
{
  Job_ListSchemas* pJob_ListSchemas = static_cast<Job_ListSchemas*>(aAddress);
  
  HeapMem::vFree(pJob_ListSchemas);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_ListSchemas::Job_ListSchemas(Interface::Session* pSession)
: Job(CN_JOB_LISTSCHEMAS, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTSCHEMAS,_FOOTER_);
  
  _checkInitialised_(CN_JOB_LISTSCHEMAS);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTSCHEMAS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_ListSchemas::~Job_ListSchemas(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTSCHEMAS,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_ListSchemas::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_ListSchemas* pJob_ListSchemas = _pJob_ListSchemas(pObject);

  pJob_ListSchemas->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_ListSchemas::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTSCHEMAS,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("lssch"_Stz));
  
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
void Job_ListSchemas::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTSCHEMAS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_ListSchemas::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTSCHEMAS);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_ListSchemas_pre(Interface::Session* pSession)
{
  Job_ListSchemas* pJob_ListSchemas = new Job_ListSchemas(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommand->pTknVec();
  
  bool bResult = pJob_ListSchemas->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_ListSchemas->m_bHasError = true;
    pJob_ListSchemas->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_ListSchemas;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_ListSchemas_post(Interface::Session* pSession, Job* pJob)
{
  Job_ListSchemas* pJob_ListSchemas = _pJob_ListSchemas(pJob);
  
  if ( pJob_ListSchemas->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_ListSchemas->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
  else {
    i8 iRowsProcessed = 0;
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("%s"_Stz, pJob_ListSchemas->m_xNode[i_Node].m_sResponse.aChar());
      iRowsProcessed += *pJob_ListSchemas->m_xNode[i_Node].m_ixRowsProcessed;
    }
    vTrace("success: %lld rows affected\n"_Stz, iRowsProcessed);
  }
  
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_ListSchemas_delete(Interface::Session* pSession, Job* pJob)
{
  Job_ListSchemas* pJob_ListSchemas = _pJob_ListSchemas(pJob);

  delete pJob_ListSchemas;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_ListSchemas_show(Job* pJob)
{
  Job_ListSchemas* pJob_ListSchemas = _pJob_ListSchemas(pJob);

  pJob_ListSchemas->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
