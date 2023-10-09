/*============================================================================*/
/* eGA.cpp -- Log Actions                                                     */
/*============================================================================*/
#include "BASE.h"
#include "eGA.h"

Stz* aGA[GA_ENUMS]
{
  /*  0 */ "NONE"_Stz,
  
  /*  0 */ "GA_LOG_SECTION"_Stz,
  /*  0 */ "GA_LOG_OPENED_API"_Stz,
  /*  0 */ "GA_LOG_OPENED_SE"_Stz,
  /*  0 */ "GA_LOG_WRITING"_Stz,

  /*  0 */ "GA_LOG_TIME"_Stz,
  /*  0 */ "GA_LOG_WELCOME"_Stz,
  /*  0 */ "GA_LOG_HOST"_Stz,
  /*  0 */ "GA_LOG_UPTIME"_Stz,
  /*  0 */ "GA_LOG_COPYRIGHT"_Stz,

//  /*    */ "ENUMS"_Stz,
};

Stz* aGA_short[GA_ENUMS]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "Log section"_Stz,
  /*    */ "Log opened by api"_Stz,
  /*    */ "Log opened by se"_Stz,
  /*    */ "Log writing"_Stz,

  /*    */ "Log time"_Stz,
  /*    */ "Log welcome"_Stz,
  /*    */ "Log host"_Stz,
  /*    */ "Log uptime"_Stz,
  /*    */ "Log copyright"_Stz,

//  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
