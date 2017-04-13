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
  
  };
  
  KeyVal* _aKeyVal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
