/*============================================================================*/
/* eJT.h -- Join Type                                                         */
/*============================================================================*/
#pragma once


enum eJT // Join Type
{
  /*  0 */ JT_NONE=0,
  /*  1 */ JT_INNER,
  /*  2 */ JT_CROSS,
  /*  3 */ JT_SEMI,
  /*  4 */ JT_ANTI,
  /*  5 */ JT_LEFT_OUTER,
  /*  6 */ JT_RIGHT_OUTER,
  /*  7 */ JT_FULL_OUTER,
  /*  8 */ JT_NATURAL_INNER,
  /*  9 */ JT_NATURAL_LEFT_OUTER,
  /* 10 */ JT_NATURAL_RIGHT_OUTER,
  /* 11 */ JT_NATURAL_FULL_OUTER,
  /* 12 */ JT_NENUMS
};

extern Stz* aJT[JT_NENUMS];
extern Stz* aJT_pretty[JT_NENUMS];


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
