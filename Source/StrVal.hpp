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
    void  vAssign(Stz* szStr);
    char* azChar(void);
    i8    nSize(void);
    Stz*  szStz(void);
    void  vToString(Stz* szStz);
  };

  StrVal* _aStrVal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
