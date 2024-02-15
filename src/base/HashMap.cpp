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
HashMap::HashMap(void)
: Object(CN_HASHMAP)
, m_okLock(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HASHMAP,_FOOTER_);

  m_uNoOfEntries        = 0;
  m_uNoOfSlots          = C_INIT_HASHMAP_SIZE;

#ifdef WITH_HASHMAP_DEBUG
  m_uCollison           = 0;
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

  for(u8 u_slot = 0; u_slot < m_uNoOfSlots; u_slot++) {
    HashMapEntry* pDummyEntry = new HashMapEntry(nullptr, nullptr, BI_NONE);
    pDummyEntry->m_pNext = nullptr;
    m_pHashMapSlot[u_slot] = new HashMapSlot();
    m_pHashMapSlot[u_slot]->m_pEntry = pDummyEntry;
  }

  _checkInitialised_(CN_HASHMAP);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_HASHMAP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
HashMap::~HashMap(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HASHMAP,_FOOTER_);
//  for(i8 i_slot = 0; i_slot < m_uNoOfSlots; i_slot++) {
//    HashMapEntry* pDummyEntry = new HashMapEntry(nullptr, nullptr);
//    pDummyEntry->m_pNext = nullptr;
//    m_pHashMapSlot[i_slot].m_pEntry = pDummyEntry;
//  }
//  delete m_pHashMapSlot;
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
  vTrace("Key Collisons:%llu\n"_Stz, m_uCollison);

  for(u8 i =0; (i < C_HISTORY_DUR) && (i < m_nHistory); i++)
  {
    vTrace("    #%llu, CurrCap: %llu, Load: %f, Key: %p, Deleted: %d, Inserted: %d\n"_Stz,

           xHistory[i].m_uHistoryIndex,
           xHistory[i].m_uCurrTotalEntries,
           (d8)xHistory[i].m_uCurrTotalEntries/m_pHashMapSlot->uGetNoSlots(),
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
#ifdef WITHOUT_BUFFERIDHASHMAP
  // TODO: Hack for performance test
  UNUSED(pKey);
  return;
#else
  u8 uIndex = uHashFunction(pKey);

  m_pHashMapSlot[uIndex]->m_okSlotLock.vGrabWrite();

  HashMapEntry* pDummyEntry = pGetEntryAtIndex(uIndex);
  ASSERT(pDummyEntry != nullptr);

  // First entry is a dummy entry
  HashMapEntry* pPrevIter = pDummyEntry;
  HashMapEntry* pIter = pDummyEntry->m_pNext;

#ifdef WITH_HASHMAP_DEBUG
  vAppendHistory(pKey, true, false);
#endif

  while ( pIter != nullptr ) {

    if ( pIter->m_pKeyObject == pKey ) {
      //ASSERT(1 <= m_uNoOfEntries);
      ASSERT(pPrevIter != nullptr);
      pPrevIter->m_pNext = pIter->m_pNext;
      delete pIter;

      //m_uNoOfEntries--;
      m_pHashMapSlot[uIndex]->m_okSlotLock.vDropWrite();
      return;
    }

    pPrevIter = pIter;
    pIter = pIter->m_pNext;
  }
  m_pHashMapSlot[uIndex]->m_okSlotLock.vDropWrite();
#endif
}

/*----------------------------------------------------------------------------*/
/* Dump                                                                       */
/*----------------------------------------------------------------------------*/
void HashMap::vDump(void)
{
#ifdef WITHOUT_BUFFERIDHASHMAP
  return;
#else

  for(u8 u_index = 0; u_index < m_uNoOfSlots; u_index++) {
    HashMapEntry* pIter = m_pHashMapSlot[u_index]->m_pEntry;
    if(pIter)
      vTrace("\n"_Stz);
    while( pIter != nullptr) {
      vTrace("[%p %p %lld]\t"_Stz, pIter->m_pKeyObject, pIter->m_pValObject, *(pIter->m_ixAtomicLock));
      if(pIter->m_pValObject) {
        DataBuffer* pDataBuffer = _pDataBuffer(pIter->m_pValObject);
        eCN eClass = pDataBuffer->eGetClassNo();
        if(eClass == CN_PDS) {
          Pds* pPds = _pPds(pDataBuffer);
          vTrace("%p\n"_Stz, pPds);
        } else if(eClass == CN_CRWBUFFER) {
          CrwBuffer* pCrwBuffer = _pCrwBuffer(pDataBuffer);
          vTrace("%p\n"_Stz, pCrwBuffer);
        }
      }

      pIter = pIter->m_pNext;
    }
  }
  vTrace("\n"_Stz);
#endif
}

/*----------------------------------------------------------------------------*/
/* Drop read lock on this entry
 *
 * NOTE: This function is used to tell the paging demon that it's now safe to
 * page out this buffer. Uses an atomic integer                               */
/*----------------------------------------------------------------------------*/
void HashMap::vDropKeyLock(void* pKey)
{
#ifdef WITHOUT_BUFFERIDHASHMAP
  // TODO: Hack for performance test
  UNUSED(pKey);
  return;
#else
  u8 uIndex = uHashFunction(pKey);

  m_pHashMapSlot[uIndex]->m_okSlotLock.vGrabRead();

  HashMapEntry* pIter = pGetEntryAtIndex(uIndex);

  while ( pIter != nullptr ) {

    if ( pIter->m_pKeyObject == pKey ) {
      ASSERT(pIter->bLocked());
      pIter->vDropLock();
      m_pHashMapSlot[uIndex]->m_okSlotLock.vDropRead();
//      if(MY::pTask())
//        vTrace("Drop key %p read locks task %p %s\n"_Stz, pIter->m_pKeyObject, MY::pTask(), MY::pTask()->szTaskNm());
//      else
//        vTrace("Drop key %p read lock\n"_Stz, pIter->m_pKeyObject);
      return;
    }
    pIter = pIter->m_pNext;
  }
  m_pHashMapSlot[uIndex]->m_okSlotLock.vDropRead();
  NEVER_REACH;
#endif
}

/*----------------------------------------------------------------------------*/
/* Get the HashMapEntry at the given key                                      */
/*----------------------------------------------------------------------------*/
HashMapEntry* HashMap::pEntryAt(void* pKey, bool bLockThis)
{
#ifdef WITHOUT_BUFFERIDHASHMAP
  // TODO: Hack for performance test
  return (Object*) pKey;
#else
  u8 uIndex = uHashFunction(pKey);

  m_pHashMapSlot[uIndex]->m_okSlotLock.vGrabRead();

  HashMapEntry* pIter = pGetEntryAtIndex(uIndex);
  ASSERT(pIter != nullptr);

  while ( pIter != nullptr ) {

    if ( pIter->m_pKeyObject == pKey ) {
      if(bLockThis)
        pIter->vGrabLock();

      m_pHashMapSlot[uIndex]->m_okSlotLock.vDropRead();
      return pIter;
    }

    pIter = pIter->m_pNext;
  }

  m_pHashMapSlot[uIndex]->m_okSlotLock.vDropRead();
  return nullptr;
#endif
}

/*----------------------------------------------------------------------------*/
/* Does the given key exist in the hashmap                                    */
/*----------------------------------------------------------------------------*/
bool HashMap::bExists(void* pKey)
{
  u8 uIndex = uHashFunction(pKey);

  m_pHashMapSlot[uIndex]->m_okSlotLock.vGrabRead();

  HashMapEntry* pIter = pGetEntryAtIndex(uIndex);

  while ( pIter != nullptr ) {

    if ( pIter->m_pKeyObject == pKey ) {
      m_pHashMapSlot[uIndex]->m_okSlotLock.vDropRead();
      return true;
    }

    pIter = pIter->m_pNext;
  }

  m_pHashMapSlot[uIndex]->m_okSlotLock.vDropRead();
  return false;
}

/*----------------------------------------------------------------------------*/
/* hash key                                                                   */
/*----------------------------------------------------------------------------*/
HashMapEntry* HashMap::pGetEntryAtIndex(u8 uIndex)
{
  if(uIndex >= m_uNoOfSlots)
    return nullptr;
  return m_pHashMapSlot[uIndex]->m_pEntry;
}

/*----------------------------------------------------------------------------*/
/* hash key                                                                   */
/*----------------------------------------------------------------------------*/
u8 HashMap::uHashFunction(void* pKey)
{
  return ( (u8) pKey % m_uNoOfSlots );
}

/*----------------------------------------------------------------------------*/
/* hashmap init (add pds used in the hashmap into itself                      */
/*----------------------------------------------------------------------------*/
void HashMap::vInit(void)
{
  // Do we really need to add the Pds buffers used in the hashmap to the hashmap itself?
  for(u8 u_slot = 0; u_slot < m_uNoOfSlots; u_slot++) {
    Pds* pPds = m_pHashMapSlot[u_slot]->m_pPdsOfEntries;
    AM::pHashMap()->vInsertKeyValue((void *) pPds->oGetBufferId().m_pvAsAddress, (Object *) pPds, BI_HASHMAPSLOT);
  }
}

/*----------------------------------------------------------------------------*/
/* given an entry pointer and index, insert into hashmap                      */
/*----------------------------------------------------------------------------*/
//void HashMap::vInsertEntryAtIndex(HashMapEntry* pEntry, u8 uIndex)
//{
//#ifdef WITH_HASHMAP_DEBUG
//  if(pGetEntryAtIndex(uIndex) != nullptr) {
//    m_uCollison++;
//  }
//#endif
//
//  pEntry->m_pNext = pGetEntryAtIndex(uIndex);
//  // Grab a read lock, to prevent this entry from
//  // being paged out
////  pEntry->m_okLock.vGrabRead();
////  vTrace("Inserting key %p read locks task %p %s\n"_Stz, pEntry->m_pKeyObject, MY::pTask(), MY::pTask()->szTaskNm());
//
//  vSetEntryAtIndex(pEntry, uIndex);
//
//  m_uNoOfEntries++;
//}

/*----------------------------------------------------------------------------*/
/* Insert a key,value at the front of the chain.                              */
/* Update value if key already exists
 * Grabs a lock on the entry as soon as it is created                         */
/*----------------------------------------------------------------------------*/
void HashMap::vInsertKeyValue(void* pKey, Object* pValue, eBI eBufferIdType)
{
#ifdef WITHOUT_BUFFERIDHASHMAP
  // TODO: Hack for performance test
  UNUSED(pKey);
  UNUSED(pValue);
  return;
#else
//  if(m_uNoOfEntries/m_pHashMapSlot->uGetNoSlots() > 0.75) {
//    vReHash();
//  }

  u8 uIndex = uHashFunction(pKey);

  m_pHashMapSlot[uIndex]->m_okSlotLock.vGrabWrite();

  HashMapEntry* pDummyEntry = pGetEntryAtIndex(uIndex);
  RASSERT(pDummyEntry != nullptr);

  // First entry is a dummy entry
  HashMapEntry* pIter = pDummyEntry->m_pNext;

  while ( pIter != nullptr ) {

    // Key already exists, update the value
    if ( pIter->m_pKeyObject == pKey ) {
      pIter->vGrabLock();
      pIter->m_pValObject = pValue;
      m_pHashMapSlot[uIndex]->m_okSlotLock.vDropWrite();

#ifdef WITH_HASHMAP_DEBUG
      vTrace("Key %p already exists, updating value\n"_Stz, pKey);
      vAppendHistory(pKey, false, true);
#endif
      return;
    }

    pIter = pIter->m_pNext;
  }

  HashMapEntry* pEntry = new HashMapEntry(pKey, pValue, eBufferIdType);
  pEntry->m_pNext = pDummyEntry->m_pNext;
  pDummyEntry->m_pNext = pEntry;
  //m_uNoOfEntries++;
  m_pHashMapSlot[uIndex]->m_okSlotLock.vDropWrite();

#ifdef WITH_HASHMAP_DEBUG
  vAppendHistory(pKey, false, true);
#endif
#endif
}

/*----------------------------------------------------------------------------*/
/* Is this HashMapEntry locked?                                               */
/*----------------------------------------------------------------------------*/
bool HashMap::bIsLocked(void* pKey)
{

  u8 uIndex = uHashFunction(pKey);

  m_pHashMapSlot[uIndex]->m_okSlotLock.vGrabRead();
  HashMapEntry* pIter = pGetEntryAtIndex(uIndex);

  while ( pIter != nullptr ) {

    // Find the entry that matches this key
    if ( pIter->m_pKeyObject == pKey ) {
      bool bIsLocked = pIter->bLocked();
      m_pHashMapSlot[uIndex]->m_okSlotLock.vDropRead();
      return bIsLocked;
    }

    pIter = pIter->m_pNext;
  }

  m_pHashMapSlot[uIndex]->m_okSlotLock.vDropRead();
  return false;
}

/*----------------------------------------------------------------------------*/
/* Create a new Slot array twice the size of the old one. Rehash and move all */
/* key-value pairs in old slot array to new slot array. Delete old slot array */
/*----------------------------------------------------------------------------*/
//void HashMap::vReHash(void)
//{
//  // Low memory check?
//  // Can we lock individual slot chains?
//
//  m_okLock.vGrabWrite();
//
//  u8 uOldNoOfSlots = m_pHashMapSlot->uGetNoSlots();
//  m_pHashMapSlot->uGetNoSlots() = m_pHashMapSlot->uGetNoSlots() * 2;
//
//  HashMapSlot* paOldHashMapSlots = m_pHashMapSlot;
//  m_paHashMapSlots = new HashMapSlot[m_pHashMapSlot->uGetNoSlots()];
//
//  u8 uCount = m_uNoOfEntries;
//  for(u8 i = 0; i < uOldNoOfSlots && uCount > 0; i++)
//  {
//    HashMapEntry* pEntry = paOldHashMapSlots[i].m_pEntry;
//    while(pEntry != nullptr && uCount > 0)
//    {
//      u8 uIndex = uHashFunction(pEntry->m_pKeyObject, m_pHashMapSlot->uGetNoSlots());
//
//      HashMapEntry* pTmpEntry = pEntry->m_pNext;
//
//      //Move Slot from old hash map to new hash map
//      pEntry->m_pNext = m_pHashMapSlot[uIndex]->m_pEntry;
//
//      m_pHashMapSlot[uIndex]->m_pEntry = pEntry;
//
//      uCount--;
//
//      pEntry = pTmpEntry;
//    }
//  }
//
//  ASSERT(0 == uCount);
//
//  delete paOldHashMapSlots;
//
//  m_okLock.vDropWrite();
//}

/*----------------------------------------------------------------------------*/
/* Get value stored at key if it exists                                       */
/*----------------------------------------------------------------------------*/
//void HashMap::vSetEntryAtIndex(HashMapEntry* pEntry, u8 uIndex)
//{
//  m_pHashMapSlot->m_pEntry[uIndex] = pEntry;
//}

/*----------------------------------------------------------------------------*/
/* Get value stored at key if it exists. Someone is using this buffer.
 * Hence, lock this entry, unless specified, to prevent it from being
 * paged out.                                                                 */
/*----------------------------------------------------------------------------*/
Object* HashMap::pValueAt(void* pKey, bool bLockThis)
{
#ifdef WITHOUT_BUFFERIDHASHMAP
  // TODO: Hack for performance test
  return (Object*) pKey;
#else
  u8 uIndex = uHashFunction(pKey);

  m_pHashMapSlot[uIndex]->m_okSlotLock.vGrabRead();

  HashMapEntry* pIter = pGetEntryAtIndex(uIndex);
  ASSERT(pIter != nullptr);

  while ( pIter != nullptr ) {

    if ( pIter->m_pKeyObject == pKey ) {
      if(bLockThis)
        pIter->vGrabLock();

      m_pHashMapSlot[uIndex]->m_okSlotLock.vDropRead();
      return pIter->m_pValObject;
    }

    pIter = pIter->m_pNext;
  }

  m_pHashMapSlot[uIndex]->m_okSlotLock.vDropRead();
  return nullptr;
#endif
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
