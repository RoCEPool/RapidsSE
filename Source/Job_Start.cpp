/*============================================================================*/
/* Job_Start.cpp -- Library Start Job                                         */
/*============================================================================*/

#include "Jobs.h"

JobClass xJobClass_Start =
{
  CN_JOB_START,       // class number
  JK_BOOT,            // job kind
  "system start"_Stz, // comment about the job
  nullptr,            // the prompt command
  nullptr,            // the second word
  "start_run"_Stz,    // the _run task seed
  nullptr,            // the _req task seed
  nullptr,            // the _rcv task seed
  nullptr,            // the _exe task seed
  nullptr,            // the _end task seed
  nullptr,            // the _can task seed
  false,              // check resources?
  false,              // detached job?
  false,              // buffer attached?
  DL_NONE,            // ddl lock model
  PY_NORMAL,          // priority
  JF_NONE,            // flags

  eJob_checkCluster,  // prompt check function
  nullptr,            // prompt pre-process function
  nullptr,            // prompt post-process function
  pJob_Start_new,     // job new function
  vJob_Start_delete,  // job delete function
  nullptr,            // job pre-send function
  nullptr,            // job post-receive function
  vJob_Start_show,    // job show function
  
  // help and example
  false,
  C_NO_NAME,
  C_NO_NAME,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_Start_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)nullptr;
  
  return &xJobClass_Start;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Start* Jobs::_pJob_Start(void* aAddress)
{
  Job_Start* pJob_Start = reinterpret_cast<Job_Start*>(aAddress);
  
  RASSERT_TYPE(pJob_Start,CN_JOB_START);
  
  return pJob_Start;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Start::operator new(size_t)
{
  Job_Start* pJob_Start =
    static_cast<Job_Start*>(HeapMem::pAllocate(CN_JOB_START));
  
  return pJob_Start;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Start::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Start::operator delete(void* aAddress)
{
  Job_Start* pJob_Start = static_cast<Job_Start*>(aAddress);
  
  HeapMem::vFree(pJob_Start);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Start::Job_Start(JobClass* aJobClass, ReqTuple* aReqTuple)
: Job(aJobClass, aReqTuple)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_START,_FOOTER_);
  
  _checkInitialised_(CN_JOB_START);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_START,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Start::~Job_Start(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_START,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Start::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_Start* pJob_Start = _pJob_Start(pObject);

  pJob_Start->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Start::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_START,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Start::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_START);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Start_new(JobClass* aJobClass, ReqTuple* aReqTuple)
{
  ReqTuple xReqTuple("**START"_Stz,false);
  
  Job_Start* pJob_Start = new Job_Start(aJobClass,&xReqTuple);
  
  return pJob_Start;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Start_delete(Job* pJob)
{
  Job_Start* pJob_Start = _pJob_Start(pJob);
  
  delete pJob_Start;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Start_show(Job* pJob)
{
  Job_Start* pJob_Start = _pJob_Start(pJob);

  pJob_Start->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
