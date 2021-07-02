/*============================================================================*/
/* eXK.h - Pipeline Send Kind                                                 */
/*============================================================================*/
#pragma once

enum eXK // send kind
{
  /* 0 */ XK_NONE = 0,   // none
  /* 1 */ XK_HASHED,     // hashed
  /* 2 */ XK_LOCALROBIN, // local round robin
  /* 3 */ XK_NORMAL,     // normal
  /* 4 */ XK_ROBIN       // round robin
};

static Stz* aXK[]
{
  /* 0 */ "XK_NONE"_Stz,
  /* 1 */ "XK_HASHED"_Stz,
  /* 2 */ "XK_LOCALROBIN"_Stz,
  /* 3 */ "XK_NORMAL"_Stz,
  /* 4 */ "XK_ROBIN"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
