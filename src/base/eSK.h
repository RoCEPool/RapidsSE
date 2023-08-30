/*============================================================================*/
/* eSK.h -- Sketch kind                                                       */
/*============================================================================*/
#pragma once

enum eSK // sketch kind
{
   /*  */ SK_NONE=0,
  /*   */ SK_AGGREG,      // projection with aggregate
  /*   */ SK_ANTIJOIN,    // anti-join
  /*   */ SK_EOS,         // eos
  /*   */ SK_FILTER,      // filter
  /*   */ SK_GBYAGG,      // aggregate with group by
  /*   */ SK_INDEXJOIN,   // index join
  /*   */ SK_INDEXSCAN,   // index scan
  /*   */ SK_INFLATE_CRW, // inflate CRW
  /*   */ SK_INFLATE_ROW, // inflate ROW
  /*   */ SK_LIMIT,       // limit
  /*   */ SK_ORDAGG,      // ordered aggregate with group by
  /*   */ SK_POT,         // pot
  /*   */ SK_PROJECT,     // projection
  /*   */ SK_SEMIJOIN,    // semi-join
  /*   */ SK_SORT,        // sort
  /*   */ SK_STREAMDEF,   // streamDef
  /*   */ SK_TOP,         // top
  /*   */ SK_TUB,         // tub
  /*   */ SK_ENUMS,
};

extern Stz* aSK[SK_ENUMS];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
