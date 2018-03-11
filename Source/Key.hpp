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
    
    void  vShow(IdxColVec* pIdxColVec);
  
    void  vKey(Key& xKey);
    void  vKey(KeyWrk& xKeyWrk);
  
    i8    nKeySize(IdxColVec* pIdxColVec);
    
    static char* aEachDatum(Column* pColumn, i8& nBytes, char** aaCursor);
  
    bool bLessThan(Key* pKey, IdxColVec* pIdxColVec);
    bool bLessEqual(Key* pKey, IdxColVec* pIdxColVec);
    bool bEqualTo(Key* pKey, IdxColVec* pIdxColVec);
    bool bNotEqual(Key* pKey, IdxColVec* pIdxColVec);
    bool bGreaterEqual(Key* pKey, IdxColVec* pIdxColVec);
    bool bGreaterThan(Key* pKey, IdxColVec* pIdxColVec);
    
    i8 iCmp(Key* pKey, IdxColVec* pIdxColVec);
    
    bool bMatches(Key* pKey, IdxColVec* pIdxColVec, RelOpAry* pOpAry);

    bool bJuxtaCompare(eJC eJuxtaCompare, Key* pKey,
                       IdxColVec* pIdxColVec, i8 nSearchKeyCols,
                       bool& bIsEqual, eRO eRelnOp=RO_NONE);
    bool bJuxtaBefore(Key* pSearchKey, IdxColVec* pIdxColVec, i8 nSearchKeyCols,
                      bool& bIsEqual, eRO eRelnOp=RO_NONE);
    bool bJuxtaSame(Key* pSearchKey, IdxColVec* pIdxColVec, i8 nSearchKeyCols,
                    bool& bIsEqual);
    bool bJuxtaAfter(Key* pSearchKey, IdxColVec* pIdxColVec, i8 nSearchKeyCols,
                     bool& bIsEqual, eRO eRelnOp=RO_NONE);
  
    void vSprintSortKey(s48K& sBuff, IdxColVec* pIdxColVec);
  
    char* aWspArea(void);
  
    i8 iGetInt(void);
  };
  
  Key* _aKey(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
