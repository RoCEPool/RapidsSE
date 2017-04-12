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
  
//    KeyVal(void);
//    KeyVal(i8 iInt);
    KeyVal(KeyWsp& xKeyWsp);
    ~KeyVal(void);
  };
  
  KeyVal* _aKeyVal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
