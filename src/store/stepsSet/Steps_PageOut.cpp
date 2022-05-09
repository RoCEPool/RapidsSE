/*============================================================================*/
/* Steps_PageOut.cpp -- Page out step group                                   */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"
#include "CmdLineArgs.hpp"
#include "PageFile.hpp"

extern CmdLineArgs gxCmdLineArgs;
extern Stz *gsBuildGitHash;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_PageOut(void) {
  vAddStep("PageOut::testPrint"_Stz, &Step::vPageOutTestPrint);
  vAddStep("PageOut::pageOutPoolBuffer"_Stz, &Step::vPageOutPoolBuffer);
}

/*----------------------------------------------------------------------------*/
/* PageOut::poolBuffer -- page out pool buffer to page file on disk           */
/*----------------------------------------------------------------------------*/
void Step::vPageOutPoolBuffer(void) {
  ENTRY();

  //---------------------------------------------------------------------------
  // Pop an operand address off the stack. It may or may not be a BufferId.
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));

  // Let the FileMap decide where it should be written to.
  AM::pFileMap()->vToDisk(pOperand);

  EXIT();
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