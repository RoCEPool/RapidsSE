/*============================================================================*/
/* Job_DropSchema.cpp -- Drop Schema Job                                      */
/*============================================================================*/

#include "JobModel.h"

JobNote mJobNote_DropSchema (
  "g35"_Stz,            // name of the job class
  CN_JOB_DROPSCHEMA,    // class enum
  "comment"_Stz,        // comment about the job
  "drop"_Stz,           // the prompt command
  "schema"_Stz,         // the second word
  "dropSchemaR35_req"_Stz, // the task seed for this job
  false,                // detached job?
  pJob_DropSchema_pre,  // prompt pre-process function
  vJob_DropSchema_post  // prompt post-process function
  );

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_DropSchema* JobsModel::_pJob_DropSchema(void* aAddress)
{
  Job_DropSchema* pJob_DropSchema = reinterpret_cast<Job_DropSchema*>(aAddress);
  
  RASSERT( !pJob_DropSchema || CN_JOB_DROPSCHEMA == pJob_DropSchema->eClass());
  
  return pJob_DropSchema;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_DropSchema::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_DropSchema* pJob_DropSchema =
    static_cast<Job_DropSchema*>(Heap::pAllocate(CN_JOB_DROPSCHEMA));
  
  return pJob_DropSchema;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_DropSchema::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_DropSchema::operator delete(void* aAddress)
{
  Job_DropSchema* pJob_DropSchema = static_cast<Job_DropSchema*>(aAddress);
  
  Heap::vFree(pJob_DropSchema);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_DropSchema::Job_DropSchema(Interface::SEitf* pSEitf)
: Job(CN_JOB_DROPSCHEMA), JobDicPath(pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPSCHEMA,_FOOTER_);
    
  //m_sExecute = "Job_DropSchema.execute"_Stz;
  
  _checkInitialised_(CN_JOB_DROPSCHEMA);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPSCHEMA,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_DropSchema::~Job_DropSchema(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPSCHEMA,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_DropSchema::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_DROPSCHEMA == eClass);
  RASSERT(pObject->eClass() == CN_JOB_DROPSCHEMA);
  
  Job_DropSchema* pJob_DropSchema = _pJob_DropSchema(pObject);

  pJob_DropSchema->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_DropSchema::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPSCHEMA,_FOOTER_);
  
                                                               //! Alka to write
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_DropSchema::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPSCHEMA);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_DropSchema_pre(Interface::SEitf* pSEitf)
{
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_DropSchema_post(Prompt::SePrompt* pPrompt, Job* pJob)
{
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
