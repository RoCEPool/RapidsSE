#include "BASE.h"

/*============================================================================*/
/* InternalError.cpp - Internal Error Exception                               */
/*============================================================================*/



/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
InternalError::InternalError(void)
{
  m_eFailure = FK_NONE;
  MemLib::vZero(m_zFuncNm,sizeof(m_zFuncNm),CN_TASK);
  MemLib::vZero(m_zText  ,sizeof(m_zText),  CN_TASK);
  MemLib::vZero(m_zFileNm,sizeof(m_zFileNm),CN_TASK);
  m_iLineNo = C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* constructor (long-form)                                                    */
/*----------------------------------------------------------------------------*/
InternalError::InternalError(eFK   eFailure,
                     const char* azFuncNm,
                     const char* azText,
                     const char* azFileNm,
                     i8    iLineNo) //: LowMemory(eFailure, azFuncNm, azText, azFileNm, iLineNo)
{
  m_eFailure = eFailure;
  StrLib::vCopy(m_zFuncNm, sizeof(m_zFuncNm), azFuncNm);
  StrLib::vCopy(m_zText,   sizeof(m_zText),   azText);
  StrLib::vCopy(m_zFileNm, sizeof(m_zFileNm), azFileNm);
  m_iLineNo = iLineNo;

}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
InternalError::~InternalError(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/* get name of file where exception was thrown                                */
/*----------------------------------------------------------------------------*/
char* InternalError::azFileNm(void)
{
  return m_zFileNm;
}

/*----------------------------------------------------------------------------*/
/* get name of function where exception was thrown                            */
/*----------------------------------------------------------------------------*/
char* InternalError::azFuncNm(void)
{
  return m_zFuncNm;
}

/*----------------------------------------------------------------------------*/
/* get exception kind                                                         */
/*----------------------------------------------------------------------------*/
eFK InternalError::eFailure(void)
{
  return m_eFailure;
}

/*----------------------------------------------------------------------------*/
/* get line number where excception was thrown                                */
/*----------------------------------------------------------------------------*/
i8 InternalError::iLineNo(void)
{
  return m_iLineNo;
}

/*----------------------------------------------------------------------------*/
/* get text describing the exception                                          */
/*----------------------------------------------------------------------------*/
char* InternalError::azText(void)
{
  return m_zText;
}

/*----------------------------------------------------------------------------*/
/* print the exception                                                        */
/*----------------------------------------------------------------------------*/
void InternalError::vPrint(void)
{
  const i8 nBufSize = 1024;
  char zIntoBuf[nBufSize];

  vSprint(zIntoBuf, nBufSize);

  printf("%s\n",zIntoBuf);

  //vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* sprint the exception into given char buffer                                */
/*----------------------------------------------------------------------------*/
void InternalError::vSprint(char* azIntoBuf, i8 nBufSize)
{
  StrLib::vSprint((Stz*)azIntoBuf,
                  nBufSize,

                  "Exception eFK=%lld:%s`%s`; "
                  "%s() "
                  "(%s:%lld)"_Stz

      ,(i8)m_eFailure
      ,aFK[eFailure()]->azChar()
      ,azText()

      ,m_zFuncNm

      ,azFileNm()
      ,iLineNo()
  );
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
