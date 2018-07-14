/*============================================================================*/
/* Job_DropTable.cpp -- drop table Job                                        */
/*============================================================================*/

#include "JobModel.h"

JobNote mJobNote_DropTable (
  "Job_DropTable"_Stz,  // name of the job class
  "comment"_Stz,        // comment about the job
  "drop"_Stz,           // the prompt command
  "table"_Stz,          // the second word
  "seed"_Stz,           // the task seed for this job
  pJob_DropTable_pre,   // prompt pre-process function
  vJob_DropTable_post   // prompt post-process function
  );


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_DropTable* JobsModel::_pJob_DropTable(void* aAddress)
{
  Job_DropTable* pJob_DropTable = reinterpret_cast<Job_DropTable*>(aAddress);
  
  RASSERT( !pJob_DropTable || CN_JOB_DROPTABLE == pJob_DropTable->eClass());
  
  return pJob_DropTable;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_DropTable::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_DropTable* pJob_DropTable =
    static_cast<Job_DropTable*>(Heap::pAllocate(CN_JOB_DROPTABLE));
  
  return pJob_DropTable;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_DropTable::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_DropTable::operator delete(void* aAddress)
{
  Job_DropTable* pJob_DropTable = static_cast<Job_DropTable*>(aAddress);
  
  Heap::vFree(pJob_DropTable);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_DropTable::Job_DropTable(Interface::SEitf* pSEitf)
: Job(CN_JOB_DROPTABLE), JobDicPath(pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPTABLE,_FOOTER_);
    
  m_sExecute = "Job_DropTable.execute"_Stz;
  
  _checkInitialised_(CN_JOB_DROPTABLE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPTABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_DropTable::~Job_DropTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPTABLE,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_DropTable::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_DROPTABLE == eClass);
  RASSERT(pObject->eClass() == CN_JOB_DROPTABLE);
  
  Job_DropTable* pJob_DropTable = _pJob_DropTable(pObject);

  pJob_DropTable->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_DropTable::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPTABLE,_FOOTER_);
  
                                                               //! Alka to write
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_DropTable::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPTABLE);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_DropTable_pre(Prompt::SePrompt* pPrompt)
{
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_DropTable_post(Prompt::SePrompt* pPrompt)
{
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
