/*============================================================================*/
/* Trace.cpp - message reporter                                               */
/*============================================================================*/

#include "Resources.h"

/*----------------------------------------------------------------------------*/
/* report an information message                                              */
/*----------------------------------------------------------------------------*/
void Resources::vInfo(Stz* aFormat, ...)
{
  va_list xVaArgs;
  
  va_start(xVaArgs,aFormat);
  AM::vWriteTrace_vargs(YA_SYS_INFO,aFormat,xVaArgs);
  va_end(xVaArgs);
}

/*----------------------------------------------------------------------------*/
/* report a trace message                                                     */
/*----------------------------------------------------------------------------*/
void Resources::vTrace(Stz* aFormat, ...)
{
  va_list xVaArgs;

  va_start(xVaArgs,aFormat);
  AM::vWriteTrace_vargs(YA_SYS_TRACE,aFormat,xVaArgs);
  va_end(xVaArgs);
}

/*----------------------------------------------------------------------------*/
/* report a trace message (and optionally write to file)                      */
/*----------------------------------------------------------------------------*/
void Resources::vTraceF(File* pFile, Stz* aFormat, ...)
{
  va_list xVaArgs;

  va_start(xVaArgs,aFormat);
  if (pFile) {
    AM::vWriteTrace_vargs(pFile,YA_SYS_TRACE,aFormat,xVaArgs);
  }
  else {
    AM::vWriteTrace_vargs(YA_SYS_TRACE,aFormat,xVaArgs);
  }
  va_end(xVaArgs);
}

/*----------------------------------------------------------------------------*/
/* report a warning message                                                   */
/*----------------------------------------------------------------------------*/
void Resources::vWarn(Stz* aFormat, ...)
{
  va_list xVaArgs;
  
  va_start(xVaArgs,aFormat);
  AM::vWriteTrace_vargs(YA_SYS_WARN,aFormat,xVaArgs);
  va_end(xVaArgs);
}

/*============================================================================*/
/* end of file                                                                */
/*============================================================================*/
