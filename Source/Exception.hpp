/*============================================================================*/
/* Exception.hpp : Exception -- conveys information about an exception        */
/*============================================================================*/
#pragma once

namespace Correctness
{
  class Exception
  {
  private:
    i8   m_eKind;         // kind of exception
    char m_zText[1024];   // descriptive text
    char m_zFileNm[256];  // file name/path
    i8   m_iLineNo;       // line number
  
  public:
    Exception(i8    eKind,
              char* azText,
              char* azFileNm,
              i8    iLineNo);
  
   ~Exception(void) noexcept(false);
  
    char* azFileNm(void);
    i8    eKind(void);
    i8    iLineNo(void);
    char* azText(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
