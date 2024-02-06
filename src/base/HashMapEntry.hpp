/*============================================================================*/
/* HashMapEntry.hpp -- HashMapEntry                                           */
/*============================================================================*/
#pragma once

namespace BASE
{
  class HashMapEntry : public Object
  {
    friend HashMap;
    friend HashMapSlot;

  public:
    void*         m_pKeyObject;
    Object*       m_pValObject;

    HashMapEntry* m_pNext;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void* pHashMapEntryAddr);

    HashMapEntry(void* pKey, Object* pValue);
    ~HashMapEntry(void) noexcept(false);

    bool bInUse(void);

  private:

    Footer* aFooter(void);
  };

  HashMapEntry* _pHashMapEntry(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
