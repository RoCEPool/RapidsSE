/*============================================================================*/
/* eCK.h -- Card Kind                                                         */
/*============================================================================*/
#pragma once

enum eCK // card kind
{
  /* */ CK_NONE=0,
    
  /* */ CK_AGGREG,
  /* */ CK_AGGREGPRED,
  /* */ CK_ANDPRED,
  /* */ CK_AS,
  
  /* */ CK_BOOLPRED,
  /* */ CK_BY,
  /* */ CK_BYLIST,
  /* */ CK_BYPRED,
  
  /* */ CK_CALCPRED,
  /* */ CK_CANDIDATEINDEX,
  /* */ CK_COMBINE,
  /* */ CK_COMPAREPRED,
  /* */ CK_CORRSUBQUERY,
  /* */ CK_CREATETABLE,
  
  /* */ CK_DEBUG,
  /* */ CK_DROPTABLE,
  
  /* */ CK_EXISTS,
  /* */ CK_EXPR,
  
  /* */ CK_FILTER,

  /* */ CK_HASH,
  /* */ CK_HASHCAST,
  /* */ CK_HAVING,
  
  /* */ CK_INDEX,
  /* */ CK_INDEXJOIN,
  /* */ CK_INDEXPRED,
  /* */ CK_INDEXSCAN,
  
  /* */ CK_LBOUND,
  /* */ CK_LIMIT,
  
  /* */ CK_MAINSUBQUERY,
  
  /* */ CK_NODECAST,
  /* */ CK_NONCSUBQUERY,
  
  /* */ CK_ON,
  /* */ CK_ORDAGG,
  /* */ CK_ORIGINCAST,
  /* */ CK_ORPRED,
  
  /* */ CK_PREDICATE,
  /* */ CK_PROJECT,
  /* */ CK_PROJECTION,
  
  /* */ CK_SCAN,
  /* */ CK_SEND,
  /* */ CK_SORT,
  /* */ CK_STAGE,
  /* */ CK_STREAMDEF,
  
  /* */ CK_TABLE,
  /* */ CK_TERM,
  /* */ CK_TERMLIST,
  /* */ CK_TYPE,

  /* */ CK_ENUMS
};

static Stz* aCK[]
{
  /* */ "NONE"_Stz,
  
  /* */ "CK_AGGREG"_Stz,          // CK_AGGREG
  /* */ "CK_AGGREGPRED"_Stz,      // CK_AGGREGPRED
  /* */ "CK_ANDPRED"_Stz,         // CK_ANDPRED
  /* */ "CK_AS"_Stz,              // CK_AS
  
  /* */ "CK_BOOLPRED"_Stz,        // CK_BOOLPRED
  /* */ "CK_BY"_Stz,              // CK_BY
  /* */ "CK_BYLIST"_Stz,          // CK_BYLIST
  /* */ "CK_BYPRED"_Stz,          // CK_BYPRED
  
  /* */ "CK_CALCPRED"_Stz,        // CK_CALCPRED
  /* */ "CK_CANDIDATEINDEX"_Stz,  // CK_CANDIDATEINDEX
  /* */ "CK_COMBINE"_Stz,         // CK_COMBINE
  /* */ "CK_COMPAREPRED"_Stz,     // CK_COMPAREPRED
  /* */ "CK_CORRSUBQUERY"_Stz,    // CK_CORRSUBQUERY
  /* */ "CK_CREATETABLE"_Stz,     // CK_CREATETABLE
  
  /* */ "CK_DEBUG"_Stz,           // CK_DEBUG
  /* */ "CK_DROPTABLE"_Stz,       // CK_DROPTABLE
  
  /* */ "CK_EXISTS"_Stz,          // CK_EXISTS
  /* */ "CK_EXPR"_Stz,            // CK_EXPR
  
  /* */ "CK_FILTER"_Stz,          // CK_FILTER

  /* */ "CK_HASH"_Stz,            // CK_HASH
  /* */ "CK_HASHCAST"_Stz,        // CK_HASHCAST
  /* */ "CK_HAVING"_Stz,          // CK_HAVING
  
  /* */ "CK_INDEX"_Stz,           // CK_INDEX
  /* */ "CK_INDEXJOIN"_Stz,       // CK_INDEXJOIN
  /* */ "CK_INDEXPRED"_Stz,       // CK_INDEXPRED
  /* */ "CK_INDEXSCAN"_Stz,       // CK_INDEXSCAN
  
  /* */ "CK_LBOUND"_Stz,          // CK_LBOUND
  /* */ "CK_LIMIT"_Stz,           // CK_LIMIT
  
  /* */ "CK_MAINSUBQUERY"_Stz,    // CK_MAINSUBQUERY
  
  /* */ "CK_NODECAST"_Stz,        // CK_NODECAST
  /* */ "CK_NONCSUBQUERY"_Stz,    // CK_NONCSUBQUERY
  
  /* */ "CK_ON"_Stz,              // CK_ON
  /* */ "CK_ORDAGG"_Stz,          // CK_ORDAGG
  /* */ "CK_ORIGINCAST"_Stz,      // CK_ORIGINCAST
  /* */ "CK_ORPRED"_Stz,          // CK_ORPRED
  
  /* */ "CK_PREDICATE"_Stz,       // CK_PREDICATE
  /* */ "CK_PROJECT"_Stz,         // CK_PROJECT
  /* */ "CK_PROJECTION"_Stz,      // CK_PROJECTION
  
  /* */ "CK_SCAN"_Stz,            // CK_SEND
  /* */ "CK_SEND"_Stz,            // CK_SCAN
  /* */ "CK_SORT"_Stz,            // CK_SORT
  /* */ "CK_STAGE"_Stz,           // CK_STAGE
  /* */ "CK_STREAMDEF"_Stz,       // CK_STREAMDEF
  
  /* */ "CK_TABLE"_Stz,           // CK_TABLE,
  /* */ "CK_TERM"_Stz,            // CK_TERM,
  /* */ "CK_TERMLIST"_Stz,        // CK_TERMLIST,
  /* */ "CK_TYPE"_Stz,            // CK_TYPE,

  /* */ "ENUMS"_Stz
};

