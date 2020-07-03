/*============================================================================*/
/* eUK.h -- Dynamic Function Kind                                             */
/*============================================================================*/
#pragma once

enum eUK // function kind
{
  /* 0 */ UK_NONE = 0, // none
  /* 1 */ UK_AGG,      // aggregate function
  /* 2 */ UK_FILTER,   // filter function
  /* 3 */ UK_KEY,      // key function (group by, sort by etc)
  /* 4 */ UK_PROJECT   // project function
};

static Stz* aUK[]
{
  /* 0 */ "UK_NONE"_Stz,
  /* 1 */ "UK_AGG"_Stz,
  /* 2 */ "UK_FILTER"_Stz,
  /* 3 */ "UK_KEY"_Stz,
  /* 4 */ "UK_PROJECT"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
