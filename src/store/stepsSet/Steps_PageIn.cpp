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
  vAddStep("PageIn::popRequest"_Stz, &Step::vPageInPopRequest);
}

/*----------------------------------------------------------------------------*/
/* PageIn::popRequest -- pop request received to page in buffer by task       */
/*----------------------------------------------------------------------------*/
void Step::vPageInPopRequest(void) {
  ENTRY();
#ifdef WITH_PAGING
  BufferId oBufferId;
  oBufferId.m_iAsI8 = m_pDataStack->iPopAsInt(__func__ );
  RASSERT(oBufferId.m_bIdFlag == 1);

  Task* pReqTask = _pTask(m_pDataStack->iPopAsInt(__func__));

#ifdef WITH_PAGING_DEBUG
  vTrace("Page In got TaskPtr %p and BufferId %p\n"_Stz, pReqTask, oBufferId.m_pvAsAddress);
#endif

  FileMap* pFileMap = AM::pFileMap();
  HashMap* pHashMap = AM::pHashMap();

  // Ask the FileMap to get the buffer from disk.
  pFileMap->vStartPageIn();

  // Page in the requested BufferId
  DataBuffer* pDataBufferFromDisk = pFileMap->pDataBufferFromDisk(oBufferId);

  // Update hashmap with new address with this bufferId
  pHashMap->vInsertKeyValue(
      (void *) pDataBufferFromDisk->oGetBufferId().m_pvAsAddress,
      (Object *) pDataBufferFromDisk);

  pHashMap->vDropKeyLock(oBufferId.m_pvAsAddress);

  pFileMap->vEndPageIn();

#ifdef WITH_PAGING_DEBUG
  vTrace("PageIn Operator: Read buffer from disk %p\n"_Stz, pDataBufferFromDisk->oGetBufferId().m_pvAsAddress);
#endif

  // Wake up task
  AM::vGrabTaskLock(pReqTask,(Stz*)__func__);
  AM::TASK::vTriggerBidingPageIn(pReqTask, this);
#endif

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/