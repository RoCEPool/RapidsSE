/*============================================================================*/
/* Job_Waker.cpp -- Waker Job                                                 */
/*============================================================================*/

#include "Jobs.h"

JobClass xJobClass_Waker =
{
  CN_JOB_WAKER,          // class number
  JK_SERVICE,             // job kind
  RS_NONE,               // response kind
  "waker"_Stz,           // comment about the job
  nullptr,               // the prompt command
  nullptr,               // the second word
  "waker_run"_Stz,       // the _run task seed
  nullptr,               // the _req task seed
  nullptr,               // the _rcv task seed
  nullptr,               // the _exe task seed
  nullptr,               // the _end task seed
  nullptr,               // the _can task seed
  false,                 // check resources?
  false,                 // detached job?
  false,                 // buffer attached?
  DL_NONE,               // ddl lock model
  PY_NORMAL,             // priority
  JF_NONE,               // flags

  eJob_checkCluster,     // prompt check function
  nullptr,               // prompt pre-process function
  nullptr,               // prompt post-process function
  pJob_Waker_new,        // job new function
  vJob_Waker_delete,     // job delete function
  nullptr,               // job pre-send function
  nullptr,               // job post-receive function
  vJob_Waker_show,       // job show function
  
  // help and example
  false,
  C_NO_NAME,
  C_NO_NAME,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_Waker_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_WriteBuffer_class;
  
  return &xJobClass_Waker;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Waker* Jobs::_pJob_Waker(void* aAddress)
{
  Job_Waker* pJob_Waker = reinterpret_cast<Job_Waker*>(aAddress);
  
  RASSERT_TYPE(pJob_Waker,CN_JOB_WAKER);
  
  return pJob_Waker;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Waker::operator new(size_t)
{
  Job_Waker* pJob_Waker =
    static_cast<Job_Waker*>(HeapMem::pAllocate(CN_JOB_WAKER));
  
  return pJob_Waker;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Waker::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Waker::operator delete(void* aAddress)
{
  Job_Waker* pJob_Waker = static_cast<Job_Waker*>(aAddress);
  
  HeapMem::vFree(pJob_Waker);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Waker::Job_Waker(JobClass* aJobClass, ReqTuple* aReqTuple)
: Job(aJobClass, aReqTuple)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_WAKER,_FOOTER_);
  
   _checkInitialised_(CN_JOB_WAKER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_WAKER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Waker::~Job_Waker(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_WAKER,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Waker::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_Waker* pJob_Waker = _pJob_Waker(pObject);

  pJob_Waker->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Waker::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_WAKER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Waker::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_WAKER);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Waker_new(JobClass* aJobClass, ReqTuple* aReqTuple)
{
  ReqTuple xReqTuple("**WAKER"_Stz,false);
  
  Job_Waker* pJob_Waker = new Job_Waker(aJobClass,&xReqTuple);
  
  return pJob_Waker;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Waker_delete(Job* pJob)
{
  Job_Waker* pJob_Waker = _pJob_Waker(pJob);
  
  delete pJob_Waker;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Waker_show(Job* pJob)
{
  Job_Waker* pJob_Waker = _pJob_Waker(pJob);

  pJob_Waker->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
