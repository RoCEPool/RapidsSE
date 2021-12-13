/*============================================================================*/
/* eSY.h -- Send Why                                                          */
/*============================================================================*/
#pragma once

enum eSY // send why
{
  /*  0 */ SY_NONE=0,
  
  /*  1 */ SY_ALL_NODES,
  /*  2 */ SY_NEXT_NODE,
  /*  3 */ SY_ORIGIN,
  /*  4 */ SY_SAME_NODE,
  
  /*  5 */ SY_BROADCAST,
  /*  6 */ SY_FANCAST,
  /*  7 */ SY_HASHCAST,
  /*  8 */ SY_ROBINDIST,
  /*  9 */ SY_ROBINLOCAL,
  /* 10 */ XK_UNICAST,

  /* 11 */ SY_ENUMS
};

static Stz* aSY[]
{
  /*  0 */ "SY_NONE"_Stz,
  
  /*  1 */ "SY_ALL_NODES"_Stz,
  /*  2 */ "SY_NEXT_NODE"_Stz,
  /*  3 */ "SY_ORIGIN"_Stz,
  /*  4 */ "SY_SAME_NODE"_Stz,
  
  /*  5 */ "SY_BROADCAST"_Stz,
  /*  6 */ "SY_FANCAST"_Stz,
  /*  7 */ "SY_HASHCAST"_Stz,
  /*  8 */ "SY_ROBINDIST"_Stz,
  /*  9 */ "SY_ROBINLOCAL"_Stz,
  /* 10 */ "SY_UNICAST"_Stz,
  
  /* 11 */ "SY_ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
