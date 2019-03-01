/*============================================================================*/
/* Steps_JobExport.cpp - jobExport step group                                 */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobWalk(void)
{
  vAddStep("job_walk.begin"_Stz,  &Step::vJob_WalkBegin);
  vAddStep("job_walk.end"_Stz,    &Step::vJob_WalkEnd);
  vAddStep("job_walk.index!"_Stz, &Step::vJob_WalkIndexStore);
  vAddStep("job_walk.table!"_Stz, &Step::vJob_WalkTableStore);
}

/*----------------------------------------------------------------------------*/
/* job_walk.begin -- begin walk                                               */
/*----------------------------------------------------------------------------*/
void Step::vJob_WalkBegin(MP_Task mpTask)
{
  ENTRY("vJob_WalkBegin"_Stz);
  
  Object* pPtrJobWalk;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJobWalk) )
    THROW_INVALID_DSTACK("job_walk.begin"_Stz);
  Job_Walk* pJob_Walk = _pJob_Walk(pPtrJobWalk);
  
  bool bBegun = pJob_Walk->bBegin();
  
  mpTask->m_pDataStack->vPushBool(bBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_walk.end -- end walk                                                   */
/*----------------------------------------------------------------------------*/
void Step::vJob_WalkEnd(MP_Task mpTask)
{
  ENTRY("vJob_WalkEnd"_Stz);
  
  Object* pPtrJobWalk;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJobWalk) )
    THROW_INVALID_DSTACK("job_walk.end"_Stz);
  Job_Walk* pJob_Walk = _pJob_Walk(pPtrJobWalk);
  
  pJob_Walk->vEnd();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_walk.index! -- store index in the job                                  */
/*----------------------------------------------------------------------------*/
void Step::vJob_WalkIndexStore(MP_Task mpTask)
{
  ENTRY("vJob_WalkIndexStore"_Stz);
  
  Object* pPtrJobWalk;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJobWalk) )
    THROW_INVALID_DSTACK("Job_walk.index"_Stz);
  Job_Walk* pJob_Walk = _pJob_Walk(pPtrJobWalk);
  
  Object* pIndexPtr;
  if ( ! mpTask->m_pDataStack->bPop(pIndexPtr) )
    THROW_INVALID_DSTACK("Job_walk.index!"_Stz);
  Index* pIndex = _pIndex(pIndexPtr);
  
  pJob_Walk->m_pIndex = pIndex;
  
  //ActionSequence::vAdd(pJob_Walk,AN_INDEX_USE,pIndex->aIndexName());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_walk.table! -- store table in the job                                 */
/*----------------------------------------------------------------------------*/
void Step::vJob_WalkTableStore(MP_Task mpTask)
{
  ENTRY("vJob_WalkTableStore"_Stz);
  
  Object* pPtrJobWalk;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJobWalk) )
    THROW_INVALID_DSTACK("Job_walk.table!"_Stz);
  Job_Walk* pJob_Walk = _pJob_Walk(pPtrJobWalk);
  
  Object* pTablePtr;
  if ( ! mpTask->m_pDataStack->bPop(pTablePtr) )
    THROW_INVALID_DSTACK("Job_walk.table!"_Stz);
  Table* pTable = _pTable(pTablePtr);
  
  pJob_Walk->m_pTable = pTable;
  
  ActionSequence::vAdd(pJob_Walk,AN_TABLE_USE,pTable->aTableName());
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
