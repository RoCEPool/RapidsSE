/*============================================================================*/
/* HashMapSlot.hpp -- HashMapSlot                                             */
/*============================================================================*/
#pragma once

namespace BASE
{
  class HashMapSlot : public Object
  {
  public:
    u8 m_uKey;
//    Object* m_pObject;
    i8 m_iValue;
    HashMapSlot* m_pNext;
//    HashMapSlot* m_pPrev;

    // must be last
    Footer m_xFooter;

    void* operator new(size_t);
    void  operator delete(void* pHashMapSlotAddress);

    HashMapSlot(u8 uKey, i8 iValue);
    ~HashMapSlot(void) noexcept(false);
  private:
    Footer* aFooter(void);
  };

  HashMapSlot* _pHashMapSlot(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
