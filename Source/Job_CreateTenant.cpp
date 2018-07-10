/*============================================================================*/
/* Job_CreateTenant.cpp -- Create Tennant Job                                 */
/*============================================================================*/

#include "JobModel.h"

JobNote mJobNote_CreateTenant (
  "Job_CreateTenant"_Stz,  // name of the job class
  "comment"_Stz,           // comment about the job
  "create tenant"_Stz,     // the prompt command
  "seed"_Stz               // the task seed for this job
);


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_CreateTenant* JobsModel::_pJob_CreateTenant(void* aAddress)
{
  Job_CreateTenant* pJob_CreateTenant = reinterpret_cast<Job_CreateTenant*>(aAddress);
  
  //RASSERT( !pJob_CreateTenant || CN_JOB_CREATETENANT == pJob_CreateTenant->eClass());
  
  return pJob_CreateTenant;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_CreateTenant::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_CreateTenant* pJob_CreateTenant =
    static_cast<Job_CreateTenant*>(Heap::pAllocate(CN_JOB_CREATETENANT));
  
  return pJob_CreateTenant;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_CreateTenant::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_CreateTenant::operator delete(void* aAddress)
{
  Job_CreateTenant* pJob_CreateTenant = static_cast<Job_CreateTenant*>(aAddress);
  
  Heap::vFree(pJob_CreateTenant);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_CreateTenant::Job_CreateTenant(Interface::SEitf* pSEitf)
: Job(CN_JOB_CREATESCHEMA), JobDicPath(pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATETENANT,_FOOTER_);
    
  m_sExecute = "Job_CreateTenant.execute"_Stz;
  
  _checkInitialised_(CN_JOB_CREATETENANT);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATETENANT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_CreateTenant::~Job_CreateTenant(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATETENANT,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_CreateTenant::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_CREATETENANT == eClass);
  RASSERT(pObject->eClass() == CN_JOB_CREATETENANT);
  
  Job_CreateTenant* pJob_CreateTenant = _pJob_CreateTenant(pObject);

  pJob_CreateTenant->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_CreateTenant::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATETENANT,_FOOTER_);
  
                                                               //! Alka to write
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_CreateTenant::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATETENANT);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
