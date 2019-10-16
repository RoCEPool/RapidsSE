/*============================================================================*/
/* StrVal.hpp -- null terminated string value                                 */
/*============================================================================*/
#pragma once

namespace SenVM
{
  class StrVal : public ForthValue
  {
  protected:
    i8   m_nBytesFollowing;
    char m_caStr[256];
  
  public:
    void* operator new(size_t);
  
    StrVal(void);
  
  public:
    void  vAssign(Stz* aStr);
    char* azChar(void);
    i8    nSize(void);
    Stz*  aStz(void);
    void  vToString(Stz* aStz);
  };

  StrVal* _aStrVal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
