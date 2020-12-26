/*============================================================================*/
/* eXK.h - Pipeline Xmit Kind                                                 */
/*============================================================================*/
#pragma once

enum eXK // system call
{
  /* 0 */ XK_NONE = 0,   // none
  /* 1 */ XK_HASHED,     // hashed
  /* 2 */ XK_LOCALROBIN, // local round robin
  /* 3 */ XK_NORMAL,     // normal
  /* 4 */ XK_ROBIN       // round robin
};

static Stz* aXK[]
{
  /* 0 */ "NONE"_Stz,
  /* 1 */ "HASHED"_Stz,
  /* 2 */ "LOCALROBIN"_Stz,
  /* 3 */ "NORMAL"_Stz,
  /* 4 */ "ROBIN"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
