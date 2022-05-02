/*============================================================================*/
/* Steps_PageOut.cpp -- Page out step group                                   */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"
#include "CmdLineArgs.hpp"


extern CmdLineArgs gxCmdLineArgs;
extern Stz *gsBuildGitHash;


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_PageOut(void) {
  vAddStep("PageOut::testPrint"_Stz, &Step::vPageOutTestPrint);
}

/*----------------------------------------------------------------------------*/
/* PageOut::testPrint -- test print for page out demon                        */
/*----------------------------------------------------------------------------*/
void Step::vPageOutTestPrint(void) {
  ENTRY();
  printf("Page Out Demon test print...\n");
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/