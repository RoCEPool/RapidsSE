/*============================================================================*/
/* Key.hpp -- key part of an index entry                                      */
/*============================================================================*/
#pragma once

namespace DBobjects
{
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
    ~Key(void);
  
    bool operator <(Key* pKeyArg);
    bool operator <=(Key* pKeyArg);
    
    void  vShow(IdxColVec* pColVec);
  
    void  vKey(Key& xKey);
    void  vKey(KeyWrk& xKeyWrk);
  
    i8    nKeySize(IdxColVec* pIdxColVec);
    
    static char* aEachDatum(Column* pColumn, i8& nBytes, char** aaCursor);
  
    bool bLessThan(Key* pKey, IdxColVec* pColVec);
    bool bLessEqual(Key* pKey, IdxColVec* pColVec);
    bool bEqualTo(Key* pKey, IdxColVec* pColVec);
    bool bNotEqual(Key* pKey, IdxColVec* pColVec);
    bool bGreaterEqual(Key* pKey, IdxColVec* pColVec);
    bool bGreaterThan(Key* pKey, IdxColVec* pColVec);
    
    i8 iCmp(Key* pKey, IdxColVec* pColVec);
    
    bool bMatches(Key* pKey, IdxColVec* pColVec, RelOpAry* pOpAry);

    bool bJuxtaCompare(eJC eJuxtaCompare, Key* pKey, IdxColVec* pColVec, bool& bIsEqual, eRO eRelnOp=RO_NONE);
    bool bJuxtaBefore(Key* pKey, IdxColVec* pColVec, bool& bIsEqual, eRO eRelnOp=RO_NONE);
    bool bJuxtaSame(Key* pKey, IdxColVec* pColVec, bool& bIsEqual);
    bool bJuxtaAfter(Key* pKey, IdxColVec* pColVec, bool& bIsEqual);
  
    void vSprintSortKey(s48K& sBuff, IdxColVec* pColVec);
  
    char* aWspArea(void);
    Stz*  aWspStr(void);
  };
  
  Key* _aKey(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
