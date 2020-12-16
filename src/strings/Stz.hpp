/*============================================================================*/
/* Stz.hpp -- null terminated string                                          */
/*============================================================================*/
#pragma once

namespace Resources
{
  class Stz
  {
  private:
    char m_caChar[1];
  
  public:
    void* operator new(size_t);
  
  //Stz() = default;
    Stz(void);
    Stz(Stz* szArg);
   ~Stz(void) noexcept(false);
  
    static Stz* szTab(i8 iDepth);
    static Stz* szHeight(i8 iHeight);

    void operator =(i8);
    void operator =(Stz*);
    void operator =(void*);
  
    void  vCatSprint(Stz* szFormat, ...);
    char* azChar(void);
    i8    iCompare(Stz* szArg);
    void  vConcat(Stz* szArg);
    bool  bContains(Stz* szArg);
    void  vCopy(Stz* szArg);
    void  vCopyLen(Stz* szArg, i8 nBytes);
    bool  bEqual(Stz* szArg);
    bool  bGreater(Stz* szArg);
    bool  bIsNumeric(void);
    bool  bIsSomeFalse(void);
    bool  bIsSomeTrue(void);
    i8    nLength(void);
    bool  bOneOf(Stz* szArg, ...);
    bool  bSame(Stz* szArg);
    i8    nSize(void);
    void  vSprint(Stz* szFormat, ...);
    i8    iToI8(void);
    void  vToLower(void);
  };
  
  // user defined string literal
  Stz* operator "" _Stz(const char* azLiteral, size_t nSize);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
