/*============================================================================*/
/* eSK.h -- Sketch kind                                                       */
/*============================================================================*/
#pragma once

enum eSK // sketch kind
{
   /*  */ SK_NONE=0,
  /*   */ SK_AGGORD,      // ordered aggregate with group by
  /*   */ SK_AGGREG,      // projection with aggreagte
  /*   */ SK_ANTIJOIN,    // anti-join
  /*   */ SK_EOS,         // eos
  /*   */ SK_FILTER,      // filter
  /*   */ SK_GBYAGG,      // aggregate with group by
  /*   */ SK_HASH,        // hash
  /*   */ SK_INDEXJOIN,   // index join
  /*   */ SK_INDEXSCAN,   // index scan
  /*   */ SK_INFLATE_CRW, // inflate CRW
  /*   */ SK_INFLATE_ROW, // inflate ROW
  /*   */ SK_LIMIT,       // limit
  /*   */ SK_POT,         // pot
  /*   */ SK_PROJECT,     // projection
  /*   */ SK_SEMIJOIN,    // semi-join
  /*   */ SK_SORT,        // sort
  /*   */ SK_STREAMDEF,   // streamDef
  /*   */ SK_TOP,         // top
  /*   */ SK_TUB,         // tub
};

static Stz* aSK[]
{
  /*   */ "NONE"_Stz,
  /*   */ "AGGORD"_Stz,
  /*   */ "AGGREG"_Stz,
  /*   */ "ANTIJOIN"_Stz,
  /*   */ "EOS"_Stz,
  /*   */ "FILTER"_Stz,
  /*   */ "GBYAGG"_Stz,
  /*   */ "HASH"_Stz,
  /*   */ "INDEXJOIN"_Stz,
  /*   */ "INDEXSCAN"_Stz,
  /*   */ "INFLATE_CRW"_Stz,
  /*   */ "INFLATE_ROW"_Stz,
  /*   */ "LIMIT"_Stz,
  /*   */ "POT"_Stz,
  /*   */ "PROJECT"_Stz,
  /*   */ "SEMIJOIN"_Stz,
  /*   */ "SORT"_Stz,
  /*   */ "STREAMDEF"_Stz,
  /*   */ "TOP"_Stz,
  /*   */ "TUB"_Stz,

 /*   */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
