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
  
  va_list xArgs;
  
  va_start(xArgs,aFormat);
  StorageEngine::AM::vWriteTrace(aFormat,xArgs);
  va_end(xArgs);
  
  Process::vExit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* report an information message                                              */
/*----------------------------------------------------------------------------*/
void Correctness::vInfo(Stz* aFormat, ...)
{
  va_list xArgs;
  
  va_start(xArgs,aFormat);
  StorageEngine::AM::vWriteTrace(aFormat,xArgs);
  va_end(xArgs);
}

/*----------------------------------------------------------------------------*/
/* report a trace message                                                     */
/*----------------------------------------------------------------------------*/
void Correctness::vTrace(Stz* aFormat, ...)
{
  va_list xArgs;

  va_start(xArgs,aFormat);
  StorageEngine::AM::vWriteTrace(aFormat,xArgs);
  va_end(xArgs);
}

/*----------------------------------------------------------------------------*/
/* report a warning message                                                   */
/*----------------------------------------------------------------------------*/
void Correctness::vWarn(Stz* aFormat, ...)
{
  va_list xArgs;
  
  va_start(xArgs,aFormat);
  StorageEngine::AM::vWriteTrace(aFormat,xArgs);
  va_end(xArgs);
}

/*============================================================================*/
/* end of file                                                                */
/*============================================================================*/
