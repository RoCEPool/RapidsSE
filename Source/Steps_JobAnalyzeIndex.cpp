/*============================================================================*/
/* Steps_job.cpp - job step group                                             */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap* gpAttachMap;

// job: ////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_JobAnalyzeIndex(void)
{
  vAddPrimitive("jobAnalyzeIndex.isDetailed"_Stz, &vJobAnalyzeIndexIsDetailed, false);
  vAddPrimitive("jobAnalyzeTable.isDetailed"_Stz, &vJobAnalyzeTableIsDetailed, false);
}

/*----------------------------------------------------------------------------*/
/* jobAnalyzeIndex.isDetailed -- is it a detailed analysis of the index       */
/*----------------------------------------------------------------------------*/
void Task::vJobAnalyzeIndexIsDetailed(MP_Task mpTask)
{
  ENTRY("vJobAnalyzeIndexIsDetailed"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("jobAnalyzeIndex.isDetailed"_Stz);
  
  Job_AnalyzeIndex* pJoAnalyzeIndex = _pJob_AnalyzeIndex(pJobPtr);
  ASSERT(pJoAnalyzeIndex->eClass() == CN_JOB_ANALYZEINDEX);
  
  mpTask->m_pDataStack->vPushInt(pJoAnalyzeIndex->m_bIsDetailed);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobAnalyzeTable.isDetailed -- is it a detailed analysis of the index       */
/*----------------------------------------------------------------------------*/
void Task::vJobAnalyzeTableIsDetailed(MP_Task mpTask)
{
  ENTRY("vJobAnalyzeTableIsDetailed"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("jobAnalyzeTable.isDetailed"_Stz);
  
  Job_AnalyzeTable* pJoAnalyzeTable = _pJob_AnalyzeTable(pJobPtr);
  ASSERT(pJoAnalyzeTable->eClass() == CN_JOB_ANALYZETABLE);
  
  mpTask->m_pDataStack->vPushInt(pJoAnalyzeTable->m_bIsDetailed);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
