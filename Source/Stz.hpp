/*============================================================================*/
/* Stz.hpp -- null terminated string datum                                    */
/*============================================================================*/
#pragma once

namespace Base
{
  class Stz
  {
  private:
    
  public:
    void* operator new(size_t);
  
  //Stz() = default;
    Stz(void);
  
    Stz(Stz* aArg);

    ~Stz(void) noexcept(false);
  
    static Stz* aTab(i8 iDepth);

    void operator =(d8);
    void operator =(i8);
    void operator =(Stz*);
    void operator =(void*);
  
    void vCatSprint(Stz* aFormat, ...);
    char* aChar(void);
    void vConcat(Stz* aArg);
    void vCopy(Stz* aArg);
    void vCopyLen(Stz* aArg, i8 nBytes);
    bool bEqual(Stz* aArg);
    bool bIsNumeric(void);
    bool bIsSomeFalse(void);
    bool bIsSomeTrue(void);
    i8 iLength(void);
    bool bSame(Stz* aArg);
    i8 nSize(void);
    void vSprint(Stz* aFormat, ...);
    i8 iToI8(void);
    void vToLower(void);
  };
  
  // user defined string literal
  Stz* operator "" _Stz(const char* aLiteral, size_t nSize);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
