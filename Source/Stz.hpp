/*============================================================================*/
/* Stz.hpp -- null terminated string                                          */
/*============================================================================*/
#pragma once

namespace Strings
{
  class Stz
  {
  private:
    char m_caStr[1];
  
  public:
    void* operator new(size_t);
  
  //Stz() = default;
    Stz(void);
    Stz(Stz* aArg);
   ~Stz(void) noexcept(false);
  
    static Stz* szTab(i8 iDepth);

    void operator =(d8);
    void operator =(i8);
    void operator =(Stz*);
    void operator =(void*);
  
    void  vCatSprint(Stz* szFormat, ...);
    char* aChar(void);
    void  vConcat(Stz* szArg);
    void  vCopy(Stz* szaArg);
    void  vCopyLen(Stz* szArg, i8 nBytes);
    bool  bEqual(Stz* szArg);
    bool  bIsNumeric(void);
    bool  bIsSomeFalse(void);
    bool  bIsSomeTrue(void);
    i8    iLength(void);
    bool  bSame(Stz* szArg);
    i8    nSize(void);
    void  vSprint(Stz* szFormat, ...);
    i8    iToI8(void);
    void  vToLower(void);
  };
  
  // user defined string literal
  Stz* operator "" _Stz(const char* aLiteral, size_t nSize);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
