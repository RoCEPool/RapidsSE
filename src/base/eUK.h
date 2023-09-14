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
  /* 4 */ UK_PROJECT,  // project function
  /* 5 */ UK_ENUMS
};

extern Stz* aUK[UK_ENUMS];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
