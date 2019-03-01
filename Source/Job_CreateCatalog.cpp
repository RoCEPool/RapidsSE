/*============================================================================*/
/* Job_CreateCatalog.cpp -- Create Catalog Job                                */
/*============================================================================*/

// Job Short: CreateCatalog

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_CreateCatalog =
{
  "Job_CreateCatalog"_Stz,  // name of the job class
  CN_JOB_CREATECATALOG,     // class enum
  JK_DIST2PC,               // job kind
  "create catalog"_Stz,     // comment about the job
  "create"_Stz,             // the prompt command
  "catalog"_Stz,            // the second word
  nullptr,                  // the _run task seed
  "createCatalog_req"_Stz,  // the _req task seed
  "createCatalog_beg"_Stz,  // the _rcv task seed
  "createCatalog_exe"_Stz,  // the _exe task seed
  "createCatalog_end"_Stz,  // the _end task seed
  "createCatalog_can"_Stz,  // the _can task seed
  true,                     // check resources?
  false,                    // detached job?
  false,                    // buffer attached?
  JT_NONE,                  // job trace
  DL_GRAB_WRITE,            // ddl lock model
  PY_NORMAL,                // priority
  JF_NONE,                  // flags

  eJob_checkCluster,        // prompt check function
  pJob_CreateCatalog_pre,   // prompt pre-process function
  vJob_CreateCatalog_post,  // prompt post-process function
  nullptr,                  // job new function
  vJob_CreateCatalog_delete,// job delete function
  vJob_send,                // job pre-send function
  vJob_recv,                // job post-receive function
  vJob_CreateCatalog_show,  // job show function
  
  // help and example
  true,
 "create catalog <catalogNm>"_Stz,
  "create catalog tpchCatalog ;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_CreateCatalog_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_CreateIndex_class;
  
  return &xJobClass_CreateCatalog;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_CreateCatalog* Jobs::_pJob_CreateCatalog(void* aAddress)
{
  Job_CreateCatalog* pJob_CreateCatalog = reinterpret_cast<Job_CreateCatalog*>(aAddress);
  
  RASSERT_TYPE(pJob_CreateCatalog,CN_JOB_CREATECATALOG);
  
  return pJob_CreateCatalog;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_CreateCatalog::operator new(size_t)
{
  Job_CreateCatalog* pJob_CreateCatalog =
    static_cast<Job_CreateCatalog*>(HeapMem::pAllocate(CN_JOB_CREATECATALOG));
  
  return pJob_CreateCatalog;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_CreateCatalog::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_CreateCatalog::operator delete(void* aAddress)
{
  Job_CreateCatalog* pJob_CreateCatalog = static_cast<Job_CreateCatalog*>(aAddress);
  
  HeapMem::vFree(pJob_CreateCatalog);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_CreateCatalog::Job_CreateCatalog(Interface::Session* pSession)
: Job(CN_JOB_CREATECATALOG, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATECATALOG,_FOOTER_);
    
  _checkInitialised_(CN_JOB_CREATECATALOG);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATECATALOG,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_CreateCatalog::~Job_CreateCatalog(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATECATALOG,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_CreateCatalog::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_CreateCatalog* pJob_CreateCatalog = _pJob_CreateCatalog(pObject);

  pJob_CreateCatalog->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_CreateCatalog::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATECATALOG,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("create"_Stz));
  ASSERT(pTokenVec->pGet(1)->bSame("catalog"_Stz));
  
  // get number of items in token vector
  iNumTokens = pTokenVec->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3)
  return false;
  
  // set the table name
  m_sObjectName = pTokenVec->pGet(2)->aText();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_CreateCatalog::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATECATALOG,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_CreateCatalog::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATECATALOG);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_CreateCatalog_pre(Interface::Session* pSession)
{
  Job_CreateCatalog* pJob_CreateCatalog = new Job_CreateCatalog(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommand->pTknVec();
  
  bool bResult = pJob_CreateCatalog->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_CreateCatalog->m_bHasError = true;
    pJob_CreateCatalog->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_CreateCatalog;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_CreateCatalog_post(Interface::Session* pSession, Job* pJob)
{
  Job_CreateCatalog* pJob_CreateCatalog = _pJob_CreateCatalog(pJob);
  
  if ( pJob_CreateCatalog->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_CreateCatalog->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
  else {
    vTrace("success: 0 rows affected\n"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_CreateCatalog_delete(Interface::Session* pSession, Job* pJob)
{
  Job_CreateCatalog* pJob_CreateCatalog = _pJob_CreateCatalog(pJob);
  
  delete pJob_CreateCatalog;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_CreateCatalog_show(Job* pJob)
{
  Job_CreateCatalog* pJob_CreateCatalog = _pJob_CreateCatalog(pJob);

  pJob_CreateCatalog->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
