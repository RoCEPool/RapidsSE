/*============================================================================*/
/* KeyVal.hpp -- Key Value                                                    */
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
  
    KeyVal(KeyWrk& xKeyWrk);
    ~KeyVal(void) noexcept(false);
  
    char* aEachDatum (Column* pColumn, i8& nBytes, char** aaCursor);
 
    bool bLessThan(KeyVal* aKeyVal, IdxColVec* pColVec);
    bool bLessEqual(KeyVal* aKeyVal, IdxColVec* pColVec);
    bool bEqualTo(KeyVal* aKeyVal, IdxColVec* pColVec);
    bool bNotEqual(KeyVal* aKeyVal, IdxColVec* pColVec);
    bool bGreaterEqual(KeyVal* aKeyVal, IdxColVec* pColVec);
    bool bGreaterThan(KeyVal* aKeyVal, IdxColVec* pColVec);
  
    i8 iCmp(KeyVal* aKeyVal, IdxColVec* pColVec);
  
    bool bMatches(KeyVal* aKeyVal, IdxColVec* pColVec, RelOpAry* pOpAry);
    void vShow(IdxColVec* pColVec);
  
    Key& xKey(void);
  
  };
  
  KeyVal* _aKeyVal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
