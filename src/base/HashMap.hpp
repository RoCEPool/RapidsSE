/*============================================================================*/
/* HashMap.hpp -- HashMap                                                     */
/*============================================================================*/
#pragma once
#define C_HASHMAP_SIZE 10000
namespace BASE
{
  class HashMap : public Object
  {
  friend HashMapSlot;

  private:
    bool m_bTrace;
    u8 m_uNoOfSlots;// Total number of slots
    u8 m_uNoOfEntries;
    HashMapSlot* m_paMap[C_HASHMAP_SIZE];

    // must be last
    Footer m_xFooter;
    
   public:
    void* operator new(size_t);
    void  operator delete(void* pHashMapAddress);
 
    HashMap(bool bTrace=false, u8 uNoOfSlots=C_HASHMAP_SIZE);
    ~HashMap(void) noexcept(false);

    void vAnalysis(void);
    void vDeleteKey(Object* pKey);
    bool bExists(Object* pKey);
    u8 uHashFunction(Object* pKey);
    void vInsertAtFront(Object* pKey, Object* pValue);
    Object* pValueAt(Object* pKey);

  private:

    Footer* aFooter(void);
  };
  
  HashMap* _pHashMap(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
