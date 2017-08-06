/*===========================================================================*/
/* Report.cpp - message reporter                                             */
/*===========================================================================*/

#include "Debugging.h"

#include "System.h"

extern File* gpTraceFile;

char gaReportBuffer[C_1MB] = {'\0'};
  
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
  i8 iLen = 0;
  char* s = gaReportBuffer;
  *s = '\0';
  
  assert(iLevel != 0);
  vsnprintf(s,sizeof(gaReportBuffer)-iLen-3,aFormat->aChar(),xArgs);
  
  if (StrLib::bContains(gaReportBuffer,"__compiled__"))
    i8 iDebug = 1;
  if (StrLib::bContains(gaReportBuffer,"serverThread"))
    i8 iDebug = 1;

  // print to file
  fprintf(stdout,"%s",gaReportBuffer);
  fflush(stdout);
  
  if (gpTraceFile) {
    gpTraceFile->vPrint("%s"_Stz,gaReportBuffer);
    gpTraceFile->vFlush();
  }
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
  char* s = gaReportBuffer;
  *s = '\0';
  
  assert(iLevel != 0);
  vsnprintf(s,sizeof(gaReportBuffer)-iLen-3,aFormat->aChar(),xArgs);
  
  if (StrLib::bContains(gaReportBuffer,"__compiled__"))
    i8 iDebug = 1;
  
  // print to file
  fprintf(stderr,"%s",gaReportBuffer);
  fflush(stderr);
  
  if (gpTraceFile) {
    gpTraceFile->vPrint("%s"_Stz,gaReportBuffer);
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
