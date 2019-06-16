/*============================================================================*/
/* StrVal.hpp -- null terminated string value                                 */
/*============================================================================*/
#pragma once

namespace Values
{
  class StrVal : public Value
  {
  protected:
    i8   m_nBytesFollowing;
    char m_caBytes[256];
  
  public:
    void* operator new(size_t);
  
    StrVal(void);
  
    operator Stz&(void);
    operator Stz*(void);
  
  public:
    void  vAssign(Stz* aStr);
    char* aChar(void);
    i8    nSize(void);
    Stz*  aStz(void);
    void  vToString(Stz* aStz);
  };

  StrVal* _aStrVal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
