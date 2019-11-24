/*============================================================================*/
/* StrVal.hpp -- Layout of a String when on Stack                             */
/*============================================================================*/
#pragma once

namespace SenVM
{
  class StrVal : public StkVal
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
    Stz*  szStz(void);

    i8    nStackBytes(void);
    void  vToString(Stz* szStz);
  };

  StrVal* _aStrVal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
