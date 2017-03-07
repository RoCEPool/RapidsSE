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
void Debugging::vInfo(Str* aFormat, ...)
{
  va_list xArgs;
  
  va_start(xArgs,aFormat);
  
  vReport(C_REPORT_INFO,aFormat,xArgs);
  
  va_end(xArgs);
}

/*---------------------------------------------------------------------------*/
/* report a trace message                                                    */
/*---------------------------------------------------------------------------*/
void Debugging::vTrace(Str* aFormat, ...)
{
  va_list xArgs;
  
  va_start(xArgs,aFormat);
  
  vReport(C_REPORT_TRACE,aFormat,xArgs);
  
  va_end(xArgs);
}

/*---------------------------------------------------------------------------*/
/* report a warning message                                                  */
/*---------------------------------------------------------------------------*/
void Debugging::vWarn(Str* aFormat, ...)
{
  va_list xArgs;
  
  va_start(xArgs,aFormat);
  
  vReport(C_REPORT_WARNING,aFormat,xArgs);
  
  va_end(xArgs);
}

/*---------------------------------------------------------------------------*/
/* report a recoverable error message                                        */
/*---------------------------------------------------------------------------*/
void Debugging::vError(Str* aFormat, ...)
{
  va_list xArgs;
  
  va_start(xArgs,aFormat);
  
  vReport(C_REPORT_ERROR,aFormat,xArgs);
  
  va_end(xArgs);
}

/*---------------------------------------------------------------------------*/
/* vFatal: report a non-recoverable fatal message                            */
/*---------------------------------------------------------------------------*/
void Debugging::vFatal(Str* aFormat, ...)
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
void Debugging::vReport(i8 iLevel, Str* psFormat, va_list xArgs)
{
  i8 iLen = 0;
  char* s = gaReportBuffer;
  *s = '\0';
  
  assert(iLevel != 0);
  vsnprintf(s,sizeof(gaReportBuffer)-iLen-3,psFormat->a(),xArgs);
    
  // print to file
  fprintf(stderr,"%s",gaReportBuffer);
  fflush(stderr);
  
  if (gpTraceFile) {
    gpTraceFile->vPrint("%s"_Str,gaReportBuffer);
    gpTraceFile->vFlush();
  }
}

/*---------------------------------------------------------------------------*/
/* dump ascii string                                                         */
/*---------------------------------------------------------------------------*/
void Debugging::vDumpAscii(Str* aText, i8 nBytes)
{
  printf(">>>");
  while (nBytes--) {
    printf("%c",*aText->a());
    aText++;
  }
  printf("<<<\n");
}


/*===========================================================================*/
/* end of file                                                               */
/*===========================================================================*/
