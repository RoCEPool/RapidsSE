/*============================================================================*/
/* Steps_JobAnalyzeIndex.cpp - jobAnalyzeIndex step group                     */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "StepTiming.hpp"


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
  ENTRY();
  
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobAnalyzeTable.isDetailed -- is it a detailed analysis of the index       */
/*----------------------------------------------------------------------------*/
void Step::vJob_AnalyzeTableIsDetailed(void)
{
  ENTRY();
  
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
