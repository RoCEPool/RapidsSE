/*============================================================================*/
/* Report.hpp -- reporting                                                    */
/*============================================================================*/
#pragma once

#include "Common.h"

namespace Common
{
  static const i8 C_REPORT_TIMESTAMP    = 1;
  static const i8 C_REPORT_NO_TIMESTAMP = 0;
    
  const i8 C_REPORT_INFO    = 1; // information feedback
  const i8 C_REPORT_TRACE   = 2; // trace feedback
  const i8 C_REPORT_WARNING = 3; // warning feedback
  const i8 C_REPORT_ERROR   = 4; // recoverable error feedback
  const i8 C_REPORT_FATAL   = 5; // non-recoverable error feedback
  
  void  vReportOption(i8 iReportOption);
  
  void  vInfo(const char* pFormat,...);
  void  vTrace(const char* pFormat,...);
  void  vWarn(const char* pFormat,...);
  void  vError(const char* pFormat,...);
  void  vFatal(const char* pFormat,...);

  void  vReport(i8 iLevel, const char* pFormat, va_list xArgs);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
