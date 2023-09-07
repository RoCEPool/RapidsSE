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
/* report a trace message                                                     */
/*----------------------------------------------------------------------------*/
void BASE::vTrace1(Stz1<char> aFormat, ...)
{
  va_list xVarArgs;

  va_start(xVarArgs,aFormat);
  AM::vWriteTrace1_vargs(aFormat,xVarArgs);
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

/*----------------------------------------------------------------------------*/
/* report a trace message from a shared object calling back into SE.          */
/*----------------------------------------------------------------------------*/
extern "C" void vTraceExternal(char* psFormat, ...)
{
  va_list xVarArgs;

  va_start(xVarArgs,(Stz*) psFormat);
  AM::vWriteTrace_vargs((Stz*) psFormat,xVarArgs);
  va_end(xVarArgs);
}

/*============================================================================*/
/* end of file                                                                */
/*============================================================================*/
