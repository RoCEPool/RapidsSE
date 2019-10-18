/*============================================================================*/
/* Exception.hpp : Exception -- conveys information about an exception        */
/*============================================================================*/
#pragma once

namespace Correctness
{
  class Exception
  {
  private:
    eFK  m_eKind;         // kind of exception
    char m_zText[1024];   // descriptive text
    char m_zFileNm[256];  // file name/path
    i8   m_iLineNo;       // line number
  
    Job* m_pJob;          // in which job the exception occurred
  
  public:
    Exception(eFK   eKind,
              const char* azText,
              const char* azFileNm,
              i8    iLineNo);
  
   ~Exception(void) noexcept(false);
  
    char* azFileNm(void);
    Job*  pJob(void);
    void  vJob(Job* pJob);
    eFK   eKind(void);
    i8    iLineNo(void);
    char* azText(void);
  
    void  vPrint(void);
    void  vSprint(char* azText);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
