/*============================================================================*/
/* Int.hpp -- int datum                                                       */
/*============================================================================*/
#pragma once

namespace Datums
{
  class Int
  {
  friend class DtmLib;
  private:
    
  public:
    void* operator new(size_t);
  
    Int(void);
    ~Int(void);
  
    void vInitZero(void);
  
    static bool bExtract(Str* aDst, Str* aSrc, i8& nBytesTaken);
    static void vSprint(Str* aDst, Str* aSrc, i8& nBytesTaken);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
