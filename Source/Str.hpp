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
  
    Str() = default;
  
  //Str(void);
    ~Str(void);
  
    Str(const char* aLiteral) { };
  
    operator char*();
    operator const char*();
  
    void vPrint(void);
    bool bEqual(Str* aArg);
    bool bSame(Str* aArg);

    i8 iToI8(void);

    char* a(void);
  };
  
  Str* operator "" _Str(const char* aLiteral, size_t nSize);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
