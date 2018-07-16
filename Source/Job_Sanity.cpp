/*============================================================================*/
/* Job_Sanity.cpp -- Sanity Job                                               */
/*============================================================================*/
#include "StorageEngine.h"
#include "JobModel.h"

extern AttachMap*      gpAttachMap;

JobNote mJobNote_Sanity (
  "Job_Sanity"_Stz, // name of the job class
  CN_JOB_SANITY,    // class enum
  "comment"_Stz,    // comment about the job
  "sanity"_Stz,     // the prompt command
  nullptr,          // the second word
  "nodeSanityR35_req"_Stz, // the task seed for this job
  false,            // detached job?
  pJob_Sanity_pre,  // prompt pre-process function
  vJob_Sanity_post  // prompt post-process function
  );


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Sanity* JobsModel::_pJob_Sanity(void* aAddress)
{
  Job_Sanity* pJob_Sanity = reinterpret_cast<Job_Sanity*>(aAddress);
  
  //RASSERT( !pJob_Status || CN_JOB_SANITY == Job_Space->eClass());
  
  return pJob_Sanity;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Sanity::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_Sanity* pJob_Sanity =
    static_cast<Job_Sanity*>(Heap::pAllocate(CN_JOB_SANITY));
  
  return pJob_Sanity;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Sanity::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Sanity::operator delete(void* aAddress)
{
  Job_Sanity* pJob_Status = static_cast<Job_Sanity*>(aAddress);
  
  Heap::vFree(pJob_Status);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Sanity::Job_Sanity(Interface::SEitf* pSEitf)
: Job(CN_JOB_SANITY), JobDicPath(pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SANITY,_FOOTER_);
    
  m_sExecute = "Job_Sanity.execute"_Stz;
  m_iRegionSize = gpAttachMap->m_iRegionSize;
  m_iPartitions = gpAttachMap->m_pGrid->m_nSlots;
  
  _checkInitialised_(CN_JOB_STATUS);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_SANITY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Sanity::~Job_Sanity(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SANITY,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Sanity::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_SANITY == eClass);
  RASSERT(pObject->eClass() == CN_JOB_SANITY);
  
  Job_Sanity* pJob_Sanity = _pJob_Sanity(pObject);

  pJob_Sanity->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Sanity::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SANITY,_FOOTER_);
  
                                                               //! Alka to write
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Sanity::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SANITY);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_Sanity_pre(Prompt::SePrompt* pPrompt)
{
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_Sanity_post(Prompt::SePrompt* pPrompt, Job* pJob)
{
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
