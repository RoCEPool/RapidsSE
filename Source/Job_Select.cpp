/*============================================================================*/
/* Job_Select.cpp -- Select Statement Job                                     */
/*============================================================================*/

#include "Jobs.h"

JobClass xJobClass_Select =
{
  CN_JOB_SELECT,             // class number
  JK_LOCAL,                  // job kind
  RS_NONE,                   // response kind
  "select"_Stz,              // comment about the job
  "select"_Stz,              // the prompt command
  nullptr,                   // the second word
  nullptr,                   // the _run task seed
  "select_req"_Stz,          // the _req task seed
  nullptr,                   // the _rcv task seed
  nullptr,                   // the _exe task seed
  nullptr,                   // the _end task seed
  nullptr,                   // the _can task seed
  true,                      // check resources?
  false,                     // detached job?
  false,                     // buffer attached?
  JT_NONE,                   // job trace
  DL_GRAB_READ,              // ddl lock model
  PY_NORMAL,                 // priority
  JF_NONE,                   // flags

  eJob_checkCluster,         // prompt check function
  pJob_Select_pre,           // prompt pre-process function
  vJob_post,                 // prompt post-process function
  nullptr,                   // job new function
  vJob_Select_delete,        // job delete function
  nullptr,                   // job pre-send function
  nullptr,                   // job post-receive function
  vJob_Select_show,          // job show function
  
  // help and example
  true,
  "select <tableNm> <partitionNo>"_Stz,
  "fetch supplier 1;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_Select_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_SelectTable_class;
  
  return &xJobClass_Select;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Select* Jobs::_pJob_Select(void* aAddress)
{
  Job_Select* pJob_Select = reinterpret_cast<Job_Select*>(aAddress);
  
  RASSERT_TYPE(pJob_Select,CN_JOB_SELECT);
  
  return pJob_Select;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Select::operator new(size_t)
{
  Job_Select* pJob_Select =
    static_cast<Job_Select*>(HeapMem::pAllocate(CN_JOB_SELECT));
  
  return pJob_Select;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Select::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Select::operator delete(void* aAddress)
{
  Job_Select* pJob_Select = static_cast<Job_Select*>(aAddress);
  
  HeapMem::vFree(pJob_Select);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Select::Job_Select(Session* pSession)
: Job(CN_JOB_SELECT, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SELECT,_FOOTER_);
      
  _checkInitialised_(CN_JOB_SELECT);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_SELECT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Select::~Job_Select(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SELECT,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Select::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_Select* pJob_Select = _pJob_Select(pObject);

  pJob_Select->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_Select::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SELECT,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("select"_Stz));
  
  // get number of items in token vector
  iNumTokens = pTokenVec->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - -

  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Select::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SELECT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Select::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SELECT);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Select_pre(Session* pSession)
{
  Job_Select* pJob_Select = new Job_Select(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommand->pTknVec();
  
  bool bResult = pJob_Select->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    pJob_Select->vAddError(EC_INVALID_COMMAND);
  }
  
  return pJob_Select;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Select_delete(Session* pSession, Job* pJob)
{
  Job_Select* pJob_Select = _pJob_Select(pJob);
  
  delete pJob_Select;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Select_show(Job* pJob)
{
  Job_Select* pJob_Select = _pJob_Select(pJob);

  pJob_Select->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
