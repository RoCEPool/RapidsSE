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
  
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobAnalyzeTable.isDetailed -- is it a detailed analysis of the index       */
/*----------------------------------------------------------------------------*/
void Step::vJob_AnalyzeTableIsDetailed(void)
{
  ENTRY(); // ENTRY("vJob_AnalyzeTableIsDetailed"_Stz);
  
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
