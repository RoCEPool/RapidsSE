/*============================================================================*/
/* Report.cpp - message reporter                                              */
/*============================================================================*/

#include "Correctness.h"
#include "AM.hpp"

/*----------------------------------------------------------------------------*/
/* report a non-recoverable fatal message                                     */
/*----------------------------------------------------------------------------*/
void Correctness::vFatal(Stz* aFormat, ...)
{
  vStackDump();
  
  va_list xVaArgs;
  
  va_start(xVaArgs,aFormat);
  AM::vWriteTrace(aFormat,xVaArgs);
  va_end(xVaArgs);
  
  AM::vProcessExit();
}

/*----------------------------------------------------------------------------*/
/* report an information message                                              */
/*----------------------------------------------------------------------------*/
void Correctness::vInfo(Stz* aFormat, ...)
{
  va_list xVaArgs;
  
  va_start(xVaArgs,aFormat);
  AM::vWriteTrace(aFormat,xVaArgs);
  va_end(xVaArgs);
}

/*----------------------------------------------------------------------------*/
/* report a trace message                                                     */
/*----------------------------------------------------------------------------*/
void Correctness::vTrace(Stz* aFormat, ...)
{
  va_list xVaArgs;

  va_start(xVaArgs,aFormat);
  AM::vWriteTrace(aFormat,xVaArgs);
  va_end(xVaArgs);
}

/*----------------------------------------------------------------------------*/
/* report a warning message                                                   */
/*----------------------------------------------------------------------------*/
void Correctness::vWarn(Stz* aFormat, ...)
{
  va_list xVaArgs;
  
  va_start(xVaArgs,aFormat);
  AM::vWriteTrace(aFormat,xVaArgs);
  va_end(xVaArgs);
}

/*============================================================================*/
/* end of file                                                                */
/*============================================================================*/
