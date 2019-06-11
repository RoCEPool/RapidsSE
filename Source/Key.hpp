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
  
  static eKC eCompare(Ent* aEnt1, EntWrk& xEntWrk2);
  static eKC eCompare(EntWrk& xEntWrk1, EntWrk& xEntWrk2);
  static eKC eCompare(EntWrk& xEntWrk1, Ent* aEnt2);
  static eKC eCompare(Ent* aEnt1, Ent* aEnt2);
  
  static byte* ayEachElm(Column* pColumn, i8& nBytes, byte** aayCursor);
  
  static bool bEqualTo(Ent* aEnt1, Ent* aEnt2);
  static bool bGreaterThan(Ent* aEnt1, Ent* aEnt2);
  static bool bGreaterEqual(Ent* aEnt1, EntWrk& xEntWrk2);
  static bool bGreaterEqual(Ent* aEnt1, Ent* aEnt2);
  static bool bLessEqual(EntWrk& xEntWrk1, Ent* aEnt2);
  static bool bLessThan(EntWrk& xEntWrk1, Ent* aEnt2);
  static bool bNotEqual(Ent* aEnt1, Ent* aEnt2);

  
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
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
