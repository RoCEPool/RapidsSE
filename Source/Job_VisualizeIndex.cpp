/*============================================================================*/
/* Job_VisualizeIndex.cpp -- visualize index Job                              */
/*============================================================================*/

// Job Short: VisualizeIndex

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_VisualizeIndex =
{
  CN_JOB_VISUALIZEINDEX,      // class enum
  JK_LOCAL,                   // job kind
  "visualize index"_Stz,      // comment about the job
  "visidx"_Stz,               // the prompt command
  nullptr,                    // the second word
  nullptr,                    // the _run task seed
  "visualizeIndex_req"_Stz,   // the _req task seed
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
  pJob_VisualizeIndex_pre,    // prompt pre-process function
  vJob_VisualizeIndex_post,   // prompt post-process function
  nullptr,                    // job new function
  vJob_VisualizeIndex_delete, // job delete function
  nullptr,                    // job pre-send function
  nullptr,                    // job post-receive function
  vJob_VisualizeIndex_show,   // job show function
  
  // help and example
  true,
  "visidx <tableNm> <indexNm>"_Stz,
  "visidx SUPPLIER idx1"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_VisualizeIndex_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_WaitCluster_class;
  
  return &xJobClass_VisualizeIndex;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_VisualizeIndex* Jobs::_pJob_VisualizeIndex(void* aAddress)
{
  Job_VisualizeIndex* pJob_VisualizeIndex = reinterpret_cast<Job_VisualizeIndex*>(aAddress);
  
  RASSERT_TYPE(pJob_VisualizeIndex,CN_JOB_VISUALIZEINDEX);
  
  return pJob_VisualizeIndex;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_VisualizeIndex::operator new(size_t)
{
  Job_VisualizeIndex* pJob_VisualizeIndex =
    static_cast<Job_VisualizeIndex*>(HeapMem::pAllocate(CN_JOB_VISUALIZEINDEX));
  
  return pJob_VisualizeIndex;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_VisualizeIndex::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_VisualizeIndex::operator delete(void* aAddress)
{
  Job_VisualizeIndex* pJob_VisualizeIndex = static_cast<Job_VisualizeIndex*>(aAddress);
  
  HeapMem::vFree(pJob_VisualizeIndex);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_VisualizeIndex::Job_VisualizeIndex(Interface::Session* pSession)
: Job(CN_JOB_VISUALIZEINDEX, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_VISUALIZEINDEX,_FOOTER_);
    
  _checkInitialised_(CN_JOB_VISUALIZEINDEX);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_VISUALIZEINDEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_VisualizeIndex::~Job_VisualizeIndex(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_VISUALIZEINDEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_VisualizeIndex::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_VisualizeIndex* pJob_VisualizeIndex = _pJob_VisualizeIndex(pObject);

  pJob_VisualizeIndex->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_VisualizeIndex::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_VISUALIZEINDEX,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("visidx"_Stz));
  
  // get number of items in token vector
  iNumTokens = pTokenVec->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the index name
  m_sIndexName = pTokenVec->pGet(2)->aText();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the table name
  m_sObjectName = pTokenVec->pGet(1)->aText();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_VisualizeIndex::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_VISUALIZEINDEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_VisualizeIndex::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_VISUALIZEINDEX);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_VisualizeIndex_pre(Interface::Session* pSession)
{
  Job_VisualizeIndex* pJob_VisualizeIndex = new Job_VisualizeIndex(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommand->pTknVec();
  
  bool bResult = pJob_VisualizeIndex->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_VisualizeIndex->m_bHasError = true;
    pJob_VisualizeIndex->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_VisualizeIndex;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_VisualizeIndex_post(Interface::Session* pSession, Job* pJob)
{
  Job_VisualizeIndex* pJob_VisualizeIndex = _pJob_VisualizeIndex(pJob);
  
  if ( pJob_VisualizeIndex->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_VisualizeIndex->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_VisualizeIndex_delete(Interface::Session* pSession, Job* pJob)
{
  Job_VisualizeIndex* pJob_VisualizeIndex = _pJob_VisualizeIndex(pJob);
  
  delete pJob_VisualizeIndex;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_VisualizeIndex_show(Job* pJob)
{
  Job_VisualizeIndex* pJob_VisualizeIndex = _pJob_VisualizeIndex(pJob);

  pJob_VisualizeIndex->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
