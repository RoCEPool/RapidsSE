/*============================================================================*/
/* eJK.h -- job kind                                                          */
/*============================================================================*/
#pragma once

enum eJK // job kind
{
  /*  0 */ JK_NONE=0,
  /*  1 */ JK_CHILD,     // subordinate helper run on originator
  /*  2 */ JK_DIST2PC,   // distributed two-phase jobs
  /*  3 */ JK_GENERIC,   // the generic jobs for two-phase messaging
  /*  4 */ JK_GRANDKID,  // subordinate helper run on participant
  /*  5 */ JK_INFO,      // job is for info purposes only
  /*  6 */ JK_LOCAL,     // job is run on the local node
  /*  7 */ JK_SYSTEM,    // system job (such as socket receiver)
  /*  8 */ JK_THREAD,    // job is run by calling thread
  /*  9 */ JK_OTHER,     // any other
};

static Stz* aJK[]
{
  /*  0 */ "NONE"_Stz,
  /*  1 */ "CHILD"_Stz,
  /*  2 */ "DIST2PC"_Stz,
  /*  3 */ "GENERIC"_Stz,
  /*  4 */ "GRANDKID"_Stz,
  /*  5 */ "INFO"_Stz,
  /*  6 */ "LOCAL"_Stz,
  /*  7 */ "SYSTEM"_Stz,
  /*  8 */ "THREAD"_Stz,
  /*  9 */ "OTHER"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
