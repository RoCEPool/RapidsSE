/*============================================================================*/
/* CreateTable_job.cpp -- Create Table Job                                    */
/*============================================================================*/

#include "JobsModel.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
CreateTable_Job* JobsModel::_pCreateTable_Job(void* aAddress)
{
  CreateTable_Job* pCreateTable_Job = reinterpret_cast<CreateTable_Job*>(aAddress);
  
  RASSERT( !pCreateTable_Job || CN_JOB == pCreateTable_Job->eClass());
  
  return pCreateTable_Job;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* CreateTable_Job::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  CreateTable_Job* pCreateTable_Job =
    static_cast<CreateTable_Job*>(Heap::pAllocate(CN_JOB));
  
  return pCreateTable_Job;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* CreateTable_Job::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void CreateTable_Job::operator delete(void* aAddress)
{
  CreateTable_Job* pCreateTable_Job = static_cast<CreateTable_Job*>(aAddress);
  
  Heap::vFree(pCreateTable_Job);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CreateTable_Job::CreateTable_Job(void)
: Job(CN_JOB)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB,_FOOTER_);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB,_FOOTER_);
  
  _checkInitialised_(CN_JOB);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CreateTable_Job::~CreateTable_Job(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump entry buffer                                                          */
/*----------------------------------------------------------------------------*/
void CreateTable_Job::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB == eClass);
  RASSERT(pObject->eClass() == CN_JOB);
  
  CreateTable_Job* pCreateTable_Job = _pCreateTable_Job(pObject);

  pCreateTable_Job->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show contents of buffer                                                    */
/*----------------------------------------------------------------------------*/
void CreateTable_Job::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
                                                               //! Alka to write
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* CreateTable_Job::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
