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
  
  static Str* aJC[]
  {
    /*  0 */ "NONE"_Str,
    /*  1 */ "BEFORE"_Str,
    /*  2 */ "SAME"_Str,
    /*  3 */ "AFTER"_Str,
    /*  4 */ "ENUMS"_Str,
  };
  
  class Key
  {
  private:
  //i8  m_nSize;
  Wsp m_xWsp;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Key*);
  
    Key(KeyWsp& xKeyWsp);
    ~Key(void);
  
    bool operator <(Key* pKeyArg);
    bool operator <=(Key* pKeyArg);
    
    void  vShow(IdxColVec* pColVec);
  
    void  vKey(Key& xKey);
    void  vKey(KeyWsp& xKeyWsp);
  
    char* aElement(Column* pColumn, i8& nBytes, char** aaCursor);
  
    bool bLessThan(Key* pKey, IdxColVec* pColVec);
    bool bLessEqual(Key* pKey, IdxColVec* pColVec);
    bool bEqualTo(Key* pKey, IdxColVec* pColVec);
    bool bNotEqual(Key* pKey, IdxColVec* pColVec);
    bool bGreaterEqual(Key* pKey, IdxColVec* pColVec);
    bool bGreaterThan(Key* pKey, IdxColVec* pColVec);
    
    i8 iCmp(Key* pKey, IdxColVec* pColVec);
    
    bool bMatches(Key* pKey, IdxColVec* pColVec, RelOpAry* pOpAry);

    bool bJuxtaCompare(eJC ePosition, Key* pKey, IdxColVec* pColVec);
    bool bJuxtaBefore(Key* pKey, IdxColVec* pColVec);
    bool bJuxtaSame(Key* pKey, IdxColVec* pColVec);
    bool bJuxtaAfter(Key* pKey, IdxColVec* pColVec);
  
    char* aWspArea(void);
    Str*  aWspStr(void);
  };
  
  Key* _aKey(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
