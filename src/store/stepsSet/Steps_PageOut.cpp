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

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_PageOut(void) {
  vAddStep("PageOut::pageOutCheckLowMemory"_Stz, &Step::vPageOutCheckLowMemory);
  vAddStep("PageOut::testPrint"_Stz, &Step::vPageOutTestPrint);
}

/*----------------------------------------------------------------------------*/
/* PageOut::pageOutCheckLowMemory -- page out when memory has exceeded a threshold */
/*----------------------------------------------------------------------------*/
void Step::vPageOutCheckLowMemory(void) {
  ENTRY();

#ifdef WITH_PAGING

  d8 dUsedSpacePercent = gpBufferPool->dUsedSpacePercent();
  if( dUsedSpacePercent < C_MAX_MEMORY_LIMIT) {
    return;
  }
#ifdef WITH_PAGING_DEBUG
  vTrace("PageOut Demon: Memory usage above %f%%, paging out buffers to free memory\n"_Stz, C_MAX_MEMORY_LIMIT);
#endif

  // Figure out how many buffers to page out, to get down to 50% usage
  i8 nTotal = gpBufferPool->nBufferCount() * ((C_MAX_MEMORY_LIMIT - 50)/100);

  Paging::vRandomAlgo(nTotal);
#ifdef WITH_PAGING_DEBUG
  vTrace("PageOut Demon: Freed up %lld buffers\n"_Stz, nTotal);
  vTrace("PageOut Demon: Current usage is %f%%\n"_Stz, gpBufferPool->dUsedSpacePercent());
#endif
#endif
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PageOut::testPrint -- test print for page out demon                        */
/*----------------------------------------------------------------------------*/
void Step::vPageOutTestPrint(void) {
  ENTRY();
  vTrace("Page Out Demon test print...\n"_Stz);
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
