/*============================================================================*/
/* HashMapSlot.hpp -- HashMapSlot                                             */
/*============================================================================*/
#pragma once
#define C_INIT_HASHMAP_SIZE 10000

namespace BASE
{
  class HashMapSlot : public Object
  {
  friend HashMap;

  public:
    u8 m_uNoOfSlots;
    HashMapEntry* m_pEntry[C_INIT_HASHMAP_SIZE];

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void* pHashMapSlotAddress);

    HashMapSlot(void);
    ~HashMapSlot(void) noexcept(false);

    u8 uGetNoSlots(void);

  private:

    Footer* aFooter(void);
  };

  HashMapSlot* _pHashMapSlot(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
