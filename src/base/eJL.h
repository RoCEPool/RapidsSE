/*============================================================================*/
/* eJL.h -- Arguments List                                                    */
/*============================================================================*/
#pragma once

enum eJL // jobs list
{
  /* 0 */ JL_NONE=0,
  /* 1 */ JL_DEMON,
  /* 2 */ JL_JOBS,
  /* 3 */ JL_LOCAL,
  /* 4 */ JL_ORIGINATING,
  /* 5 */ JL_PARTICIPATING,
  /* 6 */ JL_RETIRED,
  /* 7 */ JL_ENUMS
};

static Stz* aJL[]
{
  /* 0 */ "JL_NONE"_Stz,
  /* 1 */ "JL_DEMON"_Stz,
  /* 2 */ "JL_JOBS"_Stz,
  /* 3 */ "JL_LOCAL"_Stz,
  /* 4 */ "JL_ORIGINATING"_Stz,
  /* 5 */ "JL_PARTICIPATING"_Stz,
  /* 6 */ "JL_RETIRED"_Stz,
  /* 7 */ "JL_ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
