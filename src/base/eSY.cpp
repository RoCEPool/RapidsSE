/*============================================================================*/
/* eSY.cpp -- Send Why                                                        */
/*============================================================================*/
#include "BASE.h"
#include "eSY.h"

Stz* aSY[SY_ENUMS]
{
  /*  0 */ "SY_NONE"_Stz,
  
  /*  1 */ "SY_ALL_NODES"_Stz,
  /*  2 */ "SY_NEXT_NODE"_Stz,
  /*  3 */ "SY_ORIGIN_NODE"_Stz,
  /*  4 */ "SY_SAME_NODE"_Stz,
  
  /*  5 */ "SY_BROADCAST"_Stz,
  /*  6 */ "SY_FANCAST"_Stz,
  /*  7 */ "SY_HASHCAST"_Stz,
  /*  8 */ "SY_ROBINDIST"_Stz,
  /*  9 */ "SY_ROBINLOCAL"_Stz,
  /* 10 */ "SY_UNICAST"_Stz,
  
//  /* 11 */ "SY_ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
