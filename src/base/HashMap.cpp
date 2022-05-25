/*============================================================================*/
/* HashMap.cpp -- HashMap                                                     */
/*============================================================================*/

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
HashMap* BASE::_pHashMap(void* aAddr)
{
  HashMap* pHashMap = reinterpret_cast<HashMap*>(aAddr);
  
  RASSERT_TYPE(pHashMap,CN_HASHMAP);
  
  return pHashMap;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* HashMap::operator new(size_t)
{
  void* pObject = Heaps::pAllocate(CN_HASHMAP);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void HashMap::operator delete(void* pHashMapAddress)
{
  HashMap* pHashMap = reinterpret_cast<HashMap*>(pHashMapAddress);
  
  Heaps::vFree(pHashMap);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
HashMap::HashMap(u8 uNoOfSlots)
: Object(CN_HASHMAP)
, m_okLock(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HASHMAP,_FOOTER_);
  m_uNoOfSlots          = uNoOfSlots;
  m_uNoOfEntries        = 0;

  m_paHashMapSlots = new HashMapSlot*[uNoOfSlots];

  for(u8 i = 0; i < m_uNoOfSlots; i++)
    m_paHashMapSlots[i] = nullptr;

#ifdef WITH_HASHMAP_DEBUG
  m_nHistory            = 0;
  for(u8 i = 0; i< C_HISTORY_DUR; i++)
  {
    xHistory[i].m_uHistoryIndex = 0;
    xHistory[i].m_uCurrTotalEntries = 0;
    xHistory[i].m_pKey = nullptr;
    xHistory[i].m_bDeleted = false;
    xHistory[i].m_bInserted = false;
  }
#endif

  _checkInitialised_(CN_HASHMAP);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_HASHMAP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
HashMap::~HashMap(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HASHMAP,_FOOTER_);
  for(u8 i = 0; i < m_uNoOfSlots; i++)
  {
    if(m_paHashMapSlots[i] != nullptr)
    {
      HashMapSlot* uIter1 = m_paHashMapSlots[i];
      HashMapSlot* uIter2;
      while(uIter1 != nullptr)
      {
        uIter2 = uIter1->m_pNext;
        delete uIter1;
        uIter1 = uIter2;
      }
    }
  }

}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* print analysis of this hash map                                            */
/*----------------------------------------------------------------------------*/
void HashMap::vAnalysis(void)
{
  vTrace("HM: (with %llu buckets/slots and %llu total entries)\n"
         "    Current Load Factor: %f\n"_Stz,

         m_uNoOfSlots,
         m_uNoOfEntries,
         (d8)m_uNoOfEntries/m_uNoOfSlots);

#ifdef WITH_HASHMAP_DEBUG

  vTrace("HM History:\n"_Stz);

  for(u8 i =0; (i < C_HISTORY_DUR) && (i < m_nHistory); i++)
  {
    vTrace("    #%llu, CurrCap: %llu, Load: %f, Key: %p, Deleted: %d, Inserted: %d\n"_Stz,

           xHistory[i].m_uHistoryIndex,
           xHistory[i].m_uCurrTotalEntries,
           (d8)xHistory[i].m_uCurrTotalEntries/m_uNoOfSlots,
           xHistory[i].m_pKey,
           xHistory[i].m_bDeleted,
           xHistory[i].m_bInserted);
  }
#endif
}

#ifdef WITH_HASHMAP_DEBUG
/*----------------------------------------------------------------------------*/
/* Save current state to history                                              */
/*----------------------------------------------------------------------------*/
void HashMap::vAppendHistory(void* pKey, bool bDeleted, bool bInserted)
{
  xHistory[m_nHistory % C_HISTORY_DUR].m_uHistoryIndex     = m_nHistory;
  xHistory[m_nHistory % C_HISTORY_DUR].m_uCurrTotalEntries = m_uNoOfEntries;
  xHistory[m_nHistory % C_HISTORY_DUR].m_pKey              = pKey;
  xHistory[m_nHistory % C_HISTORY_DUR].m_bDeleted          = bDeleted;
  xHistory[m_nHistory % C_HISTORY_DUR].m_bInserted         = bInserted;
  m_nHistory++;
}
#endif

/*----------------------------------------------------------------------------*/
/* delete a given key                                                         */
/*----------------------------------------------------------------------------*/
void HashMap::vDeleteKey(void* pKey)
{
  u8 uIndex = uHashFunction(pKey, m_uNoOfSlots);
  HashMapSlot* pIter = m_paHashMapSlots[uIndex];
  HashMapSlot* pPrevIter = nullptr;

#ifdef WITH_HASHMAP_DEBUG
  vAppendHistory(pKey, true, false);
#endif
  m_okLock.vGrabWrite();

  while ( pIter != nullptr ) {

    if ( pIter->m_pKeyObject == pKey ) {

      if ( pPrevIter != nullptr )
        pPrevIter->m_pNext = pIter->m_pNext;
      else
        m_paHashMapSlots[uIndex] = pIter->m_pNext;
      delete pIter;

      // Assert before decrement
      // since unsigned underflow could occur
      ASSERT(1 <= m_uNoOfEntries);
      m_uNoOfEntries--;

      m_okLock.vDropWrite();

      return;
    }

    pPrevIter = pIter;
    pIter = pIter->m_pNext;
  }
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* Does the given key exist in the hashmap                                    */
/*----------------------------------------------------------------------------*/
bool HashMap::bExists(void* pKey)
{
  u8 uIndex = uHashFunction(pKey, m_uNoOfSlots);

  m_okLock.vGrabRead();

  HashMapSlot* pIter = m_paHashMapSlots[uIndex];

  while ( pIter != nullptr ) {

    if ( pIter->m_pKeyObject == pKey ) {
      m_okLock.vDropRead();
      return true;
    }

    pIter = pIter->m_pNext;
  }

  m_okLock.vDropRead();
  return false;
}

/*----------------------------------------------------------------------------*/
/* hash key                                                                   */
/*----------------------------------------------------------------------------*/
u8 HashMap::uHashFunction(void* pKey, u8 NoOfSlots)
{
  _static_
  return ( (u8) pKey % NoOfSlots );
}

/*----------------------------------------------------------------------------*/
/* hash key                                                                   */
/*----------------------------------------------------------------------------*/
void HashMap::vInsertSlotAtIndex(HashMapSlot* pSlot, u8 uIndex)
{
  pSlot->m_pNext = m_paHashMapSlots[uIndex];
  m_paHashMapSlots[uIndex] = pSlot;

  m_uNoOfEntries++;
}

/*----------------------------------------------------------------------------*/
/* Insert a key,value at the front of the chain.                              */
/* Update value if key already exists                                         */
/*----------------------------------------------------------------------------*/
void HashMap::vInsertKeyValue(void* pKey, Object* pValue)
{
  if(m_uNoOfEntries/m_uNoOfSlots > 0.75) {
    vReHash();
  }

  u8 uIndex = uHashFunction(pKey, m_uNoOfSlots);

  m_okLock.vGrabWrite();

  HashMapSlot* pIter = m_paHashMapSlots[uIndex];

  while ( pIter != nullptr ) {

    if ( pIter->m_pKeyObject == pKey ) {
      pIter->m_pValObject = pValue;

#ifdef WITH_HASHMAP_DEBUG
      vTrace("Key %p already exists, updating value\n"_Stz, pKey);
      vAppendHistory(pKey, false, true);
#endif
      m_okLock.vDropWrite();
      return;
    }

    pIter = pIter->m_pNext;
  }

  HashMapSlot* pTmpSlot = new HashMapSlot(pKey, pValue);

  vInsertSlotAtIndex(pTmpSlot, uIndex);

  m_okLock.vDropWrite();

#ifdef WITH_HASHMAP_DEBUG
  vAppendHistory(pKey, false, true);
#endif
}

/*----------------------------------------------------------------------------*/
/* Create a new Slot array twice the size of the old one. Rehash and move all */
/* key-value pairs in old slot array to new slot array. Delete old slot array */
/*----------------------------------------------------------------------------*/
void HashMap::vReHash(void)
{
  // Low memory check?
  // Can we lock individual slot chains?

  m_okLock.vGrabWrite();

  u8 uOldNoOfSlots = m_uNoOfSlots;
  m_uNoOfSlots = m_uNoOfSlots * 2;

  HashMapSlot** paOldHashMapSlots = m_paHashMapSlots;
  m_paHashMapSlots = new HashMapSlot*[m_uNoOfSlots];

  for(u8 i = 0; i < m_uNoOfSlots; i++)
    m_paHashMapSlots[i] = nullptr;

  u8 uCount = m_uNoOfEntries;
  for(u8 i = 0; i < uOldNoOfSlots && uCount > 0; i++)
  {
    HashMapSlot* pSlot = paOldHashMapSlots[i];
    while(pSlot != nullptr && uCount > 0)
    {
      u8 uIndex = uHashFunction(pSlot->m_pKeyObject, m_uNoOfSlots);

      HashMapSlot* pTmpSlot = pSlot->m_pNext;

      //Move Slot from old hash map to new hash map
      pSlot->m_pNext = m_paHashMapSlots[uIndex];

      m_paHashMapSlots[uIndex] = pSlot;

      uCount--;

      pSlot = pTmpSlot;
    }
  }

  ASSERT(0 == uCount);

  delete paOldHashMapSlots;

  m_okLock.vDropWrite();
}
/*----------------------------------------------------------------------------*/
/* Get value stored at key if it exists                                       */
/*----------------------------------------------------------------------------*/
Object* HashMap::pValueAt(void* pKey)
{
  u8 uIndex = uHashFunction(pKey, m_uNoOfSlots);

  m_okLock.vGrabRead();

  HashMapSlot* pIter = m_paHashMapSlots[uIndex];

  while ( pIter != nullptr ) {

    if ( pIter->m_pKeyObject == pKey ) {
      m_okLock.vDropRead();
      return pIter->m_pValObject;
    }

    pIter = pIter->m_pNext;
  }

  m_okLock.vDropRead();
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* HashMap::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HASHMAP);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
