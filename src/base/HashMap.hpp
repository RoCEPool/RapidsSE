/*============================================================================*/
/* HashMap.hpp -- HashMap                                                     */
/*============================================================================*/
#pragma once
#define C_HASHMAP_SIZE 128
namespace BASE
{
  class HashMap : public Object
  {
  private:
    bool m_bTrace;
    u8 m_uNoOfSlots;// Total number of slots
    HashMapSlot* m_paMap[C_HASHMAP_SIZE];

    // must be last
    Footer m_xFooter;
    
   public:
    void* operator new(size_t);
    void  operator delete(void* pHashMapAddress);
 
    HashMap(bool bTrace=false, u8 uNoOfSlots=C_HASHMAP_SIZE);
    ~HashMap(void) noexcept(false);

    bool bExists(u8 uKey);
    u8 uHashFunction(u8 uKey);
    void vInsert(u8 uKey, i8 iValue);

  private:

    Footer* aFooter(void);
  };
  
  HashMap* _pHashMap(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
