/*============================================================================*/
/* Job_CreateIndex.cpp -- Create Index Job                                    */
/*============================================================================*/

#include "JobModel.h"

JobNote mJobNote_CreateIndex (
  "Job_CreateIndex"_Stz, // name of the job class
  CN_JOB_CREATEINDEX,    // class enum
  "comment"_Stz,         // comment about the job
  "create"_Stz,          // the prompt command
  "index"_Stz,           // the second word
  "createIndexR35_req"_Stz, // the task seed for this job
  false,                 // detached job?
  pJob_CreateIndex_pre,  // prompt pre-process function
  vJob_CreateIndex_post  // prompt post-process function
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_CreateIndex* JobsModel::_pJob_CreateIndex(void* aAddress)
{
  Job_CreateIndex* pJob_CreateIndex = reinterpret_cast<Job_CreateIndex*>(aAddress);
  
  RASSERT( !pJob_CreateIndex || CN_JOB_CREATEINDEX == pJob_CreateIndex->eClass());
  
  return pJob_CreateIndex;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_CreateIndex::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_CreateIndex* pJob_CreateIndex =
    static_cast<Job_CreateIndex*>(Heap::pAllocate(CN_JOB_CREATEINDEX));
  
  return pJob_CreateIndex;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_CreateIndex::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_CreateIndex::operator delete(void* aAddress)
{
  Job_CreateIndex* pJob_CreateIndex = static_cast<Job_CreateIndex*>(aAddress);
  
  Heap::vFree(pJob_CreateIndex);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_CreateIndex::Job_CreateIndex(Interface::SEitf* pSEitf)
: Job(CN_JOB_CREATEINDEX), JobDicPath(pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATEINDEX,_FOOTER_);
  
  m_sExecute = "Job_CreateIndex.execute"_Stz;
  
  m_sTableName = ""_Stz;
  m_bIsUnique  = false;
  m_iColumns   = 0;
  
  for (i8 i_Column = 0; i_Column < C_MAX_COLUMNS; i_Column++) {
    m_sColumnName[i_Column] = ""_Stz;
  }
  
  _checkInitialised_(CN_JOB_CREATEINDEX);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATEINDEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_CreateIndex::~Job_CreateIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATEINDEX,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_CreateIndex::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_CREATEINDEX == eClass);
  RASSERT(pObject->eClass() == CN_JOB_CREATEINDEX);
  
  Job_CreateIndex* pJob_CreateIndex = _pJob_CreateIndex(pObject);

  pJob_CreateIndex->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_CreateIndex::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATEINDEX,_FOOTER_);
  
  vTrace("\n\nJob_CreateIndex: %s\n"_Stz, m_sObjectName.aChar());
  
  vTrace("m_sExecute = %s\n"_Stz,m_sExecute.aChar());
  vTrace("m_sTableName = %s\n"_Stz,m_sTableName.aChar());
  vTrace("m_iColumns = %lld\n"_Stz,m_iColumns);
  
  for (i8 i_Column = 0; i_Column < m_iColumns; i_Column++) {
    vTrace("m_sColumnName[%lld] = %s\n"_Stz, i_Column, m_sColumnName[i_Column].aChar());
  }
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_CreateIndex::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATEINDEX);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_CreateIndex_pre(Prompt::SePrompt* pPrompt)
{
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_CreateIndex_post(Prompt::SePrompt* pPrompt, Job* pJob)
{
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
