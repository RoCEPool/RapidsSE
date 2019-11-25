/*============================================================================*/
/* Steps_JobAnalyzeIndex.cpp - jobAnalyzeIndex step group                     */
/*============================================================================*/

#include "Steps.h"

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
void Step::vJob_AnalyzeIndexIsDetailed(void)
{
  ENTRY(); // ENTRY("vJob_AnalyzeIndexIsDetailed"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopPtr(pJobPtr) )
    THROW_DSTACK();
  
//  Job_AnalyzeIndex* pJoAnalyzeIndex = _pJob_AnalyzeIndex(pJobPtr);
//  ASSERT(pJoAnalyzeIndex->eClass() == CN_JOB_ANALYZEINDEX);
  
//  m_pDataStack->vPushI8(pJoAnalyzeIndex->m_bIsDetailed);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobAnalyzeTable.isDetailed -- is it a detailed analysis of the index       */
/*----------------------------------------------------------------------------*/
void Step::vJob_AnalyzeTableIsDetailed(void)
{
  ENTRY(); // ENTRY("vJob_AnalyzeTableIsDetailed"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopPtr(pJobPtr) )
    THROW_DSTACK();
  
  //Job_AnalyzeTable* pJoAnalyzeTable = _pJob_AnalyzeTable(pJobPtr);
  //ASSERT(pJoAnalyzeTable->eClass() == CN_JOB_ANALYZETABLE);
  
  //m_pDataStack->vPushI8(pJoAnalyzeTable->m_bIsDetailed);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
