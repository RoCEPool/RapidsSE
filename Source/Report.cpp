/*===========================================================================*/
/* Report.cpp - message reporter                                             */
/*===========================================================================*/

#include "Debugging.h"

#include "StorageEngine.h"

extern File*       gpTraceFile;
extern BufferPool* gpBufferPool;
extern AttachMap*  gpAttachMap;


/*---------------------------------------------------------------------------*/
/* report an information message                                             */
/*---------------------------------------------------------------------------*/
void Debugging::vInfo(Stz* aFormat, ...)
{
  va_list xArgs;
  
  va_start(xArgs,aFormat);
  vPrint(C_REPORT_INFO,aFormat,xArgs);
  va_end(xArgs);
}

/*---------------------------------------------------------------------------*/
/* report a trace message                                                    */
/*---------------------------------------------------------------------------*/
void Debugging::vTrace(Stz* aFormat, ...)
{
  va_list xArgs;

  va_start(xArgs,aFormat);
  vPrint(C_REPORT_TRACE,aFormat,xArgs);
  va_end(xArgs);
}

/*---------------------------------------------------------------------------*/
/* report a warning message                                                  */
/*---------------------------------------------------------------------------*/
void Debugging::vWarn(Stz* aFormat, ...)
{
  va_list xArgs;
  
  va_start(xArgs,aFormat);
  vPrint(C_REPORT_WARNING,aFormat,xArgs);
  va_end(xArgs);
}

/*---------------------------------------------------------------------------*/
/* report a recoverable error message                                        */
/*---------------------------------------------------------------------------*/
void Debugging::vError(Stz* aFormat, ...)
{
  va_list xArgs;
  
  va_start(xArgs,aFormat);
  vPrint(C_REPORT_ERROR,aFormat,xArgs);
  va_end(xArgs);
}

/*---------------------------------------------------------------------------*/
/* vFatal: report a non-recoverable fatal message                            */
/*---------------------------------------------------------------------------*/
__attribute__((noreturn)) void Debugging::vFatal(Stz* aFormat, ...)
{
  vStackDump();

  va_list xArgs;
  
  va_start(xArgs,aFormat);
  vPrint(C_REPORT_FATAL,aFormat,xArgs);
  va_end(xArgs);
  
  exit(EXIT_FAILURE);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* write to trace file etc                                                   */
/*---------------------------------------------------------------------------*/
void Debugging::vPrint(i8 iLevel, Stz* aFormat, va_list xArgs)
{
  static Lock oLock(LK_TRACE);
  static char caPrintLine[C_1MB] = {'\0'};
  
  bool bLock   = (gpAttachMap ? true : false);
  bool bLowMem = gpBufferPool ? gpBufferPool->bLowMemory() : false;
  
  if (bLock)
    oLock.vGrabWrite();
  
  i8 iLen = 0;
  char* s = caPrintLine;
  *s = '\0';
  
  assert(iLevel != 0);
  vsnprintf(s,sizeof(caPrintLine)-iLen-3,aFormat->aChar(),xArgs);
  
  // print to file
  fprintf(stdout,"%s%s",bLowMem ? "LOW-MEMORY! " : "",caPrintLine);
  fflush(stdout);
  
  if (gpTraceFile) {
    gpTraceFile->vPrint("%s%s"_Stz,bLowMem ? "LOW-MEMORY! " : "",caPrintLine);
    gpTraceFile->vFlush();
  }
  
  if (bLock)
    oLock.vDropWrite();
}

/*===========================================================================*/
/* end of file                                                               */
/*===========================================================================*/
