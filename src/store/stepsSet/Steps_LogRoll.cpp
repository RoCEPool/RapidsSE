/*============================================================================*/
/* Steps_LogRoll.cpp -- Log roll step group                                   */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"
#include "Engine.hpp"

extern Stz *gsBuildGitHash;
extern AttachMap*    gpAttachMap;
extern File*         gpTraceFile;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_LogRoll(void) {
  vAddStep("LogRoll::traceRoll"_Stz, &Step::vLogRollTraceRoll);
}

/*----------------------------------------------------------------------------*/
/* LogRoll::traceRoll -- roll over log file                                   */
/*----------------------------------------------------------------------------*/
void Step::vLogRollTraceRoll(void) {
  ENTRY();
  if ( ! gpTraceFile || ! gpAttachMap) {
    return;
  }

  Engine::vRollTrace();

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
