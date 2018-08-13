/*============================================================================*/
/* Job__Cancelled.cpp -- Job Cancelled                                        */
/*============================================================================*/

#include "JobModel.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job__Cancelled* JobsModel::_pJob__Cancelled(void* aAddress)
{
  Job__Cancelled* pJob__Cancelled = reinterpret_cast<Job__Cancelled*>(aAddress);
  
  RASSERT( !pJob__Cancelled || CN_JOB__CANCELLED == pJob__Cancelled->eClass());
  
  return pJob__Cancelled;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job__Cancelled::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job__Cancelled* pJob__Cancelled =
    static_cast<Job__Cancelled*>(Heap::pAllocate(CN_JOB__CANCELLED));
  
  return pJob__Cancelled;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job__Cancelled::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job__Cancelled::operator delete(void* aAddress)
{
  Job__Cancelled* pJob__Executed = static_cast<Job__Cancelled*>(aAddress);
  
  Heap::vFree(pJob__Executed);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job__Cancelled::Job__Cancelled(void)
: Job(CN_JOB__CANCELLED)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB__CANCELLED,_FOOTER_);
  
  m_sSeed = "Job__Cancelled::execute"_Stz;
  m_eErrorCode = EC_NONE;
  
  _checkInitialised_(CN_JOB__CANCELLED);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB__CANCELLED,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job__Cancelled::~Job__Cancelled(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB__CANCELLED,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job__Cancelled::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB__EXECUTED == eClass);
  RASSERT(pObject->eClass() == CN_JOB__CANCELLED);
  
  Job__Cancelled* pJob__Cancelled = _pJob__Cancelled(pObject);

  pJob__Cancelled->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job__Cancelled::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB__CANCELLED,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job__Cancelled::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB__CANCELLED);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
