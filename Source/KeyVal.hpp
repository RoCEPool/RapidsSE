/*============================================================================*/
/* KeyVal.hpp -- key value                                                    */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class KeyVal : public Object
  {
  public:
    Key m_xKey;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
//    KeyVal(void);
//    KeyVal(i8 iInt);
    KeyVal(KeyWsp& xKeyWsp);
    ~KeyVal(void);
  
    char* aEachDatum (Column* pColumn, i8& nBytes, char** aaCursor);
 
    bool bLessThan(KeyVal* pKeyVal, IdxColVec* pColVec);
    bool bLessEqual(KeyVal* pKeyVal, IdxColVec* pColVec);
    bool bEqualTo(KeyVal* pKeyVal, IdxColVec* pColVec);
    bool bNotEqual(KeyVal* pKeyVal, IdxColVec* pColVec);
    bool bGreaterEqual(KeyVal* pKeyVal, IdxColVec* pColVec);
    bool bGreaterThan(KeyVal* pKeyVal, IdxColVec* pColVec);
  
    i8 iCmp(KeyVal* pKeyVal, IdxColVec* pColVec);
  
    bool bMatches(KeyVal* pKeyVal, IdxColVec* pColVec, RelOpAry* pOpAry);
    void vShow(IdxColVec* pColVec);
  
    Key& xKey(void);
  
  };
  
  KeyVal* _aKeyVal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
