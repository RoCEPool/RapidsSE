/*============================================================================*/
/* Error -- errors                                                            */
/*============================================================================*/

#include "Error.hpp"
#include "String.hpp"

/*----------------------------------------------------------------------------*/
/* Constructor                                                                */
/*----------------------------------------------------------------------------*/
Error::Error(eEC eErrorCode, const char* sFile, i8 iLine)
{
  m_eErrorCode = eErrorCode;
  String::vCopy(m_sFileName.a(), sFile);
  m_iLineNumber = iLine;
  String::vCopy(m_sErrorMsg.a(), pEC[m_eErrorCode]);
  
  _checkInitialisedSize_(sizeof(Error));
}

/*----------------------------------------------------------------------------*/
/* Constructor                                                                */
/*----------------------------------------------------------------------------*/
Error::Error(eEC eErrorCode, const char* pErrorMsg, const char* sFile, i8 iLine)
{
  m_eErrorCode = eErrorCode;
  String::vCopy(m_sFileName.a(), sFile);
  m_iLineNumber = iLine;
  String::vCopy(m_sErrorMsg.a(), pEC[m_eErrorCode]);
  String::vConcat(m_sErrorMsg.a(), " (");
  String::vConcat(m_sErrorMsg.a(), pErrorMsg);
  String::vConcat(m_sErrorMsg.a(), ") ");

  _checkInitialisedSize_(sizeof(Error));
}

/*----------------------------------------------------------------------------*/
/* return error message                                                       */
/*----------------------------------------------------------------------------*/
const char* Error::pErrorMsg(void)
{
  return m_sErrorMsg.a();
}

/*----------------------------------------------------------------------------*/
/* return filename                                                            */
/*----------------------------------------------------------------------------*/
char* Error::pFileName(void)
{
  return m_sFileName.a();
}

/*----------------------------------------------------------------------------*/
/* return line number                                                         */
/*----------------------------------------------------------------------------*/
i8 Error::iLineNumber(void)
{
  return m_iLineNumber;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Error::pFooter(void)
{
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/