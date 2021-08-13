/*============================================================================*/
/* Exception.cpp - Exception                                                  */
/*============================================================================*/

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Exception::Exception(void)
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
Exception::Exception(eFK   eFailure,
                     const char* azFuncNm,
                     const char* azText,
                     const char* azFileNm,
                     i8    iLineNo)
{
  m_eFailure = eFailure;
  StrLib::vCopyLen(m_zFuncNm,azFuncNm,sizeof(m_zFuncNm)-1);
  StrLib::vCopyLen(m_zText,  azText,  sizeof(m_zText)-1);
  StrLib::vCopyLen(m_zFileNm,azFileNm,sizeof(m_zFileNm)-1);
  m_iLineNo = iLineNo;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Exception::~Exception(void) noexcept(false)
{
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get name of file where exception was thrown                                */
/*----------------------------------------------------------------------------*/
char* Exception::azFileNm(void)
{
  return m_zFileNm;
}

/*----------------------------------------------------------------------------*/
/* get name of function where exception was thrown                            */
/*----------------------------------------------------------------------------*/
char* Exception::azFuncNm(void)
{
  return m_zFuncNm;
}

/*----------------------------------------------------------------------------*/
/* get exception kind                                                         */
/*----------------------------------------------------------------------------*/
eFK Exception::eFailure(void)
{
  return m_eFailure;
}

/*----------------------------------------------------------------------------*/
/* get line number where excception was thrown                                */
/*----------------------------------------------------------------------------*/
i8 Exception::iLineNo(void)
{
  return m_iLineNo;
}

/*----------------------------------------------------------------------------*/
/* get text describing the exception                                          */
/*----------------------------------------------------------------------------*/
char* Exception::azText(void)
{
  return m_zText;
}

/*----------------------------------------------------------------------------*/
/* print the exception                                                        */
/*----------------------------------------------------------------------------*/
void Exception::vPrint(void)
{
  char zIntoBuf[1024];
  
  vSprint(zIntoBuf);
  
  printf("%s\n",zIntoBuf);
  
  //vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* sprint the exception into given char buffer                                */
/*----------------------------------------------------------------------------*/
void Exception::vSprint(char* azIntoBuf)
{
  StrLib::vSprint((Stz*)azIntoBuf,
                  
                  "Exception eFK=%lld:%s`%s`; "
                  "__func__=%s() "
                  "__FILE__=%s:%lld"_Stz
         
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
