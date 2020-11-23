/*============================================================================*/
/* eGA.h -- Log Actions                                                       */
/*============================================================================*/
#pragma once

enum eGA // log actions
{
  /*  0 */ GA_NONE=0,
  
  /*    */ GA_LOG_SECTION,
  /*    */ GA_LOG_OPENED_API,
  /*    */ GA_LOG_OPENED_SE,
  /*    */ GA_LOG_WRITING,
  
  /*    */ GA_LOG_TIME,
  /*    */ GA_LOG_WELCOME,
  /*    */ GA_LOG_HOST,
  /*    */ GA_LOG_UPTIME,
  /*    */ GA_LOG_COPYRIGHT,

  /*    */ GA_ENUMS,
};

static Stz* aGA[]
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

  /*    */ "ENUMS"_Stz,
};

static Stz* aGA_short[]
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

  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
