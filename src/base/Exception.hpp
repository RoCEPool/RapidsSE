/*============================================================================*/
/* Exception.hpp : Exception -- conveys information about an exception        */
/*============================================================================*/
#pragma once

namespace BASE
{
  class Exception
  {
  private:
    eFK  m_eFailure;      // kind of failure
    char m_zText[1024];   // descriptive text
  
    char m_zFuncNm[256];  // function name
    char m_zFileNm[256];  // file name/path
    i8   m_iLineNo;       // line number

  public:
    Exception(void);

    Exception(eFK   eFailure,
              const char* azFuncNm,
              const char* azText,
              const char* azFileNm,
              i8    iLineNo);
  
   ~Exception(void) noexcept(false);
  
    eFK   eFailure(void);
    char* azFileNm(void);
    i8    iLineNo(void);
    char* azText(void);
  
    void  vPrint(void);
    void  vSprint(char* azText);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
