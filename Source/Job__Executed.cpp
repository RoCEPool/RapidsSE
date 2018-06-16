/*============================================================================*/
/* Job__Executed.cpp -- Job Executed                                          */
/*============================================================================*/

#include "JobsModel.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job__Executed* JobsModel::_pJob__Executed(void* aAddress)
{
  Job__Executed* pJob__Executed = reinterpret_cast<Job__Executed*>(aAddress);
  
  RASSERT( !pJob__Executed || CN_JOB__EXECUTED == pJob__Executed->eClass());
  
  return pJob__Executed;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job__Executed::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job__Executed* pJob__Executed =
    static_cast<Job__Executed*>(Heap::pAllocate(CN_JOB__EXECUTED));
  
  return pJob__Executed;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job__Executed::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job__Executed::operator delete(void* aAddress)
{
  Job__Executed* pJob__Executed = static_cast<Job__Executed*>(aAddress);
  
  Heap::vFree(pJob__Executed);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job__Executed::Job__Executed(void)
: Job(CN_JOB__EXECUTED)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB__EXECUTED,_FOOTER_);
  
  m_sExecute = "Job__Executed_Exe"_Stz;
  
  _checkInitialised_(CN_JOB__EXECUTED);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB__EXECUTED,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job__Executed::~Job__Executed(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB__EXECUTED,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job__Executed::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB__EXECUTED == eClass);
  RASSERT(pObject->eClass() == CN_JOB__EXECUTED);
  
  Job__Executed* pJob__Executed = _pJob__Executed(pObject);

  pJob__Executed->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job__Executed::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB__EXECUTED,_FOOTER_);
  
                                                               //! Alka to write
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job__Executed::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB__EXECUTED);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
