/*============================================================================*/
/* eXK.h - Pipeline Cast Kind                                                 */
/*============================================================================*/
#pragma once

enum eXK // cast kind
{
  /* 0 */ XK_NONE = 0,   // none
  /* 1 */ XK_BROADCAST,  // broad cast
  /* 2 */ XK_HASHCAST,   // hash cast
  /* 4 */ XK_ROBINDIST,  // distributed round robin
  /* 3 */ XK_ROBINLOCAL, // local round robin
  /* 5 */ XK_UNICAST     // unicast
};

static Stz* aXK[]
{
  /* 0 */ "XK_NONE"_Stz,
  /* 1 */ "XK_BROADCAST"_Stz,
  /* 2 */ "XK_HASHCAST"_Stz,
  /* 4 */ "XK_ROBINDIST"_Stz,
  /* 3 */ "XK_ROBINLOCAL"_Stz,
  /* 5 */ "XK_UNICAST"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
