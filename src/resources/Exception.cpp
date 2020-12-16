/*============================================================================*/
/* Exception.cpp - Exception                                                  */
/*============================================================================*/

#include "Resources.h"


/*
 Notes:
 1) An exception generally occurs in the context of a job
 2) The exception is thrown by value (ie the Excpetion object is copied
    entirely on the stack
 3) The exception is caught by value in a task-level catch
 4) The task-level catch can determine job etc
 */

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor (long-form)                                                    */
/*----------------------------------------------------------------------------*/
Exception::Exception(eFK   eKind,
                     eVL   eSeverity,
                     const char* azFuncNm,
                     const char* azText,
                     const char* azFileNm,
                     i8    iLineNo)
{
  m_eKind = eKind;
  m_eSeverity = eSeverity;
  StrLib::vCopy(m_zFuncNm,azFuncNm);
  StrLib::vCopy(m_zText,azText);   //! #030 this should be length delimited copy
  StrLib::vCopy(m_zFileNm,azFileNm);
  m_iLineNo = iLineNo;
  
  // at the time of the exception we may not know the job
  m_pJob = nullptr;
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
/* get name of file where exception occurred                                  */
/*----------------------------------------------------------------------------*/
char* Exception::azFileNm(void)
{
  return m_zFileNm;
}

/*----------------------------------------------------------------------------*/
/* get job where exception occurred                                           */
/*----------------------------------------------------------------------------*/
Job* Exception::pJob(void)
{
  return m_pJob;
}

/*----------------------------------------------------------------------------*/
/* set job where exception occurred                                           */
/*----------------------------------------------------------------------------*/
void Exception::vJob(Job* pJob)
{
  m_pJob = pJob;
}

/*----------------------------------------------------------------------------*/
/* get exception kind                                                         */
/*----------------------------------------------------------------------------*/
eFK Exception::eKind(void)
{
  return m_eKind;
}

/*----------------------------------------------------------------------------*/
/* get line number where excception occurred                                  */
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
                  
                  "Exception : %s of condition (%s) "
                  "for job %s "
                  "thrown in func %s() file %s line %lld"_Stz,
         
                  aFK[eKind()]->azChar(),
                  azText(),
                  
                  pJob()
                    ? aCN[((Object*)pJob())->eClass()]->azChar()
                    : "<null>",

                  m_zFuncNm,
                  azFileNm(),
                  iLineNo()
                  );
}


/*----------------------------------------------------------------------------*/
/* handle an assertion failure                                                */
/*----------------------------------------------------------------------------*/
void Exception::vCaught(void)
{
  // put a breakpoint here to trap throws
  sleep(0);
  
  vPrint();
  
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
