/*============================================================================*/
/* Key.hpp -- key part of an index entry                                      */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  //-----------------------------------------------------------------------------
  
  enum ePN // Position
  {
    /*  0 */ PN_NONE=0,
    /*  1 */ PN_BEFORE,
    /*  2 */ PN_SAME,
    /*  3 */ PN_AFTER,
    /*  4 */ PN_ENUMS
  };
  
  static Str* aPN[]
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

    bool bJuxtaCompare(ePN ePosition, Key* pKey, IdxColVec* pColVec);
    bool bBefore(Key* pKey, IdxColVec* pColVec);
    bool bSame(Key* pKey, IdxColVec* pColVec);
    bool bAfter(Key* pKey, IdxColVec* pColVec);
  
    char* aWspArea(void);
    Str*  aWspStr(void);
  };
  
  Key* _aKey(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
