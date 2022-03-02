/*============================================================================*/
/* eEK.h -- expression kind                                                   */
/*============================================================================*/
#pragma once

enum eEK // expression kind
{
  /* 0 */ EK_NONE=0,
  /* 1 */ EK_AGGGBY,    // aggregate with group by
  /*   */ EK_AGGORD,    // aggregate with ordered group by
  /* 2 */ EK_AGGREG,    // projection with aggreagte
  /* 3 */ EK_FILTER,    // filter
  /* 4 */ EK_GROUPBY,   // group by expression
  /* 5 */ EK_INDEXJOIN, // index scan
  /* 6 */ EK_INDEXSCAN, // index scan
  /* 4 */ EK_ORDGROUPBY, // group by expression with ordAgg
  /* 8 */ EK_PROJECT,   // projection
  /* 9 */ EK_TOP,       // top

  /*10 */ ET_ENUMS
};

static Stz* aEK[]
{
  /* 0 */ "NONE"_Stz,
  /* 1 */ "AGGGBY"_Stz,
  /*   */ "AGGORD"_Stz,
  /* 2 */ "AGGREG"_Stz,
  /* 3 */ "FILTER"_Stz,
  /* 4 */ "GROUPBY"_Stz,
  /* 5 */ "INDEXJOIN"_Stz,
  /* 6 */ "INDEXSCAN"_Stz,
  /* 7 */ "EK_ORDGROUPBY"_Stz,
  /* 8 */ "PROJECT"_Stz,
  /* 9 */ "TOP"_Stz,

  /*10 */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
