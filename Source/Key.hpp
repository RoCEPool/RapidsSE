/*============================================================================*/
/* Key.hpp -- key part of an index entry                                      */
/*============================================================================*/
#pragma once

namespace Transform
{
  class Key
  {
  private:
    i8 m_nElmSeqSize; // size of element sequence (in bytes)
  
    // .. element sequence (of the key)

  public:
    void* operator new(size_t);
    void* operator new(size_t, Key*);
  
    Key(void);
    ~Key(void) noexcept(false);
  
    static eKC eCompare(Key* aLeft, Key* aRight);
  
    i8   iGetInt(void);
    void vIncElmSeqSize(i8 nDeltaBytes);
    i8   nKeyBytes(void);
    void vKeyBytes(i8 nKeyBytes);
    void vShow(IdxColVec* pIdxColVec);

    /*------------------------------------------------------------------------*/
    /* get the element sequence of this key                                   */
    /*------------------------------------------------------------------------*/
    inline byte* ayElmSeq(void)
    {
      return aAdd<byte*>(this,sizeof(m_nElmSeqSize));
    }
  };
  
  Key* _aKey(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
