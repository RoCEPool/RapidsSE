/*============================================================================*/
/* Error.cpp -- runtime errors                                                */
/*============================================================================*/

#include "Error.hpp"

/*----------------------------------------------------------------------------*/
/* Constructor                                                                */
/*----------------------------------------------------------------------------*/
Error::Error(i8 iErrorCode, const char* aFileName, i8 iLineNo)
{
  m_iErrorCode = iErrorCode;
  m_aErrorMsg[0] = '\0';

  String::vCopyLen(m_aFileName, aFileName, sizeof(m_aFileName));
  m_iLineNo = iLineNo;
  
  _checkInitialisedSize_(sizeof(Error));
}

/*----------------------------------------------------------------------------*/
/* Constructor                                                                */
/*----------------------------------------------------------------------------*/
Error::Error(i8 iErrorCode, const char* aErrorMsg, const char* aFileName, i8 iLineNo)
{
  m_iErrorCode = iErrorCode;
  String::vCopyLen(m_aErrorMsg, aErrorMsg, sizeof(m_aErrorMsg));

  String::vCopyLen(m_aFileName, aFileName, sizeof(m_aFileName));
  m_iLineNo = iLineNo;

  _checkInitialisedSize_(sizeof(Error));
}

/*----------------------------------------------------------------------------*/
/* return error message                                                       */
/*----------------------------------------------------------------------------*/
const char* Error::aErrorMsg(void)
{
  return m_aErrorMsg;
}

/*----------------------------------------------------------------------------*/
/* return filename                                                            */
/*----------------------------------------------------------------------------*/
const char* Error::aFileName(void)
{
  return m_aFileName;
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
