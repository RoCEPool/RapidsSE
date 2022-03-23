/*============================================================================*/
/* HashMapSlot.hpp -- HashMapSlot                                             */
/*============================================================================*/
#pragma once

namespace BASE
{
  class HashMap;
  class HashMapSlot : public Object
  {
  friend HashMap;

  private:
    Object* m_pKeyObject;
    Object* m_pValObject;

    //u8 m_uKey;
    //i8 m_iValue;

    HashMapSlot* m_pNext;
    //HashMapSlot* m_pPrev;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void* pHashMapSlotAddress);

    HashMapSlot(Object* pKey, Object* pValue);
    ~HashMapSlot(void) noexcept(false);
  private:
    Footer* aFooter(void);
  };

  HashMapSlot* _pHashMapSlot(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
