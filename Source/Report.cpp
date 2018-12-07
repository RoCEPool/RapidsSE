/*===========================================================================*/
/* Report.cpp - message reporter                                             */
/*===========================================================================*/

#include "Debugging.h"

#include "StorageEngine.h"

extern File*       gpTraceFile;
extern BufferPool* gpBufferPool;
extern AttachMap*  gpAttachMap;

char gxTraceWorkspace[C_1MB] = {'\0'};
  
bool mbReportTimestamp = true;

/*---------------------------------------------------------------------------*/
/* set reporting behavior                                                    */
/*---------------------------------------------------------------------------*/
void Debugging::vReportOption(i8 iTrace)
{
  if (C_REPORT_TIMESTAMP == iTrace)
    mbReportTimestamp = true;

  if (C_REPORT_NO_TIMESTAMP == iTrace)
    mbReportTimestamp = false;
}
  
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
/* vPrint:write to stdout                                                    */
/*---------------------------------------------------------------------------*/
void Debugging::vPrint(i8 iLevel, Stz* aFormat, va_list xArgs)
{
  static Lock xLock(LK_TRACE);
  
  bool bLock = (gpAttachMap ? true : false);
  
  if (bLock)
    xLock.vGrabWrite();
  
  i8 iLen = 0;
  char* s = gxTraceWorkspace;
  *s = '\0';
  
  assert(iLevel != 0);
  vsnprintf(s,sizeof(gxTraceWorkspace)-iLen-3,aFormat->aChar(),xArgs);
  
  //if (StrLib::bContains(gaReportBuffer,"serverThread"))
  //  i8 iDebug = 1;

  bool bLowMem = gpBufferPool ? gpBufferPool->bLowMemory() : false;
  
  // print to file
  fprintf(stdout,"%s%s",
          bLowMem ? "LOW-MEMORY! " : "",
          gxTraceWorkspace);
  fflush(stdout);
  
  if (gpTraceFile) {
    gpTraceFile->vPrint("%s%s"_Stz,
                        bLowMem ? "LOW-MEMORY! " : "",
                        gxTraceWorkspace);
    gpTraceFile->vFlush();
  }

  if (bLock)
    xLock.vDropWrite();
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
  
  vReport(C_REPORT_WARNING,aFormat,xArgs);
  
  va_end(xArgs);
}

/*---------------------------------------------------------------------------*/
/* report a recoverable error message                                        */
/*---------------------------------------------------------------------------*/
void Debugging::vError(Stz* aFormat, ...)
{
  va_list xArgs;
  
  va_start(xArgs,aFormat);
  
  vReport(C_REPORT_ERROR,aFormat,xArgs);
  
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
  
  vReport(C_REPORT_FATAL,aFormat,xArgs);
  
  va_end(xArgs);
  exit(EXIT_FAILURE);
}

/*---------------------------------------------------------------------------*/
/* vReport: common reporter                                                  */
/*---------------------------------------------------------------------------*/
void Debugging::vReport(i8 iLevel, Stz* aFormat, va_list xArgs)
{
  i8 iLen = 0;
  char* s = gxTraceWorkspace;
  *s = '\0';
  
  time_t tNow = time(0);
  char* sTime = ctime(&tNow);
  sTime[ strlen(sTime) - 1 ] = '\0';
  
  assert(iLevel != 0);
  vsnprintf(s,sizeof(gxTraceWorkspace)-iLen-3,aFormat->aChar(),xArgs);
  
  // print to file
  fprintf(stderr,"%s: %s", sTime, gxTraceWorkspace);
  fflush(stderr);
  
  if (gpTraceFile) {
    gpTraceFile->vPrint("%s: %s"_Stz, sTime, gxTraceWorkspace);
    gpTraceFile->vFlush();
  }
}

/*---------------------------------------------------------------------------*/
/* dump ascii string                                                         */
/*---------------------------------------------------------------------------*/
void Debugging::vDumpAscii(Stz* aText, i8 nBytes)
{
  vInfo(">>>"_Stz);
  while (nBytes--) {
    vInfo("%c"_Stz,*aText->aChar());
    aText++;
  }
  vInfo("<<<\n"_Stz);
}

/*===========================================================================*/
/* end of file                                                               */
/*===========================================================================*/
