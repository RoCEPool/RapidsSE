/*============================================================================*/
/* Job_Xmit.cpp -- xmit Job                                                   */
/*============================================================================*/

#include "Jobs.h"

JobClass xJobClass_Xmit =
{
  CN_JOB_XMIT,         // class number
  JK_GRANDKID,         // job kind
  RS_NONE,             // response kind
  "xmit operand"_Stz,  // comment about the job
  nullptr,             // the prompt command
  nullptr,             // the second word
  "Xmit_run"_Stz,      // the _run task seed
  nullptr,             // the _req task seed
  "Xmit_kid"_Stz,      // the _rcv task seed
  nullptr,             // the _exe task seed
  nullptr,             // the _end task seed
  nullptr,             // the _can task seed
  false,               // check resources?
  false,               // detached job?
  true,                // buffer attached?
  DL_NONE,             // ddl lock model
  PY_NORMAL,           // priority
  JF_NONE,             // flags

  nullptr,             // prompt check function
  pJob_Xmit_pre,   // prompt pre-process function
  nullptr,             // prompt post-process function
  pJob_Xmit_new,   // job new function
  vJob_Xmit_delete,// job delete function
  vJob_send,           // job pre-send function
  vJob_recv,           // job post-receive function
  vJob_Xmit_show,  // job show function
  
  // help and example
  false,
  "<none>"_Stz,
  "<none>"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_Xmit_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)nullptr;
  
  return &xJobClass_Xmit;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Xmit* Jobs::_pJob_Xmit(void* aAddress)
{
  Job_Xmit* pJob_Xmit = reinterpret_cast<Job_Xmit*>(aAddress);
  
  RASSERT_TYPE(pJob_Xmit,CN_JOB_XMIT);
  
  return pJob_Xmit;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Xmit::operator new(size_t)
{
  Job_Xmit* pJob_Xmit =
    static_cast<Job_Xmit*>(HeapMem::pAllocate(CN_JOB_XMIT));
  
  return pJob_Xmit;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Xmit::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Xmit::operator delete(void* aAddress)
{
  Job_Xmit* pJob_Xmit = static_cast<Job_Xmit*>(aAddress);
  //vTrace("DELETING INSERTBUFFER\n"_Stz);
  HeapMem::vFree(pJob_Xmit);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Xmit::Job_Xmit(JobClass* aJobClass, ReqTuple* aReqTuple, i8 iForId, Session* pSession)
: Job(aJobClass, aReqTuple, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_XMIT,_FOOTER_);
  
  m_pOp            = nullptr;
  m_iNode          = C_UNKNOWN;
  m_iParentJobId   = C_UNKNOWN;
  
  _checkInitialised_(CN_JOB_XMIT);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_XMIT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Xmit::~Job_Xmit(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_XMIT,_FOOTER_);
  
//  ASSERT(m_pOperand);
//  Operand::vDelete(m_pOperand);
//  m_pOperand = nullptr;
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Xmit::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_Xmit* pJob_Xmit = _pJob_Xmit(pObject);

  pJob_Xmit->vShow();
}

/*----------------------------------------------------------------------------*/
/* new a xmit job                                                           */
/*----------------------------------------------------------------------------*/
Job_Xmit* Job_Xmit::pNewJobXmit(Session* pSession,
                                Task* pTask,
                                Operand* pOperand,
                                i8 iParentJobId,
                                Op* pOp,
                                i8 iNode)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // analysis
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a write buffer job
  
  //! this needs to be optimised
  JobClass* pJobClass_Xmit = JobClass::pFind(CN_JOB_XMIT);
  
  ReqTuple xReqTuple("!!XMIT"_Stz,false);
  
  Job_Xmit* pJob_Xmit =
  static_cast<Job_Xmit*>
  (pJobClass_Xmit->pPreJob(pSession,&xReqTuple,iParentJobId));
  
  pJob_Xmit->vSeed(SS_SEED_RUN);
  
  pJob_Xmit->m_iParentId = iParentJobId;
  pJob_Xmit->m_pOp       = pOp;
  pJob_Xmit->m_iNode     = iNode;
  pJob_Xmit->m_pOperand  = pOperand;
  
  // call the job pre-send function
  //  if (pJobClass_Xmit->m_fpSend)
  //    (*pJobClass_Xmit->m_fpSend)(pJob_Xmit);
  //
  
  return pJob_Xmit;
  
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Xmit::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_XMIT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Xmit::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_XMIT);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Xmit_pre(JobClass* aJobClass, ReqTuple* aReqTuple, i8 iForId, Session* pSession)
{
  Job_Xmit* pJob_Xmit = new Job_Xmit(aJobClass,aReqTuple,iForId,pSession);
  //pJob->vDicPath(pSession);

  return pJob_Xmit;
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Xmit_new(JobClass* aJobClass)
{
  Job_Xmit* pJob_Xmit =
    new Job_Xmit(aJobClass,nullptr,C_UNKNOWN,nullptr);
  
  return pJob_Xmit;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Xmit_delete(Job* pJob)
{
  Job_Xmit* pJob_Xmit = _pJob_Xmit(pJob);
  
  //vTrace("DELETING INSERTBUFFER\n"_Stz);
  delete pJob_Xmit;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Xmit_show(Job* pJob)
{
  Job_Xmit* pJob_Xmit = _pJob_Xmit(pJob);

  pJob_Xmit->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
