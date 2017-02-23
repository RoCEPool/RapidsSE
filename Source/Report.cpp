/*===========================================================================*/
/* Report.cpp - message reporter                                             */
/*===========================================================================*/

#include "Debugging.h"
#include "File.hpp"

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
void Debugging::vInfo(const char* psFormat, ...)
{
  va_list xArgs;
  
  va_start(xArgs,psFormat);
  
  vReport(C_REPORT_INFO,psFormat,xArgs);
  
  va_end(xArgs);
}

/*---------------------------------------------------------------------------*/
/* report a trace message                                                    */
/*---------------------------------------------------------------------------*/
void Debugging::vTrace(const char* psFormat, ...)
{
  va_list xArgs;
  
  va_start(xArgs,psFormat);
  
  vReport(C_REPORT_TRACE,psFormat,xArgs);
  
  va_end(xArgs);
}

/*---------------------------------------------------------------------------*/
/* report a warning message                                                  */
/*---------------------------------------------------------------------------*/
void Debugging::vWarn(const char* psFormat, ...)
{
  va_list xArgs;
  
  va_start(xArgs,psFormat);
  
  vReport(C_REPORT_WARNING,psFormat,xArgs);
  
  va_end(xArgs);
}

/*---------------------------------------------------------------------------*/
/* report a recoverable error message                                        */
/*---------------------------------------------------------------------------*/
void Debugging::vError(const char* psFormat, ...)
{
  va_list xArgs;
  
  va_start(xArgs,psFormat);
  
  vReport(C_REPORT_ERROR,psFormat,xArgs);
  
  va_end(xArgs);
}

/*---------------------------------------------------------------------------*/
/* vFatal: report a non-recoverable fatal message                            */
/*---------------------------------------------------------------------------*/
void Debugging::vFatal(const char* psFormat, ...)
{
  va_list xArgs;
  
  va_start(xArgs,psFormat);
  
  vReport(C_REPORT_FATAL,psFormat,xArgs);
  
  va_end(xArgs);
  exit(EXIT_FAILURE);
}

/*---------------------------------------------------------------------------*/
/* vReport: common reporter                                                  */
/*---------------------------------------------------------------------------*/
void Debugging::vReport(i8 iLevel, const char* psFormat, va_list xArgs)
{
  i8 iLen = 0;
  char* s = gaReportBuffer;
  *s = '\0';
  
  assert(iLevel != 0);
  vsnprintf(s,sizeof(gaReportBuffer)-iLen-3,psFormat,xArgs);
    
  // print to file
  fprintf(stderr,"%s",gaReportBuffer);
  fflush(stderr);
  
  if (gpTraceFile) {
    gpTraceFile->vPrint("%s",gaReportBuffer);
    gpTraceFile->vFlush();
  }
}

/*---------------------------------------------------------------------------*/
/* dump ascii string                                                         */
/*---------------------------------------------------------------------------*/
void Debugging::vDumpAscii(const char* aText, i8 nBytes)
{
  printf(">>>");
  while (nBytes--)
    printf("%c",*aText++);
  printf("<<<\n");
}


/*===========================================================================*/
/* end of file                                                               */
/*===========================================================================*/
