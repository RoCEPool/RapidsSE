/*============================================================================*/
/* HashMapSlot.hpp -- HashMapSlot                                             */
/*============================================================================*/
#pragma once

namespace BASE
{
  class HashMapSlot : public Object
  {
  friend HashMap;

  public:
    HashMapEntry* m_pEntry;
    Lock m_okSlotLock;
    Pds* m_pPdsOfEntries;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void* pHashMapSlotAddress);

    HashMapSlot(void);
    ~HashMapSlot(void) noexcept(false);

  private:

    Footer* aFooter(void);
  };

  HashMapSlot* _pHashMapSlot(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
