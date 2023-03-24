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
  vAddStep("PageOut::pageOutPoolBuffer"_Stz, &Step::vPageOutPoolBuffer);
  vAddStep("PageOut::pageOutCrwBuffer"_Stz, &Step::vPageOutCrwBuffer);
  vAddStep("PageOut::pageOutCheckLowMemory"_Stz, &Step::vPageOutCheckLowMemory);
  vAddStep("PageOut::testPrint"_Stz, &Step::vPageOutTestPrint);
}

/*----------------------------------------------------------------------------*/
/* PageOut::pageOutPoolBuffer -- page out pool buffer to page file on disk    */
/*----------------------------------------------------------------------------*/
void Step::vPageOutPoolBuffer(void) {
  ENTRY();

  //---------------------------------------------------------------------------
  // Pop an operand address off the stack. It may or may not be a BufferId.
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));
  vTrace("PageOut Demon: Received RowBuffer: %p\n"_Stz, pOperand);
  RASSERT(pOperand->eGetClassNo() == BASE::CN_ROWBUFFER);

  RowBuffer* pRowBuffer = _pRowBuffer(pOperand);

  // Let the FileMap decide where it should be written to.
  AM::pFileMap()->vToDisk((Operand*) pRowBuffer->oGetBufferId().m_pvAsAddress);

  m_pDataStack->vPushPtr((Object*)pRowBuffer->oGetBufferId().m_pvAsAddress);
  AM::vDeleteOperand(pOperand,nullptr);
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PageOut::pageOutCrwBuffer -- page out crw buffer to page file on disk      */
/*----------------------------------------------------------------------------*/
void Step::vPageOutCrwBuffer(void) {
  ENTRY();

  //---------------------------------------------------------------------------
  // Pop an operand address off the stack. It may or may not be a BufferId.
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));
  vTrace("PageOut Demon: Received CrwBuffer: %p\n"_Stz, pOperand);
  RASSERT(pOperand->eGetClassNo() == BASE::CN_CRWBUFFER);

  CrwBuffer* pCrwBuffer = _pCrwBuffer(pOperand);

  // Let the FileMap decide where it should be written to.
  AM::pFileMap()->vToDisk((Operand*) pCrwBuffer->oGetBufferId().m_pvAsAddress);

  m_pDataStack->vPushPtr((Object*)pCrwBuffer->oGetBufferId().m_pvAsAddress);
  AM::vDeleteOperand(pOperand,nullptr);
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PageOut::pageOutCheckLowMemory -- page out when memory has exceeded a threshold */
/*----------------------------------------------------------------------------*/
void Step::vPageOutCheckLowMemory(void) {
  ENTRY();

  d8 dUsedSpacePercent = gpBufferPool->dUsedSpacePercent();
  if( dUsedSpacePercent < C_MAX_MEMORY_LIMIT) {
    return;
  }
  vTrace("PageOut Demon: Memory usage above %f%%, paging out buffers to free memory\n"_Stz, C_MAX_MEMORY_LIMIT);
  
  // Figure out how many buffers to page out, to get down to 50% usage
  i8 nTotal = gpBufferPool->nBufferCount() * ((C_MAX_MEMORY_LIMIT - 50)/100);

  HashMap* pHashMap = AM::pHashMap();
  Paging::vRandomAlgo(pHashMap, nTotal);

//  RASSERT(gpBufferPool->dUsedSpacePercent() < 60);
  vTrace("PageOut Demon: Freed up %lld buffers\n"_Stz, nTotal);
  vTrace("PageOut Demon: Current usage is %f%%\n"_Stz, gpBufferPool->dUsedSpacePercent());

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
