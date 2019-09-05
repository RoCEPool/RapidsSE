/*============================================================================*/
/* eOP.h -- Operators                                                         */
/*============================================================================*/
#pragma once

enum eOP // operators
{
  /*  0 */ ON_NONE=0,
  
  /*    */ ON_AGG,
  /*    */ ON_BOTH,
  /*    */ ON_COMBINE,
  /*    */ ON_COUNT,
  /*    */ ON_DISTINCT,
  /*    */ ON_FETCH,
  /*    */ ON_FILTER,
  /*    */ ON_GENERATE,
  /*    */ ON_GETKEY,
  /*    */ ON_HASH,
  /*    */ ON_INFLATE,
  /*    */ ON_INTERSECT,
  /*    */ ON_JOIN,
  /*    */ ON_LIMIT,
  /*    */ ON_MERGE,
  /*    */ ON_OUTPUT,
  /*    */ ON_PROJECT,
  /*    */ ON_REDUCE,
  /*    */ ON_SCAN,
  /*    */ ON_SORT,
  /*    */ ON_SUPERMERGE,
  /*    */ ON_TAKEAWAY,
  /*    */ ON_WHERE,
  /*    */ ON_XMIT,
  /*    */ ON_ZED,

  /*    */ ON_ENUMS,
};

static Stz* aOP[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "agg"_Stz,
  /*    */ "both"_Stz,
  /*    */ "combine"_Stz,
  /*    */ "count"_Stz,
  /*    */ "distinct"_Stz,
  /*    */ "fetch"_Stz,
  /*    */ "filter"_Stz,
  /*    */ "generate"_Stz,
  /*    */ "getkey"_Stz,
  /*    */ "hash"_Stz,
  /*    */ "inflate"_Stz,
  /*    */ "intersect"_Stz,
  /*    */ "join"_Stz,
  /*    */ "limit"_Stz,
  /*    */ "merge"_Stz,
  /*    */ "output"_Stz,
  /*    */ "project"_Stz,
  /*    */ "reduce"_Stz,
  /*    */ "scan"_Stz,
  /*    */ "sort"_Stz,
  /*    */ "supermerge"_Stz,
  /*    */ "takeaway"_Stz,
  /*    */ "where"_Stz,
  /*    */ "xmit"_Stz,
  /*    */ "zed"_Stz,

  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
