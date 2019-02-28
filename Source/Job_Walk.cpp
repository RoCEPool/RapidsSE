/*============================================================================*/
/* Job_Walk.cpp -- walk index                                                 */
/*============================================================================*/

// Job Short: Walk

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_Walk =
{
  "Job_Walk"_Stz,            // name of the job class
  CN_JOB_WALK,               // class enum
  JK_DIST2PC,                // job kind
  "walk"_Stz,                // comment about the job
  "tr"_Stz,                  // the prompt command
  nullptr,                   // the second word
  nullptr,                   // the _run task seed
  "truncateTable_req"_Stz,   // the _req task seed
  "truncateTable_beg"_Stz,   // the _rcv task seed
  "truncateTable_exe"_Stz,   // the _exe task seed
  "truncateTable_end"_Stz,   // the _end task seed
  "truncateTable_can"_Stz,   // the _can task seed
  true,                      // check resources?
  false,                     // detached job?
  false,                     // buffer attached?
  JT_NONE,                   // job trace
  DL_GRAB_WRITE,             // ddl lock model

  eJob_checkCluster,         // prompt check function
  pJob_Walk_pre,    // prompt pre-process function
  vJob_Walk_post,   // prompt post-process function
  nullptr,                   // job new function
  vJob_Walk_delete, // job delete function
  vJob_send,                 // job pre-send function
  vJob_recv,                 // job post-receive function
  vJob_Walk_show,   // job show function
  
  // help and example
  true,
  "tr <tableName>"_Stz,
  "tr SUPPLIER;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_Walk_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_WriteBuffer_class;
  
  return &xJobClass_Walk;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Walk* Jobs::_pJob_Walk(void* aAddress)
{
  Job_Walk* pJob_Walk = reinterpret_cast<Job_Walk*>(aAddress);
  
  RASSERT_TYPE(pJob_Walk,CN_JOB_WALK);
  
  return pJob_Walk;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Walk::operator new(size_t)
{
  Job_Walk* pJob_Walk =
    static_cast<Job_Walk*>(HeapMem::pAllocate(CN_JOB_WALK));
  
  return pJob_Walk;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Walk::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Walk::operator delete(void* aAddress)
{
  Job_Walk* pJob_Walk = static_cast<Job_Walk*>(aAddress);
  
  HeapMem::vFree(pJob_Walk);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Walk::Job_Walk(Interface::Session* pSession)
: Job(CN_JOB_WALK, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_WALK,_FOOTER_);
  
  _checkInitialised_(CN_JOB_WALK);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_WALK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Walk::~Job_Walk(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_WALK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Walk::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_Walk* pJob_Walk = _pJob_Walk(pObject);

  pJob_Walk->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_Walk::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_WALK,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("tr"_Stz));
  
  // get number of items in token vector
  iNumTokens = pTokenVec->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 2 tokens
  if (iNumTokens != 2)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the table name
  m_sObjectName = pTokenVec->pGet(1)->aText();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Walk::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_WALK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Walk::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_WALK);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Walk_pre(Interface::Session* pSession)
{
  Job_Walk* pJob_Walk = new Job_Walk(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommand->pTknVec();
  
  bool bResult = pJob_Walk->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_Walk->m_bHasError = true;
    pJob_Walk->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_Walk;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Walk_post(Interface::Session* pSession, Job* pJob)
{
  Job_Walk* pJob_Walk = _pJob_Walk(pJob);
  
  if ( pJob_Walk->m_bHasError ) {
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_Walk->m_xNode[i_Node].m_eErrorCode]);
    }
    
  }
  else {
    vTrace("success: 0 rows affected\n"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Walk_delete(Interface::Session* pSession, Job* pJob)
{
  Job_Walk* pJob_Walk = _pJob_Walk(pJob);
  
  delete pJob_Walk;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Walk_show(Job* pJob)
{
  Job_Walk* pJob_Walk = _pJob_Walk(pJob);

  pJob_Walk->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
