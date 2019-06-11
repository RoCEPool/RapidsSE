/*============================================================================*/
/* Key.hpp -- key part of an index entry                                      */
/*============================================================================*/
#pragma once

namespace Indexes
{
  class Key
  {
  private:
    // ..bytes..   // variable byte sequence of the key

  public:
    void* operator new(size_t);
    void* operator new(size_t, Key*);
  
    Key(void);
    ~Key(void) noexcept(false);
  
    //! this is the final (one and only) function we should aim for
    //static eKC eCompare(Key* aLeft, Key* aRight);
  
    i8   iGetInt(void);
    void vShow(IdxColVec* pIdxColVec);
  
    /*------------------------------------------------------------------------*/
    /* getthis key as a byte sequence                                         */
    /*------------------------------------------------------------------------*/
    inline byte* ayByteSequence(void)
    {
      return reinterpret_cast<byte*>(this);
    }
  };
  
  Key* _aKey(void* aAddr);
  
  
  
  
  
  class Key2
  {
  private:
    // ..bytes..   // variable byte sequence of the key
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, Key*);
  
    Key2(void);
    ~Key2(void) noexcept(false);
  
    //! this is the final (one and only) function we should aim for
    //static eKC eCompare(Key* aLeft, Key* aRight);
  
    i8   iGetInt(void);
    void vShow(IdxColVec* pIdxColVec);
  
    /*------------------------------------------------------------------------*/
    /* getthis key as a byte sequence                                         */
    /*------------------------------------------------------------------------*/
    inline byte* ayByteSequence(void)
    {
      return reinterpret_cast<byte*>(this);
    }
  };
  
  Key2* _aKey2(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
