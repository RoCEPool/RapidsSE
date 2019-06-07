/*============================================================================*/
/* Key.hpp -- key part of an index entry                                      */
/*============================================================================*/
#pragma once

namespace Data
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
  
    Key(KeyWrk& xKeyWrk);
    ~Key(void) noexcept(false);
  
    bool operator <  (Key* aKeyArg);
    bool operator <= (Key* aKeyArg);
  
    static eKC eCompare(Ent* aKeyItem1, KeyWrk& xKeyItem2);
    static eKC eCompare(KeyWrk& xKeyItem1, KeyWrk& xKeyItem2);
    static eKC eCompare(KeyWrk& xKeyItem1, Ent* aKeyItem2);
    static eKC eCompare(Ent* aKeyItem1, Ent* aKeyItem2);
    
    static char* aEachDatum(Column* pColumn, i8& nBytes, char** aaCursor);
    
    static bool bEqualTo(Ent* aKeyItem1, Ent* aKeyItem2);
    static bool bGreaterThan(Ent* aKeyItem1, Ent* aKeyItem2);
    static bool bGreaterEqual(Ent* aKeyItem1, KeyWrk& xKeyItem2);
    static bool bGreaterEqual(Ent* aKeyItem1, Ent* aKeyItem2);
    static bool bLessEqual(KeyWrk& xKeyItem1, Ent* aKeyItem2);
    static bool bLessThan(KeyWrk& xKeyItem1, Ent* aKeyItem2);
    static bool bNotEqual(Ent* aKeyItem1, Ent* aKeyItem2);
  
    i8   iGetInt(void);
    void vKey(Key& xKey);
    void vKey(KeyWrk& xKeyWrk);
    void vShow(IdxColVec* pIdxColVec);
    void vSprintSortKey(s48K& sBuff, IdxColVec* pIdxColVec);
  
    /*------------------------------------------------------------------------*/
    /* getthis key as a byte sequence                                         */
    /*------------------------------------------------------------------------*/
    inline char* aByteSequence(void)
    {
      return reinterpret_cast<char*>(this);
    }
  };
  
  Key* _aKey(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
