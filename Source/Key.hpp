/*============================================================================*/
/* Key.hpp -- key part of an index entry                                      */
/*============================================================================*/
#pragma once

namespace Indexes
{
  class Key
  {
  private:
    i8  m_nKeyBytes; // size of key (in bytes)
    // ..bytes..     // variable byte sequence of the key

  public:
    void* operator new(size_t);
    void* operator new(size_t, Key*);
  
    Key(void);
    ~Key(void) noexcept(false);
  
    static eKC eCompare(Key* aLeft, Key* aRight);
  
    i8   iGetInt(void);
    void vIncSize(i8 nDeltaBytes);
    void vShow(IdxColVec* pIdxColVec);
    i8   nSizeKey(void);
    void vSizeKey(i8 nSizeKey);

    /*------------------------------------------------------------------------*/
    /* getthis key as a byte sequence                                         */
    /*------------------------------------------------------------------------*/
    inline byte* ayKeyBytes(void)
    {
      return aAdd<byte*>(this,sizeof(m_nKeyBytes));
    }
  };
  
  Key* _aKey(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
