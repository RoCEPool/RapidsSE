/*============================================================================*/
/* HashMapEntry.hpp -- HashMapEntry                                           */
/*============================================================================*/
#pragma once
#include "eBI.h"

namespace BASE
{
  class HashMapEntry : public Object
  {
    friend HashMap;
    friend HashMapSlot;

  public:
    eBI           m_eBufferIdType;
    Atomic        m_ixAtomicLock;
    void*         m_pKeyObject;
    Object*       m_pValObject;

    HashMapEntry* m_pNext;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void* pHashMapEntryAddr);

    HashMapEntry(void* pKey, Object* pValue, eBI eBufferIdType = BI_NONE);
    ~HashMapEntry(void) noexcept(false);

    void vDropLock(void);
    void vGrabLock(void);
    bool bLocked(void);

  private:

    Footer* aFooter(void);
  };

  HashMapEntry* _pHashMapEntry(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
