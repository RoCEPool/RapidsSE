/*============================================================================*/
/* Key.hpp -- key part of an index entry                                      */
/*============================================================================*/
#pragma once

namespace Indexes
{
  class Key2
  {
  protected:
    i4  m_nKeyBytes; // size of key (in bytes)
    // ..bytes..     // variable byte sequence of the key
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, Key2*);
  
    Key2(void);
    ~Key2(void) noexcept(false);
  
    //! this is the final (one and only) function we should aim for
    //static eKC eCompare(Key* aLeft, Key* aRight);
  
    i8   iGetInt(void);
    void vShow(IdxColVec* pIdxColVec);
    i8   nSizeKey(void);
    void vSizeKey(i8 nSizeKey);

    /*------------------------------------------------------------------------*/
    /* getthis key as a byte sequence                                         */
    /*------------------------------------------------------------------------*/
    inline byte* ayByteSequence(void)
    {
      return reinterpret_cast<byte*>(this + sizeof(m_nKeyBytes));
    }
  };
  
  Key2* _aKey2(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
