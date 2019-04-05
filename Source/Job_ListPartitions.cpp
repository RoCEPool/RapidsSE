/*============================================================================*/
/* Job_ListPartitions.cpp -- List Partitions Job                              */
/*============================================================================*/

// Job Short: ListPartitions

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_ListPartitions =
{
  CN_JOB_LISTPARTITIONS,    // class enum
  JK_LOCAL,                 // job kind
  RK_LOCAL_ROWS,            // response kind
  "list partitions"_Stz,    // comment about the job
  "lspart"_Stz,             // the prompt command
  nullptr,                  // the second word
  nullptr,                  // the _run task seed
  "listPartitions_req"_Stz, // the _req task seed
  "listPartitions_beg"_Stz, // the _exe task seed
  "listPartitions_exe"_Stz, // the _rcv task seed
  "listPartitions_end"_Stz, // the _end task seed
  "listPartitions_can"_Stz, // the _can task seed
  false,                   // check resources?
  false,                   // detached job?
  false,                   // buffer attached?
  JT_NONE,                 // job trace
  DL_NONE,                 // ddl lock model
  PY_NORMAL,               // priority
  JF_NONE,                 // flags

  eJob_checkCluster,        // prompt check function
  pJob_ListPartitions_pre,  // prompt pre-process function
  vJob_post,                // prompt post-process function
  nullptr,                  // job new function
  vJob_ListPartitions_delete, // job delete function
  vJob_send,                  // job pre-send function
  vJob_recv,                  // job post-receive function
  vJob_ListPartitions_show,  // job show function
  
  // help and example
  true,
  "lspart"_Stz,
  "lspart;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_ListPartitions_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_ListSchemas_class;
  
  return &xJobClass_ListPartitions;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_ListPartitions* Jobs::_pJob_ListPartitions(void* aAddress)
{
  Job_ListPartitions* pJob_ListPartitions = reinterpret_cast<Job_ListPartitions*>(aAddress);
  
  RASSERT_TYPE(pJob_ListPartitions,CN_JOB_LISTPARTITIONS);
  
  return pJob_ListPartitions;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_ListPartitions::operator new(size_t)
{
  Job_ListPartitions* pJob_ListPartitions =
    static_cast<Job_ListPartitions*>(HeapMem::pAllocate(CN_JOB_LISTPARTITIONS));
  
  return pJob_ListPartitions;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_ListPartitions::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_ListPartitions::operator delete(void* aAddress)
{
  Job_ListPartitions* pJob_ListPartitions = static_cast<Job_ListPartitions*>(aAddress);
  
  HeapMem::vFree(pJob_ListPartitions);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_ListPartitions::Job_ListPartitions(Interface::Session* pSession)
: Job(CN_JOB_LISTPARTITIONS, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTPARTITIONS,_FOOTER_);
  
  _checkInitialised_(CN_JOB_LISTPARTITIONS);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTPARTITIONS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_ListPartitions::~Job_ListPartitions(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTPARTITIONS,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_ListPartitions::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_ListPartitions* pJob_ListPartitions = _pJob_ListPartitions(pObject);

  pJob_ListPartitions->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_ListPartitions::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTPARTITIONS,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("lspart"_Stz));
  
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
void Job_ListPartitions::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTPARTITIONS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_ListPartitions::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTPARTITIONS);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_ListPartitions_pre(Interface::Session* pSession)
{
  Job_ListPartitions* pJob_ListPartitions = new Job_ListPartitions(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommand->pTknVec();
  
  bool bResult = pJob_ListPartitions->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    pJob_ListPartitions->vAddError(EC_INVALID_COMMAND);
  }
  
  return pJob_ListPartitions;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_ListPartitions_delete(Interface::Session* pSession, Job* pJob)
{
  Job_ListPartitions* pJob_ListPartitions = _pJob_ListPartitions(pJob);
  
  delete pJob_ListPartitions;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_ListPartitions_show(Job* pJob)
{
  Job_ListPartitions* pJob_ListPartitions = _pJob_ListPartitions(pJob);

  pJob_ListPartitions->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
