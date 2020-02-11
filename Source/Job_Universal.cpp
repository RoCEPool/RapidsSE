/*============================================================================*/
/* Job_Universal.cpp -- Universal Job                                         */
/*============================================================================*/

#include "Jobs.h"

JobClass xJobClass_Universal =
{
  CN_JOB_UNIVERSAL,       // class number
  JK_DIST2PC,             // job kind
  RS_PARTICIPANT_RESULT,           // response kind
  "universal"_Stz,        // comment about the job
  nullptr,                // the prompt command
  nullptr,                // the second word
  "universal_run"_Stz,    // the _run task seed
  "universal_req"_Stz,    // the _req task seed
  "universal_beg"_Stz,    // the _rcv task seed
  "universal_exe"_Stz,    // the _exe task seed
  "universal_end"_Stz,    // the _end task seed
  "universal_can"_Stz,    // the _can task seed
  true,                   // check resources?
  false,                  // detached job?
  false,                  // buffer attached?
  DL_GRAB_WRITE,          // ddl lock model
  PY_NORMAL,              // priority
  JF_LOWMEM,              // flags

  nullptr,                // prompt check function
  pJob_Universal_pre,     // prompt pre-process function
  vJob_post,              // prompt post-process function
  pJob_Universal_new,     // job new function
  vJob_Universal_delete,  // job delete function
  vJob_send,              // job pre-send function
  vJob_recv,              // job post-receive function
  vJob_Universal_show,    // job show function
  
  // help
  false,
  C_NO_NAME,
  C_NO_NAME,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_Universal_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_WriteBuffer_class;
  
  return &xJobClass_Universal;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Universal* Jobs::_pJob_Universal(void* aAddress)
{
  Job_Universal* pJob_Universal = reinterpret_cast<Job_Universal*>(aAddress);
  
  RASSERT_TYPE(pJob_Universal,CN_JOB_UNIVERSAL);
  
  return pJob_Universal;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Universal::operator new(size_t)
{
  Job_Universal* pJob_Universal =
    static_cast<Job_Universal*>(HeapMem::pAllocate(CN_JOB_UNIVERSAL));
  
  return pJob_Universal;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Universal::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Universal::operator delete(void* aAddress)
{
  Job_Universal* pJob_Universal = static_cast<Job_Universal*>(aAddress);
  
  HeapMem::vFree(pJob_Universal);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Universal::Job_Universal(JobClass* aJobClass, ReqTuple* aReqTuple)
: Job(aJobClass, aReqTuple)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_UNIVERSAL,_FOOTER_);
  
  _checkInitialised_(CN_JOB_UNIVERSAL);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_UNIVERSAL,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Universal::Job_Universal(Job* pParentJob, JobClass* aJobClass, ReqTuple* aReqTuple, i8 iForId)
: Job(pParentJob, aJobClass, aReqTuple)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_UNIVERSAL,_FOOTER_);
  
  _checkInitialised_(CN_JOB_UNIVERSAL);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_UNIVERSAL,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Universal::~Job_Universal(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_UNIVERSAL,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Universal::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_Universal* pJob_Universal = _pJob_Universal(pObject);

  pJob_Universal->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate node                                                              */
/*----------------------------------------------------------------------------*/
eEC Job_Universal::ePopulateJob(TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_UNIVERSAL,_FOOTER_);
  
//  Dic* pRootDic = AM::pRootDic();
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//  Word* pWord_jobsyn = pRootDic->pFindWord("jobSyntax:"_Stz);
//
//  Word* pWord_jobdef = nullptr;
//
//  while ( (pWord_jobdef = pRootDic->pEachWord(pWord_jobsyn,pWord_jobdef)) ) {
//    vTrace("  jobSyntax: %s\n"_Stz,pWord_jobdef->sName().szStz());
//
//    if (JobHelper::bMatchesSyntax(pTknVec,pWord_jobdef->pEnameSet())) {
//      vTrace("    matches\n"_Stz);
//
//      m_oEnameSet.vAdd("<jobClass>"_Stz, CN_JOB_UNIVERSAL,TN_INTEGER);
//      s32 sJobName(pWord_jobdef->sName());
//      StrLib::vRemoveSuffix(sJobName.azChar(),'_');
//      m_oEnameSet.vAdd("<jobNm>"_Stz,sJobName);
//      break;
//    }
//  }
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//  bool b = JobHelper::bExtractArgs(&m_oEnameSet,pWord_jobdef->pEnameSet(),pTknVec);
//  if (! b) {
//    vTrace("Unable to match job syntax\n"_Stz);
//    return EC_INVALID_COMMAND;
//  }

  bool b = JobHelper::bParseCmd(pEnameSet(),pTknVec);
  if (! b) {
    //vTrace("Unable to match job syntax\n"_Stz);
    vParticipantHasError(true);
    return EC_INVALID_COMMAND;
  }
  
  EnameSet* pEnameSet_ji = pJobInfoEnameSet();
  //pEnameSet_ji->vPrint();
  
  if (pEnameSet_ji) {
    m_eJobType = (eJT) pEnameSet_ji->iValue("<?jobType?>"_Stz);
    vJobKind( (eJK) pEnameSet_ji->iValue("<?jobKind?>"_Stz) );
  }
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Universal::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_UNIVERSAL,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Universal::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_UNIVERSAL);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Universal_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession)
{
  Job_Universal* pJob_Universal = new Job_Universal(aJobClass,aReqTuple);
  //pJob->vDicPath(pSession);

  TokenVec* pTokenVec = pSession->pTokenVec();

  eEC eError = pJob_Universal->ePopulateJob(pTokenVec);
  
  if ( eError != EC_NONE ) {
    pJob_Universal->vSyntaxError(eError);
  }
  
  return pJob_Universal;
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Universal_new(JobClass* aJobClass, ReqTuple* aReqTuple)
{
  Job_Universal* pJob_Universal = new Job_Universal(aJobClass,nullptr);
  
  return pJob_Universal;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Universal_delete(Job* pJob)
{
  Job_Universal* pJob_Universal = _pJob_Universal(pJob);
    
  delete pJob_Universal;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Universal_show(Job* pJob)
{
  Job_Universal* pJob_Universal = _pJob_Universal(pJob);

  pJob_Universal->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
