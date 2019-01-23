/*============================================================================*/
/* Report.hpp -- reporting                                                    */
/*============================================================================*/
#pragma once

namespace Debugging
{
  static const i8 C_REPORT_TIMESTAMP    = 1;
  static const i8 C_REPORT_NO_TIMESTAMP = 0;
    
  const i8 C_REPORT_INFO    = 1; // information feedback
  const i8 C_REPORT_TRACE   = 2; // trace feedback
  const i8 C_REPORT_WARNING = 3; // warning feedback
  const i8 C_REPORT_ERROR   = 4; // recoverable error feedback
  const i8 C_REPORT_FATAL   = 5; // non-recoverable error feedback
  
  void  vReportOption(i8 iReportOption);
  
  void  vInfo(Stz* aFormat,...);
  void  vTrace(Stz* aFormat,...);
  void  vWarn(Stz* aFormat,...);
  void  vError(Stz* aFormat,...);
  void  vFatal(Stz* aFormat,...);
  
  void  vPrint(i8 iLevel, Stz* aFormat, va_list xArgs);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
