/*============================================================================*/
/* Job_CreateCatalog.cpp -- Create Catalog Job                                */
/*============================================================================*/

#include "JobModel.h"

JobNote mJobNote_CreateCatalog (
  "Job_CreateCatalog"_Stz, // name of the job class
  "comment"_Stz,           // comment about the job
  "create"_Stz,            // the prompt command
  "catalog"_Stz,           // the second word
  "seed"_Stz,              // the task seed for this job
  bJob_CreateCatalog_pre,  // prompt pre-process function
  vJob_CreateCatalog_post  // prompt post-process function
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_CreateCatalog* JobsModel::_pJob_CreateCatalog(void* aAddress)
{
  Job_CreateCatalog* pJob_CreateCatalog = reinterpret_cast<Job_CreateCatalog*>(aAddress);
  
  //RASSERT( !pJob_CreateSchema || CN_JOB_CREATECATALOG == pJob_CreateSchema->eClass());
  
  return pJob_CreateCatalog;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_CreateCatalog::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_CreateCatalog* pJob_CreateCatalog =
    static_cast<Job_CreateCatalog*>(Heap::pAllocate(CN_JOB_CREATECATALOG));
  
  return pJob_CreateCatalog;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_CreateCatalog::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_CreateCatalog::operator delete(void* aAddress)
{
  Job_CreateCatalog* pJob_CreateCatalog = static_cast<Job_CreateCatalog*>(aAddress);
  
  Heap::vFree(pJob_CreateCatalog);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_CreateCatalog::Job_CreateCatalog(Interface::SEitf* pSEitf)
: Job(CN_JOB_CREATESCHEMA), JobDicPath(pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATECATALOG,_FOOTER_);
    
  m_sExecute = "Job_CreateCatalog.execute"_Stz;
  
  _checkInitialised_(CN_JOB_CREATECATALOG);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATECATALOG,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_CreateCatalog::~Job_CreateCatalog(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATECATALOG,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_CreateCatalog::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_CREATECATALOG == eClass);
  RASSERT(pObject->eClass() == CN_JOB_CREATECATALOG);
  
  Job_CreateCatalog* pJob_CreateCatalog = _pJob_CreateCatalog(pObject);

  pJob_CreateCatalog->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_CreateCatalog::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATECATALOG,_FOOTER_);
  
                                                               //! Alka to write
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_CreateCatalog::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATECATALOG);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Prompt ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
bool JobsModel::bJob_CreateCatalog_pre(TokenVec* pTokenVec)
{
  return true;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_CreateCatalog_post(void)
{
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
