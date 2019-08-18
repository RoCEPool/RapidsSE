/*============================================================================*/
/* eGA.h -- Log Actions                                                       */
/*============================================================================*/
#pragma once

enum eGA // log actions
{
  /*  0 */ GA_NONE=0,
  
  /*    */ GA_LOG_HOST,
  /*    */ GA_LOG_OPENED_API,
  /*    */ GA_LOG_OPENED_SE,
  /*    */ GA_LOG_TIME,
  /*    */ GA_LOG_UPTIME,
  /*    */ GA_LOG_WELCOME,
  /*    */ GA_LOG_WRITING,

  /*    */ GA_ENUMS,
};

static Stz* aGA[]
{
  /*  0 */ "NONE"_Stz,
  
  /*  0 */ "GA_LOG_HOST"_Stz,
  /*  0 */ "GA_LOG_OPENED_API"_Stz,
  /*  0 */ "GA_LOG_OPENED_SE"_Stz,
  /*  0 */ "GA_LOG_TIME"_Stz,
  /*  0 */ "GA_LOG_UPTIME"_Stz,
  /*  0 */ "GA_LOG_WELCOME"_Stz,
  /*  0 */ "GA_LOG_WRITING"_Stz,

  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
