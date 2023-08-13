/*============================================================================*/
/* LowMemory.cpp - Low Memory Exception                                       */
/*============================================================================*/

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
LowMemory::LowMemory(void)
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
LowMemory::LowMemory(eFK   eFailure,
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
LowMemory::~LowMemory(void) noexcept(false)
{
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get name of file where exception was thrown                                */
/*----------------------------------------------------------------------------*/
char* LowMemory::azFileNm(void)
{
  return m_zFileNm;
}

/*----------------------------------------------------------------------------*/
/* get name of function where exception was thrown                            */
/*----------------------------------------------------------------------------*/
char* LowMemory::azFuncNm(void)
{
  return m_zFuncNm;
}

/*----------------------------------------------------------------------------*/
/* get exception kind                                                         */
/*----------------------------------------------------------------------------*/
eFK LowMemory::eFailure(void)
{
  return m_eFailure;
}

/*----------------------------------------------------------------------------*/
/* get line number where excception was thrown                                */
/*----------------------------------------------------------------------------*/
i8 LowMemory::iLineNo(void)
{
  return m_iLineNo;
}

/*----------------------------------------------------------------------------*/
/* get text describing the exception                                          */
/*----------------------------------------------------------------------------*/
char* LowMemory::azText(void)
{
  return m_zText;
}

/*----------------------------------------------------------------------------*/
/* print the exception                                                        */
/*----------------------------------------------------------------------------*/
void LowMemory::vPrint(void)
{
  i8 nBufSize = 1024;
  char zIntoBuf[nBufSize];

  vSprint(zIntoBuf, nBufSize);

  printf("%s\n",zIntoBuf);

  //vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* sprint the exception into given char buffer                                */
/*----------------------------------------------------------------------------*/
void LowMemory::vSprint(char* azIntoBuf, i8 nBufSize)
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
