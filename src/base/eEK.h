/*============================================================================*/
/* eEK.h -- expression kind                                                   */
/*============================================================================*/
#pragma once

enum eEK // expression kind
{
  /*   */ EK_NONE=0,
  /*   */ EK_AGGORD,     // ordered aggregate with group by
  /*   */ EK_AGGREG,     // projection with aggreagte
  /*   */ EK_FILTER,     // filter
  /*   */ EK_GBYAGG,     // aggregate with group by
  /*   */ EK_HASH,       // hash
  /*   */ EK_INDEXJOIN,  // index join
  /*   */ EK_INDEXSCAN,  // index scan
  /*   */ EK_PROJECT,    // projection
  /*   */ EK_SORT,       // sort
  /*   */ EK_TOP,        // top

  /*   */ ET_ENUMS
};

static Stz* aEK[]
{
  /*   */ "NONE"_Stz,
  /*   */ "AGGORD"_Stz,
  /*   */ "AGGREG"_Stz,
  /*   */ "FILTER"_Stz,
  /*   */ "GBYAGG"_Stz,
  /*   */ "HASH"_Stz,
  /*   */ "INDEXJOIN"_Stz,
  /*   */ "INDEXSCAN"_Stz,
  /*   */ "PROJECT"_Stz,
  /*   */ "SORT"_Stz,
  /*   */ "TOP"_Stz,

  /*   */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
