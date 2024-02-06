/*============================================================================*/
/* HashMap.hpp -- HashMap                                                     */
/*============================================================================*/
#pragma once
#define C_INIT_HASHMAP_SIZE 100
#ifdef WITH_HASHMAP_DEBUG
#define C_HISTORY_DUR 100
#endif

namespace BASE
{
  class HashMap : public Object
  {

  public:
    Lock          m_okLock;        // lock for the entire hash map
    u8            m_uNoOfEntries;
    u8            m_uNoOfSlots;

    HashMapSlot  m_pHashMapSlot[C_INIT_HASHMAP_SIZE];

#ifdef WITH_HASHMAP_DEBUG
    u8 m_uCollison;
    u8 m_nHistory;

    struct {
      u8      m_uHistoryIndex;
      u8      m_uCurrTotalEntries;
      void*   m_pKey;
      bool    m_bDeleted;
      bool    m_bInserted;
    } xHistory[C_HISTORY_DUR];
#endif

    // must be last
    Footer m_xFooter;
    
   public:
    void* operator new(size_t);
    void  operator delete(void* pHashMapAddress);
 
    HashMap(void);
    ~HashMap(void) noexcept(false);

    void    vAnalysis(void);
    void    vDeleteKey(void* pKey);
    void    vDropKeyLock(void* pKey);
    bool    bExists(void* pKey);
    HashMapEntry* pGetEntryAtIndex(u8 uIndex);

    u8 uHashFunction(void* pKey);

    void    vInsertEntryAtIndex(HashMapEntry* pEntry, u8 uIndex);
    void    vInsertKeyValue(void* pKey, Object* pValue);
    bool    bIsLocked(void* pKey);
    void    vSetEntryAtIndex(HashMapEntry* pEntry, u8 uIndex);
//    void    vReHash(void);
    Object* pValueAt(void* pKey);

#ifdef WITH_HASHMAP_DEBUG
    void    vAppendHistory(void* pKey,
                           bool bDeleted,
                           bool bInserted);
#endif

  private:

    Footer* aFooter(void);
  };
  
  HashMap* _pHashMap(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
