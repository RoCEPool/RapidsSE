/*============================================================================*/
/* Key.hpp -- key part of an index entry                                      */
/*============================================================================*/
#pragma once

namespace DBobjects
{
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
    
    char* aWspArea(void);
    Str*  aWspStr(void);
  };
  
  Key* _aKey(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
