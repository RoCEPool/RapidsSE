/*============================================================================*/
/* Report.cpp - message reporter                                              */
/*============================================================================*/

#include "Debugging.h"
#include "StorageEngine.h"

extern File*       gpTraceFile;
extern BufferPool* gpBufferPool;
extern AttachMap*  gpAttachMap;


/*----------------------------------------------------------------------------*/
/* report a non-recoverable fatal message                                     */
/*----------------------------------------------------------------------------*/
void Debugging::vFatal(Stz* aFormat, ...)
{
  vStackDump();
  
  va_list xArgs;
  
  va_start(xArgs,aFormat);
  vPrint(aFormat,xArgs);
  va_end(xArgs);
  
  Process::vExit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* report an information message                                              */
/*----------------------------------------------------------------------------*/
void Debugging::vInfo(Stz* aFormat, ...)
{
  va_list xArgs;
  
  va_start(xArgs,aFormat);
  vPrint(aFormat,xArgs);
  va_end(xArgs);
}

/*----------------------------------------------------------------------------*/
/* report a trace message                                                     */
/*----------------------------------------------------------------------------*/
void Debugging::vTrace(Stz* aFormat, ...)
{
  va_list xArgs;

  va_start(xArgs,aFormat);
  vPrint(aFormat,xArgs);
  va_end(xArgs);
}

/*----------------------------------------------------------------------------*/
/* report a warning message                                                   */
/*----------------------------------------------------------------------------*/
void Debugging::vWarn(Stz* aFormat, ...)
{
  va_list xArgs;
  
  va_start(xArgs,aFormat);
  vPrint(aFormat,xArgs);
  va_end(xArgs);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* write to trace file etc                                                    */
/*----------------------------------------------------------------------------*/
void Debugging::vPrint(Stz* aFormat, va_list xArgs)
{
  static Lock oLock(LK_TRACE);
  static char caFormatted[C_1MB] = {'\0'};
  
  bool bUseLock = (gpAttachMap ? true : false);
  bool bLowMem  = gpBufferPool ? gpBufferPool->bLowMemory() : false;
  
  if (bUseLock)
    oLock.vGrabWrite();
  
  // produce formatted line
  vsnprintf(caFormatted,sizeof(caFormatted) - 3,aFormat->aChar(),xArgs);
  
  // print formatted line
  fprintf(stdout,"%s%s",bLowMem ? "LOW-MEMORY! " : "",caFormatted);
  fflush(stdout);
  
  if (gpTraceFile) {
    gpTraceFile->vPrint("%s%s"_Stz,bLowMem ? "LOW-MEMORY! " : "",caFormatted);
    gpTraceFile->vFlush();
  }
  
  if (bUseLock)
    oLock.vDropWrite();
}

/*============================================================================*/
/* end of file                                                                */
/*============================================================================*/
