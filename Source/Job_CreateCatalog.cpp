/*============================================================================*/
/* Job_CreateCatalog.cpp -- Create Catalog Job                                */
/*============================================================================*/

// Job Short: CreateCatalog

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobNote mJobNote_CreateCatalog (
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
  pJob_CreateCatalog_pre,   // prompt pre-process function
  vJob_CreateCatalog_post,  // prompt post-process function
  nullptr,                  // job new function
  vJob_CreateCatalog_delete,// job delete function
  vJob_send,                // job pre-send function
  vJob_recv,                // job post-receive function
  vJob_CreateCatalog_show,  // job show function
  true,                     // job is enabled
  "<help>"_Stz,             // help
  "<example>"_Stz           // example
);

JobClass_s xJobClass_CreateCatalog =
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
  pJob_CreateCatalog_pre,   // prompt pre-process function
  vJob_CreateCatalog_post,  // prompt post-process function
  nullptr,                  // job new function
  vJob_CreateCatalog_delete,// job delete function
  vJob_send,                // job pre-send function
  vJob_recv,                // job post-receive function
  vJob_CreateCatalog_show,  // job show function
  true,                     // job is enabled
  "<help>"_Stz,             // help
  "<example>"_Stz,          // example

  { 0, 0, 0, 0 ,0, 0 }      // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass_s* JobsModel::pJob_CreateCatalog_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_WaitCluster_next;
  
  return &xJobClass_CreateCatalog;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_CreateCatalog* JobsModel::_pJob_CreateCatalog(void* aAddress)
{
  Job_CreateCatalog* pJob_CreateCatalog = reinterpret_cast<Job_CreateCatalog*>(aAddress);
  
  RASSERT( !pJob_CreateCatalog || CN_JOB_CREATECATALOG == pJob_CreateCatalog->eClass());
  
  return pJob_CreateCatalog;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_CreateCatalog::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_CreateCatalog* pJob_CreateCatalog =
    static_cast<Job_CreateCatalog*>(Heap::pAllocate(CN_JOB_CREATECATALOG));
  
  return pJob_CreateCatalog;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_CreateCatalog::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_CreateCatalog::operator delete(void* aAddress)
{
  Job_CreateCatalog* pJob_CreateCatalog = static_cast<Job_CreateCatalog*>(aAddress);
  
  Heap::vFree(pJob_CreateCatalog);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_CreateCatalog::Job_CreateCatalog(Interface::SEitf* pSEitf)
: Job(CN_JOB_CREATECATALOG, pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATECATALOG,_FOOTER_);
    
  _checkInitialised_(CN_JOB_CREATECATALOG);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATECATALOG,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_CreateCatalog::~Job_CreateCatalog(void)
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
  RASSERT(CN_JOB_CREATECATALOG == eClass);
  RASSERT(pObject->eClass() == CN_JOB_CREATECATALOG);
  
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
Job* JobsModel::pJob_CreateCatalog_pre(Interface::SEitf* pSEitf)
{
  Job_CreateCatalog* pJob_CreateCatalog = new Job_CreateCatalog(pSEitf);
  
  TokenVec* pTokenVec = pSEitf->m_pCommandProcessor->pTknVec();
  
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
void JobsModel::vJob_CreateCatalog_post(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_CreateCatalog* pJob_CreateCatalog = _pJob_CreateCatalog(pJob);
  
  if ( pJob_CreateCatalog->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("ERROR (NODE%lld): %s\n"_Stz, i_Node, aEC[pJob_CreateCatalog->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_CreateCatalog_delete(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_CreateCatalog* pJob_CreateCatalog = _pJob_CreateCatalog(pJob);
  
  delete pJob_CreateCatalog;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_CreateCatalog_show(Job* pJob)
{
  Job_CreateCatalog* pJob_CreateCatalog = _pJob_CreateCatalog(pJob);

  pJob_CreateCatalog->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
