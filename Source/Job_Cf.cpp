/*============================================================================*/
/* Job_Cf.cpp -- Change Folder Job                                            */
/*============================================================================*/

// Job Short: Cf

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_Cf =
{
  CN_JOB_CF,           // class enum
  JK_LOCAL,            // job kind
  RK_LOCAL_ROWS,       // response kind
  "change folder"_Stz, // comment about the job
  "cf"_Stz,            // the prompt command
  nullptr,             // the second word
  nullptr,             // the _run task seed
  "cf_req"_Stz,        // the _req task seed
  nullptr,             // the _rcv task seed
  nullptr,             // the _exe task seed
  nullptr,             // the _end task seed
  nullptr,             // the _can task seed
  false,               // check resources?
  false,               // detached job?
  false,               // buffer attached?
  JT_NONE,             // job trace
  DL_NONE,             // ddl lock model
  PY_NORMAL,           // priority
  JF_NONE,             // flags

  eJob_checkCluster,   // prompt check function
  pJob_Cf_pre,         // prompt pre-process function
  vJob_post,           // prompt post-process function
  nullptr,             // job new function
  vJob_Cf_delete,      // job delete function
  nullptr,             // job pre-send function
  nullptr,             // job post-receive function
  vJob_Cf_show,        // job show function
  
  // help and example
  true,
  "cf [<folderNm>|..]"_Stz,
  "cf tables;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_Cf_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_CompactIndex_class;
  
  return &xJobClass_Cf;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Cf* Jobs::_pJob_Cf(void* aAddress)
{
  Job_Cf* pJob_Cf = reinterpret_cast<Job_Cf*>(aAddress);
  
  RASSERT_TYPE(pJob_Cf,CN_JOB_CF);
  
  return pJob_Cf;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Cf::operator new(size_t)
{
  Job_Cf* pJob_Cf =
    static_cast<Job_Cf*>(HeapMem::pAllocate(CN_JOB_CF));
  
  return pJob_Cf;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Cf::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Cf::operator delete(void* aAddress)
{
  Job_Cf* pJob_Cf = static_cast<Job_Cf*>(aAddress);
  
  HeapMem::vFree(pJob_Cf);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Cf::Job_Cf(Interface::Session* pSession)
: Job(CN_JOB_CF, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CF,_FOOTER_);
  
  _checkInitialised_(CN_JOB_DOCOL);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_CF,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Cf::~Job_Cf(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CF,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Cf::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_Cf* pJob_Cf = _pJob_Cf(pObject);

  pJob_Cf->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_Cf::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CF,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("cf"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - -
  // get no of tokens in token vector
  iNumTokens = pTokenVec->nTokens();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 2 tokens
  if (iNumTokens != 2) {
    return nullptr;
  }
  
  // set the docol command name
  m_sObjectName = pTokenVec->pGet(1)->aText();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Cf::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CF,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Cf::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CF);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Cf_pre(Interface::Session* pSession)
{
  Job_Cf* pJob_Cf = new Job_Cf(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommand->pTknVec();
  
  bool bResult = pJob_Cf->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    pJob_Cf->vAddError(EC_INVALID_COMMAND);
  }
  
  return pJob_Cf;
}

/*----------------------------------------------------------------------------*/
/* delete job function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Cf_delete(Interface::Session* pSession, Job* pJob)
{
  Job_Cf* pJob_Cf = _pJob_Cf(pJob);
  
  delete pJob_Cf;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Cf_show(Job* pJob)
{
  Job_Cf* pJob_Cf = _pJob_Cf(pJob);
  
  pJob_Cf->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
