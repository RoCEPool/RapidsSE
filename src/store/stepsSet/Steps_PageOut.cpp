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

  pHashMap->m_okLock.vGrabWrite();

  // Ensure we have enough buffers to page out
  RASSERT(pHashMap->m_uNoOfEntries > nTotal);

  u8 uSize = pHashMap->m_pHashMapSlot->uGetNoSlots();

  i8 nPagedOut = 0;
  while(nPagedOut < nTotal)
  {
    // Start iterating the hashmap from a random point
    for(u8 i = arc4random() % uSize; i < uSize; i++) {
      HashMapEntry* pEntry = pHashMap->pGetEntryAtIndex(i);

      if(pEntry == nullptr)
        continue;

      // Check if this entry is in use (locked)
      if(pEntry->m_okLock.bIsLocked())
        continue;
      pEntry->m_okLock.vGrabWrite();

      // Check if this entry is queued for output
      DataBuffer* pDataBuffer = _pDataBuffer(pEntry->m_pValObject);
      if(pDataBuffer->bOutputQueued()) {
        pEntry->m_okLock.vDropWrite();
        continue;
      }

      // remove this entry from the hashmap by setting the current index
      // to point to the next entry
      pHashMap->m_pHashMapSlot->m_pEntry[i] = pEntry->m_pNext;

      // Write the buffer to disk
      AM::pFileMap()->vToDisk(pDataBuffer);

      // HACK: to prevent dead lock, since the delete of databuffer will try to grab
      // a lock on the hashmap
      BufferId oBufferId;
      oBufferId.m_iAsI8 = 0;
      pDataBuffer->vSetBufferId(oBufferId);

      switch(pDataBuffer->eGetClassNo()) {
        case BASE::CN_ROWBUFFER:
          delete _pRowBuffer(pDataBuffer);
          break;

        case BASE::CN_CRWBUFFER:
          delete _pCrwBuffer(pDataBuffer);
          break;

        default:
          NEVER_REACH;
      }

      pEntry->m_okLock.vDropWrite();
      delete pEntry;

      pHashMap->m_uNoOfEntries--;

      nPagedOut++;
      if(nPagedOut >= nTotal)
        break;
    }
  }

  pHashMap->m_okLock.vDropWrite();

  RASSERT(gpBufferPool->dUsedSpacePercent() < 60);
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
