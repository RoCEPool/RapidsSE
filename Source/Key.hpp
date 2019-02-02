/*============================================================================*/
/* KeyData.hpp -- key part of an index entry                                      */
/*============================================================================*/
#pragma once

namespace Data
{
  enum eJC // JuxtaPosition
  {
    /*  0 */ JC_NONE=0,
    /*  1 */ JC_BEFORE,
    /*  2 */ JC_SAME,
    /*  3 */ JC_AFTER,
    /*  4 */ JC_ENUMS
  };
  
  static Stz* aJC[]
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
    
    static char* aEachDatum(Column* pColumn, i8& nBytes, char** aaCursor);
  
    bool bLessThan(KeyData* aKey, IdxColVec* pIdxColVec);
    bool bLessEqual(KeyData* aKey, IdxColVec* pIdxColVec);
    bool bEqualTo(KeyData* aKey, IdxColVec* pIdxColVec);
    bool bNotEqual(KeyData* aKey, IdxColVec* pIdxColVec);
    bool bGreaterEqual(KeyData* aKey, IdxColVec* pIdxColVec);
    bool bGreaterEqualSingle(KeyData* aKey, Index* pIndex);
    bool bGreaterThan(KeyData* aKey, IdxColVec* pIdxColVec);
    bool bGreaterThanSingle(KeyData* aKey, Index* pIndex);
    
    i8 iCmp(KeyData* aKey, IdxColVec* pIdxColVec);
    
    bool bMatches(KeyData* aKey, IdxColVec* pIdxColVec, RelOpAry* pOpAry);

    bool bJuxtaCompare(eJC eJuxtaCompare, KeyData* aKey,
                       IdxColVec* pIdxColVec, i8 nSearchKeyCols,
                       bool& bIsEqual, eRO eRelnOp=RO_NONE);
    bool bJuxtaBefore(KeyData* aSearchKey, IdxColVec* pIdxColVec, i8 nSearchKeyCols,
                      bool& bIsEqual, eRO eRelnOp=RO_NONE);
    bool bJuxtaSame(KeyData* aSearchKey, IdxColVec* pIdxColVec, i8 nSearchKeyCols,
                    bool& bIsEqual);
    bool bJuxtaAfter(KeyData* aSearchKey, IdxColVec* pIdxColVec, i8 nSearchKeyCols,
                     bool& bIsEqual, eRO eRelnOp=RO_NONE);
  
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
