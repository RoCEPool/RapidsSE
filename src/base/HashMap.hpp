/*============================================================================*/
/* HashMap.hpp -- HashMap                                                     */
/*============================================================================*/
#pragma once
#define C_INIT_HASHMAP_SIZE 1000

#ifdef WITH_HASHMAP_DEBUG
#define C_HISTORY_DUR 100
#endif

namespace BASE
{
  class HashMap : public Object
  {
  friend HashMapSlot;

  private:
    u8 m_uNoOfSlots;
    u8 m_uNoOfEntries;

    HashMapSlot** m_paHashMapSlots;

#ifdef WITH_HASHMAP_DEBUG
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
 
    HashMap(u8 uNoOfSlots=C_INIT_HASHMAP_SIZE);
    ~HashMap(void) noexcept(false);

    void    vAnalysis(void);
    void    vDeleteKey(void* pKey);
    bool    bExists(void* pKey);

    static u8 uHashFunction(void* pKey, u8 NoOfSlots);

    void    vInsertSlotAtIndex(HashMapSlot* pSlot, u8 uIndex);
    void    vInsertKeyValue(void* pKey, Object* pValue);
    void    vReHash(void);
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
