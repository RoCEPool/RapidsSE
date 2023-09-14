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

  /*   */ EK_ENUMS
};

extern Stz* aEK[EK_ENUMS];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
