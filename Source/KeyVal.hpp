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
  
    char* aElement(Column* pColumn, i8& nBytes, char** aaCursor);
 
    bool bLessThan(KeyVal* pKeyVal, ColumnVec* pColVec);
    bool bLessEqual(KeyVal* pKeyVal, ColumnVec* pColVec);
    bool bEqualTo(KeyVal* pKeyVal, ColumnVec* pColVec);
    bool bNotEqual(KeyVal* pKeyVal, ColumnVec* pColVec);
    bool bGreaterEqual(KeyVal* pKeyVal, ColumnVec* pColVec);
    bool bGreaterThan(KeyVal* pKeyVal, ColumnVec* pColVec);
  
    i8 iCmp(KeyVal* pKeyVal, ColumnVec* pColVec);
  
  };
  
  KeyVal* _aKeyVal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
