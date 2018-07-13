/*============================================================================*/
/* Job_Status.cpp -- Status Job                                               */
/*============================================================================*/

#include "JobModel.h"

JobNote mJobNote_Status (
  "Job_Status"_Stz, // name of the job class
  "comment"_Stz,    // comment about the job
  "status"_Stz,     // the prompt command
  nullptr,          // the second word
  "seed"_Stz,       // the task seed for this job
  pJob_Status_pre,  // prompt pre-process function
  vJob_Status_post  // prompt post-process function
  );

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Status* JobsModel::_pJob_Status(void* aAddress)
{
  Job_Status* pJob_Status = reinterpret_cast<Job_Status*>(aAddress);
  
  //RASSERT( !pJob_Status || CN_JOB_STATUS == Job_Space->eClass());
  
  return pJob_Status;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Status::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_Status* pJob_Status =
    static_cast<Job_Status*>(Heap::pAllocate(CN_JOB_STATUS));
  
  return pJob_Status;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Status::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Status::operator delete(void* aAddress)
{
  Job_Status* pJob_Status = static_cast<Job_Status*>(aAddress);
  
  Heap::vFree(pJob_Status);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Status::Job_Status(Interface::SEitf* pSEitf)
: Job(CN_JOB_STATUS), JobDicPath(pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_STATUS,_FOOTER_);
    
  m_sExecute = "Job_Status.execute"_Stz;
  
  _checkInitialised_(CN_JOB_STATUS);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_STATUS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Status::~Job_Status(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_STATUS,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Status::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_STATUS == eClass);
  RASSERT(pObject->eClass() == CN_JOB_STATUS);
  
  Job_Status* pJob_Status = _pJob_Status(pObject);

  pJob_Status->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Status::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_STATUS,_FOOTER_);
  
                                                               //! Alka to write
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Status::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_STATUS);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_Status_pre(SePrompt* pPrompt)
{
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_Status_post(SePrompt* pPrompt)
{
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
