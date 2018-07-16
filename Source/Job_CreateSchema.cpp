/*============================================================================*/
/* Job_CreateSchema.cpp -- Create Schema Job                                  */
/*============================================================================*/

#include "JobModel.h"

JobNote mJobNote_CreateSchema (
  "Job_CreateSchema"_Stz, // name of the job class
  CN_JOB_CREATESCHEMA,    // class enum
  "comment"_Stz,          // comment about the job
  "create"_Stz,           // the prompt command
  "schema"_Stz,           // the second word
  "createSchemaR35_req"_Stz, // the task seed for this job
  false,                   // detached job?
  pJob_CreateSchema_pre,  // prompt pre-process function
  vJob_CreateSchema_post  // prompt post-process function
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_CreateSchema* JobsModel::_pJob_CreateSchema(void* aAddress)
{
  Job_CreateSchema* pJob_CreateSchema = reinterpret_cast<Job_CreateSchema*>(aAddress);
  
  //RASSERT( !pJob_CreateSchema || CN_JOB_CREATESCHEMA == pJob_CreateSchema->eClass());
  
  return pJob_CreateSchema;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_CreateSchema::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_CreateTable* pJob_CreateSchema =
    static_cast<Job_CreateTable*>(Heap::pAllocate(CN_JOB_CREATESCHEMA));
  
  return pJob_CreateSchema;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_CreateSchema::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_CreateSchema::operator delete(void* aAddress)
{
  Job_CreateSchema* pJob_CreateSchema = static_cast<Job_CreateSchema*>(aAddress);
  
  Heap::vFree(pJob_CreateSchema);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_CreateSchema::Job_CreateSchema(Interface::SEitf* pSEitf)
: Job(CN_JOB_CREATESCHEMA), JobDicPath(pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATESCHEMA,_FOOTER_);
    
  //m_sExecute = "Job_CreateSchema.execute"_Stz;
  
  _checkInitialised_(CN_JOB_CREATESCHEMA);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATESCHEMA,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_CreateSchema::~Job_CreateSchema(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATESCHEMA,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_CreateSchema::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_CREATESCHEMA == eClass);
  RASSERT(pObject->eClass() == CN_JOB_CREATESCHEMA);
  
  Job_CreateSchema* pJob_CreateSchema = _pJob_CreateSchema(pObject);

  pJob_CreateSchema->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_CreateSchema::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATESCHEMA,_FOOTER_);
  
                                                               //! Alka to write
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_CreateSchema::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATESCHEMA);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_CreateSchema_pre(Prompt::SePrompt* pPrompt)
{
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_CreateSchema_post(Prompt::SePrompt* pPrompt, Job* pJob)
{
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
