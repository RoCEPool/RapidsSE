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
  
  /*--------------------------------------------------------------------------*/
  /* KeyVec -- Key Vector                                                     */
  /*--------------------------------------------------------------------------*/
  typedef Vector<CN_KEYVEC,KeyVal,CN_KEYVAL,1024,u8> KeyVec;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
