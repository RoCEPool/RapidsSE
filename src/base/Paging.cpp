/*============================================================================*/
/* Paging.hpp -- Paging                                                       */
/*============================================================================*/
#include "BASE.h"
#include "Paging.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Paging::Paging(void)
{
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Paging::~Paging(void) noexcept(false)
{
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void Paging::vRandomAlgo(i8 nTotal)
{
  _static_

  FileMap* pFileMap = AM::pFileMap();
  HashMap* pHashMap = AM::pHashMap();


  pFileMap->vStartPageOut();
  pHashMap->m_okLock.vGrabWrite();

  u8 uSize = pHashMap->m_pHashMapSlot->uGetNoSlots();

  i8 iTimeOut = 0;
  i8 nPagedOut = 0;
  while(nPagedOut < nTotal)
  {
    iTimeOut++;
    if(iTimeOut > 3)
      break;

    i8 iRand = rand()% uSize;
    // Start iterating the hashmap from a random point
    for(u8 i = iRand; i < uSize; i++) {
      HashMapEntry* pEntry = pHashMap->pGetEntryAtIndex(i);

      if(pEntry == nullptr)
        continue;

      // Check if this entry is in use (locked)
      if(pEntry->m_okLock.bIsLocked())
        continue;
      pEntry->m_okLock.vGrabWrite();

      // Check if this entry is queued for output
      DataBuffer* pDataBuffer = _pDataBuffer(pEntry->m_pValObject);
      if(pDataBuffer->eGetClassNo() == CN_ROWBUFFER ||
         pDataBuffer->bOutputQueued()) {
        pEntry->m_okLock.vDropWrite();
        continue;
      }

      // remove this entry from the hashmap by setting the current index
      // to point to the next entry
      pHashMap->m_pHashMapSlot->m_pEntry[i] = pEntry->m_pNext;

      // Write the buffer to disk
      pFileMap->vToDisk(pDataBuffer);

      // HACK: to prevent dead lock, since the delete of databuffer will try to grab
      // a lock on the hashmap
      pDataBuffer->vSetBufferId(nullptr);

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

  pFileMap->vEndPageOut();

//  Thread* pThread = Thread::pThisTLSThread();
//  pThread->pPsxThd()->vSleepSsec(30,CN_WORD);

  pHashMap->m_okLock.vDropWrite();
  pFileMap->vDropWriteLock();

  return;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/