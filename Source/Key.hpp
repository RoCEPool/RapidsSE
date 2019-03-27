/*============================================================================*/
/* KeyData.hpp -- key part of an index entry                                      */
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
  
  class KeyData
  {
  private:
  // ..data..   // variable data area

  public:
    void* operator new(size_t);
    void* operator new(size_t, KeyData*);
  
    KeyData(KeyWrk& xKeyWrk);
    ~KeyData(void) noexcept(false);
  
    bool operator <(KeyData* aKeyArg);
    bool operator <=(KeyData* aKeyArg);
    
    void  vShow(IdxColVec* pIdxColVec);
  
    void  vKey(KeyData& xKey);
    void  vKey(KeyWrk& xKeyWrk);
  
    i8    nKeySize(IdxColVec* pIdxColVec);
    
    static eKC eCompare(Item* aKeyItem1, KeyWrk& xKeyItem2);
    static eKC eCompare(KeyWrk& xKeyItem1, KeyWrk& xKeyItem2);
    static eKC eCompare(KeyWrk& xKeyItem1, Item* aKeyItem2);
    static eKC eCompare(Item* aKeyItem1, Item* aKeyItem2);
    
    static char* aEachDatum(Column* pColumn, i8& nBytes, char** aaCursor);
    
    static bool bEqualTo(Item* aKeyItem1, Item* aKeyItem2);
    static bool bGreaterThan(Item* aKeyItem1, Item* aKeyItem2);
    static bool bGreaterEqual(Item* aKeyItem1, KeyWrk& xKeyItem2);
    static bool bGreaterEqual(Item* aKeyItem1, Item* aKeyItem2);
    static bool bLessEqual(KeyWrk& xKeyItem1, Item* aKeyItem2);
    static bool bLessThan(KeyWrk& xKeyItem1, Item* aKeyItem2);
    static bool bNotEqual(Item* aKeyItem1, Item* aKeyItem2);
  
    void vSprintSortKey(s48K& sBuff, IdxColVec* pIdxColVec);
  
    //char* aWspArea(void);
    /*----------------------------------------------------------------------------*/
    /* get wsp area                                                               */
    /*----------------------------------------------------------------------------*/
    inline char* aByteSequence(void)
    {
      return reinterpret_cast<char*>(this);
    }
    i8 iGetInt(void);
    
  };
  
  KeyData* _aKeyData(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
