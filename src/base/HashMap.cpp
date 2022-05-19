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
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HASHMAP,_FOOTER_);
  m_uNoOfSlots          = C_HASHMAP_SIZE;
  m_uNoOfEntries        = 0;

  for(u8 i = 0; i < m_uNoOfSlots; i++)
    m_paMap[i] = nullptr;

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
    if(m_paMap[i] != nullptr)
    {
      HashMapSlot* uIter1 = m_paMap[i];
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
void HashMap::vAppendHistory(Object* pKey, bool bDeleted, bool bInserted)
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
  u8 uIndex = uHashFunction(pKey);
  HashMapSlot* pIter = m_paMap[uIndex];
  HashMapSlot* pPrevIter = nullptr;

  while ( pIter != nullptr ) {

    if ( pIter->m_pKeyObject == pKey ) {

      if ( pPrevIter != nullptr )
        pPrevIter->m_pNext = pIter->m_pNext;
      else
        m_paMap[uIndex] = pIter->m_pNext;
      delete pIter;

      // Assert before decrement
      // since unsigned underflow could occur
      ASSERT(1 <= m_uNoOfEntries);
      m_uNoOfEntries--;

#ifdef WITH_HASHMAP_DEBUG
      vAppendHistory(pKey, true, false);
#endif

      return;
    }

    pPrevIter = pIter;
    pIter = pIter->m_pNext;
  }
}

/*----------------------------------------------------------------------------*/
/* Does the given key exist in the hashmap                                    */
/*----------------------------------------------------------------------------*/
bool HashMap::bExists(void* pKey)
{
  u8 uIndex = uHashFunction(pKey);
  HashMapSlot* pIter = m_paMap[uIndex];

  while ( pIter != nullptr ) {

    if ( pIter->m_pKeyObject == pKey )
      return true;

    pIter = pIter->m_pNext;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* hash key                                                                   */
/*----------------------------------------------------------------------------*/
u8 HashMap::uHashFunction(void* pKey)
{
  return ( (u8) pKey % m_uNoOfSlots );
}

/*----------------------------------------------------------------------------*/
/* Insert a key,value at the front of the chain.                              */
/* Update value if key already exists                                         */
/*----------------------------------------------------------------------------*/
void HashMap::vInsertAtFront(void* pKey, Object* pValue)
{
  u8 uIndex = uHashFunction(pKey);
  HashMapSlot* pIter = m_paMap[uIndex];

  while ( pIter != nullptr ) {

    if ( pIter->m_pKeyObject == pKey ) {
      pIter->m_pValObject = pValue;

#ifdef WITH_HASHMAP_DEBUG
      vTrace("Key %p already exists, updating value\n"_Stz, pKey);
      vAppendHistory(pKey, false, true);
#endif

      return;
    }

    pIter = pIter->m_pNext;
  }

  HashMapSlot* pTmpSlot = new HashMapSlot(pKey, pValue);
  pTmpSlot->m_pNext = m_paMap[uIndex];
  m_paMap[uIndex] = pTmpSlot;
  m_uNoOfEntries++;

#ifdef WITH_HASHMAP_DEBUG
  vAppendHistory(pKey, false, true);
#endif
}

/*----------------------------------------------------------------------------*/
/* Get value stored at key if it exists                                       */
/*----------------------------------------------------------------------------*/
Object* HashMap::pValueAt(void* pKey)
{
  u8 uIndex = uHashFunction(pKey);
  HashMapSlot* pIter = m_paMap[uIndex];

  while ( pIter != nullptr ) {

    if ( pIter->m_pKeyObject == pKey )
      return pIter->m_pValObject;

    pIter = pIter->m_pNext;
  }

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
