/*============================================================================*/
/* Error.cpp -- runtime errors                                                */
/*============================================================================*/

#include "Debugging.h"

/*----------------------------------------------------------------------------*/
/* Constructor                                                                */
/*----------------------------------------------------------------------------*/
Error::Error(i8 iErrorCode, Str* aFileName, i8 iLineNo)
{
  m_iErrorCode = iErrorCode;
  m_aErrorMsg[0] = '\0';

  StrLib::vCopyLen(m_aFileName, aFileName->a(), sizeof(m_aFileName));
  m_iLineNo = iLineNo;
  
  //_checkInitialisedSize_(sizeof(Error));
}

/*----------------------------------------------------------------------------*/
/* Constructor                                                                */
/*----------------------------------------------------------------------------*/
Error::Error(i8 iErrorCode, Str* aErrorMsg, Str* aFileName, i8 iLineNo)
{
  m_iErrorCode = iErrorCode;
  StrLib::vCopyLen(m_aErrorMsg, aErrorMsg->a(), sizeof(m_aErrorMsg));

  StrLib::vCopyLen(m_aFileName, aFileName->a(), sizeof(m_aFileName));
  m_iLineNo = iLineNo;

  //_checkInitialisedSize_(sizeof(Error));
}

/*----------------------------------------------------------------------------*/
/* return error message                                                       */
/*----------------------------------------------------------------------------*/
Str* Error::aErrorMsg(void)
{
  return (Str*)m_aErrorMsg;
}

/*----------------------------------------------------------------------------*/
/* return filename                                                            */
/*----------------------------------------------------------------------------*/
Str* Error::aFileName(void)
{
  return (Str*)m_aFileName;
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