static Stz* aCK_short[]
{
  /* */ "NONE"_Stz,
    
  /* */ "aggreg"_Stz,          // CK_AGGREG
  /* */ "aggregPred"_Stz,      // CK_AGGREGPRED
  /* */ "andPred"_Stz,         // CK_ANDPRED
  /* */ "as"_Stz,              // CK_AS
  
  /* */ "boolPred"_Stz,        // CK_BOOLPRED
  /* */ "by"_Stz,              // CK_BY
  /* */ "byList"_Stz,          // CK_BYLIST
  /* */ "byPred"_Stz,          // CK_BYPRED
  
  /* */ "calcPred"_Stz,        // CK_CALCPRED
  /* */ "candidateIndex"_Stz,  // CK_CANDIDATEINDEX
  /* */ "combine"_Stz,         // CK_COMBINE
  /* */ "comparePred"_Stz,     // CK_COMPAREPRED
  /* */ "corrSubquery"_Stz,    // CK_CORRSUBQUERY
  /* */ "createTable"_Stz,     // CK_CREATETABLE
  
  /* */ "debug"_Stz,           // CK_DEBUG
  /* */ "dropTable"_Stz,       // CK_DROPTABLE
  
  /* */ "exists"_Stz,          // CK_EXISTS
  /* */ "expr"_Stz,            // CK_EXPR
  
  /* */ "filter"_Stz,          // CK_FILTER

  /* */ "hash"_Stz,            // CK_HASH
  /* */ "hashCast"_Stz,        // CK_HASHCAST
  /* */ "having"_Stz,          // CK_HAVING
  
  /* */ "index"_Stz,           // CK_INDEX
  /* */ "indexJoin"_Stz,       // CK_INDEXJOIN
  /* */ "indexPred"_Stz,       // CK_INDEXPRED
  /* */ "indexScan"_Stz,       // CK_INDEXSCAN
  
  /* */ "lbound"_Stz,          // CK_LBOUND
  /* */ "limit"_Stz,           // CK_LIMIT
  
  /* */ "mainSubquery"_Stz,    // CK_MAINSUBQUERY
  
  /* */ "nodeCast"_Stz,        // CK_NODECAST
  /* */ "noncSubquery"_Stz,    // CK_NONCSUBQUERY
  
  /* */ "on"_Stz,              // CK_ON
  /* */ "ordAgg"_Stz,          // CK_ORDAGG
  /* */ "originCast"_Stz,      // CK_ORIGINCAST
  /* */ "orPred"_Stz,          // CK_ORPRED
  
  /* */ "predicate"_Stz,       // CK_PREDICATE
  /* */ "project"_Stz,         // CK_PROJECT
  /* */ "projection"_Stz,      // CK_PROJECTION
  
  /* */ "scan"_Stz,            // CK_SEND
  /* */ "send"_Stz,            // CK_SCAN
  /* */ "sort"_Stz,            // CK_SORT
  /* */ "stage"_Stz,           // CK_STAGE
  /* */ "streamDef"_Stz,       // CK_STREAMDEF
  
  /* */ "table"_Stz,           // CK_TABLE,
  /* */ "term"_Stz,            // CK_TERM,
  /* */ "termList"_Stz,        // CK_TERMLIST,
  /* */ "type"_Stz,            // CK_TYPE,

  /* */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
