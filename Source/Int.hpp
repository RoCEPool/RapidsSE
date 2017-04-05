/*============================================================================*/
/* Int.hpp -- int datum                                                       */
/*============================================================================*/
#pragma once

namespace Datums
{
  class Int
  {
  private:
    
  public:
    void* operator new(size_t);
  
    Int(void);
    ~Int(void);
  
    static bool bExtractInt(Str* aDst, Str* aSrc, i8& nBytesTaken);
    static void vSprint(Str* aDst, Str* aSrc, i8& nBytes);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
