/*============================================================================*/
/* Str.hpp -- str datum                                                       */
/*============================================================================*/
#pragma once

namespace Base
{
  class Str
  {
  private:
    
  public:
    void* operator new(size_t);
  
  //Str() = default;
    Str(void);
  
    Str(Str* aArg);

    ~Str(void);
    
    void operator =(Str*);
  
    void vCopy(Str* aArg);
    void vCopyLen(Str* aArg, i8 nBytes);
    void vConcat(Str* aArg);
    bool bEqual(Str* aArg);
    bool bIsNumeric(void);

    void vPrint(void);
    bool bSame(Str* aArg);
    void vSprint(Str* aFormat, ...);

    i8    iToI8(void);
    void  vToLower(void);
  
    i8 iLength(void);

    char* a(void);
  };
  
  Str* operator "" _Str(const char* aLiteral, size_t nSize);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
