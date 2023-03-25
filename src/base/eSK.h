/*============================================================================*/
/* eSK.h -- Sketch kind                                                       */
/*============================================================================*/
#pragma once

enum eSK // sketch kind
{
   /*  */ SK_NONE=0,
  /*   */ SK_AGGGBY,      // aggregate with group by
  /*   */ SK_AGGORD,      // ordered aggregate with group by
  /*   */ SK_AGGREG,      // projection with aggreagte
  /*   */ SK_ANTIJOIN,    // anti-join
  /*   */ SK_FILTER,      // filter
  /*   */ SK_HASH,        // hash
  /*   */ SK_INDEXJOIN,   // index join
  /*   */ SK_INDEXSCAN,   // index scan
  /*   */ SK_INFLATE_CRW, // inflate CRW
  /*   */ SK_INFLATE_ROW, // inflate ROW
  /*   */ SK_PROJECT,     // projection
  /*   */ SK_SEMIJOIN,    // semi-join
  /*   */ SK_SORT,        // sort
  /*   */ SK_TOP,         // top
};

static Stz* aSK[]
{
  /*   */ "NONE"_Stz,
  /*   */ "AGGGBY"_Stz,
  /*   */ "AGGORD"_Stz,
  /*   */ "AGGREG"_Stz,
  /*   */ "ANTIJOIN"_Stz,
  /*   */ "FILTER"_Stz,
  /*   */ "HASH"_Stz,
  /*   */ "INDEXJOIN"_Stz,
  /*   */ "INDEXSCAN"_Stz,
  /*   */ "INFLATE_CRW"_Stz,
  /*   */ "INFLATE_ROW"_Stz,
  /*   */ "PROJECT"_Stz,
  /*   */ "SEMIJOIN"_Stz,
  /*   */ "SORT"_Stz,
  /*   */ "TOP"_Stz,

 /*   */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
