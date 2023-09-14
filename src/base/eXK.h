/*============================================================================*/
/* eXK.h - Pipeline Stage Cast Kind                                           */
/*============================================================================*/
#pragma once

enum eXK // cast kind
{
  /* 0 */ XK_NONE = 0,   // none
  /* 1 */ XK_BROADCAST,  // broad cast
  /* 2 */ XK_FANCAST,    // fan cast
  /* 3 */ XK_HASHCAST,   // hash cast
  /* 4 */ XK_ROBINDIST,  // distributed round robin
  /* 5 */ XK_ROBINLOCAL, // local round robin
  /* 6 */ XK_UNICAST,    // unicast
  /* 7 */ XK_ENUMS
};

extern Stz* aXK[XK_ENUMS];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
