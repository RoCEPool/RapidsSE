/*============================================================================*/
/* Steps_PageIn.cpp -- Page in step group                                     */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"
#include "CmdLineArgs.hpp"
#include "PageFile.hpp"
#include "LogicUnit.hpp"

extern CmdLineArgs gxCmdLineArgs;
extern Stz *gsBuildGitHash;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_PageIn(void) {
  vAddStep("PageIn::testPrint"_Stz, &Step::vPageInTestPrint);
  vAddStep("PageIn::PageInPoolBuffer"_Stz, &Step::vPageInPoolBuffer);
}

/*----------------------------------------------------------------------------*/
/* PageIn::PageInPoolBuffer -- page in pool buffer from page file on disk     */
/*----------------------------------------------------------------------------*/
void Step::vPageInPoolBuffer(void) {
  ENTRY();

  //---------------------------------------------------------------------------
  // Pop an operand address off the stack. It is actually a Buffer Id
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));

  //---------------------------------------------------------------------------
  // Temporary for testing
  RowBuffer* pRowBuffer = _pRowBuffer(pOperand);

  BufferId oBufferId = pRowBuffer->oGetBufferId();
//  oBufferId.m_iAsI8=0;
  AM::vDeleteOperand(pOperand,nullptr);

  RASSERT(pOperand->eGetClassNo() == BASE::CN_FREECHUNK);
  //---------------------------------------------------------------------------


  // Ask the FileMap to get the buffer from disk.
  RowBuffer* pRowBufferFromDisk = AM::pFileMap()->pFromDisk(oBufferId);
  RASSERT(pRowBufferFromDisk->eGetClassNo() == BASE::CN_ROWBUFFER);

  vTrace("PageIn Operator: Read buffer from disk %p\n"_Stz, pRowBufferFromDisk);

  ASSERT(pRowBufferFromDisk != nullptr);

  m_pLogicUnit->m_xpRowBuffer = pRowBufferFromDisk;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PageIn::testPrint -- test print for page in demon                          */
/*----------------------------------------------------------------------------*/
void Step::vPageInTestPrint(void) {
  ENTRY();

  BufferId oBufferId;
  oBufferId.m_iAsI8 = m_pDataStack->iPopAsInt(__func__ );

  Task* pTask = _pTask(m_pDataStack->iPopAsInt(__func__));

  vTrace("Page In got TaskPtr %p and BufferId %p\n"_Stz, pTask, oBufferId.m_pvAsAddress);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/