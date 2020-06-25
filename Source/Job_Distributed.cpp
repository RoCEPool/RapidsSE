/*============================================================================*/
/* Job_Distributed.cpp -- Distributed Job                                     */
/*============================================================================*/

#include "Jobs.h"

JobClass xJobClass_Distributed =
{
  CN_JOB_DISTRIBUTED,       // class number
  JK_DIST2PC,               // job kind
  "distributed"_Stz,        // comment about the job
  nullptr,                  // the prompt command
  nullptr,                  // the second word

  nullptr,                  // prompt check function
  pJob_Distributed_pre,     // prompt pre-process function
  vJob_post,                // prompt post-process function
  pJob_Distributed_new,     // job new function
  vJob_Distributed_delete,  // job delete function
  vJob_send,                // job pre-send function
  vJob_recv,                // job post-receive function
  vJob_Distributed_show,    // job show function

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_Distributed_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Local_class;
  
  return &xJobClass_Distributed;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Distributed* Jobs::_pJob_Distributed(void* aAddress)
{
  Job_Distributed* pJob_Distributed = reinterpret_cast<Job_Distributed*>(aAddress);
  
  RASSERT_TYPE(pJob_Distributed,CN_JOB_DISTRIBUTED);
  
  return pJob_Distributed;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Distributed::operator new(size_t)
{
  Job_Distributed* pJob_Distributed =
    static_cast<Job_Distributed*>(HeapMem::pAllocate(CN_JOB_DISTRIBUTED));
  
  return pJob_Distributed;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Distributed::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Distributed::operator delete(void* aAddress)
{
  Job_Distributed* pJob_Distributed = static_cast<Job_Distributed*>(aAddress);
  
  HeapMem::vFree(pJob_Distributed);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Distributed::Job_Distributed(JobClass* aJobClass, ReqTuple* aReqTuple)
: Job(aJobClass, aReqTuple)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DISTRIBUTED,_FOOTER_);
  
  _checkInitialised_(CN_JOB_DISTRIBUTED);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_DISTRIBUTED,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Distributed::~Job_Distributed(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DISTRIBUTED,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Distributed::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_Distributed* pJob_Distributed = _pJob_Distributed(pObject);

  pJob_Distributed->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate node                                                              */
/*----------------------------------------------------------------------------*/
eEC Job_Distributed::ePopulateJob(TagSet* pTagSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DISTRIBUTED,_FOOTER_);
  
//  bool b = CmdHelper::bParseCmd(pJobTags(),pTknVec);
//  if (! b) {
//    //vTrace("Unable to match job syntax\n"_Stz);
//    vParticipantHasError(true);
//    return EC_INVALID_COMMAND;
//  }
  //pTagSet->vDumpSet();
  
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
void Job_Distributed::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DISTRIBUTED,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Distributed::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DISTRIBUTED);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Distributed_pre(JobClass* aJobClass, ReqTuple* aReqTuple, TagSet* pTagSet)
{
  Job_Distributed* pJob_Distributed = new Job_Distributed(aJobClass,aReqTuple);
  
  eEC eError = pJob_Distributed->ePopulateJob(pTagSet);

  RASSERT(EC_NONE == eError);
  
//  if ( eError != EC_NONE ) {
//    pJob_Distributed->vJobError(eError);
//  }
  
  return pJob_Distributed;
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Distributed_new(JobClass* aJobClass, ReqTuple* aReqTuple)
{
  Job_Distributed* pJob_Distributed = new Job_Distributed(aJobClass,aReqTuple);
  
  return pJob_Distributed;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Distributed_delete(Job* pJob)
{
  Job_Distributed* pJob_Distributed = _pJob_Distributed(pJob);
    
  delete pJob_Distributed;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Distributed_show(Job* pJob)
{
  Job_Distributed* pJob_Distributed = _pJob_Distributed(pJob);

  pJob_Distributed->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
