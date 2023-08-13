/*============================================================================*/
/* LowMemory.hpp : Low Memory Exception --                                    */
/*============================================================================*/
#pragma once

#include "Exception.hpp"

namespace BASE
{
  class LowMemory
  {
  private:
    eFK  m_eFailure;      // kind of failure
    char m_zText[1024];   // descriptive text

    char m_zFuncNm[256];  // function name
    char m_zFileNm[256];  // file name/path
    i8   m_iLineNo;       // line number

  public:
    LowMemory(void);

    LowMemory(eFK   eFailure,
              const char* azFuncNm,
              const char* azText,
              const char* azFileNm,
              i8    iLineNo);
  
   ~LowMemory(void) noexcept(false);

    eFK   eFailure(void);
    char* azFileNm(void);
    char* azFuncNm(void);
    i8    iLineNo(void);
    char* azText(void);

    void  vPrint(void);
    void  vSprint(char* azText, i8 nBufSize);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
