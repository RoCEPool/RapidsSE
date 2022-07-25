/*============================================================================*/
/* Steps_LogRoll.cpp -- Log roll step group                                   */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

extern Stz *gsBuildGitHash;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_LogRoll(void) {
  vAddStep("LogRoll::testPrint"_Stz, &Step::vLogRollTestPrint);
}

/*----------------------------------------------------------------------------*/
/* LogRoll::testPrint -- test print for logging demon                         */
/*----------------------------------------------------------------------------*/
void Step::vLogRollTestPrint(void) {
  ENTRY();
  vTrace("Log Roll Demon test print...\n"_Stz);
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
