/*============================================================================*/
/* Error.cpp -- runtime errors                                                */
/*============================================================================*/

#include "Debugging.h"

/*----------------------------------------------------------------------------*/
/* Constructor                                                                */
/*----------------------------------------------------------------------------*/
Error::Error(eEC eErrorCode, Str* aFileName, i8 iLineNo)
{
  m_eErrorCode = eErrorCode;
  m_cErrorMsg[0] = '\0';

  StrLib::vCopyLen(m_cFileName, aFileName->s(), sizeof(m_cFileName));
  m_iLineNo = iLineNo;
  
  //_checkInitialisedSize_(sizeof(Error));
}

/*----------------------------------------------------------------------------*/
/* Constructor                                                                */
/*----------------------------------------------------------------------------*/
Error::Error(eEC eErrorCode, Str* aErrorMsg, Str* aFileName, i8 iLineNo)
{
  m_eErrorCode = eErrorCode;
  StrLib::vCopyLen(m_cErrorMsg, aErrorMsg->s(), sizeof(m_cErrorMsg));

  StrLib::vCopyLen(m_cFileName, aFileName->s(), sizeof(m_cFileName));
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
