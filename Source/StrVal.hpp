/*============================================================================*/
/* StrVal.hpp -- abstract string value                                        */
/*============================================================================*/
#pragma once

namespace Values
{
  class StrVal : public Value
  {
  protected:
    WspVbl m_xWsp;

  public:
    void* operator new(size_t);
    void* operator new(size_t, StrVbl* pThis);
  
    StrVal(eCN eClassNo);
    StrVal(eCN eClass, i8 nBytes);
    ~StrVal(void);
  
    operator Str& (void);
    operator Str* (void);
  
    Str* aStrArea(void);
  };

  StrVal* _pStrVal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
