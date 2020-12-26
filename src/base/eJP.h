/*============================================================================*/
/* eJP.h -- Job Phase                                                         */
/*============================================================================*/
#pragma once

enum eJP // job phase
{
  /*  0 */ JP_NONE=0,
  
  /*  1 */ JP_BEGINNING,
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
  /*  0 */ "JP_NONE"_Stz,
  
  /*  1 */ "JP_BEGINNING"_Stz,
  /*  2 */ "JP_BEGUN"_Stz,
  /*  3 */ "JP_EXECUTING"_Stz,
  /*  4 */ "JP_EXECUTED"_Stz,
  /*  5 */ "JP_ENDING"_Stz,
  /*  6 */ "JP_ENDED"_Stz,
  /*  7 */ "JP_CANCELING"_Stz,
  /*  8 */ "JP_CANCELED"_Stz,
  
  /*  9 */ "JP_ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
