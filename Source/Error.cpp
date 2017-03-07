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
  m_sErrorMsg[0] = '\0';

  StrLib::vCopyLen(m_sFileName, aFileName->a(), sizeof(m_sFileName));
  m_iLineNo = iLineNo;
  
  //_checkInitialisedSize_(sizeof(Error));
}

/*----------------------------------------------------------------------------*/
/* Constructor                                                                */
/*----------------------------------------------------------------------------*/
Error::Error(i8 iErrorCode, Str* aErrorMsg, Str* aFileName, i8 iLineNo)
{
  m_iErrorCode = iErrorCode;
  StrLib::vCopyLen(m_sErrorMsg, aErrorMsg->a(), sizeof(m_sErrorMsg));

  StrLib::vCopyLen(m_sFileName, aFileName->a(), sizeof(m_sFileName));
  m_iLineNo = iLineNo;

  //_checkInitialisedSize_(sizeof(Error));
}

/*----------------------------------------------------------------------------*/
/* return error message                                                       */
/*----------------------------------------------------------------------------*/
Str* Error::aErrorMsg(void)
{
  return (Str*)m_sErrorMsg;
}

/*----------------------------------------------------------------------------*/
/* return filename                                                            */
/*----------------------------------------------------------------------------*/
Str* Error::aFileName(void)
{
  return (Str*)m_sFileName;
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
