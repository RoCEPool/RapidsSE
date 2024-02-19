/*============================================================================*/
/* HashMapSlot.hpp -- HashMapSlot                                             */
/*============================================================================*/
#pragma once

namespace BASE
{
  class HashMapSlot : public Object
  {
  friend class HashMap;

  public:
    Lock m_okSlotLock;
    Pds* m_pPdsOfEntries;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void* pHashMapSlotAddress);

    HashMapSlot(void);
    ~HashMapSlot(void) noexcept(false);

//    void vInsert()

  private:

    Footer* aFooter(void);
  };

  static i8 iHashMapEntryKeyCompare(void* aItem_l, void* aItem_r);

  HashMapSlot* _pHashMapSlot(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
