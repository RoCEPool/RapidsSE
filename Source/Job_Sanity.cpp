/*============================================================================*/
/* Job_Sanity.cpp -- Sanity Job                                               */
/*============================================================================*/

// Job Short: Sanity

#include "StorageEngine.h"
#include "Jobs.h"

extern AttachMap*      gpAttachMap;

JobClass xJobClass_Sanity =
{
  "Job_Sanity"_Stz,  // name of the job class
  CN_JOB_SANITY,     // class enum
  JK_DIST2PC,        // job kind
  "sanity check"_Stz,// comment about the job
  "sanity"_Stz,      // the prompt command
  nullptr,           // the second word
  nullptr,           // the _run task seed
  "sanity_req"_Stz,  // the _req task seed
  "sanity_beg"_Stz,  // the _rcv task seed
  "sanity_exe"_Stz,  // the _exe task seed
  "sanity_end"_Stz,  // the _end task seed
  "sanity_can"_Stz,  // the _can task seed
  false,             // check resources?
  false,             // detached job?
  false,             // buffer attached?
  JT_NONE,           // job trace
  DL_NONE,           // ddl lock model

  eJob_checkCluster, // prompt check function
  pJob_Sanity_pre,   // prompt pre-process function
  vJob_Sanity_post,  // prompt post-process function
  nullptr,           // job new function
  vJob_Sanity_delete,// job delete function
  vJob_send,         // job pre-send function
  vJob_recv,         // job post-receive function
  vJob_Sanity_show,  // job show function
  
  // help and example
  true,
  "sanity"_Stz,
  "sanity;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_Sanity_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_SelectTable_class;
  
  return &xJobClass_Sanity;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Sanity* Jobs::_pJob_Sanity(void* aAddress)
{
  Job_Sanity* pJob_Sanity = reinterpret_cast<Job_Sanity*>(aAddress);
  
  RASSERT_TYPE(pJob_Sanity,CN_JOB_SANITY);
  
  return pJob_Sanity;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Sanity::operator new(size_t)
{
  Job_Sanity* pJob_Sanity =
    static_cast<Job_Sanity*>(HeapMem::pAllocate(CN_JOB_SANITY));
  
  return pJob_Sanity;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Sanity::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Sanity::operator delete(void* aAddress)
{
  Job_Sanity* pJob_Status = static_cast<Job_Sanity*>(aAddress);
  
  HeapMem::vFree(pJob_Status);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Sanity::Job_Sanity(Interface::Session* pSession)
: Job(CN_JOB_SANITY, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SANITY,_FOOTER_);
      
  m_iRegionSize = gpAttachMap->Arg.m_iRegionSize;
  m_iPartitions = gpAttachMap->m_pGrid->m_nSlotsPerNode;
  
  _checkInitialised_(CN_JOB_STATUS);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_SANITY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Sanity::~Job_Sanity(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SANITY,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Sanity::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_Sanity* pJob_Sanity = _pJob_Sanity(pObject);

  pJob_Sanity->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_Sanity::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SANITY,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("sanity"_Stz));
  
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
void Job_Sanity::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SANITY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Sanity::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SANITY);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Sanity_pre(Interface::Session* pSession)
{
  Job_Sanity* pJob_Sanity = new Job_Sanity(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommand->pTknVec();
  
  bool bResult = pJob_Sanity->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_Sanity->m_bHasError = true;
    pJob_Sanity->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_Sanity;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Sanity_post(Interface::Session* pSession, Job* pJob)
{
  Job_Sanity* pJob_Sanity = _pJob_Sanity(pJob);
  
  if ( pJob_Sanity->m_bHasError ) {
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_Sanity->m_xNode[i_Node].m_eErrorCode]);
    }
  }
  else {
    if (gpAttachMap->m_bMetadata) {
      vTrace("metadata: %s\n"_Stz, pJob_Sanity->m_sMetadata.aStz());
    }
    
    i8 iRowsProcessed = 0;
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("%s\n"_Stz, pJob_Sanity->m_xNode[i_Node].m_sResponse.aChar());
      iRowsProcessed += *pJob_Sanity->m_xNode[i_Node].m_ixRowsProcessed;
    }
    vTrace("success: %lld rows affected\n"_Stz, iRowsProcessed);
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Sanity_delete(Interface::Session* pSession, Job* pJob)
{
  Job_Sanity* pJob_Sanity = _pJob_Sanity(pJob);
  
  delete pJob_Sanity;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Sanity_show(Job* pJob)
{
  Job_Sanity* pJob_Sanity = _pJob_Sanity(pJob);

  pJob_Sanity->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
