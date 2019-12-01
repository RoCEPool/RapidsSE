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
  
//  Object* pJobPtr = m_pDataStack->pPopPtr(__func__); 
//  Job_AnalyzeIndex* pJoAnalyzeIndex = _pJob_AnalyzeIndex(pJobPtr);
//  ASSERT(pJoAnalyzeIndex->eClass() == CN_JOB_ANALYZEINDEX);
  
//  m_pDataStack->vPushInt(pJoAnalyzeIndex->m_bIsDetailed);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobAnalyzeTable.isDetailed -- is it a detailed analysis of the index       */
/*----------------------------------------------------------------------------*/
void Step::vJob_AnalyzeTableIsDetailed(void)
{
  ENTRY(); // ENTRY("vJob_AnalyzeTableIsDetailed"_Stz);
  
  //Job_AnalyzeTable* pJoAnalyzeTable = _pJob_AnalyzeTable(m_pDataStack->pPopPtr(__func__));
  //ASSERT(pJoAnalyzeTable->eClass() == CN_JOB_ANALYZETABLE);
  
  //m_pDataStack->vPushInt(pJoAnalyzeTable->m_bIsDetailed);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
