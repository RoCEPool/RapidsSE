/*============================================================================*/
/* Error -- errors                                                            */
/*============================================================================*/

#pragma once

#include "Common.h"

namespace Common
{
  enum eEC // error code
  {
    /* 0 */ EC_NONE=0,
    /* 1 */ EC_ASSERT,
    /* 2 */ EC_INVALID_CLASS,
    /* 3 */ EC_NULLPTR,
    /* 4 */ EC_ENUMS
  };
  
  static const char *pEC[]
  {
    /* 0 */ "NONE",
    /* 1 */ "ASSERT",
    /* 2 */ "INVALID_CLASS",
    /* 3 */ "NULLPTR",
    /* 4 */ "ENUMS"
  };

  class Error
  {
  private:
    eEC   m_eErrorCode;
    s256  m_sFileName;
    i8    m_iLineNumber;
    s256  m_sErrorMsg;
    
  public:
    Error(eEC eErrorCode, const char* sFile, i8 iLine);
    Error(eEC eErrorCode, const char *errormsg, const char* sFile, i8 iLine);
    
    const char* pErrorMsg(void);
    char*       pFileName(void);
    i8          iLineNumber(void);
  };
  
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
