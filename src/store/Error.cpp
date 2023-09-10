/*============================================================================*/
/* Error.cpp -- runtime errors                                                */
/*============================================================================*/

#include "BASE.h"

#include "Error.hpp"

/*----------------------------------------------------------------------------*/
/* Constructor                                                                */
/*----------------------------------------------------------------------------*/
Error::Error(void)
{
  m_eErrorCode = EC_NONE;
  m_caErrorMsg[0] = '\0';

  m_caFileName[0] = '\0';
  m_iLineNo = C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* Constructor                                                                */
/*----------------------------------------------------------------------------*/
Error::Error(eEC eErrorCode, Stz* aErrorMsg)
{
  m_eErrorCode = eErrorCode;
  StrLib::vCopyLen(m_caErrorMsg, aErrorMsg->azChar(), sizeof(m_caErrorMsg));

  m_caFileName[0] = '\0';
  m_iLineNo = C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* Constructor                                                                */
/*----------------------------------------------------------------------------*/
Error::Error(eEC eErrorCode, Stz* aFileName, i8 iLineNo)
{
  m_eErrorCode = eErrorCode;
  m_caErrorMsg[0] = '\0';

  StrLib::vCopyLen(m_caFileName, aFileName->azChar(), sizeof(m_caFileName));
  m_iLineNo = iLineNo;
  
  //_checkInitialisedSize_(sizeof(Error));
}

/*----------------------------------------------------------------------------*/
/* Constructor                                                                */
/*----------------------------------------------------------------------------*/
Error::Error(eEC eErrorCode, Stz* aErrorMsg, Stz* aFileName, i8 iLineNo)
{
  m_eErrorCode = eErrorCode;
  StrLib::vCopyLen(m_caErrorMsg, aErrorMsg->azChar(), sizeof(m_caErrorMsg));

  StrLib::vCopyLen(m_caFileName, aFileName->azChar(), sizeof(m_caFileName));
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
  return (Stz*)m_caErrorMsg;
}

/*----------------------------------------------------------------------------*/
/* return filename                                                            */
/*----------------------------------------------------------------------------*/
Stz* Error::aFileName(void)
{
  return (Stz*)m_caFileName;
}

/*----------------------------------------------------------------------------*/
/* return line number                                                         */
/*----------------------------------------------------------------------------*/
i8 Error::iLineNo(void)
{
  return m_iLineNo;
}

/*----------------------------------------------------------------------------*/
/* set error                                                                  */
/*----------------------------------------------------------------------------*/
void Error::vSet(Error& rxError)
{
  m_eErrorCode = rxError.eErrorCode();
  StrLib::vCopyLen(m_caErrorMsg, rxError.aErrorMsg()->azChar(), sizeof(m_caErrorMsg));

  StrLib::vCopyLen(m_caFileName, rxError.aFileName()->azChar(), sizeof(m_caFileName));
  m_iLineNo = rxError.iLineNo();
}

/*----------------------------------------------------------------------------*/
/* set error                                                                  */
/*----------------------------------------------------------------------------*/
void Error::vSet(eEC eErrorCode, Stz* aErrorMsg)
{
  m_eErrorCode = eErrorCode;
  StrLib::vCopyLen(m_caErrorMsg, aErrorMsg->azChar(), sizeof(m_caErrorMsg));
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
