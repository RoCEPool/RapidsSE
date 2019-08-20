/*============================================================================*/
/* eYA.h -- System Actions                                                    */
/*============================================================================*/
#pragma once

enum eYA // system actions
{
  /*  0 */ YA_NONE=0,
  
  /*    */ YA_SYS_ARGS,
  /*    */ YA_SYS_FATAL,
  /*    */ YA_SYS_HOST,
  /*    */ YA_SYS_INFO,
  /*    */ YA_SYS_TIME,
  /*    */ YA_SYS_TRACE,
  /*    */ YA_SYS_UPTIME,
  /*    */ YA_SYS_WARN,
  /*    */ YA_SYS_WELCOME,
  
  /*    */ YA_SYS_PAUSE_GENERAL,

  /*    */ YA_ENUMS,
};

static Stz* aYA[]
{
  /*  0 */ "NONE"_Stz,
  
  /*  0 */ "YA_SYS_ARGS"_Stz,
  /*  0 */ "YA_SYS_FATAL"_Stz,
  /*  0 */ "YA_SYS_HOST"_Stz,
  /*  0 */ "YA_SYS_INFO"_Stz,
  /*  0 */ "YA_SYS_TIME"_Stz,
  /*  0 */ "YA_SYS_TRACE"_Stz,
  /*  0 */ "YA_SYS_UPTIME"_Stz,
  /*  0 */ "YA_SYS_WARN"_Stz,
  /*  0 */ "YA_SYS_WELCOME"_Stz,

  /*  0 */ "YA_SYS_PAUSE_GENERAL"_Stz,

  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
