/*============================================================================*/
/* eJK.h -- Job Kind                                                          */
/*============================================================================*/
#pragma once

enum eJK // job kind
{
  /*  0 */ JK_NONE=0,
  /*  1 */ JK_BOOT,      // job is run by calling thread
  /*  2 */ JK_CHILD,     // subordinate helper run on originator
  /*  3 */ JK_DIST2PC,   // distributed two-phase jobs
  /*  4 */ JK_GENERIC,   // the generic jobs for two-phase messaging
  /*  5 */ JK_GRANDKID,  // subordinate helper run on participant
  /*  6 */ JK_INFO,      // job is for info purposes only
  /*  7 */ JK_LOCAL,     // job is run on the local node
  /*  8 */ JK_META,      // meta job (such as compiler)
  /*  9 */ JK_OP,        // op job (such as scan)
  /* 10 */ JK_SERVICE,   // service job (such as socket receiver)
  /* 11 */ JK_OTHER,     // any other
};

static Stz* aJK[]
{
  /*  0 */ "NONE"_Stz,
  /*  1 */ "BOOT"_Stz,
  /*  2 */ "CHILD"_Stz,
  /*  3 */ "DIST2PC"_Stz,
  /*  4 */ "GENERIC"_Stz,
  /*  5 */ "GRANDKID"_Stz,
  /*  6 */ "INFO"_Stz,
  /*  7 */ "LOCAL"_Stz,
  /*  8 */ "META"_Stz,
  /*  9 */ "OP"_Stz,
  /* 10 */ "SERVICE"_Stz,
  /* 11 */ "OTHER"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
