/*============================================================================*/
/* Job_DropTenant.cpp -- Drop Tenant Job                                      */
/*============================================================================*/

#include "JobModel.h"

JobNote mJobNote_DropTenant (
  "Job_DropTenant"_Stz, // name of the job class
  CN_JOB_DROPTENANT,    // class enum
  "comment"_Stz,        // comment about the job
  "drop"_Stz,           // the prompt command
  "tenant"_Stz,         // the second word
  "dropTenantR35_req"_Stz, // the task seed for this job
  false,                // detached job?
  pJob_DropTenant_pre,  // prompt pre-process function
  vJob_DropTenant_post  // prompt post-process function
  );


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_DropTenant* JobsModel::_pJob_DropTenant(void* aAddress)
{
  Job_DropTenant* pJob_DropTenant = reinterpret_cast<Job_DropTenant*>(aAddress);
  
  RASSERT( !pJob_DropTenant || CN_JOB_DROPTENANT == pJob_DropTenant->eClass());
  
  return pJob_DropTenant;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_DropTenant::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_DropTenant* pJob_DropTenant =
    static_cast<Job_DropTenant*>(Heap::pAllocate(CN_JOB_DROPTENANT));
  
  return pJob_DropTenant;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_DropTenant::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_DropTenant::operator delete(void* aAddress)
{
  Job_DropTenant* pJob_DropTenant = static_cast<Job_DropTenant*>(aAddress);
  
  Heap::vFree(pJob_DropTenant);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_DropTenant::Job_DropTenant(Interface::SEitf* pSEitf)
: Job(CN_JOB_DROPSCHEMA), JobDicPath(pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPTENANT,_FOOTER_);
    
  //m_sExecute = "Job_DropSchema.execute"_Stz;
  
  _checkInitialised_(CN_JOB_DROPTENANT);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPTENANT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_DropTenant::~Job_DropTenant(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPTENANT,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_DropTenant::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_DROPSCHEMA == eClass);
  RASSERT(pObject->eClass() == CN_JOB_DROPTENANT);
  
  Job_DropTenant* pJob_DropTenant = _pJob_DropTenant(pObject);

  pJob_DropTenant->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_DropTenant::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPTENANT,_FOOTER_);
  
                                                               //! Alka to write
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_DropTenant::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPTENANT);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_DropTenant_pre(Interface::SEitf* pSEitf)
{
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_DropTenant_post(Prompt::SePrompt* pPrompt, Job* pJob)
{
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
