/*============================================================================*/
/* Job_Local.cpp -- Local Job                                                 */
/*============================================================================*/

#include "Jobs.h"

JobClass xJobClass_Local =
{
  CN_JOB_LOCAL,             // class number
  JK_LOCAL,                 // job kind
  "local"_Stz,              // comment about the job
  nullptr,                  // the prompt command
  nullptr,                  // the second word
  nullptr,                  // the _run task seed
  "local_req"_Stz,          // the _req task seed
  nullptr,                  // the _rcv task seed
  nullptr,                  // the _exe task seed
  nullptr,                  // the _end task seed
  nullptr,                  // the _can task seed
  false,                    // check resources?
  false,                    // detached job?
  false,                    // buffer attached?
  DL_GRAB_WRITE,            // ddl lock model
  PY_NORMAL,                // priority
  JF_LOWMEM,                // flags

  nullptr,                  // prompt check function
  pJob_Local_pre,           // prompt pre-process function
  vJob_post,                // prompt post-process function
  pJob_Local_new,           // job new function
  vJob_Local_delete,        // job delete function
  nullptr,                  // job pre-send function
  nullptr,                  // job post-receive function
  vJob_Local_show,          // job show function
  
  // help
  false,
  C_NO_NAME,
  C_NO_NAME,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_Local_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)nullptr;
  
  return &xJobClass_Local;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Local* Jobs::_pJob_Local(void* aAddress)
{
  Job_Local* pJob_Local = reinterpret_cast<Job_Local*>(aAddress);
  
  RASSERT_TYPE(pJob_Local,CN_JOB_LOCAL);
  
  return pJob_Local;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Local::operator new(size_t)
{
  Job_Local* pJob_Local =
    static_cast<Job_Local*>(HeapMem::pAllocate(CN_JOB_LOCAL));
  
  return pJob_Local;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Local::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Local::operator delete(void* aAddress)
{
  Job_Local* pJob_Local = static_cast<Job_Local*>(aAddress);
  
  HeapMem::vFree(pJob_Local);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Local::Job_Local(JobClass* aJobClass, ReqTuple* aReqTuple)
: Job(aJobClass, aReqTuple)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOCAL,_FOOTER_);
  
  _checkInitialised_(CN_JOB_LOCAL);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOCAL,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Local::~Job_Local(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOCAL,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Local::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_Local* pJob_Local = _pJob_Local(pObject);

  pJob_Local->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate node                                                              */
/*----------------------------------------------------------------------------*/
eEC Job_Local::ePopulateJob(TagSet* pTagSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOCAL,_FOOTER_);
  
//  bool b = CmdHelper::bParseCmd(pJobTags(),pTknVec);
//  if (! b) {
//    //vTrace("Unable to match job syntax\n"_Stz);
//    vParticipantHasError(true);
//    return EC_INVALID_COMMAND;
//  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // add the distid
  pJobTags()->vInsert(C_TAG_DISTID,m_iFocalId);
  
  // copy the tagset into the job tag set
  pJobTags()->vCopy(pTagSet);
  eJK eJobKind = (eJK) pJobTags()->iValue(C_TAG_JOBKIND);
  vJobKind(eJobKind);
  
  Stz* szJobNm = pJobTags()->szValue(C_TAG_JOBNM);
  vJobNm(szJobNm);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Local::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOCAL,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Local::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LOCAL);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Local_pre(JobClass* aJobClass, ReqTuple* aReqTuple, TagSet* pTagSet)
{
  Job_Local* pJob_Local = new Job_Local(aJobClass,aReqTuple);

  eEC eError = pJob_Local->ePopulateJob(pTagSet);
  
  RASSERT(EC_NONE == eError);
  
  //  if ( eError != EC_NONE ) {
  //    pJob_Local->vJobError(eError);
  //  }
  
  return pJob_Local;
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Local_new(JobClass* aJobClass, ReqTuple* aReqTuple)
{
  Job_Local* pJob_Local = new Job_Local(aJobClass,aReqTuple);
  
  return pJob_Local;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Local_delete(Job* pJob)
{
  Job_Local* pJob_Local = _pJob_Local(pJob);
    
  delete pJob_Local;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Local_show(Job* pJob)
{
  Job_Local* pJob_Local = _pJob_Local(pJob);

  pJob_Local->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
