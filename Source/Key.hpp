/*============================================================================*/
/* Key.hpp -- key part of an index entry                                      */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Index;
  
  //-----------------------------------------------------------------------------
  
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
  
  class Key
  {
  private:
  //i8  m_nSize;
  Wsp m_xWsp;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Key*);
  
    Key(KeyWrk& xKeyWrk);
    ~Key(void) noexcept(false);
  
    bool operator <(Key* aKeyArg);
    bool operator <=(Key* aKeyArg);
    
    void  vShow(IdxColVec* pIdxColVec);
  
    void  vKey(Key& xKey);
    void  vKey(KeyWrk& xKeyWrk);
  
    i8    nKeySize(IdxColVec* pIdxColVec);
    
    static char* aEachDatum(Column* pColumn, i8& nBytes, char** aaCursor);
  
    bool bLessThan(Key* aKey, IdxColVec* pIdxColVec);
    bool bLessEqual(Key* aKey, IdxColVec* pIdxColVec);
    bool bEqualTo(Key* aKey, IdxColVec* pIdxColVec);
    bool bNotEqual(Key* aKey, IdxColVec* pIdxColVec);
    bool bGreaterEqual(Key* aKey, IdxColVec* pIdxColVec);
    bool bGreaterEqualSingle(Key* aKey, Index* pIndex);
    bool bGreaterThan(Key* aKey, IdxColVec* pIdxColVec);
    bool bGreaterThanSingle(Key* aKey, Index* pIndex);
    
    i8 iCmp(Key* aKey, IdxColVec* pIdxColVec);
    
    bool bMatches(Key* aKey, IdxColVec* pIdxColVec, RelOpAry* pOpAry);

    bool bJuxtaCompare(eJC eJuxtaCompare, Key* aKey,
                       IdxColVec* pIdxColVec, i8 nSearchKeyCols,
                       bool& bIsEqual, eRO eRelnOp=RO_NONE);
    bool bJuxtaBefore(Key* aSearchKey, IdxColVec* pIdxColVec, i8 nSearchKeyCols,
                      bool& bIsEqual, eRO eRelnOp=RO_NONE);
    bool bJuxtaSame(Key* aSearchKey, IdxColVec* pIdxColVec, i8 nSearchKeyCols,
                    bool& bIsEqual);
    bool bJuxtaAfter(Key* aSearchKey, IdxColVec* pIdxColVec, i8 nSearchKeyCols,
                     bool& bIsEqual, eRO eRelnOp=RO_NONE);
  
    void vSprintSortKey(s48K& sBuff, IdxColVec* pIdxColVec);
  
    //char* aWspArea(void);
    /*----------------------------------------------------------------------------*/
    /* get wsp area                                                               */
    /*----------------------------------------------------------------------------*/
    inline char* aWspArea(void)
    {
      return m_xWsp.aArea();
    }
    i8 iGetInt(void);
    
  };
  
  Key* _aKey(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
