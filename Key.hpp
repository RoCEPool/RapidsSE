/*============================================================================*/
/* Key.hpp -- key part of an index entry                                      */
/*============================================================================*/
#pragma once

namespace Indexes
{
  enum eKC // KeyCompare
  {
    /*  0 */ KC_NONE=0,
    /*  1 */ KC_BEFORE,
    /*  2 */ KC_SAME,
    /*  3 */ KC_AFTER,
    /*  4 */ KC_ENUMS
  };
  
  static Stz* aKC[]
  {
    /*  0 */ "NONE"_Stz,
    /*  1 */ "BEFORE"_Stz,
    /*  2 */ "SAME"_Stz,
    /*  3 */ "AFTER"_Stz,
    /*  4 */ "ENUMS"_Stz,
  };
  
  class Key
  {
  private:
    // ..bytes..   // variable byte sequence of the key

  public:
    void* operator new(size_t);
    void* operator new(size_t, Key*);
  
    Key(EntWrk& xEntWrk);
    ~Key(void) noexcept(false);
  
    bool operator <  (Key* aKeyArg);
    bool operator <= (Key* aKeyArg);
  
    static eKC eCompare(Ent* aKeyItem1, EntWrk& xKeyItem2);
    static eKC eCompare(EntWrk& xKeyItem1, EntWrk& xKeyItem2);
    static eKC eCompare(EntWrk& xKeyItem1, Ent* aKeyItem2);
    static eKC eCompare(Ent* aKeyItem1, Ent* aKeyItem2);
    
    static byte* ayEachElm(Column* pColumn, i8& nBytes, byte** aayCursor);
    
    static bool bEqualTo(Ent* aKeyItem1, Ent* aKeyItem2);
    static bool bGreaterThan(Ent* aKeyItem1, Ent* aKeyItem2);
    static bool bGreaterEqual(Ent* aKeyItem1, EntWrk& xKeyItem2);
    static bool bGreaterEqual(Ent* aKeyItem1, Ent* aKeyItem2);
    static bool bLessEqual(EntWrk& xKeyItem1, Ent* aKeyItem2);
    static bool bLessThan(EntWrk& xKeyItem1, Ent* aKeyItem2);
    static bool bNotEqual(Ent* aKeyItem1, Ent* aKeyItem2);
  
    i8   iGetInt(void);
    void vKey(EntWrk& xEntWrk);
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
