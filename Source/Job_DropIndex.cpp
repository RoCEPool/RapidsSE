/*============================================================================*/
/* Job_DropIndex.cpp -- drop index Job                                        */
/*============================================================================*/

#include "JobModel.h"

JobNote mJobNote_DropIndex (
  "Job_DropIndex"_Stz,  // name of the job class
  CN_JOB_DROPINDEX,     // class enum
  "comment"_Stz,        // comment about the job
  "drop"_Stz,           // the prompt command
  "index"_Stz,          // the second word
  "dropIndexR35_req"_Stz, // the task seed for this job
  false,                // detached job?
  pJob_DropIndex_pre,   // prompt pre-process function
  vJob_DropIndex_post   // prompt post-process function
);


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_DropIndex* JobsModel::_pJob_DropIndex(void* aAddress)
{
  Job_DropIndex* pJob_DropIndex = reinterpret_cast<Job_DropIndex*>(aAddress);
  
  RASSERT( !pJob_DropIndex || CN_JOB_DROPINDEX == pJob_DropIndex->eClass());
  
  return pJob_DropIndex;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_DropIndex::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_DropIndex* pJob_DropIndex =
    static_cast<Job_DropIndex*>(Heap::pAllocate(CN_JOB_DROPINDEX));
  
  return pJob_DropIndex;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_DropIndex::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_DropIndex::operator delete(void* aAddress)
{
  Job_DropIndex* pJob_DropIndex = static_cast<Job_DropIndex*>(aAddress);
  
  Heap::vFree(pJob_DropIndex);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_DropIndex::Job_DropIndex(Interface::SEitf* pSEitf)
: Job(CN_JOB_DROPINDEX), JobDicPath(pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPINDEX,_FOOTER_);
    
  //m_sExecute = "Job_DropTable.execute"_Stz;
  
  _checkInitialised_(CN_JOB_DROPINDEX);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPINDEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_DropIndex::~Job_DropIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPINDEX,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_DropIndex::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_DROPINDEX == eClass);
  RASSERT(pObject->eClass() == CN_JOB_DROPINDEX);
  
  Job_DropIndex* pJob_DropIndex = _pJob_DropIndex(pObject);

  pJob_DropIndex->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_DropIndex::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPINDEX,_FOOTER_);
  
                                                               //! Alka to write
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_DropIndex::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPINDEX);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_DropIndex_pre(Interface::SEitf* pSEitf)
{
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_DropIndex_post(Prompt::SePrompt* pPrompt, Job* pJob)
{
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
