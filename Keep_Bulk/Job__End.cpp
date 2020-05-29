/*============================================================================*/
/* Job__End.cpp -- Job End                                                    */
/*============================================================================*/

#include "Jobs.h"

JobClass xJobClass__End =
{
  CN_JOB__END,             // class number
  JK_GENERIC,              // job kind
  RS_NONE,                 // response kind
  "job to end"_Stz,        // comment about the job
  nullptr,                 // the prompt command
  nullptr,                 // the second word
  nullptr,                 // the _run task seed
  nullptr,                 // the _req task seed
  "Job__End_rcv"_Stz,      // the _rcv task seed
  nullptr,                 // the _exe task seed
  nullptr,                 // the _end task seed
  nullptr,                 // the _can task seed
  false,                   // check resources?
  false,                   // detached job?
  false,                   // buffer attached?
  DL_NONE,                 // ddl lock model
  PY_NORMAL,               // priority
  JF_NONE,                 // flags

  nullptr,                 // prompt check function
  nullptr,                 // prompt pre-process function
  nullptr,                 // prompt post-process function
  nullptr,                 // job new function
  vJob__End_delete,        // job delete function
  vJob_send,               // job pre-send function
  vJob_recv,               // job post-receive function
  nullptr,                 // job show function
  false,                   // job is not prompt enabled
  "<help>"_Stz,            // help
  "<example>"_Stz,         // example

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob__End_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob__Ended_class;
  
  return &xJobClass__End;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job__End* Jobs::_pJob__End(void* aAddress)
{
  Job__End* pJob__End = reinterpret_cast<Job__End*>(aAddress);
  
  RASSERT_TYPE(pJob__End,CN_JOB__END);
  
  return pJob__End;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job__End::operator new(size_t)
{
  Job__End* pJob__End =
    static_cast<Job__End*>(HeapMem::pAllocate(CN_JOB__END));
  
  return pJob__End;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job__End::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job__End::operator delete(void* aAddress)
{
  Job__End* pJob__End = static_cast<Job__End*>(aAddress);
  
  HeapMem::vFree(pJob__End);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job__End::Job__End(Job* pDistJob)
: Job(JobClass::pFind(CN_JOB__END),&pDistJob->m_xReqTuple)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB__END,_FOOTER_);
  
  m_iParentId = pDistJob->m_iFocalId;

  _checkInitialised_(CN_JOB__END);
  
  //ActLog::vAdd(this,AN_CONSTRUCT,CN_JOB__END,_EG_CN);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB__END,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job__End::~Job__End(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB__END,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job__End::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job__End* pJob__End = _pJob__End(pObject);

  pJob__End->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job__End::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB__END,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job__End::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB__END);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob__End_delete(Job* pJob)
{
  Job__End* pJob__End = _pJob__End(pJob);
  
  delete pJob__End;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
