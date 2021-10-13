/*============================================================================*/
/* Trace.cpp - message reporter                                               */
/*============================================================================*/

#include "BASE.h"

#include "SP_Margin.hpp"

/*----------------------------------------------------------------------------*/
/* report a trace message                                                     */
/*----------------------------------------------------------------------------*/
void BASE::vTrace(Stz* aFormat, ...)
{
  va_list xVarArgs;

  va_start(xVarArgs,aFormat);
  AM::vWriteTrace_vargs(aFormat,xVarArgs);
  va_end(xVarArgs);
}

/*----------------------------------------------------------------------------*/
/* report a trace message (and optionally write to file)                      */
/*----------------------------------------------------------------------------*/
void BASE::vTraceF(File* pFile, Stz* aFormat, ...)
{
  va_list xVarArgs;

  va_start(xVarArgs,aFormat);
  if (pFile) {
    AM::vWriteTrace_vargs(pFile,aFormat,xVarArgs);
  }
  else {
    AM::vWriteTrace_vargs(aFormat,xVarArgs);
  }
  va_end(xVarArgs);
}

/*----------------------------------------------------------------------------*/
/* report a trace message (with margin)                                       */
/*----------------------------------------------------------------------------*/
void BASE::vTraceM(Stz* aFormat, ...)
{
  va_list xVarArgs;

  SP_Margin spMargin;
 
  vTrace("%s "_Stz,spMargin.sz());

  va_start(xVarArgs,aFormat);
  AM::vWriteTrace_vargs(aFormat,xVarArgs);
  va_end(xVarArgs);
}

/*============================================================================*/
/* end of file                                                                */
/*============================================================================*/
