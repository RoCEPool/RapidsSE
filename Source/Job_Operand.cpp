/*============================================================================*/
/* Job_Operand.cpp -- xmit Job                                                */
/*============================================================================*/

#include "Jobs.h"

JobClass xJobClass_Xmit =
{
  CN_JOB_OPERAND,      // class number
  JK_GRANDKID,         // job kind
  "xmit operand"_Stz,  // comment about the job
  nullptr,             // the prompt command
  nullptr,             // the second word
  nullptr,             // the _run task seed
  nullptr,             // the _req task seed
  nullptr,             // the _rcv task seed
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
  pJob_Operand_pre,    // prompt pre-process function
  nullptr,             // prompt post-process function
  pJob_Operand_new,    // job new function
  vJob_Operand_delete, // job delete function
  vJob_send,           // job pre-send function
  vJob_recv,           // job post-receive function
  vJob_Operand_show,   // job show function
  
  // help and example
  false,
  C_NO_NAME,
  C_NO_NAME,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_Operand_class(pFUN** pNextFun)
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
Job_Operand* Jobs::_pJob_Operand(void* aAddress)
{
  Job_Operand* pJob_Operand = reinterpret_cast<Job_Operand*>(aAddress);
  
  RASSERT_TYPE(pJob_Operand,CN_JOB_OPERAND);
  
  return pJob_Operand;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Operand::operator new(size_t)
{
  Job_Operand* pJob_Operand =
    static_cast<Job_Operand*>(HeapMem::pAllocate(CN_JOB_OPERAND));
  
  return pJob_Operand;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Operand::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Operand::operator delete(void* aAddress)
{
  Job_Operand* pJob_Operand = static_cast<Job_Operand*>(aAddress);
  //vTrace("DELETING INSERTBUFFER\n"_Stz);
  HeapMem::vFree(pJob_Operand);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Operand::Job_Operand(JobClass* aJobClass, ReqTuple* aReqTuple)
: Job(aJobClass, aReqTuple)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_OPERAND,_FOOTER_);
  
  m_pOp          = nullptr;
  m_iNode        = C_UNKNOWN;
  m_iPartition   = C_UNKNOWN;
  
  m_eTopologyX   = PX_NONE;
  m_iNextStage   = C_UNKNOWN;
  m_iNextOp      = C_UNKNOWN;
  
  _checkInitialised_(CN_JOB_OPERAND);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_OPERAND,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Operand::~Job_Operand(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_OPERAND,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Operand::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_Operand* pJob_Operand = _pJob_Operand(pObject);

  pJob_Operand->vShow();
}

/*----------------------------------------------------------------------------*/
/* new an operand job                                                         */
/*----------------------------------------------------------------------------*/
Job_Operand* Job_Operand::pNewJobOperand(Operand* pOperand,
                                         i8 iParentJobId,
                                         Op* pOp, i8 iNde, i8 iPtn)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // analysis
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a write buffer job
  
  //! this needs to be optimised
  JobClass* pJobClass_Operand = JobClass::pFind(CN_JOB_OPERAND);
  
  ReqTuple xReqTuple("!!OPERAND"_Stz,false);

  Job_Operand* pJob_Operand = new Job_Operand(pJobClass_Operand, &xReqTuple);
  
  pJob_Operand->vSeed(SS_SEED_RUN);
  
  pJob_Operand->vParentId(iParentJobId);
  pJob_Operand->m_pOp         = pOp;
  pJob_Operand->m_iNode       = iNde;
  pJob_Operand->m_iPartition  = iPtn;
  pJob_Operand->m_pOperand    = pOperand;
  
  pJob_Operand->m_eTopologyX = pOp->eTopologyX();
  pJob_Operand->m_iNextStage = pOp->iNextStage();
  pJob_Operand->m_iNextOp    = pOp->iNextOp();
  
  return pJob_Operand;
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Operand::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_OPERAND,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Operand::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_OPERAND);
  
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
Job* Jobs::pJob_Operand_pre(JobClass* aJobClass, ReqTuple* aReqTuple, TagSet* pTagSet)
{
  Job_Operand* pJob_Operand = new Job_Operand(aJobClass,aReqTuple);
  //pJob->vDicPath(pSession);

  return pJob_Operand;
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Operand_new(JobClass* aJobClass, ReqTuple* aReqTuple)
{
  Job_Operand* pJob_Operand = new Job_Operand(aJobClass,aReqTuple);
  
  return pJob_Operand;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Operand_delete(Job* pJob)
{
  Job_Operand* pJob_Operand = _pJob_Operand(pJob);
  
  //vTrace("DELETING INSERTBUFFER\n"_Stz);
  delete pJob_Operand;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Operand_show(Job* pJob)
{
  Job_Operand* pJob_Operand = _pJob_Operand(pJob);

  pJob_Operand->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
