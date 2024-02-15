/*============================================================================*/
/* HashMapEntry.hpp -- HashMapEntry                                           */
/*============================================================================*/
#pragma once
#include "eBI.h"

namespace BASE
{
  class HashMapEntry : public Object
  {
    friend class HashMap;
    friend class HashMapSlot;

  public:
    void*         m_pKeyObject;
    eBI           m_eBufferIdType;
    Atomic        m_ixAtomicLock;
    Object*       m_pValObject;

    HashMapEntry* m_pNext;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);


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
