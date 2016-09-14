/*============================================================================*/
/* Error -- errors                                                            */
/*============================================================================*/

#pragma once

#include "Magic.h"

namespace Magic
{
  enum eEC // error code
  {
    /* 0 */ EC_NONE=0,
    /* 1 */ EC_ASSERT,
    /* 2 */ EC_INVALID_CLASS,
    /* 3 */ EC_INVALID_ARGUMENT,
    /* 4 */ EC_NULLPTR,
    /* 5 */ EC_ENUMS
  };
  
  static const char* const pEC[]
  {
    /* 0 */ "NONE",
    /* 1 */ "ASSERT",
    /* 2 */ "INVALID_CLASS",
    /* 3 */ "INVALID_ARGUMENT",
    /* 4 */ "NULLPTR",
    /* 5 */ "ENUMS"
  };

  class Error
  {
  private:
    eEC   m_eErrorCode;
    s256  m_sFileName;
    i8    m_iLineNumber;
    s256  m_sErrorMsg;
    
    Footer m_xFooter;
    
  public:
    Error(eEC eErrorCode, const char* sFile, i8 iLine);
    Error(eEC eErrorCode, const char *errormsg, const char* sFile, i8 iLine);
    
    const char* pErrorMsg(void);
    char*       pFileName(void);
    i8          iLineNumber(void);
    
    Footer* pFooter(void);
  };
  
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
