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

  /*    */ GA_ENUMS,
};

static Stz* aGA[]
{
  /*  0 */ "NONE"_Stz,
  
  /*  0 */ "GA_LOG_SECTION"_Stz,
  /*  0 */ "GA_LOG_OPENED_API"_Stz,
  /*  0 */ "GA_LOG_OPENED_SE"_Stz,
  /*  0 */ "GA_LOG_WRITING"_Stz,

  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
