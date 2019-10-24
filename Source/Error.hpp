/*============================================================================*/
/* Error.hpp -- runtime errors                                                */
/*============================================================================*/
#pragma once

namespace Steps
{
  class Error
  {
  private:
    eEC   m_eErrorCode;
    char  m_caFileName[256];
    i8    m_iLineNo;
    char  m_caErrorMsg[256];
  
  public:
    Error(eEC eErrorCode, Stz* aFileName, i8 iLineNo);
    Error(eEC eErrorCode, Stz* aErrorMsg, Stz* aFileName, i8 iLineNo);
    
    eEC  eErrorCode(void);
    Stz* aErrorMsg(void);
    Stz* aFileName(void);
    i8   iLineNo(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
