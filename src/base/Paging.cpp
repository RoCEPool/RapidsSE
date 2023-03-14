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
void Paging::vRandomAlgo(HashMap* pHashMap, i8 nTotal)
{
  _static_

  pHashMap->m_okLock.vGrabWrite();

  // Ensure we have enough buffers in the HashMap to page out
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

  pHashMap->m_okLock.vDropWrite();
  return;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/