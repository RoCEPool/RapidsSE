/*============================================================================*/
/* Job__Cancel.cpp -- Job Cancel                                              */
/*============================================================================*/

// Job Short: _Cancel

#include "Jobs.h"
#include "StorageEngine.h"

JobClass xJobClass__Cancel =
{
  CN_JOB__CANCEL,          // class enum
  JK_GENERIC,              // job kind
  "job to cancel"_Stz,     // comment about the job
  nullptr,                 // the prompt command
  nullptr,                 // the second word
  nullptr,                 // the _run task seed
  nullptr,                 // the _req task seed
  "Job__Cancel_rcv"_Stz,   // the _rcv task seed
  nullptr,                 // the _exe task seed
  nullptr,                 // the _end task seed
  nullptr,                 // the _can task seed
  false,                   // check resources?
  false,                   // detached job?
  false,                   // buffer attached?
  JT_NONE,                 // job trace
  DL_NONE,                 // ddl lock model
  PY_NORMAL,               // priority
  JF_NONE,                 // flags

  nullptr,                 // prompt check function
  nullptr,                 // prompt pre-process function
  nullptr,                 // prompt post-process function
  nullptr,                 // job new function
  vJob__Cancel_delete,     // job delete function
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
JobClass* Jobs::pJob__Cancel_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob__Cancelled_class;
  
  return &xJobClass__Cancel;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job__Cancel* Jobs::_pJob__Cancel(void* aAddress)
{
  Job__Cancel* pJob__Cancel = reinterpret_cast<Job__Cancel*>(aAddress);
  
  RASSERT_TYPE(pJob__Cancel,CN_JOB__CANCEL);
  
  return pJob__Cancel;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job__Cancel::operator new(size_t)
{
  Job__Cancel* pJob__Cancel =
    static_cast<Job__Cancel*>(HeapMem::pAllocate(CN_JOB__CANCEL));
  
  return pJob__Cancel;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job__Cancel::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job__Cancel::operator delete(void* aAddress)
{
  Job__Cancel* pJob__Cancel = static_cast<Job__Cancel*>(aAddress);
  
  HeapMem::vFree(pJob__Cancel);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job__Cancel::Job__Cancel(Job* pDistJob)
: Job(CN_JOB__CANCEL,nullptr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB__CANCEL,_FOOTER_);
  
  m_iParentId = pDistJob->m_iJobId;
  m_i_ActionRoot = pDistJob->m_i_ActionRoot;

  _checkInitialised_(CN_JOB__EXECUTE);
  
  ActionSequence::vAdd(this,AN_CONSTRUCTOR,CN_JOB__CANCEL,_EG_CN);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB__CANCEL,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job__Cancel::~Job__Cancel(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB__CANCEL,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job__Cancel::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job__Cancel* pJob__Cancele = _pJob__Cancel(pObject);

  pJob__Cancele->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job__Cancel::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB__CANCEL,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job__Cancel::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB__CANCEL);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob__Cancel_delete(Interface::Session* pSession, Job* pJob)
{
  Job__Cancel* pJob__Cancel = _pJob__Cancel(pJob);
  
  delete pJob__Cancel;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
