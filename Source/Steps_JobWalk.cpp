/*============================================================================*/
/* Steps_JobExport.cpp - jobExport step group                                 */
/*============================================================================*/

#include "Steps.h"

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
void Step::vJob_WalkBegin(Forth* pForth)
{
  ENTRY("vJob_WalkBegin"_Stz);
  
  Object* pPtrJobWalk;
  if ( ! pForth->m_pDataStack->bPop(pPtrJobWalk) )
    THROW_DSTACK();
  Job_Walk* pJob_Walk = _pJob_Walk(pPtrJobWalk);
  
  bool bBegun = pJob_Walk->bBegin();
  
  pForth->m_pDataStack->vPushBool(bBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_walk.end -- end walk                                                   */
/*----------------------------------------------------------------------------*/
void Step::vJob_WalkEnd(Forth* pForth)
{
  ENTRY("vJob_WalkEnd"_Stz);
  
  Object* pPtrJobWalk;
  if ( ! pForth->m_pDataStack->bPop(pPtrJobWalk) )
    THROW_DSTACK();
  Job_Walk* pJob_Walk = _pJob_Walk(pPtrJobWalk);
  
  pJob_Walk->vEnd();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_walk.index! -- store index in the job                                  */
/*----------------------------------------------------------------------------*/
void Step::vJob_WalkIndexStore(Forth* pForth)
{
  ENTRY("vJob_WalkIndexStore"_Stz);
  
  Object* pPtrJobWalk;
  if ( ! pForth->m_pDataStack->bPop(pPtrJobWalk) )
    THROW_DSTACK();
  Job_Walk* pJob_Walk = _pJob_Walk(pPtrJobWalk);
  
  Object* pIndexPtr;
  if ( ! pForth->m_pDataStack->bPop(pIndexPtr) )
    THROW_DSTACK();
  Index* pIndex = _pIndex(pIndexPtr);
  
  pJob_Walk->m_pIndex = pIndex;
  
  ////ActLog::vAdd(pJob_Walk,AN_INDEX_USE,pIndex->aIndexName());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_walk.table! -- store table in the job                                 */
/*----------------------------------------------------------------------------*/
void Step::vJob_WalkTableStore(Forth* pForth)
{
  ENTRY("vJob_WalkTableStore"_Stz);
  
  Object* pPtrJobWalk;
  if ( ! pForth->m_pDataStack->bPop(pPtrJobWalk) )
    THROW_DSTACK();
  Job_Walk* pJob_Walk = _pJob_Walk(pPtrJobWalk);
  
  Object* pTablePtr;
  if ( ! pForth->m_pDataStack->bPop(pTablePtr) )
    THROW_DSTACK();
  Table* pTable = _pTable(pTablePtr);
  
  pJob_Walk->m_pTable = pTable;
    
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
