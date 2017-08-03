/*============================================================================*/
/* Error.cpp -- runtime errors                                                */
/*============================================================================*/

#include "Debugging.h"

/*----------------------------------------------------------------------------*/
/* Constructor                                                                */
/*----------------------------------------------------------------------------*/
Error::Error(eEC eErrorCode, Stz* aFileName, i8 iLineNo)
{
  m_eErrorCode = eErrorCode;
  m_cErrorMsg[0] = '\0';

  StrLib::vCopyLen(m_cFileName, aFileName->aChar(), sizeof(m_cFileName));
  m_iLineNo = iLineNo;
  
  //_checkInitialisedSize_(sizeof(Error));
}

/*----------------------------------------------------------------------------*/
/* Constructor                                                                */
/*----------------------------------------------------------------------------*/
Error::Error(eEC eErrorCode, Stz* aErrorMsg, Stz* aFileName, i8 iLineNo)
{
  m_eErrorCode = eErrorCode;
  StrLib::vCopyLen(m_cErrorMsg, aErrorMsg->aChar(), sizeof(m_cErrorMsg));

  StrLib::vCopyLen(m_cFileName, aFileName->aChar(), sizeof(m_cFileName));
  m_iLineNo = iLineNo;

  //_checkInitialisedSize_(sizeof(Error));
}

/*----------------------------------------------------------------------------*/
/* return error code                                                          */
/*----------------------------------------------------------------------------*/
eEC Error::eErrorCode(void)
{
  return m_eErrorCode;
}

/*----------------------------------------------------------------------------*/
/* return error message                                                       */
/*----------------------------------------------------------------------------*/
Stz* Error::aErrorMsg(void)
{
  return (Stz*)m_cErrorMsg;
}

/*----------------------------------------------------------------------------*/
/* return filename                                                            */
/*----------------------------------------------------------------------------*/
Stz* Error::aFileName(void)
{
  return (Stz*)m_cFileName;
}

/*----------------------------------------------------------------------------*/
/* return line number                                                         */
/*----------------------------------------------------------------------------*/
i8 Error::iLineNo(void)
{
  return m_iLineNo;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
