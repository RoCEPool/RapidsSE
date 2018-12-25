/*============================================================================*/
/* Job_Space.cpp -- Space Job                                                 */
/*============================================================================*/

// Job Short: Space

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_Space =
{
  "Job_Space"_Stz,  // name of the job class
  CN_JOB_SPACE,     // class enum
  JK_DIST2PC,       // job kind
  "space"_Stz,      // comment about the job
  "space"_Stz,      // the prompt command
  nullptr,          // the second word
  nullptr,          // the _run task seed
  "space_req"_Stz,  // the _req task seed
  "space_beg"_Stz,  // the _rcv task seed
  "space_exe"_Stz,  // the _exe task seed
  "space_end"_Stz,  // the _end task seed
  "space_can"_Stz,  // the _can task seed
  false,            // check resources?
  false,            // detached job?
  false,            // buffer attached?
  JT_NONE,          // job trace
  pJob_Space_pre,   // prompt pre-process function
  vJob_Space_post,  // prompt post-process function
  nullptr,          // job new function
  vJob_Space_delete,// job delete function
  vJob_send,        // job pre-send function
  vJob_recv,        // job post-receive function
  vJob_Space_show,  // job show function
  
  // help and example
  true,
  "space"_Stz,
  "space;"_Stz,
  
  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* JobModel::pJob_Space_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Start_class;
  
  return &xJobClass_Space;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Space* JobModel::_pJob_Space(void* aAddress)
{
  Job_Space* pJob_Space = reinterpret_cast<Job_Space*>(aAddress);
  
  RASSERT_TYPE(pJob_Space,CN_JOB_SPACE);
  
  return pJob_Space;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Space::operator new(size_t)
{
  Job_Space* pJob_Space =
    static_cast<Job_Space*>(HeapMem::pAllocate(CN_JOB_SPACE));
  
  return pJob_Space;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Space::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Space::operator delete(void* aAddress)
{
  Job_Space* pJob_Space = static_cast<Job_Space*>(aAddress);
  
  HeapMem::vFree(pJob_Space);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Space::Job_Space(Interface::Session* pSession)
: Job(CN_JOB_SPACE, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SPACE,_FOOTER_);
  
  _checkInitialised_(CN_JOB_SPACE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_SPACE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Space::~Job_Space(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SPACE,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Space::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_Space* pJob_Space = _pJob_Space(pObject);

  pJob_Space->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_Space::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SPACE,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("space"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - -
  // get number of items in token
  iNumTokens = pTokenVec->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 1 token
  if (iNumTokens != 1)
  return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Space::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SPACE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Space::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SPACE);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobModel::pJob_Space_pre(Interface::Session* pSession)
{
  Job_Space* pJob_Space = new Job_Space(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommandProcessor->pTknVec();
  
  bool bResult = pJob_Space->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_Space->m_bHasError = true;
    pJob_Space->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_Space;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_Space_post(Interface::Session* pSession, Job* pJob)
{
  Job_Space* pJob_Space = _pJob_Space(pJob);

  if ( pJob_Space->m_bHasError ) {
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_Space->m_xNode[i_Node].m_eErrorCode]);
    }
  }
  else {
    if (gpAttachMap->m_bMetadata) {
      vTrace("metadata: %s\n"_Stz, pJob_Space->m_sMetadata.aStr());
    }
    
    i8 iRowsProcessed = 0;
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("%s\n"_Stz, pJob_Space->m_xNode[i_Node].m_sResponse.aChar());
      iRowsProcessed += *pJob_Space->m_xNode[i_Node].m_ziRowsProcessed;
    }
    vTrace("success: %lld rows affected\n"_Stz, iRowsProcessed);
  }
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_Space_delete(Interface::Session* pSession, Job* pJob)
{
  Job_Space* pJob_Space = _pJob_Space(pJob);
  
  delete pJob_Space;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_Space_show(Job* pJob)
{
  Job_Space* pJob_Space = _pJob_Space(pJob);

  pJob_Space->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
