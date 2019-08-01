/*============================================================================*/
/* eJP.hpp -- job phase                                                       */
/*============================================================================*/
#pragma once

enum eJP // job phase
{
  /*  0 */ JP_NONE=0,
  /*  1 */ JP_BEGINING,
  /*  2 */ JP_BEGUN,
  /*  3 */ JP_EXECUTING,
  /*  4 */ JP_EXECUTED,
  /*  5 */ JP_ENDING,
  /*  6 */ JP_ENDED,
  /*  7 */ JP_CANCELING,
  /*  8 */ JP_CANCELED,
  /*  9 */ JP_ENUMS
};

static Stz* aJP[]
{
  /*  0 */ "NONE"_Stz,
  /*  1 */ "BEGINING"_Stz,
  /*  2 */ "BEGUN"_Stz,
  /*  3 */ "EXECUTING"_Stz,
  /*  4 */ "EXECUTED"_Stz,
  /*  5 */ "ENDING"_Stz,
  /*  6 */ "ENDED"_Stz,
  /*  7 */ "CANCELING"_Stz,
  /*  8 */ "CANCELED"_Stz,
  /*  9 */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
