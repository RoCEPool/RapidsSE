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
  m_cErrorMsg[0] = '\0';

  StrLib::vCopyLen(m_cFileName, aFileName->a(), sizeof(m_cFileName));
  m_iLineNo = iLineNo;
  
  //_checkInitialisedSize_(sizeof(Error));
}

/*----------------------------------------------------------------------------*/
/* Constructor                                                                */
/*----------------------------------------------------------------------------*/
Error::Error(i8 iErrorCode, Str* aErrorMsg, Str* aFileName, i8 iLineNo)
{
  m_iErrorCode = iErrorCode;
  StrLib::vCopyLen(m_cErrorMsg, aErrorMsg->a(), sizeof(m_cErrorMsg));

  StrLib::vCopyLen(m_cFileName, aFileName->a(), sizeof(m_cFileName));
  m_iLineNo = iLineNo;

  //_checkInitialisedSize_(sizeof(Error));
}

/*----------------------------------------------------------------------------*/
/* return error message                                                       */
/*----------------------------------------------------------------------------*/
Str* Error::aErrorMsg(void)
{
  return (Str*)m_cErrorMsg;
}

/*----------------------------------------------------------------------------*/
/* return filename                                                            */
/*----------------------------------------------------------------------------*/
Str* Error::aFileName(void)
{
  return (Str*)m_cFileName;
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
