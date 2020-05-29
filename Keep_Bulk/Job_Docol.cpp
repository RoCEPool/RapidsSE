/*============================================================================*/
/* Job_Docol.cpp -- Docol Job                                                 */
/*============================================================================*/

#include "Jobs.h"

JobClass xJobClass_Docol =
{
  CN_JOB_DOCOL,      // class number
  JK_LOCAL,          // job kind
  RS_LOCAL_ERRORS,   // response kind
  "execute word"_Stz,// comment about the job
  "docol"_Stz,       // the prompt command
  nullptr,           // the second word
  nullptr,           // the _run task seed
  "docol_req"_Stz,   // the _req task seed
  nullptr,           // the _rcv task seed
  nullptr,           // the _exe task seed
  nullptr,           // the _end task seed
  nullptr,           // the _can task seed
  false,             // check resources?
  false,             // detached job?
  false,             // buffer attached?
  DL_NONE,           // ddl lock model
  PY_NORMAL,         // priority
  JF_NONE,           // flags

  nullptr,           // prompt check function
  pJob_Docol_pre,    // prompt pre-process function
  vJob_post,         // prompt post-process function
  nullptr,           // job new function
  vJob_Docol_delete, // job delete function
  nullptr,           // job pre-send function
  nullptr,           // job post-receive function
  vJob_Docol_show,   // job show function
  
  // help and example
  true,
  "docol <wordNm>"_Stz,
  "<example>"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_Docol_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_DropCatalog_class;
  
  return &xJobClass_Docol;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Docol* Jobs::_pJob_Docol(void* aAddress)
{
  Job_Docol* pJob_Docol = reinterpret_cast<Job_Docol*>(aAddress);
  
  RASSERT_TYPE(pJob_Docol,CN_JOB_DOCOL);
  
  return pJob_Docol;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Docol::operator new(size_t)
{
  Job_Docol* pJob_Docol =
    static_cast<Job_Docol*>(HeapMem::pAllocate(CN_JOB_DOCOL));
  
  return pJob_Docol;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Docol::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Docol::operator delete(void* aAddress)
{
  Job_Docol* pJob_Docol = static_cast<Job_Docol*>(aAddress);
  
  HeapMem::vFree(pJob_Docol);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Docol::Job_Docol(JobClass* aJobClass, ReqTuple* aReqTuple)
: Job(aJobClass, aReqTuple)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DOCOL,_FOOTER_);
  
  _checkInitialised_(CN_JOB_DOCOL);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_DOCOL,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Docol::~Job_Docol(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DOCOL,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Docol::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_Docol* pJob_Docol = _pJob_Docol(pObject);

  pJob_Docol->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_Docol::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DOCOL,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("docol"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - -
  // get no of tokens in token vector
  iNumTokens = pTokenVec->nTokens();;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 2 tokens
  if (iNumTokens != 2) {
    return nullptr;
  }
  
  // set the docol routine name
  m_oEnameSet.vAdd("<routineNm>"_Stz, pTokenVec->pGet(1)->szText());
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Docol::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DOCOL,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Docol::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DOCOL);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Docol_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession)
{
  Job_Docol* pJob_Docol = new Job_Docol(aJobClass,aReqTuple);
  //pJob->vDicPath(pSession);

  TokenVec* pTokenVec = pSession->m_pInput->pTknVec();
  
  bool bResult = pJob_Docol->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    pJob_Docol->vAddError(EC_INVALID_COMMAND);
  }
  
  return pJob_Docol;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Docol_delete(Job* pJob)
{
  Job_Docol* pJob_Docol = _pJob_Docol(pJob);
  
  delete pJob_Docol;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Docol_show(Job* pJob)
{
  Job_Docol* pJob_Docol = _pJob_Docol(pJob);

  pJob_Docol->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
