/*============================================================================*/
/* Steps_JobAnalyzeIndex.cpp - jobAnalyzeIndex step group                     */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobAnalyzeIndex(void)
{
  vAddStep("jobAnalyzeIndex.isDetailed"_Stz, &Step::vJob_AnalyzeIndexIsDetailed);
  vAddStep("jobAnalyzeTable.isDetailed"_Stz, &Step::vJob_AnalyzeTableIsDetailed);
}

/*----------------------------------------------------------------------------*/
/* jobAnalyzeIndex.isDetailed -- is it a detailed analysis of the index       */
/*----------------------------------------------------------------------------*/
void Step::vJob_AnalyzeIndexIsDetailed(MP_Task mpTask)
{
  ENTRY("vJob_AnalyzeIndexIsDetailed"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("jobAnalyzeIndex.isDetailed"_Stz);
  
  Job_AnalyzeIndex* pJoAnalyzeIndex = _pJob_AnalyzeIndex(pJobPtr);
  ASSERT(pJoAnalyzeIndex->eClass() == CN_JOB_ANALYZEINDEX);
  
  mpTask->m_pDataStack->vPushI8(pJoAnalyzeIndex->m_bIsDetailed);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobAnalyzeTable.isDetailed -- is it a detailed analysis of the index       */
/*----------------------------------------------------------------------------*/
void Step::vJob_AnalyzeTableIsDetailed(MP_Task mpTask)
{
  ENTRY("vJob_AnalyzeTableIsDetailed"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("jobAnalyzeTable.isDetailed"_Stz);
  
  Job_AnalyzeTable* pJoAnalyzeTable = _pJob_AnalyzeTable(pJobPtr);
  ASSERT(pJoAnalyzeTable->eClass() == CN_JOB_ANALYZETABLE);
  
  mpTask->m_pDataStack->vPushI8(pJoAnalyzeTable->m_bIsDetailed);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
