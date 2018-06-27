/*============================================================================*/
/* Job_DropCatalog.cpp -- Drop Catalog Job                                    */
/*============================================================================*/

#include "JobModel.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_DropCatalog* JobsModel::_pJob_DropCatalog(void* aAddress)
{
  Job_DropCatalog* pJob_DropCatalog = reinterpret_cast<Job_DropCatalog*>(aAddress);
  
  RASSERT( !pJob_DropCatalog || CN_JOB_DROPCATALOG == pJob_DropCatalog->eClass());
  
  return pJob_DropCatalog;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_DropCatalog::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_DropCatalog* pJob_DropSchema =
    static_cast<Job_DropCatalog*>(Heap::pAllocate(CN_JOB_DROPCATALOG));
  
  return pJob_DropSchema;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_DropCatalog::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_DropCatalog::operator delete(void* aAddress)
{
  Job_DropCatalog* pJob_DropCatalog = static_cast<Job_DropCatalog*>(aAddress);
  
  Heap::vFree(pJob_DropCatalog);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_DropCatalog::Job_DropCatalog(Interface::SEitf* pSEitf)
: Job(CN_JOB_DROPSCHEMA), JobDicPath(pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPCATALOG,_FOOTER_);
    
  m_sExecute = "Job_DropSchema.execute"_Stz;
  
  _checkInitialised_(CN_JOB_DROPSCHEMA);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPCATALOG,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_DropCatalog::~Job_DropCatalog(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPCATALOG,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_DropCatalog::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_DROPCATALOG == eClass);
  RASSERT(pObject->eClass() == CN_JOB_DROPCATALOG);
  
  Job_CreateCatalog* pJob_CreateCatalog = _pJob_CreateCatalog(pObject);

  pJob_CreateCatalog->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_DropCatalog::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPCATALOG,_FOOTER_);
  
                                                               //! Alka to write
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_DropCatalog::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPCATALOG);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
