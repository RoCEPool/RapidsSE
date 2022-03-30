/*============================================================================*/
/* eEK.h -- expression kind                                                   */
/*============================================================================*/
#pragma once

enum eEK // expression kind
{
  /*   */ EK_NONE=0,
  /*   */ EK_AGGGBY,     // aggregate with group by
  /*   */ EK_AGGORD,     // aggregate with ordered group by
  /*   */ EK_AGGREG,     // projection with aggreagte
  /*   */ EK_FILTER,     // filter
  /*   */ EK_GROUPBY,    // group by expression
  /*   */ EK_HASH,       // hash
  /*   */ EK_INDEXJOIN,  // index scan
  /*   */ EK_INDEXSCAN,  // index scan
  /*   */ EK_ORDGROUPBY, // group by expression with ordAgg
  /*   */ EK_PROJECT,    // projection
  /*   */ EK_SORT,       // sort
  /*   */ EK_TOP,        // top

  /*10 */ ET_ENUMS
};

static Stz* aEK[]
{
  /*   */ "NONE"_Stz,
  /*   */ "AGGGBY"_Stz,
  /*   */ "AGGORD"_Stz,
  /*   */ "AGGREG"_Stz,
  /*   */ "FILTER"_Stz,
  /*   */ "GROUPBY"_Stz,
  /*   */ "HASH"_Stz,
  /*   */ "INDEXJOIN"_Stz,
  /*   */ "INDEXSCAN"_Stz,
  /*   */ "EK_ORDGROUPBY"_Stz,
  /*   */ "PROJECT"_Stz,
  /*   */ "SORT"_Stz,
  /*   */ "TOP"_Stz,

  /*10 */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
