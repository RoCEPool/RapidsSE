/*============================================================================*/
/* Job_Alarm.cpp -- Alarm Job                                           */
/*============================================================================*/

// Job Short: Alarm

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_Alarm =
{
  "Job_Alarm"_Stz,       // name of the job class
  CN_JOB_ALARM,          // class enum
  JK_SYSTEM,             // job kind
  "alarm"_Stz,           // comment about the job
  nullptr,               // the prompt command
  nullptr,               // the second word
  "alarm_run"_Stz,       // the _run task seed
  nullptr,               // the _req task seed
  nullptr,               // the _rcv task seed
  nullptr,               // the _exe task seed
  nullptr,               // the _end task seed
  nullptr,               // the _can task seed
  false,                 // check resources?
  false,                 // detached job?
  false,                 // buffer attached?
  JT_NONE,               // job trace
  DL_NONE,               // ddl lock model

  eJob_checkCluster,     // prompt check function
  nullptr,               // prompt pre-process function
  nullptr,               // prompt post-process function
  pJob_Alarm_new,        // job new function
  vJob_Alarm_delete,     // job delete function
  nullptr,               // job pre-send function
  nullptr,               // job post-receive function
  vJob_Alarm_show,       // job show function
  
  // help and example
  false,
  "<none>"_Stz,
  "<none>"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_Alarm_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_AnalyzeIndex_class;
  
  return &xJobClass_Alarm;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Alarm* Jobs::_pJob_Alarm(void* aAddress)
{
  Job_Alarm* pJob_Alarm = reinterpret_cast<Job_Alarm*>(aAddress);
  
  RASSERT_TYPE(pJob_Alarm,CN_JOB_ALARM);
  
  return pJob_Alarm;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Alarm::operator new(size_t)
{
  Job_Alarm* pJob_Alarm =
    static_cast<Job_Alarm*>(HeapMem::pAllocate(CN_JOB_ALARM));
  
  return pJob_Alarm;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Alarm::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Alarm::operator delete(void* aAddress)
{
  Job_Alarm* pJob_Alarm = static_cast<Job_Alarm*>(aAddress);
  
  HeapMem::vFree(pJob_Alarm);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Alarm::Job_Alarm(Interface::Session* pSession)
: Job(CN_JOB_ALARM, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_ALARM,_FOOTER_);
  
   _checkInitialised_(CN_JOB_ALARM);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_ALARM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Alarm::~Job_Alarm(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_ALARM,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Alarm::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_Alarm* pJob_Alarm = _pJob_Alarm(pObject);

  pJob_Alarm->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Alarm::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_ALARM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Alarm::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_ALARM);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Alarm_new(Job* pArg)
{
  Job_Alarm* pJob_Alarm = new Job_Alarm(nullptr);
  
  return pJob_Alarm;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Alarm_delete(Interface::Session* pSession, Job* pJob)
{
  Job_Alarm* pJob_Alarm = _pJob_Alarm(pJob);
  
  delete pJob_Alarm;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Alarm_show(Job* pJob)
{
  Job_Alarm* pJob_Alarm = _pJob_Alarm(pJob);

  pJob_Alarm->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
