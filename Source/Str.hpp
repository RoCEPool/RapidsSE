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
    
    void operator =(i8);
    void operator =(Str*);
    void operator =(void*);
  
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
    i8 nSize(void);

    char* s(void);
  };
  
  Str* operator "" _Str(const char* aLiteral, size_t nSize);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
