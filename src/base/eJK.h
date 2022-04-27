/*============================================================================*/
/* eJK.h -- Job Kind                                                          */
/*============================================================================*/
#pragma once

enum eJK // job kind
{
  /*  0 */ JK_NONE=0,
  
  /*  1 */ JK_BOOT,      // job is run by calling thread
  /*  2 */ JK_PIPELINE,  // distributed two-phase jobs
  /*  3 */ JK_GENERIC,   // the generic jobs for two-phase messaging
  /*  4 */ JK_LOCAL,     // job is run on the local node
  /*  5 */ JK_OP,        // op job (such as scan)
  /*  6 */ JK_ROOT,      // root command job

  /*  7 */ JK_ENUMS,     // any other
};

static Stz* aJK[]
{
  /*  0 */ "JK_NONE"_Stz,
  
  /*  1 */ "JK_BOOT"_Stz,
  /*  2 */ "JK_PIPELINE"_Stz,
  /*  3 */ "JK_GENERIC"_Stz,
  /*  4 */ "JK_LOCAL"_Stz,
  /*  5 */ "JK_OP"_Stz,
  /*  6 */ "JK_BOOT"_Stz,

  /*  7 */ "JK_ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
