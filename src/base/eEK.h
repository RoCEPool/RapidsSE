/*============================================================================*/
/* eEK.h -- expression kind                                                   */
/*============================================================================*/
#pragma once

enum eEK // expression kind
{
  /* 0 */ EK_NONE=0,
  /* 1 */ EK_AGGGBY,    // aggregate with group by
  /* 2 */ EK_AGGREG,    // projection with aggreagte
  /* 3 */ EK_FILTER,    // filter
  /* 4 */ EK_INDEXJOIN, // index scan
  /* 5 */ EK_INDEXSCAN, // index scan
  /* 6 */ EK_PROJECT,   // projection
  /* 7 */ EK_TOP,       // top

  /* 8 */ ET_ENUMS
};

static Stz* aEK[]
{
  /* 0 */ "NONE"_Stz,
  /* 1 */ "AGGGBY"_Stz,
  /* 2 */ "AGGREG"_Stz,
  /* 3 */ "FILTER"_Stz,
  /* 4 */ "INDEXJOIN"_Stz,
  /* 5 */ "INDEXSCAN"_Stz,
  /* 6 */ "PROJECT"_Stz,
  /* 7 */ "TOP"_Stz,

  /* 8 */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
